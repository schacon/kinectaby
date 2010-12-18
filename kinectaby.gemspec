Gem::Specification.new do |s|
  s.name              = "kinectaby"
  s.version           = "0.0.1"
  s.date              = Time.now.strftime('%Y-%m-%d')
  s.summary           = "Kinectaby is a Ruby binding to the libfreenect OS Kinect library"
  s.homepage          = "http://github.com/schacon/kinectaby"
  s.email             = "schacon@gmail.com"
  s.authors           = [ "Scott Chacon" ]
  s.files             = %w( README.md Rakefile LICENSE )
  s.files            += Dir.glob("lib/**/*.rb")
  s.files            += Dir.glob("test/**/*")
  s.files            += Dir.glob("ext/**/*.c") + Dir.glob("ext/**/*.h")
  s.extensions        = ['ext/kinectaby/extconf.rb']
  s.description       = <<desc
Kinectaby is a Ruby binding to the libfreenect OS Kinect library.
desc
end
