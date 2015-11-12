/*
 * bus.c
 *
 *  Created on: 2015Äê10ÔÂ19ÈÕ
 *      Author: Jhon_Qiki
 */
#include "include/bus.h"

static pBus buses[BINDED_BUS_COUNT];

int get_i2c_inner_id(int busid)
{
	int actureid = -1;

	switch(busid)
	{
	case 6:
		actureid = BUS_ID_I2C_6;
		break;
		/* not implement yet
	case 2:
		actureid = BUS_ID_I2C_2;
		break;
		*/
	}

	return actureid;
}

pBus bus_is_init(int bus_id)
{
	int innerid = -1;
	if ( (innerid=get_i2c_inner_id(bus_id)) <0 )
		return 0;//invalid bus id

	return buses[innerid];
}
