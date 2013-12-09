import pcap_ext

engine = pcap_ext.PacketEngine()


devices = engine.getAvailableDevices()
assert len(devices) > 0, "Devices list is empty"

for device in devices:
	print 'Device: %s' % device
selected_dev = input("Enter number of device to sniff: ")

engine.selectDevice(devices[selected_dev])
engine.startCapture()

for i in range(1, 15):
	packet = engine.getNextPacket()
	print packet
	print "Payload: %s\n" % packet.payload()

print "\nCapture Complete."