require 'serialport'

def list_serial_ports
  if Gem.win_platform? == true
    availables = []
    (1..256).each do |p|
      begin
        ser = SerialPort.new(p)
        availables << "COM#{p}"
      rescue Errno::ENOENT => e
        File.open("ports.log", 'wb') do |f|
          f << 'could not open file'
        end
      end
    end
    availables.each do |available|
      print "\t #{available}"
    end
  else
    Dir.glob("/dev/tty.*") do |port|
      print "\t #{port}"
    end
  end
end

if __FILE__ == $0
  puts ""
  puts "Available ports:"
  puts ""
  list_serial_ports
  puts ""
end
