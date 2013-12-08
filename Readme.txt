Project Requirements
==============================

In order to compile and run PacketWarrior, you need the following libraries and tools installed:

Boost 1.55
Libpcap 1.4
GCC 4 or Apple Darwin LLVM version 5
Python 2.7

Compilation Instructions
==============================

The GUI relies on the libpcap_ext module to run, which is built using CMake. To do this,
execute the following commands from the root project directory:

	mkdir build
	cd build
	cmake ../
	make

This should result in a libpcap_ext.so in the build/ directory. Move it to the same module
as the gui:
	
	mv libpcap_ext.so ../src

To run the tests, execute (from the project root):

	python pcap_ext.py

	