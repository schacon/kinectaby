require 'mkmf'

dir_config("kinectaby")

have_library('freenect')

create_makefile("kinectaby/kinectaby")

