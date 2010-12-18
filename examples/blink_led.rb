dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'

require 'kinectaby'

context = Kinectaby::Context.new
device = context.open_device(0)
while(true) do
	puts "GREEN"
	device.set_led(Kinectaby::LED_GREEN)
	sleep 2

	puts "RED"
	device.set_led(Kinectaby::LED_RED)
	sleep 2

	puts "YELLOW"
	device.set_led(Kinectaby::LED_YELLOW)
	sleep 2

	puts "BLINK YELLOW"
	device.set_led(Kinectaby::LED_BLINK_YELLOW)
	sleep 2

	puts "BLINK GREEN"
	device.set_led(Kinectaby::LED_BLINK_GREEN)
	sleep 2

	puts "BLINK RED/YELLOW"
	device.set_led(Kinectaby::LED_BLINK_RED_YELLOW)
	sleep 2
end
