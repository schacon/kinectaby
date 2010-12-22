dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'

require 'kinectaby'
require 'pp'

puts 'start'

dframe = Kinectaby::Frame.new
vframe = Kinectaby::Frame.new

while true do
	t1 = Time.now

	150.times do
		Kinectaby.get_depth(dframe)
		Kinectaby.get_video(vframe)
	end
	t2 = Time.now
	sec = t2 - t1

	fps = 150.0 / sec
	puts "#{fps.to_s[0, 7].ljust(8)} frames per second"
end

