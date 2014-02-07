# -*- coding: utf-8 -*-
import time
import sys
import math
from itertools import *
from serial import Serial
import pyaudio

skip = 0
frequency = 0

def sine_wave(frequency=440.0, framerate=44100, amplitude=0.5,
        skip_frame=0):
    '''
    Generate a sine wave at a given frequency of infinite length.
    '''
    if amplitude > 1.0: amplitude = 1.0
    if amplitude < 0.0: amplitude = 0.0
    for i in count(skip_frame):
        sine = math.sin(2.0 * math.pi * float(frequency) * (float(i) / float(framerate)))
        yield float(amplitude) * sine

def callback(in_data, frame_count, time_info, status):
    i = 0
    data = ''
    global skip
    global frequency

    for sample in sine_wave(frequency=frequency, amplitude=1, skip_frame=skip):
        if i == frame_count:
            break

        data += chr(int((sample + 1) * 128))
        i += 1
    skip += frame_count
    return (data, pyaudio.paContinue)

def main():
    global frequency
    if len(sys.argv) != 2:
    	print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1])
    audio = pyaudio.PyAudio()
    stream = audio.open(format=pyaudio.paInt8,
                        channels=1,
                        rate=44100,
                        output=True,
                        stream_callback=callback)

    while True:
        serial.write("S")
        distance = int(serial.readline().strip())

        if distance > 50:
            frequency = 0
        else:
            frequency = 50 + (distance * 10)

        sys.stdout.write("Frequency is %s     \r" % str(distance))
        sys.stdout.flush()

    stream.stop_stream()
    stream.close()
    audio.terminate()

if __name__ == '__main__':
    main()