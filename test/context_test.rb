require File.expand_path "../test_helper", __FILE__

context "Kinectaby::Context stuff" do
  setup do
    @context = Kinectaby::Context.new
  end

  test "Initialize and shutdown Kinect" do
    assert @context.shutdown
  end

  test "Count Kinect devices" do
    assert 1, @context.num_devices
  end

  test "Kinect LED constants" do
    assert_equal 0, Kinectaby::LED_OFF
    assert_equal 6, Kinectaby::LED_BLINK_RED_YELLOW
  end
end
