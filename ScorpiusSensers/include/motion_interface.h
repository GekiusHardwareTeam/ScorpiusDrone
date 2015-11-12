/*
 * license:
 * writer:	Jhon Qiki
 * date:	2015/10/30
 *
 * motion device is on i2c1, these functions provide hardware
 * interfaces to communicate with it
 */
#pragma once

#include "gpio.h"
#include "i2c.h"

#define MPU9250

/*
 * write buffer data to motion device
 *
 * @slave_addr	motion device address, no used at all
 * @reg_addr	command or register address
 * @length		the number of bytes will be written to
 * @data
 * @return 		the value compatible with FUNC_RESULT
 */
int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);

/*
 * @slave_addr	motion device address, no used at all
 * @reg_addr	command or register address
 * @length		the number of bytes of data we desired
 * @data		the buffer storage data
 * @return 		the actual read length
 */
int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);

/*
 * register callback function for motion device
 *
 * @fn callback function pointer
 * @arg	the argument pass to callback
 * @return 0 if successful or nonezero for failure
 */
int motion_dev_reg_int_cb( fn_ir fn, void *arg );
