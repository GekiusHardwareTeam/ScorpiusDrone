/*
 * multiplexser.h
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_HEADERS_MULTIPLEXSER_H_
#define SRC_HEADERS_MULTIPLEXSER_H_

#include "pin.h"

#define MUX_COUNT 31

#ifdef __cplusplus
extern "C"{
#endif

typedef enum mux_t{
	MUX_NONE=0,
	MUX_GPIO=1,
	MUX_PWM=2,

	MUX_I2C_SCL=4,
	MUX_I2C_SDA=8,

	MUX_SPI_MOSI=0x10,
	MUX_SPI_MISO=0x20,
	MUX_SPI_SCL=0x40,
	MUX_SPI_SEL0=0x80,
}mux_t;

/*
 * multiplexer map, represent input-output relationship
 */
typedef struct mux_map_item{
	int mode;			//	multiplexer input, mode id
	int id;				//	the GPIO id, bus id or PWM id etc.
	mux_t output_type;
}mux_map_item;

typedef struct multiplexer{
	int pin_id;
	int num_item;		//	total multiplex channel
	mux_map_item *_map;
}multiplexer;

#ifdef __cplusplus
}
#endif


#endif /* SRC_HEADERS_MULTIPLEXSER_H_ */
