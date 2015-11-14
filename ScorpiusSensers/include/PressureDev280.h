/*
 * PressureDev280.h
 *
 *  Created on: 2015Äê11ÔÂ14ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef PRESSUREDEV280_H_
#define PRESSUREDEV280_H_

#include "pressure_dev_common.h"
#include "BMP_dev.h"

class PressureDev280 {
public:
	PressureDev280();
	virtual ~PressureDev280();

	//	configure pressure sensor and start sampling
	FUNC_RESULT start_dev(PRESSURE_APP_SOLUTION solution, BMP_MODE mode, uint8_t spi3w);

	/*
	 * MUST call start_dev() first
	 */
	FUNC_RESULT fetch_data(double *pressure, double *temperature);

private:
	BMP_CTL_CONFIGURATION cal_ctl_byte(PRESSURE_APP_SOLUTION solution, BMP_MODE mode);
	BMP_CFG_CONFIGURATION cal_cfg_byte(PRESSURE_APP_SOLUTION solution, uint8_t spi3w);
};

#endif /* PRESSUREDEV280_H_ */
