#!/usr/bin/env ruby

require 'serialport'

def meteo(port)
  serial = SerialPort.new("#{port}", 9600, 8, 1)
  serial.read_timeout = 5000

  loop do
    serial.write('T')
    temperature = serial.readline.strip

    sleep(0.5)
    serial.write('H')
    humidity = serial.readline.strip

    serial.write('A')
    light = serial.readline.strip

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

