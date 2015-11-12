#pragma once
#include "motion_common.h"
#include <common.h>
#include <gpio.h>
#include "motion_device.h"

#define DEFAULT_RATE 100

class MPU9250:public MotionDevice{
public:
	MPU9250();
	~MPU9250();

	//	as min_taps occur an interrupt will generate by device when you enable gesture interrupt
	//	min_taps can be set from 1 to 4
	int set_tap_count(unsigned char min_taps);

	FUNC_RESULT register_gesture_cb(gesture_cb cb);
	FUNC_RESULT register_orientation_cb(orientation_cb cb);

	// enable user specified sensors
	FUNC_RESULT MPU9250_init(MOTION_SENSOR sensor, const fn_ir cb, void* arg, int sample_rate=DEFAULT_RATE);

	//	get gesture data in its callback function
	void fetchdata(MotionData* data);

	void get_interrupt_status(short* status);
	FUNC_RESULT set_int_level(unsigned char active_low);
	FUNC_RESULT set_interrupt_latched(unsigned char enable);

	/**
	 *  @brief      Enters LP accel motion interrupt mode.
	 *
	 *  \n The hardware motion threshold can be between 32mg and 8160mg in 32mg
	 *  increments.
	 *
	 *  \n Low-power accel mode supports the following frequencies:
	 *  \n 1.25Hz, 5Hz, 20Hz, 40Hz
	 *
	 *  \n MPU6500:
	 *  \n Unlike the MPU6050 version, the hardware does not "lock in" a reference
	 *  sample. The hardware monitors the accel data and detects any large change
	 *  over a short period of time.
	 *
	 *  \n The hardware motion threshold can be between 4mg and 1020mg in 4mg
	 *  increments.
	 *
	 *  \n MPU6500 Low-power accel mode supports the following frequencies:
	 *  \n 1.25Hz, 2.5Hz, 5Hz, 10Hz, 20Hz, 40Hz, 80Hz, 160Hz, 320Hz, 640Hz
	 *
	 *  \n\n NOTES:
	 *  \n The driver will round down @e thresh to the nearest supported value if
	 *  an unsupported threshold is selected.
	 *  \n To select a fractional wake-up frequency, round down the value passed to
	 *  @e lpa_freq.
	 *  \n The MPU6500 does not support a delay parameter. If this function is used
	 *  for the MPU6500, the value passed to @e time will be ignored.
	 *  \n To disable this mode, set @e lpa_freq to zero. The driver will restore
	 *  the previous configuration.
	 *
	 *  @param[in]  thresh      Motion threshold in mg.
	 *  @param[in]  time        Duration in milliseconds that the accel data must
	 *                          exceed @e thresh before motion is reported.
	 *  @param[in]  lpa_freq    Minimum sampling rate, or zero to disable.
	 *  @return     0 if successful.
	 */
	FUNC_RESULT lp_motion_interrupt(unsigned short thresh, unsigned char time, unsigned short lpa_freq);

	//	low-power accelerometer mode enable, device go into sleep and wake to sample accel at rate
	//	valid rate value from 1.25Hz to 640Hz
	FUNC_RESULT enter_lp_accel_mode(unsigned short rate);
};
