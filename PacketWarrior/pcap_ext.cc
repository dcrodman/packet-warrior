//
//  pcap_ext.cc
//  PacketWarrior
//
//  Created by Drew Rodman on 11/26/13.
//  Copyright (c) 2013 Drew Rodman. All rights reserved.
//

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

char cconst* greet() {
    return "hello world";
}

BOOST_PYTHON_MODULE(hello_ext) {
    using namespace boost::python;
    def("greet", greet);
}