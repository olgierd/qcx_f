#!/usr/bin/python

import socket, serial

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

s.bind(('127.0.0.1', 31337))

lastmsg = ''

while True:
	data, addr = s.recvfrom(8)
	if data != lastmsg:
		ser.write(data)
		lastmsg = data
		print data
