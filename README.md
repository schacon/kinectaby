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

OUTPUT
==============

I'm still playing with the values I'm getting back (sorry, I'm kind of new at this), 
but for now the get_depth() call returns a frame with 640x480 data points, each an
11 bit value (0 to 2047, where 0 is far away and 2047 is close to the camera).

example output from the examples/viewer.rb opengl ruby script looking at the depth data:

<img src="https://img.skitch.com/20101222-csk8jrt3appq2refxtnk3trc6u.jpg">

That opengl script is horribly slow, once I figure out what kind of data I need from each
frame, I can do it in C and process them a lot faster.

I'm still playing with the video data, but it's similar.  It returns a 32-bit RGB integer
value, I believe.  I'll try to get an example gl script to show that off too.

TODO
==============

So for now only syncronous data capture is possible - eventually I would like to get the
async callbacks implemented as well.  There are also some log calls and a log callback 
that should be bound.  Finally, there is tilt and accelerometer data you can capture that
we should be able to get.  I've marked all the calls I haven't bound as 'TODO' in the 
kinectaby.c file.  Eventually I should probably also split that out into seperate files.

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

