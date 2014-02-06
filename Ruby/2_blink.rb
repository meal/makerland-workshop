#!/usr/bin/env ruby

require 'serialport'

def blink(port)
  ser = SerialPort.new("#{port}", 9600, 8, 1, SerialPort::NONE)

  led_on = false

  loop do
    led_on = !led_on
    if led_on
      ser.write "D1"
    else
      ser.write "D0"
    end

    if led_on
      state = "On"
    else
      state = "Off"
    end
    puts "LED is #{state} \r"
  end
end


if __FILE__ == $0
  if ARGV.empty?
    puts "usage: ruby #{__FILE__} <serial>"
  else
    blink(ARGV)
  end
end

