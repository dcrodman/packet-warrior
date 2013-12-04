//
//  PacketEngine.cpp
//  PacketWarrior
//
//  Created by Drew Rodman on 11/25/13.
//

#include "PacketEngine.h"

PacketEngine::PacketEngine() {
    this->devices = nullptr;
    this->handle = nullptr;
    this->selected_device = nullptr;
}

PacketEngine::~PacketEngine() {
    delete this->selected_device;
    delete [] this->devices;
}

// Returns a list of devices available for sniffing. If an error occurred,
// this method will return NULL and error_buf will contain the error message
// returned by pcap. Note that if execution continues, the caller is responsible
// for freeing the error buffer.
const char ** PacketEngine::getAvailableDevices(char *error_buf) {
    if (this->devices == NULL) {
        pcap_if_t *devs;
        if (pcap_findalldevs(&devs, error_buf) == -1 || devs == NULL) {
            if (strlen(error_buf) < 1)
                strcpy(error_buf, "No devices available; try with root priviledges.");
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

int PacketEngine::getNumDevices() {
    return this->num_devices;
}

// Select the device that will be opened for sniffing and populates network information.
// Returns true if the selection was a valid device, false otherwise.
bool PacketEngine::selectDevice(const char* dev, char *error_buf) {
    for (int i = 0; i < this->num_devices; i++) {
        if (strcmp(dev, this->devices[i]) == 0) {
            this->selected_device = new char[strlen(this->devices[i])];
            strcpy(this->selected_device, this->devices[i]);

            char *error_buffer = new char[PCAP_ERRBUF_SIZE];
            if (pcap_lookupnet(this->selected_device, &net_info,&net_mask, error_buffer) == -1) {
                strcpy(error_buf, error_buffer);
                return false;
            }
            return true;
        }
    }
    return false;
}

// Create a handle for the network device to set filters/intiate sniffing.
bool PacketEngine::createHandle(char *error_buf) {
    if (this->handle == nullptr) {
        this->handle = pcap_create(this->selected_device, error_buf);
        if (this->handle == NULL)
            return false;
    }
    return true;
}

// Apply a filter expression according to the TCPDUMP PCAP syntax.
bool PacketEngine::setFilter(const char *filter, char *error_buf) {
    if (!createHandle(error_buf))
        return false;
    if (pcap_compile(this->handle, &filter_p, filter, 0, this->net_info) == -1) {
        strcpy(error_buf, pcap_geterr(handle));
        return false;
    }
    if (pcap_setfilter(this->handle, &this->filter_p) == -1) {
        strcpy(error_buf, pcap_geterr(handle));
        return false;
    }
    return true;
}

// Activate the handle for the device and begin packet capturing loop. The caller
// is responsible for spawning this method in a separate thread if desired.
bool PacketEngine::startCapture(pcap_handler callback, char *error_buf) {
    if (!createHandle(error_buf))
        return false;

    // Manually set the buffer size so that the application does not experience a
    // substantial delay before being able to process packets.
    pcap_set_buffer_size(this->handle, 1024);

    if (pcap_activate(handle) == PCAP_WARNING) {
        strcpy(error_buf, pcap_geterr(this->handle));
        return false;
    }
    if (pcap_loop(this->handle, -1, callback, NULL) == -1) {
        strcpy(error_buf, pcap_geterr(this->handle));
        return false;
     }
    return true;
}

// End the capture session.
void PacketEngine::endCapture() {
    pcap_close(this->handle);
}