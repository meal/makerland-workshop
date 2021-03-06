#!/usr/bin/env ruby

require 'serialport'

def blink(port)
  serial = SerialPort.new("#{port}", 9600, 8, 1)
  serial.read_timeout = 5000
  led_on = false

  loop do
    led_on = !led_on
    if led_on
      serial.write "D1"
    else
      serial.write "D0"
    end

    if led_on
      state = "On"
    else
      state = "Off"
    end
    print "LED is #{state}\r"
    STDOUT.flush
    sleep(1)
  end
end


if __FILE__ == $0
  if ARGV.empty?
    puts "usage: ruby #{__FILE__} <serial>"
  else
    blink(ARGV[0])
  end
end

