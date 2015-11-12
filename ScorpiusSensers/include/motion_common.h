#ifndef _H_MOTION_COMMON_H_
#define _H_MOTION_COMMON_H_

#ifdef __cplusplus
extern "C"{
#endif

//reference motion driver, same with sensor control bits
#define MOTION_GYRO_X (0x40)
#define MOTION_GYRO_Y (0x20)
#define MOTION_GYRO_Z (0x10)
#define MOTION_GYRO_XYZ (MOTION_GYRO_X | MOTION_GYRO_Y | MOTION_GYRO_Z )
#define MOTION_ACCEL_XYZ (0x08)
#define MOTION_COMPASS_XYZ (0x01)

#define DATA_BUF_SIZE 32

typedef void (*gesture_cb)(unsigned char direction, unsigned char count);
typedef void (*orientation_cb)(unsigned char orientation);

typedef enum MOTION_SENSOR{
	GYRO_X = MOTION_GYRO_X,
	GYRO_Y = MOTION_GYRO_Y,
	GYRO_Z = MOTION_GYRO_Z,
	GYRO_XYZ = MOTION_GYRO_XYZ,
	ACCEL_XYZ = MOTION_ACCEL_XYZ,
	COMPASS_XYZ = MOTION_COMPASS_XYZ
}MOTION_SENSOR;

typedef struct MotionData{
	short gyro[DATA_BUF_SIZE];
	short accel[DATA_BUF_SIZE];
	short compass[DATA_BUF_SIZE];
	long quat[4];
	short data_type;	//	which data has fetched
	unsigned long GAtimestamp;	//6-axis data timestamp
	unsigned long compasstimestamp;	//compass data timestamp
}MotionData;

#ifdef __cplusplus
}
#endif
#endif //_H_MOTION_COMMON_H_
