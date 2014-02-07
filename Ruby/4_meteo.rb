#!/usr/bin/env ruby

require 'serialport'

def meteo(port)
  ser = SerialPort.new("#{port}", 9600, 8, 1)

  loop do
    ser.write('T')
    temperature = ser.readline().strip

    sleep(0.5)
    ser.write('H')
    humidity = ser.readline().strip

    ser.write('L')
    light = ser.readline().strip

    print "Temperature: #{temperature}*C\tHumidity: #{humidity}%\t\tLight: #{light}\r"
    STDOUT.flush
    sleep(1)
  end
end


if __FILE__ == $0
  if ARGV.empty?
    puts "usage: ruby #{__FILE__} <serial>"
  else
    meteo(ARGV[0])
  end
end

