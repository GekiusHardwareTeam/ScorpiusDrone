/*
 * brd_inteledison.h
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef SRC_INCLUDE_BRD_INTELEDISON_H_
#define SRC_INCLUDE_BRD_INTELEDISON_H_

#include "common.h"
#include "i2c.h"

#ifdef __cplusplus
extern "C"{
#endif

FUNC_RESULT board_init();

/*reflect system buses*/
FUNC_RESULT bind_buses();

/*reflect pins in system */
FUNC_RESULT bind_pins();

#ifdef __cplusplus
}
#endif


#endif /* SRC_INCLUDE_BRD_INTELEDISON_H_ */
