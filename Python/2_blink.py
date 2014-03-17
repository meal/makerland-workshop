# -*- coding: utf-8 -*-
import time
import sys
from serial import Serial

def main():
    if len(sys.argv) != 2:
        print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1])
    led_on = False

    while True:
        led_on = not led_on
        if led_on:
            serial.write("D1")
        else:
            serial.write("D0")
        state = 'On' if led_on else 'Off'
        sys.stdout.write("LED is %s \r" % state)
        sys.stdout.flush()
        time.sleep(1)

if __name__ == '__main__':
    main()
