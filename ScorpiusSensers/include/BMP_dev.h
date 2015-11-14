/*
 * BMP_dev.h
 *
 *  Created on: 2015Äê11ÔÂ13ÈÕ
 *      Author: Jhon_Qiki
 */

#ifndef BMP280_DRIVER_BMP_DEV_H_
#define BMP280_DRIVER_BMP_DEV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
 * standby duration in normal mode
 */
typedef enum NORMAL_MODE_SB{
	SB_P5 = 0,	//0.5ms
	SB_62P5 = 1, //62.5ms
	SB_125 = 2, //125ms
	SB_250 = 3, //250ms
	SB_500 = 4, //500ms
	SB_1000 = 5, //1000ms
	SB_2000 = 6,	//2000ms
	SB_4000 = 7,	//4000ms
}NORMAL_MODE_SB;


typedef enum IIR_FILTER{
	IIR_FILTER_OFF = 0,	// -x0 full ODR
	IIR_FILTER_X2 = 1,	//	-x2 0.223 ODR
	IIR_FILTER_X4 = 2, //	-x4 0.092 ODR
	IIR_FILTER_X8 = 3, //	-x8 0.042 ODR
	IIR_FILTER_X16 = 4, //	-x16 0.021 ODR
}IIR_FILTER;

/*
 * pressure oversampling measurement
 */
typedef enum OSRS_P{
	OSRS_P_SKIPPED=0,	//	-x0
	OSRS_P_ULTRA_LP=1,	//	-x1 ultra low power with 16bit/2.62pa resolution
	OSRS_P_LP=2,		//	-x2 low power with 17bit/1.31pa
	OSRS_P_STD=3,		//	-x4 standard solution with 18bit/0.66pa
	OSRS_P_HI_RESOLUTION=4,	//  -x8 high resolution with 19bit/0.33pa
	OSRS_P_ULTRA_HI_RESOLUTION=5,	// -x16 ultra high resolution with 20bit/0.16pa resolution
}OSRS_P;

/*
 * temperature oversampling
 */
typedef enum OSRS_T{
	OSRS_T_SKIPPED=0,	//	-x0
	OSRS_T_ULTRA_LOW=1,	//	-x1 16bit/0.0050 C
	OSRS_T_LOW=2,		//	-x2 17bit/0.0025 C
	OSRS_T_STD=3,		//	-x4 18bit/0.0012 C
	OSRS_T_HIGH=4,		//	-x8 19bit/0.0006 C
	OSRS_T_ULTRA_HIGH=5,	//	-x16 20bit/0.0003 C
}OSRS_T;

/*
 * BMP device mode
 */
typedef enum BMP_MODE{
	BMP_MODE_SLEEP=0,
	BMP_MODE_FORCED=1,
	BMP_MODE_NORMAL=3,
}BMP_MODE;

/*
 * control register configuration
 */
typedef struct BMP_CTL_CONFIGURATION{
	OSRS_T osrs_t;
	OSRS_P osrs_p;
	BMP_MODE mode;
}BMP_CTL_CONFIGURATION;

typedef struct BMP_CFG_CONFIGURATION{
	NORMAL_MODE_SB t_sb;
	IIR_FILTER filter;
	uint8_t spi3w;
}BMP_CFG_CONFIGURATION;
#ifdef BMP280
/*
 * registers in device
 */
#define WRITE_BIT 0x7F
#define CALIB_NUM 26
#define REG_CALIB0 0x88
#define REG_DEV_ID 0xD0
#define REG_RESET 0xE0
#define REG_STATUS 0xF3
#define REG_CTL_MEAS 0xF4
#define REG_CONFIG 0xF5
#define REG_PRESS_MSB 0xF7
#define REG_PRESS_LSB 0xF8
#define REG_PRESS_XLSB 0xF9
#define REG_TEMP_MSB 0xFA
#define REG_TEMP_LSB 0xFB
#define REG_TEMP_XLSB 0xFC
#endif

#ifdef __cplusplus
}
#endif

#endif /* BMP280_DRIVER_BMP_DEV_H_ */
