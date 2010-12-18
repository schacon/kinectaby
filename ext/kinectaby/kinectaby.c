#include <ruby.h>
#include <assert.h>

#include "libfreenect.h"

static VALUE rb_mKinectaby;

static VALUE rb_cKinectabyContext;
static VALUE rb_cKinectabyDevice;

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


void Init_kinectaby()
{
	rb_mKinectaby = rb_define_module("Kinectaby");

	/*
	 * Context
	 */
	rb_cKinectabyContext = rb_define_class_under(rb_mKinectaby, "Context", rb_cObject);
	rb_define_alloc_func(rb_cKinectabyContext, rb_freenect_context_allocate);
	rb_define_method(rb_cKinectabyContext, "initialize", rb_freenect_context_init, 0);
	rb_define_method(rb_cKinectabyContext, "num_devices", rb_freenect_context_num_devices, 0);
	rb_define_method(rb_cKinectabyContext, "open_device", rb_freenect_context_open_device, 1);
	rb_define_method(rb_cKinectabyContext, "shutdown", rb_freenect_context_shutdown, 0);

	/*
	 * Device
	 */
	rb_cKinectabyDevice = rb_define_class_under(rb_mKinectaby, "Device", rb_cObject);
	rb_define_method(rb_cKinectabyDevice, "close", rb_freenect_device_close, 0);

	/* Constants */
	rb_define_const(rb_mKinectaby, "LED_OFF", INT2FIX(LED_OFF));
	rb_define_const(rb_mKinectaby, "LED_GREEN", INT2FIX(LED_GREEN));
	rb_define_const(rb_mKinectaby, "LED_RED", INT2FIX(LED_RED));
	rb_define_const(rb_mKinectaby, "LED_YELLOW", INT2FIX(LED_YELLOW));
	rb_define_const(rb_mKinectaby, "LED_BLINK_YELLOW", INT2FIX(LED_BLINK_YELLOW));
	rb_define_const(rb_mKinectaby, "LED_BLINK_GREEN", INT2FIX(LED_BLINK_GREEN));
	rb_define_const(rb_mKinectaby, "LED_BLINK_RED_YELLOW", INT2FIX(LED_BLINK_RED_YELLOW));
}

