//
//  Packet.cc
//  PacketWarrior
//
//  Created by Drew Rodman on 12/4/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//
#include <iostream>

#include "Packet.h"

// It is assumed that these fields are passed directly (unmodified) from
// the pcap_loop callback function in order to perform all casts properly.
//
// At some point this initialization should be done lazily but it gets
// messy maintaining so much state (esp. with pointers), so it's just easier
// to handle it all in the constructor for now. It also makes it easy on
// the overloaded "<<" operator since it can't pass non-constant references
// of Packet to the object's methods and has to use member fields.

Packet::Packet(const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    sequence_num = 0;
    acknowledgement_num = 0;
    pkt_src_port = 0;
    pkt_dest_port = 0;
    pkt_payload = nullptr;

    // Length of the packet off the wire.
    pkt_length = pkthdr->len;

    // Generate a timestamp for the packet.
    char time_buf[64];
    time_t pkt_time = (time_t)pkthdr->ts.tv_sec;
    struct tm *time_info = localtime(&pkt_time);
    strftime(time_buf, sizeof time_buf, "%Y-%m-%d %H:%M:%S", time_info);
    pkt_timestamp = string(time_buf);

    /****** The following code has been adapted from the original implementation
    of TCPDump's sniffex.c got_packet handler to process incoming packets. ******/

    /* The Ethernet header */
	const struct sniff_ethernet *ethernet;
	ethernet = (struct sniff_ethernet*)(packet);

    ether_type = ntohs(ethernet->ether_type);
    packet_ethernet_type();

	/* Define/compute IP header and offset */
	const struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	int size_ip = IP_HL(ip)*4;
    valid_packet = (size_ip < 20) ? false : true;

	source_ip = string(inet_ntoa(ip->ip_src));
	destination_ip = string(inet_ntoa(ip->ip_dst));

	/* Determine Transport layer protocol */
	switch(ip->ip_p) {
		case IPPROTO_TCP:
			this->pkt_protocol =  "TCP";
			break;
		case IPPROTO_UDP:
			this->pkt_protocol = "UDP";
			break;
		case IPPROTO_ICMP:
			this->pkt_protocol = "ICMP";
			return;
		case IPPROTO_IP:
			this->pkt_protocol = "IP";
			return;
		default:
			this->pkt_protocol = "unknown";
			return;
	}

    if (pkt_protocol.compare("TCP") == 0) {
        /* The TCP header */
        const struct sniff_tcp *tcp;

        /* Define/compute TCP header offset */
        tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
        int size_tcp = TH_OFF(tcp)*4;
        if (size_tcp < 20)
            this->valid_packet = false;

        sequence_num = tcp->th_seq;
        acknowledgement_num = tcp->th_ack;

        pkt_src_port = ntohs(tcp->th_sport);
        pkt_dest_port = ntohs(tcp->th_dport);

        /* Define/compute tcp payload (segment) offset and size */
        pkt_payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
        pkt_payload_size = ntohs(ip->ip_len) - (size_ip + size_tcp);

        /****** End code adopted from sniffex.c ******/

    } else if (pkt_protocol.compare("UDP") == 0) {
        // UDP Segment structure consists of 4 header fields (src port, dest port,
        // length, and checksum), each 16 bits. The data field is variable

        // UDP header
        struct sniff_udp {
            u_short src_port;
            u_short dest_port;
            u_short len;
            u_short checksum;
        };
        const struct sniff_udp *udp;

        // TODO(drew): Check for malformed UDP packet header!

        // Compute UDP offset within the Ethernet/IP headers.
        udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
        pkt_src_port = ntohs(udp->src_port);
        pkt_src_port = ntohs(udp->dest_port);
        pkt_payload_size = udp->len;
    }
}

int Packet::length() {
    return pkt_length;
}

std::string Packet::timestamp() {
    return string(this->pkt_timestamp);
}

std::string Packet::source() {
    return string(this->source_ip);
}

std::string Packet::destination() {
    return string(this->destination_ip);
}

// Returns 0 for any protocol except TCP/UDP.
int Packet::source_port() {
    return pkt_src_port;
}

// Returns 0 for any protocol except TCP/UDP.
int Packet::destination_port() {
    return pkt_dest_port;
}

// Returns nullptr if the type is not supported.
const u_char* Packet::payload() {
    return pkt_payload;
}

// Returns 0 if the IP type is not supported.
int Packet::payload_length() {
    return pkt_payload_size;
}

// Determine the type of packet as defined by ethernet.h
std::string Packet::packet_ethernet_type() {
    if (pkt_ether_type.empty()) {
        switch (ether_type) {
            case ETHERTYPE_IP:
                pkt_ether_type = "IP";
                break;
            case ETHERTYPE_IPV6:
                pkt_ether_type = "IPv6";
                break;
            case ETHERTYPE_ARP:
                pkt_ether_type = "ARP";
                break;
            case ETHERTYPE_LOOPBACK:
                pkt_ether_type = "LOOPBACK";
                break;
            case ETHERTYPE_REVARP:
                pkt_ether_type = "REVERSE ARP";
                break;
            case ETHERTYPE_NTRAILER:
                pkt_ether_type = "NTRAILER";
                break;
            case ETHERTYPE_PAE:
                pkt_ether_type = "PAE";
                break;
            case ETHERTYPE_PUP:
                pkt_ether_type = "PUP";
                break;
            case ETHERTYPE_RSN_PREAUTH:
                pkt_ether_type = "RSN_PREAUTH";
                break;
            case ETHERTYPE_TRAIL:
                pkt_ether_type = "TRAIL";
                break;
            case ETHERTYPE_VLAN:
                pkt_ether_type = "VLAN";
                break;
            default:
                pkt_ether_type = to_string(ether_type);
                break;
        }
    }
    return string(pkt_ether_type);
}

// Transport layer protocol.
std::string Packet::protocol() {
    return string(pkt_protocol);
}

// Returns 0 if the protocol is not TCP.
u_int Packet::seq_number() {
    return this->sequence_num;
}

// Returns 0 if the protocol is not TCP.
u_int Packet::ack_number() {
    return this->acknowledgement_num;
}

// Returns false if the packet contains malformed data.
bool Packet::is_valid() {
    return this->valid_packet;
}

std::ostream& operator<<(std::ostream &strm, const Packet &packet) {
    if (packet.valid_packet) {
        return strm << "Length: " << packet.pkt_length << "\n" <<
                "Source: " << packet.source_ip << "\n" <<
                "Destination: " << packet.destination_ip << "\n" <<
                "Source Port: " << packet.pkt_src_port << "\n" <<
                "Destination Port: " << packet.pkt_dest_port << "\n" <<
                "Ether Type: " << packet.pkt_ether_type << "\n" <<
                "Protocol: " << packet.pkt_protocol << "\n" <<
                "Time: " << packet.pkt_timestamp << "\n" <<
                "Payload Length: " << packet.pkt_payload_size << "\n" <<
                "Sequence #: " << packet.sequence_num << "\n" <<
                "Acknowledgement #: " << packet.acknowledgement_num << "\n";
    } else {
        return strm << "Invalid Packet\n";
    }
}
