# stolen largely from defunkt/mustache
require 'rake/testtask'
require 'rake/rdoctask'
require 'rake/extensiontask'

Rake::ExtensionTask.new('kinectaby') do |r|
  r.lib_dir = 'lib/kinectaby'
end

#
# Tests
#

task :default => :test

desc "Run tests"
task :turn do
  suffix = "-n #{ENV['TEST']}" if ENV['TEST']
  sh "turn test/*_test.rb #{suffix}"
end

Rake::TestTask.new do |t|
  t.libs << 'lib'
  t.pattern = 'test/**/*_test.rb'
  t.verbose = false
end

desc "Update the libfreenect SHA"
task :libfreenect do
  sha = `git --git-dir=../libfreenect/.git rev-parse HEAD`
  File.open("LIBFREENECT_VERSION", 'w') do |f|
    f.puts "# git --git-dir=../libfreenect/.git rev-parse HEAD"
    f.puts sha
  end
end

