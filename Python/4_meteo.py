# -*- coding: utf-8 -*-
import time
import sys
from serial import Serial

def main():
    if len(sys.argv) != 2:
    	print "usage: python %s <serial>" % __file__
        sys.exit(2)

    serial = Serial(sys.argv[1])

    while True:
        serial.write("T")
        temperature = serial.readline().strip()

        # We have to wait a little bit to read humidity
        time.sleep(0.5)
        serial.write("H")
        humidity = serial.readline().strip()

        serial.write("L")
        light = serial.readline().strip()

        sys.stdout.write("Temperature: %s°C\tHumidity: %s%%\t\tLight: %slx%s\r" %
                         (temperature, humidity, light, ' ' * 40))
        sys.stdout.flush()
        time.sleep(1)

if __name__ == '__main__':
    main()