# -*- coding: utf-8 -*-
import time
import sys
import wave
from serial import Serial
import pyaudio

CHUNK = 1024

def play_sound():
    file = wave.open('motion_detected.wav', 'rb')
    audio = pyaudio.PyAudio()
    stream = audio.open(format=audio.get_format_from_width(file.getsampwidth()),
                        channels=file.getnchannels(),
                        rate=file.getframerate(),
                        output=True)

    data = file.readframes(CHUNK)

    while data != '':
        stream.write(data)
        data = file.readframes(CHUNK)

    stream.stop_stream()
    stream.close()

    audio.terminate()

def main():
    if len(sys.argv) != 2:
    	print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1])

    # After we send this, we have to wait 15 seconds for sensor to calibrate
    print "Calibration started..."
    serial.write("M")
    serial.readline()
    print "Calibration finished..."
    print

    while True:
        serial.write("M")
        motion = int(serial.readline().strip())

        if motion:
            state = '\033[91m DANGER! DANGER!\033[0m'
        else:
            state = 'Everything is ok'

        sys.stdout.write("%s%s\r" % (state, ' ' * 40))
        sys.stdout.flush()

        if motion:
            play_sound()

        time.sleep(1)

if __name__ == '__main__':
    main()