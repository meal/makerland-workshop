#!/usr/bin/env ruby

require 'serialport'

def alarm(port)

  ser = SerialPort.new("#{port}", 9600, 8, 1)

  puts "Calibration started..."
  ser.write('M')
  ser.readline()
  puts "Calibration finished..."
  puts ""

  loop do
    ser.write('M')
    motion = (ser.readline().strip).to_i

    if motion == 1
      state = "\033[91m DANGER! DANGER!\033[0m"
    else
      state = 'Everything is ok'
    end

    print "\t\t\t\t #{state}\r"
    STDOUT.flush

    sleep(1)
  end

end


if __FILE__ == $0
  if ARGV.empty?
    puts "usage: ruby #{__FILE__} <serial>"
  else
    alarm(ARGV[0])
  end
end

