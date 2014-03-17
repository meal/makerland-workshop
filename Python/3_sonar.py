# -*- coding: utf-8 -*-
import time
import sys
from serial import Serial

def main():
    if len(sys.argv) != 2:
        print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1], timeout=5)

    while True:
        serial.write("S")
        distance = serial.readline().strip()
        sys.stdout.write("Distance is %scm     \r" % distance)
        sys.stdout.flush()
        time.sleep(1)

if __name__ == '__main__':
    main()
