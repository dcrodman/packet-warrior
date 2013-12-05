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
Packet::Packet(const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    ether_header *eth_header;
    char time_buf[64];

    // Convert the timestamp to a human readable format.
    time_t pkt_time = (time_t)pkthdr->ts.tv_sec;
    struct tm *time_info = localtime(&pkt_time);
    strftime(time_buf, sizeof time_buf, "%Y-%m-%d %H:%M:%S", time_info);
    pkt_timestamp = string(time_buf);

    // Extract the source and dest IPs from the head.
    eth_header = (ether_header*) packet;
    source_ip = string(ether_ntoa((const struct ether_addr *)&eth_header->ether_shost));
    destination_ip =  string(ether_ntoa((const struct ether_addr *)&eth_header->ether_dhost));

    // Determine the type of packet as defined by ethernet.h
    u_short type = ntohs(eth_header->ether_type);
    switch (type) {
        case ETHERTYPE_IP:
            pkt_type = string("IP");
            break;
        case ETHERTYPE_IPV6:
            pkt_type = string("IPv6");
            break;
        case ETHERTYPE_ARP:
            pkt_type = string("ARP");
            break;
        case ETHERTYPE_LOOPBACK:
            pkt_type = string("LOOPBACK");
            break;
        case ETHERTYPE_REVARP:
            pkt_type = string("REVERSE ARP");
            break;
        case ETHERTYPE_NTRAILER:
            pkt_type = string("NTRAILER");
            break;
        case ETHERTYPE_PAE:
            pkt_type = string("PAE");
            break;
        case ETHERTYPE_PUP:
            pkt_type = string("PUP");
            break;
        case ETHERTYPE_RSN_PREAUTH:
            pkt_type = string("RSN_PREAUTH");
            break;
        case ETHERTYPE_TRAIL:
            pkt_type = string("TRAIL");
            break;
        case ETHERTYPE_VLAN:
            pkt_type = string("VLAN");
            break;
        default:
            pkt_type = std::to_string(type);
            break;
    }
}

/* Copies so that this class remains immutable.*/

std::string Packet::source() {
    return string(this->source_ip);
}

std::string Packet::destination() {
    return string(this->destination_ip);
}

std::string Packet::packet_type() {
    return string(this->pkt_type);
}

std::string Packet::timestamp() {
    return string(this->pkt_timestamp);
}
