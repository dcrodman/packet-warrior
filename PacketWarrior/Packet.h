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

using namespace std;

class Packet {
public:
    Packet(const struct pcap_pkthdr* pkthdr, const u_char* packet);
    string source();
    string destination();
    string packet_type();
    string timestamp();

private:
    string pkt_timestamp;
    string source_ip;
    string destination_ip;
    string pkt_type;
};

#endif
