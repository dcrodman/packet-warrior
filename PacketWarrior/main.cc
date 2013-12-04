#include <iostream>
#include <cstdio>
#include "PacketEngine.h"

void auxilaryHandler(u_char *_, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    Packet packet_obj(pkthdr, packet);
    std::cout << "Got packet of length " << pkthdr->len << "\n";
    std::cout << "Source IP: " << packet_obj.source() << "\n";
    std::cout << "Destination IP: " << packet_obj.destination() << "\n";
    std::cout << "Type: " << packet_obj.packet_type() << "\n";
}

int main(int argc, const char * argv[]) {
    using namespace std;
    PacketEngine engine;
    char error_buf[PCAP_ERRBUF_SIZE];
    const char **devs = engine.getAvailableDevices(error_buf);
    if (devs == NULL) {
        cout << "Error finding devices: " << error_buf << endl;
        return -1;
    }
    for (int i = 0; i < engine.getNumDevices(); i++)
        cout << "Device " << i << ": " << devs[i] << "\n";
    cout << "Enter the number of the device to sniff: ";
    int selection = 0;
    cin >> selection;

    if (!engine.selectDevice(devs[selection], error_buf)) {
        cout << "Error selecting device: " << error_buf << endl;
        return -2;
    }

    char apply_filter = ' ';
    cout << "Apply filter to " << devs[selection] << "? (y/n)\n";
    cin >> apply_filter;

    if (apply_filter == 'y' || apply_filter == 'Y') {
        char *filter_expression = new char[64];
        cout << "Enter a filter expression (under 64 chars): ";
        cin >> filter_expression;

        if (!engine.setFilter(filter_expression, error_buf)) {
            cout << "Error applying filter: " << error_buf << endl;
            return -3;
        }
    }
    cout << "Beginning capture...\n";
    if (!engine.startCapture(&auxilaryHandler, error_buf)) {
        cout << "Error initiating capture: " << error_buf << endl;
        return -4;
    }
    return 0;
}
