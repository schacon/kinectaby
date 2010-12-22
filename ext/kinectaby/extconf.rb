require 'mkmf'

dir_config("kinectaby")

have_library('freenect')
have_library('freenect_sync')

create_makefile("kinectaby/kinectaby")

