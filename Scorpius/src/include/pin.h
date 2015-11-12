/*
 * pin.h
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_INCLUDE_PIN_H_
#define SRC_INCLUDE_PIN_H_

#include "common.h"

#define SYSFS_PINMODE_PATH "/sys/kernel/debug/gpio_debug/gpio"
#define PIN_COUNT 70

#ifdef __cplusplus
extern "C"{
#endif

typedef enum pin_t{
	PIN_TYPE_NC=0,
	PIN_TYPE_ELECTRONIC=1,
	PIN_TYPE_GPIO=2,
	PIN_TYPE_PWN=4,
	PIN_TYPE_I2C=8,
	PIN_TYPE_SPI=0x10,
	PIN_TYPE_USB=0x20,
	PIN_TYPE_UART=0x40,
	PIN_TYPE_SD=0x80,
	PIN_TYPE_I2S=0x100,
}pin_t;

typedef struct pin{
	pin_t category;
	int mux_id;		//	negative value indicate NO multiplexer
}pin;

extern pin pins[PIN_COUNT];

/*
 * setting pin mode, change inner multiplexer value
 *
 * @sysfs sysfs pin id
 * @mode the mode you expect
 * @return successful: FUNC_RESULT_SUCCESS
 */
FUNC_RESULT ChangePinMode(int sysfs, int mode);


#ifdef __cplusplus
}
#endif

#endif /* SRC_INCLUDE_PIN_H_ */
