//
//  pcap_ext.cc
//  PacketWarrior
//
//  Created by Drew Rodman on 11/26/13.
//

#include <boost/python.hpp>
#include "PacketEngine.h"

boost::python::list PacketEngine_getAvailableDevices(PacketEngine& self) {
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };
    const char** devices = self.getAvailableDevices(error_buffer);

    if (error_buffer[0])
        throw std::runtime_error(error_buffer);

    // Convert the c-string array to a list of Python strings.
    namespace python = boost::python;
    python::list device_list;
    for (unsigned int i = 0; devices[i]; ++i)
    {
        const char* device = devices[i];
        device_list.append(python::str(device, strlen(device)));
    }
    return device_list;
}

BOOST_PYTHON_MODULE(libpcap_ext) {
    using namespace boost::python;
    class_<PacketEngine>("PacketEngine")
        .def("getAvailableDevices", &PacketEngine_getAvailableDevices);
}