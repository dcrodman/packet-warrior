==============
packet-warrior
==============

Packet capture engine that can be used as a standalone application or compiled to a Python library with boost::python.


==============================
Project Requirements
==============================

In order to compile and run PacketWarrior, you need the following libraries and tools installed:

Boost 1.55
Libpcap 1.4
GCC 4 or Apple Darwin LLVM version 5
Python 2.7

==============================
Compilation Instructions
==============================

PacketWarrior.xcodeproj is set to build an executable that can be run to test that the
PacketEngine will compile and work properly on your computer. If the project builds and
runs, the next step is to compile the C++ bindings in pcap_ext.cc to generate a
shared module that Python (and consequently the GUI) can import and call. This module
is built using Boost.Build (bjam).

This section assumes that you have already built a binary for Boost.Python following
the instructions below. To configure bjam, make the following changes:

In boost-build.jam, edit the indicated line to point to your BOOST_ROOT (which may
be in your Downloads folder or something similar if you haven't installed it) and
more specifically the v2 subdirectory. The one used on our machines is the default
for an example.

In Jamroot.jam, edit the location of "use-project boost" to point to the top level
BOOST_ROOT directory. Additionally, make sure that the line under "project" in
"requirements" that reads:

	<library>/usr/lib/libpcap.dylib</library>

is directed to your system's "libpcap.dylib" include. On OS X, you should not have to
touch this line if you have installed the XCode Developer Tools.

If you've installed bjam per Boost.Python instructions, build the extension module by
running this line from the project root:

	bjam

To run the test for the module, execute:

	sudo python pcap_ext.py

To run the GUI itself, enter:

	mv pcap_ext.so PacketWarrior
	sudo python PacketWarrior/warriorgui.py

==============================
Resources
==============================
Building Boost.Python (no need to install the library itself):
http://www.boost.org/doc/libs/1_41_0/libs/python/doc/building.html