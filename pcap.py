import libpcap_ext
engine = libpcap_ext.PacketEngine()
print engine.getAvailableDevices()
