/*
 * brd_inteledison.c
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */

#include "include/i2c.h"
#include "include/multiplexer.h"
#include "include/pin.h"
//static multiplexer mux[MUX_COUNT];

FUNC_RESULT bind_pins()
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;

	//	i2c_6
	pins[45].category=PIN_TYPE_GPIO|PIN_TYPE_I2C;	//	i2c_6_scl
	pins[47].category=PIN_TYPE_GPIO|PIN_TYPE_I2C;	//	i2c_6_sda
	return res;
}


/* statically initialize all buses one time, not implement yet */
FUNC_RESULT bind_buses()
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;


	return res;
}


FUNC_RESULT board_init()
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;

	if( (res=bind_pins())!= FUNC_RESULT_SUCCESS )
		return res;

	if( (res=bind_buses())!=FUNC_RESULT_SUCCESS)
		return res;

	return res;
}
