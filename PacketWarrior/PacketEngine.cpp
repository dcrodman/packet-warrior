//
//  PacketEngine.cpp
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

#include <iostream>
#include "PacketEngine.h"

PacketEngine::PacketEngine() {
    this->error_buffer =new char[PCAP_ERRBUF_SIZE];
}

PacketEngine::~PacketEngine() {
    delete this->error_buffer;
}

char* PacketEngine::findAllDevs() {
    pcap_if_t *devs;
    
    if (pcap_findalldevs(&devs, error_buffer) == -1)
        return error_buffer;
    else {
        pcap_if_t *dev = devs;
        while (dev) {
            std::cout << dev->name << std::endl;
            dev = dev->next;
        }
        return NULL;
    }
}

Packet PacketEngine::getNextPacket() {
    return Packet();
}