module Kinectaby
  class Device

    def led=(color)
      self.set_led(color)
    end

    def tilt=(degrees)
      self.set_tilt_degrees(degrees)
    end

  end
end
