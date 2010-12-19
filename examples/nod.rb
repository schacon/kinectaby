dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'

require 'kinectaby'

def tilt(device, arr_degs, sleep = 3)
	arr_degs.each do |d|
		puts "deg: #{d}"
		device.set_tilt_degrees(d)
		sleep sleep
	end
end

context = Kinectaby::Context.new
device = context.open_device(0)

puts "Full Range"
tilt(device, [-30, 35, 0])

puts "Kinect, do you like Ruby?"
tilt(device, [-5, 5, -5, 5, -5, 5, 0], 0.5)

device.close
