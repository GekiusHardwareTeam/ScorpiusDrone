/*
 * pressure_dev_common.h
 *
 *  Created on: 2015Äê11ÔÂ14ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef INCLUDE_PRESSURE_DEV_COMMON_H_
#define INCLUDE_PRESSURE_DEV_COMMON_H_

#include "type_def.h"

typedef enum PRESSURE_APP_SOLUTION{
	HANDHELD_DEV_LP = 1, // handheld device low-power solution(ultra high resolution)
	HANDHELD_DEV_DYN = 2, // handheld device dynamic solution(standard resolution)
	WEATHER_MONITORING = 3, // weather monitoring( ultra low power )
	FLOOR_DETECTION = 4, // floor detection (standard resolution )
	DROP_DETECTION = 5, // drop detection ( low power )
	INDOOR_NAVIGATION = 6, // indoor navigation ( ultra high resolution )
}PRESSURE_APP_SOLUTION;



#endif /* INCLUDE_PRESSURE_DEV_COMMON_H_ */
