#include <ruby.h>
#include <assert.h>

#include "libfreenect.h"
#include "libfreenect/libfreenect_sync.h"

static VALUE rb_mKinectaby;

static VALUE rb_cKinectabyContext;
static VALUE rb_cKinectabyDevice;
static VALUE rb_cKinectabyFrame;

/* Struct for Frame data */

typedef struct {
	short frame[480][640];
} kinectaby_frame;


/*
 * Kinectaby Context
 */
static VALUE rb_freenect_context_allocate(VALUE klass)
{
	freenect_context *context = NULL;
	return Data_Wrap_Struct(klass, NULL, NULL, context);
}

static VALUE rb_freenect_context_init(VALUE self)
{
	freenect_context *context;
	int error;

	error = freenect_init(&context, NULL);
	if (error < 0)
		rb_raise(rb_eRuntimeError, "freenect init failed");

	DATA_PTR(self) = context;
	return Qnil;
}

static VALUE rb_freenect_context_num_devices(VALUE self)
{
	freenect_context *context;
	int devices;

	Data_Get_Struct(self, freenect_context, context);

	return INT2FIX(freenect_num_devices(context));
}

static VALUE rb_freenect_context_open_device(VALUE self, VALUE device_num)
{
	freenect_context *context;
	freenect_device  *device;
	int error;

	Data_Get_Struct(self, freenect_context, context);

	error = freenect_open_device(context, &device, FIX2INT(device_num));
	if (error < 0)
		rb_raise(rb_eRuntimeError, "freenect device open failed");

	return Data_Wrap_Struct(rb_cKinectabyDevice, NULL, NULL, device);
}

static VALUE rb_freenect_context_shutdown(VALUE self)
{
	freenect_context *context;
	int error;

	Data_Get_Struct(self, freenect_context, context);

	error = freenect_shutdown(context);
	if (error < 0)
		rb_raise(rb_eRuntimeError, "freenect shutdown failed");

	return Qtrue;
}

/*
 * Kinectaby Device
 */
static VALUE rb_freenect_device_close(VALUE self)
{
	freenect_device *device;
	int error;

	Data_Get_Struct(self, freenect_device, device);

	error = freenect_close_device(device);
	if (error < 0)
		rb_raise(rb_eRuntimeError, "freenect device close failed");

	return Qtrue;
}

static VALUE rb_freenect_device_set_led(VALUE self, VALUE led)
{
	freenect_device *device;
	Data_Get_Struct(self, freenect_device, device);

	return INT2FIX(freenect_set_led(device, FIX2INT(led)));
}

static VALUE rb_freenect_device_set_tilt_degrees(VALUE self, VALUE tilt)
{
	freenect_device *device;
	Data_Get_Struct(self, freenect_device, device);

	return INT2FIX(freenect_set_tilt_degs(device, FIX2INT(tilt)));
}

static VALUE rb_freenect_sync_get_video(VALUE self, VALUE frame)
{
	int *rgb = 0;
	uint32_t ts;
	int error;

	error = freenect_sync_get_video((void**)&rgb, &ts, 0, FREENECT_DEPTH_11BIT);
	if(error > 0)
		rb_raise(rb_eRuntimeError, "freenect video capture failed");

	kinectaby_frame *kframe;
	Data_Get_Struct(frame, kinectaby_frame, kframe);

	int i,j;
	for (i = 0; i < 480; i++) {
		for (j = 0; j < 640; j++) {
			kframe->frame[i][j] = rgb[j*640+i];
		}
	}
	return Qtrue;
}

static VALUE rb_freenect_sync_get_depth(VALUE self, VALUE frame)
{
	short *depth = 0;
	uint32_t ts;
	int error;

	error = freenect_sync_get_depth((void**)&depth, &ts, 0, FREENECT_DEPTH_11BIT);
	if(error > 0)
		rb_raise(rb_eRuntimeError, "freenect depth capture failed");

	kinectaby_frame *kframe;
	Data_Get_Struct(frame, kinectaby_frame, kframe);

	int i,j;
	for (i = 0; i < 480; i++) {
		for (j = 0; j < 640; j++) {
			kframe->frame[i][j] = depth[j*640+i];
		}
	}
	return Qtrue;
}

/*
 * Kinectaby Frame (640x480)
 */
static VALUE rb_kinectaby_frame_allocate(VALUE klass)
{
	kinectaby_frame *frame = NULL;

	frame = malloc(sizeof(kinectaby_frame));
	if (frame == NULL)
		rb_raise(rb_eNoMemError, "out of memory");

	return Data_Wrap_Struct(klass, NULL, NULL, frame);
}

static VALUE rb_kinectaby_frame_init(VALUE self)
{
	kinectaby_frame *frame;
	Data_Get_Struct(self, kinectaby_frame, frame);

	int i,j;
	for (i = 0; i < 480; i++) {
		for (j = 0; j < 640; j++) {
			frame->frame[i][j] = 0;
		}
	}

	return Qnil;
}

static VALUE rb_kinectaby_frame_point(VALUE self, VALUE x, VALUE y)
{
	short point;
	kinectaby_frame *kframe;
	Data_Get_Struct(self, kinectaby_frame, kframe);
	// TODO: assert that y <= 480
	//       and x <= 640
	point = kframe->frame[FIX2INT(y)][FIX2INT(x)];
	return INT2FIX(point);
}


void Init_kinectaby()
{
	rb_mKinectaby = rb_define_module("Kinectaby");

	rb_define_module_function(rb_mKinectaby, "get_video", rb_freenect_sync_get_video, 1);
	rb_define_module_function(rb_mKinectaby, "get_depth", rb_freenect_sync_get_depth, 1);
	/*
	 * Context
	 */
	rb_cKinectabyContext = rb_define_class_under(rb_mKinectaby, "Context", rb_cObject);
	rb_define_alloc_func(rb_cKinectabyContext, rb_freenect_context_allocate);
	rb_define_method(rb_cKinectabyContext, "initialize", rb_freenect_context_init, 0);
	rb_define_method(rb_cKinectabyContext, "num_devices", rb_freenect_context_num_devices, 0);
	rb_define_method(rb_cKinectabyContext, "open_device", rb_freenect_context_open_device, 1);
	rb_define_method(rb_cKinectabyContext, "shutdown", rb_freenect_context_shutdown, 0);
  /* TODO 
	 * - set_log_level
	 * - set_log_callback
	 * - process_events
	 * */


	/*
	 * Device
	 */
	rb_cKinectabyDevice = rb_define_class_under(rb_mKinectaby, "Device", rb_cObject);
	rb_define_method(rb_cKinectabyDevice, "set_led", rb_freenect_device_set_led, 1);
	rb_define_method(rb_cKinectabyDevice, "set_tilt_degrees", rb_freenect_device_set_tilt_degrees, 1);
	rb_define_method(rb_cKinectabyDevice, "close", rb_freenect_device_close, 0);

  /* TODO 
	 * - set_user (?)
	 * - get_user (?)
	 *
	 * - set_depth_callback
	 * - set_depth_format
	 * - set_depth_buffer
	 * - start_depth
	 * - stop_depth
	 *
	 * - set_video_callback
	 * - set_video_format
	 * - set_video_buffer
	 * - start_video
	 * - stop_video
	 *
	 * - update_tilt_state
	 * - get_tilt_state
	 * - get_mks_accel
	 * */

	rb_cKinectabyFrame = rb_define_class_under(rb_mKinectaby, "Frame", rb_cObject);
	rb_define_alloc_func(rb_cKinectabyFrame, rb_kinectaby_frame_allocate);
	rb_define_method(rb_cKinectabyFrame, "initialize", rb_kinectaby_frame_init, 0);
	rb_define_method(rb_cKinectabyFrame, "point", rb_kinectaby_frame_point, 2);

	/* Constants */
	rb_define_const(rb_mKinectaby, "LED_OFF", INT2FIX(LED_OFF));
	rb_define_const(rb_mKinectaby, "LED_GREEN", INT2FIX(LED_GREEN));
	rb_define_const(rb_mKinectaby, "LED_RED", INT2FIX(LED_RED));
	rb_define_const(rb_mKinectaby, "LED_YELLOW", INT2FIX(LED_YELLOW));
	rb_define_const(rb_mKinectaby, "LED_BLINK_YELLOW", INT2FIX(LED_BLINK_YELLOW));
	rb_define_const(rb_mKinectaby, "LED_BLINK_GREEN", INT2FIX(LED_BLINK_GREEN));
	rb_define_const(rb_mKinectaby, "LED_BLINK_RED_YELLOW", INT2FIX(LED_BLINK_RED_YELLOW));

	/* Log Levels */
	rb_define_const(rb_mKinectaby, "LOG_FATAL",   INT2FIX(FREENECT_LOG_FATAL));
	rb_define_const(rb_mKinectaby, "LOG_ERROR",   INT2FIX(FREENECT_LOG_ERROR));
	rb_define_const(rb_mKinectaby, "LOG_WARNING", INT2FIX(FREENECT_LOG_WARNING));
	rb_define_const(rb_mKinectaby, "LOG_NOTICE",  INT2FIX(FREENECT_LOG_NOTICE));
	rb_define_const(rb_mKinectaby, "LOG_INFO",    INT2FIX(FREENECT_LOG_INFO));
	rb_define_const(rb_mKinectaby, "LOG_DEBUG",   INT2FIX(FREENECT_LOG_DEBUG));
	rb_define_const(rb_mKinectaby, "LOG_SPEW",    INT2FIX(FREENECT_LOG_SPEW));
	rb_define_const(rb_mKinectaby, "LOG_FLOOD",   INT2FIX(FREENECT_LOG_FLOOD));

	/* Depth Data Types */
	rb_define_const(rb_mKinectaby, "DEPTH_11BIT",   INT2FIX(FREENECT_DEPTH_11BIT));
	rb_define_const(rb_mKinectaby, "DEPTH_10BIT",   INT2FIX(FREENECT_DEPTH_10BIT));
	rb_define_const(rb_mKinectaby, "DEPTH_11BIT_PACKED",   INT2FIX(FREENECT_DEPTH_11BIT_PACKED));
	rb_define_const(rb_mKinectaby, "DEPTH_10BIT_PACKED",   INT2FIX(FREENECT_DEPTH_10BIT_PACKED));

	/* Video Data Types */
	rb_define_const(rb_mKinectaby, "VIDEO_RGB",   INT2FIX(FREENECT_VIDEO_RGB));
	rb_define_const(rb_mKinectaby, "VIDEO_BAYER",   INT2FIX(FREENECT_VIDEO_BAYER));
	rb_define_const(rb_mKinectaby, "VIDEO_IR_8BIT",   INT2FIX(FREENECT_VIDEO_IR_8BIT));
	rb_define_const(rb_mKinectaby, "VIDEO_IR_10BIT",   INT2FIX(FREENECT_VIDEO_IR_10BIT));
	rb_define_const(rb_mKinectaby, "VIDEO_IR_10BIT_PACKED",   INT2FIX(FREENECT_VIDEO_IR_10BIT_PACKED));
	rb_define_const(rb_mKinectaby, "VIDEO_IR_YUV_RGB",   INT2FIX(FREENECT_VIDEO_YUV_RGB));
	rb_define_const(rb_mKinectaby, "VIDEO_IR_YUV_RAW",   INT2FIX(FREENECT_VIDEO_YUV_RAW));

	rb_define_const(rb_mKinectaby, "TILT_STOPPED", INT2FIX(TILT_STATUS_STOPPED));
	rb_define_const(rb_mKinectaby, "TILT_LIMIT",   INT2FIX(TILT_STATUS_LIMIT));
	rb_define_const(rb_mKinectaby, "TILT_MOVING",  INT2FIX(TILT_STATUS_MOVING));

}

