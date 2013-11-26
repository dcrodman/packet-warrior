//
//  PacketEngine.h
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

#ifndef __PacketWarrior__PacketEngine__
#define __PacketWarrior__PacketEngine__

#include <queue>
#include "pcap.h"

#include "Packet.h"

class PacketEngine {

public:
    PacketEngine();
    ~PacketEngine();
    
    const char** getAvailableDevices(char *error_buf);
    bool selectDevice(const char* dev);
    Packet getNextPacket();
    
private:
    char *selected_device;
    char **devices;
    int num_devices;
    std::queue<Packet> packet_queue;
    
};

#endif /* defined(__PacketWarrior__PacketEngine__) */
