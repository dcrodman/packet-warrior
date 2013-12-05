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
    this->packet_header = *pkthdr;
    this->packet_data = *packet;
    this->eth_header = (ether_header*) packet;
}

int Packet::length() {
    return packet_header.len;
}

std::string Packet::source() {
    if (this->source_ip.empty())
        determineIPAddresses();
    return string(this->source_ip);
}

std::string Packet::destination() {
    if (this->destination_ip.empty())
        determineIPAddresses();
    return string(this->destination_ip);
}

// Determine the type of packet as defined by ethernet.h
std::string Packet::packet_type() {
    if (this->pkt_type.empty()) {
        u_short type = ntohs(eth_header->ether_type);
        switch (type) {
            case ETHERTYPE_IP:
                pkt_type = "IP";
                break;
            case ETHERTYPE_IPV6:
                pkt_type = "IPv6";
                break;
            case ETHERTYPE_ARP:
                pkt_type = "ARP";
                break;
            case ETHERTYPE_LOOPBACK:
                pkt_type = "LOOPBACK";
                break;
            case ETHERTYPE_REVARP:
                pkt_type = "REVERSE ARP";
                break;
            case ETHERTYPE_NTRAILER:
                pkt_type = "NTRAILER";
                break;
            case ETHERTYPE_PAE:
                pkt_type = "PAE";
                break;
            case ETHERTYPE_PUP:
                pkt_type = "PUP";
                break;
            case ETHERTYPE_RSN_PREAUTH:
                pkt_type = "RSN_PREAUTH";
                break;
            case ETHERTYPE_TRAIL:
                pkt_type = "TRAIL";
                break;
            case ETHERTYPE_VLAN:
                pkt_type = "VLAN";
                break;
            default:
                pkt_type = to_string(type);
                break;
        }
    }
    return string(this->pkt_type);
}

// Convert the timestamp to a human readable format.
std::string Packet::timestamp() {
    if (this->pkt_timestamp.empty()) {
        char time_buf[64];
        time_t pkt_time = (time_t)packet_header.ts.tv_sec;
        struct tm *time_info = localtime(&pkt_time);
        strftime(time_buf, sizeof time_buf, "%Y-%m-%d %H:%M:%S", time_info);
        pkt_timestamp = string(time_buf);
    }
    return string(this->pkt_timestamp);
}

// Extract the source and dest IPs from the header.
void Packet::determineIPAddresses() {
    if (source_ip.empty() || destination_ip.empty()) {
        source_ip = string(ether_ntoa((const struct ether_addr *)&eth_header->ether_shost));
        destination_ip =  string(ether_ntoa((const struct ether_addr *)&eth_header->ether_dhost));
    }
}
