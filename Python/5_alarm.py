# -*- coding: utf-8 -*-
import time
import sys
import wave
from serial import Serial

def main():
    if len(sys.argv) != 2:
    	print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1], timeout=5)

    # After we send this, we have to wait 15 seconds for sensor to calibrate
    print "Calibration started..."
    serial.write("M")
    serial.readline()
    print "Calibration finished..."
    print

    while True:
        serial.write("M")
        try:
            motion = int(serial.readline().strip())

            if motion:
                state = '\033[91m DANGER! DANGER!\033[0m'
            else:
                state = 'Everything is ok'

            sys.stdout.write("%s%s\r" % (state, ' ' * 40))
            sys.stdout.flush()

            time.sleep(1)
        except:
            pass

if __name__ == '__main__':
    main()