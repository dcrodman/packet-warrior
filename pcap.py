import pcap_ext

engine = pcap_ext.PacketEngine()


devices = engine.getAvailableDevices()
assert len(devices) > 0, "Devices list is empty"

for device in devices:
	print 'Device: %s' % device
selected_dev = input("Enter number of device to sniff: ")

engine.setFilter("tcp port 80")
engine.selectDevice(devices[selected_dev])
engine.startCapture()

print engine.getNextPacket()
print "Done"