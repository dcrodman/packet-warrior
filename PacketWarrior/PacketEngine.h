//
//  PacketEngine.h
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//

#ifndef __PacketWarrior__PacketEngine__
#define __PacketWarrior__PacketEngine__

#include <cstring>
#include <iostream>
#include <queue>
#include <thread>

#include "pcap.h"

#include "Packet.h"

class PacketEngine {

public:
    PacketEngine();
    ~PacketEngine();
    
    const char** getAvailableDevices(char *error_buf);
    int getNumDevices();
    bool selectDevice(const char* dev, char *error_buf);
    bool setFilter(const char *filter, char *error_buf);
    bool startCapture(char *error_buf);
    void endCapture();
    void resetSession();

    bool isActive();
    Packet getNextPacket(bool wait);
    
    friend void auxilaryHandler(u_char *user,
            const struct pcap_pkthdr* pkthdr, const u_char* packet);

private:
    bool createHandle(char *error_buf);
    void callbackHandler(const struct pcap_pkthdr* pkthdr, const u_char* packet);

    char **devices;
    int num_devices;
    // Handler for sniffing session.
    pcap_t *handle;
    // Network device on which to sniff.
    char *selected_device;
    // IP address of sniffing device.
    bpf_u_int32 net_info;
    // Netmask of sniffing device.
    bpf_u_int32 net_mask;
    // Compiled filter.
    struct bpf_program filter_p;
    // Whether or not the session is running.
    bool is_active;

    std::queue<Packet> packet_queue;
    
};

void auxilaryHandler(u_char *user, const struct pcap_pkthdr* pkthdr, const u_char* packet);

#endif /* defined(__PacketWarrior__PacketEngine__) */
