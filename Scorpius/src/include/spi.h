/*
 * spi.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_INCLUDE_SPI_H_
#define SRC_INCLUDE_SPI_H_

#include "bus.h"
#include "gpio.h"
#include "common.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum spi_mode
{
	SPI_MODE_0 = 0b00,	//CPOL:0 CPHA:0
	SPI_MODE_1 = 0b01,	//CPOL:0 CPHA:1
	SPI_MODE_2 = 0b10,	//CPOL:1 CPHA:0
	SPI_MODE_3 = 0b11,	//CPOL:1 CPHA:1
}spi_mode;

typedef struct spi_context
{
	int fd;				//	file descriptor to spi device
	int speed;			//	in Hz
	spi_mode mode;		//	spi mode
	unsigned int lsb;	//  least significant bit mode
	unsigned int bpw;	//	bits per word
	unsigned int cs_id;	//	chip select pin id in bus
	pBus spi_bus;		//	spi bus definition
}spi_context, *spi_context_p;

/*
 * spi device initialization
 *
 * @return valid spi device context instance or 0
 */
spi_context_p spi_init();

/*
 * set spi mode
 *
 * @mode
 * @return
 */
FUNC_RESULT set_spi_mode(spi_context_p dev, spi_mode mode);

/*
 * set spi device`s speed
 *
 * @dev	spi device
 * @speedinhz
 * @return
 */
FUNC_RESULT set_spi_speed(spi_context_p dev, int speedinhz);

/*
 * set least significant mode
 *
 * @dev spi device
 * @lsbmode the new mode
 * @return
 */
FUNC_RESULT set_spi_lsbmode(spi_context_p dev, unsigned int lsbmode );

/*
 * configure spi device word size in bits
 *
 * @dev spi device
 * @bits	word size in bits
 * @return
 */
FUNC_RESULT spi_per_word(spi_context_p dev, unsigned int bits);

/*
 * transfer buffer data to slave
 *
 * @dev	edison spi controller
 * @data the data which will be send to slave
 * @rvdata received data from slave
 * @length	sending data size
 * @return
 */
FUNC_RESULT spi_transfer_buffer(spi_context_p dev, uint8_t* data, uint8_t* rvdata, int length);

/*
 * transfer buffer data and return received data,
 * caller has responsibility to release resources of received buffer
 *
 * @dev	spi device
 * @data the data will be sent to slave
 * @length size of data
 * @return the received data from slave, user need to free it
 */
uint8_t* spi_write_buffer(spi_context_p dev, uint8_t* data, int length);

/*
 * because edison doesn`t support hardware cs0
 *
 * @dev spi device
 * @return the gpio context of cs0
 */
gpio_context_p get_cs0_gpio_context(spi_context_p dev);

/*
 * lower cs0
 *
 * @return
 */
FUNC_RESULT cs0_low(spi_context_p dev);

/*
 * higher cs0
 */
FUNC_RESULT cs0_high(spi_context_p dev);

#ifdef __cplusplus
}
#endif

#endif /* SRC_INCLUDE_SPI_H_ */
