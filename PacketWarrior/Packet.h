//
//  Packet.h
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//

#ifndef PacketWarrior_Packet_h
#define PacketWarrior_Packet_h

#include <string>
#include <ctime>

#include <netinet/in.h>
//#include <arpa/inet.h>
#include <netinet/if_ether.h>

class Packet {
public:
    Packet(const struct pcap_pkthdr* pkthdr, const u_char* packet);
    std::string source();
    std::string destination();
    std::string packet_type();

private:
    std::string source_ip;
    std::string destination_ip;
    std::string pkt_type;
};

#endif
