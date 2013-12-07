//
//  Packet.h
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//

#ifndef PacketWarrior_Packet_h
#define PacketWarrior_Packet_h

#include <cstring>
#include <string>
#include <sys/time.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include "pcap.h"
#include "sniffex.h"

using namespace std;

class Packet {
public:
    Packet(const struct pcap_pkthdr* pkthdr, const u_char* packet);
    int length();

    string timestamp();
    string source();
    string destination();

    int source_port();
    int destination_port();

    string packet_ethernet_type();
    string protocol();

    const u_char* payload();
    int payload_length();

    int seq_number();
    int ack_number();

    bool is_valid();

private:
    // Length of the packet (off-wire).
    int pkt_length;

    // Human-readable Timestamp.
    string pkt_timestamp;
    // Src and Dest IP addresses.
    string source_ip;
    string destination_ip;

    // Src and Dest ports (TCP/UDP only).
    int pkt_src_port;
    int pkt_dest_port;

    // Raw ethernet type (host byte order).
    u_short ether_type;
    // Human-readable type.
    string pkt_ether_type;
    // Human-readable Transport protocol.
    string pkt_protocol;

    // Contents of the actual datagram.
    u_char *pkt_payload;
    int pkt_payload_size;

    // TCP SEQ and ACK numbers.
    u_int sequence_num;
    u_int acknowledgement_num;

    // Whether or not any of the packet data is malformed.
    bool valid_packet;

    // Overloaded stream output operator to make printing easier.
    friend std::ostream& operator<<(std::ostream &strm, const Packet &packet);
};

#endif
