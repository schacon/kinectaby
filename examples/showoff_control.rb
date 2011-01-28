dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'
require 'kinectaby'

require 'rubygems'
require 'opengl'
require 'pp'
include Gl,Glu,Glut

$frame = Kinectaby::Frame.new
$image = []

require 'firewatir'
$ff = FireWatir::Firefox.new
$ff.goto("http://localhost:9090")

def init
  glClearColor(0.0, 0.0, 0.0, 0.0)
  glShadeModel(GL_FLAT)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
end

play = lambda do
  thisTime = glutGet(GLUT_ELAPSED_TIME)
  glutPostRedisplay()
end

# returns the next x,y coord where values are between -1.0 and 1.0
# acts as a smoothing function, too

$lastx = 0.0
$lasty = 0.0

def next_point
  Kinectaby.get_depth($frame)
	x, y, d = $frame.hand
	x = -((x.to_f / 240.0) - 1.0)
	y = -((y.to_f / 320.0) - 1.0)

  if (d > 800)
    $last_event = [:start, Time.now]
    [0.0, 0.0]
  else
    $lastx = x if $lastx == 0.0
    $lasty = y if $lasty == 0.0

    #x = $lastx if ($lastx - x).abs > 0.6
    #y = $lasty if ($lasty - y).abs > 0.6
    
    $lastx = (($lastx + x) / 2)
    $lasty = (($lasty + y) / 2)

    #pp [$lastx, $lasty]
    determine_events(x, y)

    [$lastx, $lasty]
  end
end

$last_event = [:start, Time.now]

def determine_events(x, y)
  le = $last_event.first
  elapse = (Time.now - $last_event[1])

  if (x < -0.3) # in the left quadrant
    if (le == :right_side) && (elapse > 0.1)
      $last_event = [:swipe_left, Time.now]
      puts "SWIPE LEFT (next slide)"
      $ff.button(:value,"next").click
    end
    $last_event = [:left_side, Time.now] if le == :start
  end

  if (x > 0.3) # in the right quadrant
    if (le == :left_side) && (elapse > 0.1)
      $last_event = [:swipe_right, Time.now]
      puts "SWIPE RIGHT (prev slide)"
      $ff.button(:value,"prev").click
    end
    $last_event = [:right_side, Time.now] if le == :start
  end
end

display = lambda do
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_TEXTURE_2D)
	glEnable(GL_POINT_SMOOTH)
	glPointSize(15.0)
  glBegin(GL_POINTS)
	glColor4fv([1.0, 0.8, 0.8, 1.0])

	x, y = next_point
	glVertex3f(x, y, 0.5)

  glEnd()

  glutSwapBuffers()
  glFlush()
end

# Main
glutInit()
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE)

glutInitWindowSize(640, 480)
glutInitWindowPosition(100,100)
window = glutCreateWindow("Kinectaby Demo")

init()

glutIdleFunc(play)
glutDisplayFunc(display)
#glutKeyboardFunc(key)
#glutMouseFunc(mouse)
#glDepthFunc(GL_LESS)
#glEnable(GL_DEPTH_TEST)

#glMatrixMode(GL_PROJECTION);
#gluPerspective(40.0, 1.0, 0.5, 40.0);
#glMatrixMode(GL_MODELVIEW);
#gluLookAt(0.0, 1.0, 8.0,
#  0.0, 1.0, 0.0,
#  0.0, 1.0, 0.0)
#glPushMatrix()

glutMainLoop()
