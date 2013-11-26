//
//  PacketEngine.cpp
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

#include <typeinfo>
#include <iostream>
#include <cstring>
#include "PacketEngine.h"

PacketEngine::PacketEngine() {
    this->devices = nullptr;
}

PacketEngine::~PacketEngine() {
    delete [] this->devices;
}

// Returns a list of devices available for sniffing. If an error occurred,
// this method will return NULL and error_buf will contain the error message
// returned by pcap. Note that if execution continues, the caller is responsible
// for freeing the error buffer.
const char ** PacketEngine::getAvailableDevices(char *error_buf) {
    if (this->devices == NULL) {
        pcap_if_t *devs;
        char *error_buffer = new char[PCAP_ERRBUF_SIZE];

        if (pcap_findalldevs(&devs, error_buffer) == -1) {
            error_buf = error_buffer;
            return NULL;
        } else {
            pcap_if_t *dev = devs;
            this->num_devices= sizeof(pcap_if_t) / sizeof(devs);
            this->devices = new char*[this->num_devices];

            // Copy the device name from each entry into this->devices.
            for (int i = 0; i < this->num_devices && dev != NULL; i++, dev = dev->next) {
                this->devices[i] = new char[sizeof(dev->name)];
                strncpy(this->devices[i], dev->name, sizeof(dev->name) / sizeof(char));
            }
        }
    }
    return (const char**)this->devices;
}

// Select the device that will be opened for sniffing. Returns true if the
// selection was a valid device, false otherwise.
bool PacketEngine::selectDevice(const char* dev) {
    for (int i = 0; i < this->num_devices; i++) {
        if (strcmp(dev, this->devices[i]) == 0) {
            strcpy(this->selected_device, this->devices[i]);
            return true;
        }
    }
    return false;
}