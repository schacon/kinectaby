Kinectaby - libfreenect Ruby bindings
=====================================

Kinectaby is a first pass at Ruby bindings to the amazing Kinect libfreenect
driver.

INSTALLING AND RUNNING
========================

First you need a Kinect.

Then you need to install libfreenect.

Next, you need to install rake-compiler:

    $ sudo gem install rake-compiler

Then you need to get and compile this.

    $ git clone git://github.com/schacon/kinectaby.git
    $ cd kinectaby
    $ rake compile
    $ rake test

API 
==============

    require 'kinectaby'

    context = Kinectaby::Context.new
    context.num_devices
    context.shutdown

    device = context.open_device(0)
    # OR #
    device = Kinectaby.device  # will just get the first one
    device.led = Kinectaby::LED_RED
    device.tilt = 30 # degree tilt, from -30 to 30
    device.close

    # syncronous video/depth capture (i can get about 30fps)

    dframe = Kinectaby::Frame.new
    vframe = Kinectaby::Frame.new

    while true do
      puts "depth"
      Kinectaby.get_depth(dframe)
      puts frame.point(x, y)

      puts "video"
      Kinectaby.get_video(vframe)
      puts frame.point(x, y)
    end

CONTRIBUTING
==============

Fork schacon/kinectaby on GitHub, make it awesomer (preferably in a branch named
for the topic), send me a pull request.


AUTHORS 
==============

* Scott Chacon <schacon@gmail.com>

LICENSE
==============

MIT.  See LICENSE file.

