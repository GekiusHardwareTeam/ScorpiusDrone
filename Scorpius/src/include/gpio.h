/*
 * gpio.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Jhon_Qiki
 */
#include "common.h"
#include <pthread.h>


#ifndef SRC_INCLUDE_GPIO_H_
#define SRC_INCLUDE_GPIO_H_

#ifdef __cplusplus
extern "C"{
#endif

typedef enum gpio_dir{
	GPIO_DIR_IN = 0,
	GPIO_DIR_OUT = 1,
	GPIO_DIR_HIGH = 2,
	GPIO_DIR_LOW = 3,
}gpio_dir;

//	the interrupt edge type
typedef enum gpio_interrupt_edge{
	GPIO_IR_EDGE_NONE = 0,
	GPIO_IR_EDGE_BOTH = 1,	// falling and rising
	GPIO_IR_EDGE_RISING =2,// interrupt occur on rising edge
	GPIO_IR_EDGE_FALLING = 3,// interrupt occur on falling edge
}gpio_interrupt_edge;

typedef void (*fn_ir)(volatile void* itr_ret);
typedef struct gpio_context{
	unsigned int id;	// sysfs gpio id
	int value_fd;		//	file descriptor

	//	interrupt
	//int ir_fd;			//	interrupt file descriptor
	void *ir_args;	//	interrupt handler return value
	fn_ir ir_handler;	//	interrupt handler
	pthread_t thread_id;//	pthread id
}gpio_context,*gpio_context_p;

/*
 * initialize gpio
 *
 * @id	sysfs id
 * @return
 */
gpio_context_p gpio_init(unsigned int id);

/*
 * set gpio value
 *
 * @gpio
 * @value
 * @return
 */
FUNC_RESULT gpio_write(gpio_context_p gpio, int value);

/*
 *
 */
FUNC_RESULT gpio_release(gpio_context_p gpio);

/*
 * set gpio direction
 *
 * @gpio
 * @dir	desired direction
 * @return
 */
FUNC_RESULT gpio_direction(gpio_context_p gpio, gpio_dir dir);

/*
 * set gpio active low
 *
 * @gpio
 * @value	0 or 1
 * @return error code or FUNC_RESULT_SUCCESS for successful
 */
FUNC_RESULT gpio_active_low(gpio_context_p gpio, int value);

/*
 * set gpio interrupt edge mode
 *
 * @gpio
 * @mode
 * @return error code or FUNC_RESULT_SUCCESS
 */
FUNC_RESULT gpio_ir_edge_mode(gpio_context_p gpio, gpio_interrupt_edge mode);

/*
 * register interrupt handler
 *
 * @gpio
 * @mode	interrupt edge mode
 * @fn		interrupt handler function, can not be null
 * @args	arguments pass to handler
 * @return
 */
FUNC_RESULT gpio_register_ir_handler(gpio_context_p gpio, gpio_interrupt_edge mode, fn_ir fn, void* args);

/*
 * @gpio
 * @return
 */
FUNC_RESULT gpio_unregister_ir_handler(gpio_context_p gpio);
#ifdef __cplusplus
}
#endif
#endif /* SRC_INCLUDE_GPIO_H_ */
