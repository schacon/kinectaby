dir = File.dirname(File.expand_path(__FILE__))
$LOAD_PATH.unshift dir + '/../lib'
require 'kinectaby'

require 'rubygems'
require 'opengl'
require 'pp'
include Gl,Glu,Glut

$frame = Kinectaby::Frame.new
$image = []

def init
  glClearColor(0.0, 0.0, 0.0, 0.0)
  glShadeModel(GL_FLAT)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
end

play = lambda do
  thisTime = glutGet(GLUT_ELAPSED_TIME)
  glutPostRedisplay()
end

display = lambda do
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  puts "Draw"
  Kinectaby.get_depth($frame)

  glDisable(GL_TEXTURE_2D)
  glBegin(GL_POINTS)

  for i in 0..640-1
    for j in 0..480-1
      c = $frame.point(i, j).to_f
      c = 2047.0 if c > 2047
      c = 0.0 if c < 0
      c = c / 2047.0
      glColor4fv([c, c/2, c/3, 1.0])
      x = (i.to_f / 320.0) - 1.0
      y = (j.to_f / 240.0) - 1.0
      glVertex3f(x, -y, c)
    end
  end

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
