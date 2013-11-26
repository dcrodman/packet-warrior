//
//  PacketEngine.h
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

#ifndef __PacketWarrior__PacketEngine__
#define __PacketWarrior__PacketEngine__

#include "Packet.h"
#include <queue>
#include "pcap.h"

class PacketEngine {

public:
    PacketEngine();
    ~PacketEngine();
    
    char* findAllDevs();
    Packet getNextPacket();
    
private:
    char *error_buffer;
    std::queue<Packet> packet_queue;
    
};

#endif /* defined(__PacketWarrior__PacketEngine__) */
