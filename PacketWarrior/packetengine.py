#! /usr/bin/env python
#
# packetengine.py
import socket
from struct import *
import datetime
import pcapy
import sys
import fileinput
import os, logging, socket
import pcapy, impacket, impacket.ImpactDecoder
from impacket.ImpactDecoder import EthDecoder, LinuxSLLDecoder
from threading import Thread

ETHERNET_MAX_FRAME_SIZE = 1518
PROMISC_MODE = 0

class DecoderThread(Thread):
    def __init__(self, pcapObj):
        # Query the type of the link and instantiate a decoder accordingly.
        datalink = pcapObj.datalink()
        if pcapy.DLT_EN10MB == datalink:
            self.decoder = EthDecoder()
        elif pcapy.DLT_LINUX_SLL == datalink:
            self.decoder = LinuxSLLDecoder()
        else:
            raise Exception("Datalink type not supported: " % datalink)

        self.pcap = pcapObj
        self.myfile = open("temp.txt", "w", 66)
        Thread.__init__(self)

    def run(self):
        # Sniff ad infinitum.
        # PacketHandler shall be invoked by pcap for every packet.
        self.pcap.loop(0, self.packetHandler)

    def packetHandler(self, hdr, data):
        # Use the ImpactDecoder to turn the rawpacket into a hierarchy
        # of ImpactPacket instances.
        # Display the packet in human-readable form.
        try:
            self.myfile.write(str(self.decoder.decode(data)))
        except Exception, e:
            print e

class PacketEngine():

    def __init__(self):
        self.selected_device = None
        self.available_devices = None
        self.cap = None
        self.thread = None

    def get_available_devices(self):
        self.available_devices = pcapy.findalldevs()
        return self.available_devices

    def set_device(self, choice):
        self.selected_device = choice
        self.set_packet_reader()

    def set_packet_reader(self, sniff_timeout = 1000):
        '''
        open device
        # Arguments here are:
        #   device
        #   snaplen (maximum number of bytes to capture _per_packet_)
        #   promiscious mode (1 for true)
        #   timeout (in milliseconds)
        '''
        try:
            self.cap = pcapy.open_live(self.selected_device, ETHERNET_MAX_FRAME_SIZE, PROMISC_MODE, sniff_timeout)
            print "Set to capture on %s: net=%s, mask=%s" % (self.selected_device, self.cap.getnet(), self.cap.getmask())
        except Exception, e:
            print "Failed to set packet capture reader : open_live() failed for device='%s'. Error: %s" % (self.selected_device, str(e))

    def set_filter(self, filter):
        self.cap.setfilter(filter)

    def start_capture(self):
        # Start sniffing thread and finish main thread.
        self.thread = DecoderThread(self.cap).start()

    def stop_capture(self):
        print "Attempting to stop thread"
        if self.thread is not None:
            self.thread.terminate()
            self.thread = None

if __name__ == "__main__":
  p = PacketEngine()

