//
//  packet_types.h
//  PacketWarrior
//
//  Created by Drew Rodman on 12/19/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

/* Define supported packet types as structures that can be used
 * to interpret results returned by pcap.
 */

#ifndef PacketWarrior_packet_types_h
#define PacketWarrior_packet_types_h

#include <netinet/in.h>
#include <cstdint>

/* IPv6 Header
 *
 * Source and destination addresses are each 128 bit IPv6 addresses.
 */
struct ip_ipv6 {
    uint32_t vtf; /* Version, traffic class, flow label. */
    uint16_t len; /* Payload length. */
    uint8_t next_hdr; /* Next header; indicates Transport protocol. */
    uint8_t hop_limit; /* Router hop limit. */
    struct in6_addr src;
    struct in6_addr dest;
};

#define IP6_VER(ip) (((ip->vtf) >> 28) // 4 bit version
#define IP6_TC(ip) (((ip->vtf) & 0xFF00000) >> 18) // 8 bit traffic class
#define IP6_FL(ip) (((ip->vtf) & 0xFFFF) // 20 bit flow label

/* UDP Header
 *
 * UDP Segment structure consists of 4 header fields (src port, dest port,
 * length, and checksum), each 2 bytes. The data field is variable, so we
 * need to calculate the payload based on the position of the header
 * relative to the end of the packet.
 */
struct sniff_udp {
    u_short src_port;
    u_short dest_port;
    u_short len;
    u_short checksum;
};

#endif
