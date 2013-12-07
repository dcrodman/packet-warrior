#include <iostream>
#include <cstdio>
#include "PacketEngine.h"
#include "sniffex.h"

int main(int argc, const char * argv[]) {
    using namespace std;
    char error_buf[PCAP_ERRBUF_SIZE];

    PacketEngine engine;
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
    if (!engine.startCapture(error_buf)) {
        cout << "Error initiating capture: " << error_buf << endl;
        return -4;
    }
    int num_packets = 0;
    while (num_packets++ < 30) {
        Packet *packet_obj = engine.getNextPacket(error_buf);
        std::cout << "Packet " << num_packets << "\n";
        std::cout << *packet_obj << "\n";
        if (packet_obj->protocol().compare("TCP") == 0 ||
                packet_obj->protocol().compare("UDP") == 0) {
            print_payload(packet_obj->payload(), packet_obj->payload_length());
            std::cout << "\n\n";
        }
    }
    return 0;
}
