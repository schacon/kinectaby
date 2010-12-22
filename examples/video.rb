dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'

require 'kinectaby'
require 'pp'

puts 'start'

frame = Kinectaby::Frame.new

while true do
  puts "depth"
  Kinectaby.get_depth(frame)
  puts frame.point(9, 50)
  puts "video"
  Kinectaby.get_video(frame)
  puts frame.point(9, 50)
end

