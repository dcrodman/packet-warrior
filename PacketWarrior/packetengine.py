#! /usr/bin/env python
#
# packetengine.py
import socket
from struct import *
import datetime
import sys
import time
import fileinput
import os, logging, socket
import pcapy
from pcapy import PcapError
import impacket, impacket.ImpactDecoder
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
        logging.basicConfig(filename='pw.log')
        self.pcap = pcapObj
        self.myfile = open("temp.txt", "w")
        self.is_sniffing = False
        Thread.__init__(self)

    def run(self):
        # Sniff ad infinitum.
        # PacketHandler shall be invoked by pcap for every packet.
        self.is_sniffing = True
        while self.is_sniffing:
            try:
                hdr, data = self.pcap.next()
                self.packetHandler(hdr,data)
                time.sleep(0.1)
            except Exception, e:
                logging.error(e)
        self.pcap.close()

    def get_condition(self):
        return self.is_sniffing

    def set_condition(self,value):
        self.is_sniffing = value

    def packetHandler(self, hdr, data):
        # Use the ImpactDecoder to turn the rawpacket into a hierarchy
        # of ImpactPacket instances.
        # Display the packet in human-readable form.
        try:
            packet = str(self.decoder.decode(data))
            if len(packet) > 0:
                self.myfile.write("\nTIMESTAMP: %s\n" % str(datetime.datetime.utcnow()))
            self.myfile.write(packet)
        except Exception, e:
            logging.error(e)

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
        return self.set_packet_reader()

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
            return self.selected_device, self.cap.getnet(), self.cap.getmask()
        except Exception, e:
            print "Failed to set packet capture reader : open_live() failed for device='%s'. Error: %s" % (self.selected_device, str(e))

    def set_filter(self, filter):
        self.cap.setfilter(filter)

    def start_capture(self):
        # Start sniffing thread and finish main thread.
        self.deleteContent("temp.txt")
        self.thread = DecoderThread(self.cap)
        self.thread.run()

    def stop_capture(self):
        print "Attempting to stop thread"
        if self.thread is not None:
            self.thread.set_condition(False)
            self.thread = None

    def deleteContent(self, inputfile):
        pfile = open(inputfile, "w")
        pfile.seek(0)
        pfile.truncate()

if __name__ == "__main__":
  p = PacketEngine()

