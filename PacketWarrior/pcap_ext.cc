//
//  pcap_ext.cc
//  PacketWarrior
//
//  Created by Drew Rodman on 11/26/13.
//

#include <iostream>

#include <boost/python.hpp>

#include "Packet.h"
#include "PacketEngine.h"

// Below a number of functions are declared as auxilary functions to
// facilitate calls to PacketEngine without having to modify the existing
// implementation or adapt Python calls. They are called by Python
// with references to an initialized PacketEngine object.

boost::python::list PacketEngine_getAvailableDevices(PacketEngine& self) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };
    const char** devices = self.getAvailableDevices(error_buffer);

    if (error_buffer[0])
        throw std::runtime_error(error_buffer);

    // Convert the c-string array to a list of Python strings.
    namespace python = boost::python;
    python::list device_list;
    for (unsigned int i = 0; i < self.getNumDevices(); i++)
    {
        const char* device = devices[i];
        python::str py_str = python::str(device, strlen(device));
        device_list.append(py_str);
    }
    return device_list;
}

void PacketEgnine_selectDevice(PacketEngine& self, std::string dev) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    self.selectDevice(dev.c_str(), error_buffer);
    if (error_buffer[0])
        throw std::runtime_error(error_buffer);
}


bool PacketEngine_setFilter(PacketEngine& self, std::string filter_exp) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    self.setFilter(filter_exp.c_str(), error_buffer);
    if (error_buffer[0])
        return false;
    return true;
}

void PacketEngine_startCapture(PacketEngine& self) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    self.startCapture(error_buffer, true);
    if (error_buffer[0])
        throw std::runtime_error(error_buffer);
}

Packet PacketEngine_getNextPacket(PacketEngine& self) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };
    Packet *pkt = self.getNextPacket(error_buffer);
    std::cout << "Got Packet: " << *pkt << "\n";
    return *pkt;
}

void Packet_payload(Packet& self) {
    const u_char *payload = self.payload();
}

// Define how to expose the Packet and PacketEngine attributes to Python.

BOOST_PYTHON_MODULE(pcap_ext) {
    using namespace boost::python;
    class_<PacketEngine>("PacketEngine")
        .def("getAvailableDevices", &PacketEngine_getAvailableDevices)
        .def("selectDevice", &PacketEgnine_selectDevice)
        .def("setFilter", &PacketEngine_setFilter)
        .def("startCapture", &PacketEngine_startCapture)
        .def("getNextPacket", &PacketEngine_getNextPacket)
        .def("endCapture", &PacketEngine::endCapture)
        .def("resetSession", &PacketEngine::resetSession)
        .def("isActive", &PacketEngine::isActive);

    /*
    class_<Packet>("Packet", no_init)
        .def("length", &Packet::length)
        .def("timestamp", &Packet::timestamp)
        .def("sourceIP", &Packet::source)
        .def("destinationIP", &Packet::destination)
        .def("sourcePort", &Packet::source_port)
        .def("destinationPort", &Packet::destination_port)
        .def("ethernetType", &Packet::packet_ethernet_type)
        .def("protocol", &Packet::protocol)
        .def("payload", &Packet_payload)
        .def("payloadLength", &Packet::payload_length)
        .def("sequenceNumber", &Packet::seq_number)
        .def("acknowledgementNumber", &Packet::ack_number)
        .def("isValid", &Packet::is_valid);
     */
}
