/*
 * bus.h
 *
 *  Created on: 2015Äê10ÔÂ16ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_BUS_H_
#define SRC_BUS_H_

#define BINDED_BUS_COUNT 1
// hard coded bus id
#define BUS_ID_I2C_6 0
#define BUS_ID_I2C_2 1

#ifdef __cplusplus
extern "C" {
#endif

/* system bus type*/
typedef enum bus_t{
	BUS_INVALID=0,
	BUS_I2C=1,
	BUS_SPI=2,
}bus_t;

typedef enum bus_pin_t{
	BUS_PIN_INVALID=0,

	BUS_PIN_I2C_SCL=1,
	BUS_PIN_I2C_SDA=2,

	BUS_PIN_GPIO=3,

	BUS_PIN_SPI_MOSI=4,
	BUS_PIN_SPI_MISO=5,
	BUS_PIN_SPI_SCL=6,
	BUS_PIN_SPI_CS=7,
}bus_pin_t;


typedef struct bus_pin{
	int mux_id;		// the sysfs id using by multiplexer, negative value means none-multiplex
	int mode_value;	// to enable the pin for bus, MUST set this value to multiplexer, negative for illegal value

	int _fd;		//	file descriptor

	bus_pin_t pin_type;
}bus_pin,*p_bus_pin;

/*hardware bus information*/
typedef struct bus
{
	bus_t bus_type;
	int _id;		//	bus id

	int pins_count;	// 	number of pins in bus
	bus_pin **_pins;	//	pins in the bus(type:bus_pin pointer array)
}bus;

typedef bus* pBus;
//extern static pBus buses[BINDED_BUS_COUNT];

//	function declaration
pBus bus_is_init(int bus_id);

#ifdef __cplusplus
}
#endif


#endif /* SRC_BUS_H_ */
