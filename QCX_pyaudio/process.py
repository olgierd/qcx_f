#!/usr/bin/python3
# -*- coding: utf-8 -*-
# de SQ3SWF 2019

import pyaudio, numpy, serial

# 512 samples at a time, signed 16-bit int, 1 channel, 44100 Hz
chunk = 512
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
SERIAL_BAUDRATE = 115200

p = pyaudio.PyAudio() # init audio object
ser = serial.Serial('/dev/ttyUSB0', 115200) # open serial port

# open audio stream
stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, output=True, frames_per_buffer=chunk)

lastc = -1      # last crossing position
lastfreq = 0      # lasf frequency
cnt = 0         # symbols count
zerocount = 0   # for counting zeros
currentf = 0

while True:
    data = stream.read(chunk)                            # read data and convert to signed 16b int
    buf=[ numpy.fromstring(data[q:q+2], 'Int16') for q in range(0,len(data), 2)]
    for i in range(0,len(buf)-1):                        # for all samples
        if buf[i] == 0 and zerocount < 100:              # count 100 samples with '0'
            zerocount = zerocount + 1
            if zerocount == 100:                         # if 100 zero samples in a row... (2.2ms @ 44.1kHz)
                print("Z")                               # stop TX
                ser.write(bytes("Z", "UTF-8"))           # send "Z" via serial (= stop TX)
                cnt = 0                                  # reset symbol counter
    lastc = 0
    freqs = []
    for i in range(0,len(buf)-1):                        # for all samples
        if buf[i] < 0 and buf[i+1] >= 0:                 # zero crossing
            zerocount = 0                                # clean 0-sample counter
            crossing = float(i + (-buf[i])/(-buf[i]+buf[i+1]))    # calculate 0-crossing with sub-sample resolution
            if lastc > 0:                                # if there already was a crossing...
                freq = RATE/(crossing-lastc)             # calculate sine frequency
                freqs.append(freq)
            lastc = crossing                             # ..and position of last zero-crossing
        if len(freqs) > 5 or (i == len(buf)-2 and len(freqs) != 0):
            freq = sum(freqs)/len(freqs)
            if abs(freq - lastfreq) > 1:
                print(cnt, freq)
                lastfreq = freq
                ser.write(bytes("A" + str(int( 100*(lastfreq+freq)/2)) + "X", "UTF-8"))
                print("A" + str(int( 10*(lastfreq+freq)/2)) + "X")
            cnt = cnt+1                          # increment symbol counter
            break

stream.stop_stream()
stream.close()
p.terminate()
