require 'kinectaby/kinectaby'
require 'kinectaby/device'

module Kinectaby

  # just return the first device - this is what most people will have
  def self.device
    context = Kinectaby::Context.new
    if context.num_devices > 0
      return context.open_device(0)
    end
    return false
  end
end
