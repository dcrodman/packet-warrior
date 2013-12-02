class MyPacket():
	
	def __init__(self, seqNo, ackNo, sourceIP, dstIP, protocol):
		self.seqNo = seqNo
		self.ackNo = ackNo
		self.sourceIP = sourceIP
		self.dstIP = dstIP
		self.protocol = protocol

	def __repr__(self):
		return '{0}, {1}, {2}, {3}, {4}, {5}'.format(self.seqNo, self.ackNo, self.time, self.sourceIP, self.dstIP, self.protocol)

	def __str__(self):
		return '{0}, {1}, {2}, {3}, {4}, {5}'.format(self.seqNo, self.ackNo, self.time, self.sourceIP, self.dstIP, self.protocol)
		
