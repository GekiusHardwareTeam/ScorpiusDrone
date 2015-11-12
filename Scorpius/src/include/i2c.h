/*
 * i2c.h
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_INCLUDE_I2C_H_
#define SRC_INCLUDE_I2C_H_

#include "common.h"
#include "bus.h"
#include <stdint.h>

#define I2C_FUNCS 0x0705
#define I2C_DEV_PATH "/dev/i2c-"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct i2c_context{
	unsigned short _addr;		//	i2c slave address 7 or 10 bits
	unsigned long _funcs;		//	device functions
	int fd;			//	device file descriptor

	pBus _i2c_bus;	//	i2c bus information
}i2c_context;

typedef enum edison_i2c{
	EDISON_I2C_1=1,
	EDISON_I2C_2=2,
	EDISON_I2C_3=3,
	EDISON_I2C_4=4,
	EDISON_I2C_5=5,
	EDISON_I2C_6=6,
	EDISON_I2C_7=7,
	EDISON_I2C_8=8,
}edison_i2c;

typedef enum i2c_speed_mode{
	I2C_STD=1, //up to 100kHz
	I2C_FAST=2,//up to 400kHz
	I2C_HIGH=3,//up to 2MHz
}i2c_speed_mode;

typedef i2c_context *i2c_context_t;

/*
 * system bus initialization
 *
 * @bus_num	bus number, like i2c-6, the 6 is the number
 * @return	pointer of Bus instance or NULL
 */
i2c_context_t i2c_init(edison_i2c bus_id);

/*
 * send command out following reading data from slave
 *
 * @dev		i2c device
 * @command	the command will be sent
 * @data	a buffer hold all data come from slave
 * @length	max number of bytes to read
 * @return	0 if successful or -1 if failed
 */
int i2c_read_bytes(i2c_context_t dev, uint8_t command, uint8_t* data, int length);

/*
 * read byte data
 *
 * @dev		i2c device
 * @command	a byte will be send out frist as device controlling command
 * @return 	byte data received from slave or 0 if failed
 */
uint8_t i2c_read_byte_data(i2c_context_t dev, uint8_t command);

/*
 * setting i2c frequency
 *
 * @dev 	i2c device
 * @mode	i2c working mode
 * @return
 */
FUNC_RESULT i2c_set_frequency(i2c_context_t dev, i2c_speed_mode mode);

/*
 * set slave address
 *
 * @dev		i2c master in edison
 * @addr	slave address 7 or 10 bits
 * @return
 */
FUNC_RESULT i2c_set_address(i2c_context_t dev, uint8_t addr);

/*
 * @dev i2c bus
 * @data	the first byte data[0] indicate command followed by real data
 * @length	data size plus one byte command
 * @return 	error code
 */
FUNC_RESULT i2c_write_bytes(i2c_context_t dev, uint8_t command, const uint8_t* data, int length);

#ifdef __cplusplus
}
#endif

#endif /* SRC_INCLUDE_I2C_H_ */
