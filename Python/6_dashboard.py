# -*- coding: utf-8 -*-
import time
import sys
import json
from serial import Serial
import requests


def main():
    if len(sys.argv) != 3:
    	print "usage: python %s <serial> <dashboard>" % __file__
        sys.exit(3)

    serial = Serial(sys.argv[1])
    dashboard = 'http://makerland-dashboard.herokuapp.com/widgets/%s-' % sys.argv[2]
    token = 'a8f77ce1a8d6a40403a31564f2233bcd'
    payload = {
		'auth_token': token
	}
    points = []
    last_x = 1

    # After we send this, we have to wait 15 seconds for sensor to calibrate
    print "Calibration started..."
    serial.write("M")
    serial.readline()
    print "Calibration finished..."
    print

    while True:
        serial.write("T")
        temperature = serial.readline().strip()
        payload.update({'current': temperature})
        requests.post(dashboard + 'temperature', data=json.dumps(payload))

        serial.write("M")
        motion = int(serial.readline().strip())
        payload.update({'value': 1 - motion})
        requests.post(dashboard + 'motion', data=json.dumps(payload))

        serial.write("S")
        distance = serial.readline().strip()
        payload.update({'current': distance})
        requests.post(dashboard + 'sonar', data=json.dumps(payload))

        serial.write("L")
        light = serial.readline().strip()
        for point in points:
            last_x = max(last_x, point['x'])

        points.append({ 'x': last_x + 1, 'y': light})
        points = points[-10:]
        payload.update({'points': points})
        requests.post(dashboard + 'light', data=json.dumps(payload))

        serial.write("H")
        humidity = serial.readline().strip()
        payload.update({'value': humidity})
        requests.post(dashboard + 'humidity', data=json.dumps(payload))

        time.sleep(0.5)

if __name__ == '__main__':
    main()