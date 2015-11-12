/*
 * pin.c
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: Jhon_Qiki
 */
#include "include/pin.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdio.h>

pin pins[PIN_COUNT];

FUNC_RESULT ChangePinMode(int sysfs, int mode)
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;

	if (mode<0)
		return res;

	char pinmuxpath[MAX_SIZE];
	snprintf(pinmuxpath, MAX_SIZE, SYSFS_PINMODE_PATH "%i/current_pinmux", sysfs);
	int fd = open(pinmuxpath, O_WRONLY);
	if(fd<0)
	{
		syslog(LOG_ERR, "scorpius:could not open pin multiplex file");
		return FUNC_RESULT_FILE_OPEN_FAILED;
	}

	char pinmode[MAX_MODE_SIZE];
	int length = sprintf(pinmode, "mode%u", mode);
	if ( write(fd, pinmode, length*sizeof(char))==-1)
	{
		syslog(LOG_ERR, "scorpius:failed writing pin mode value");
		res = FUNC_RESULT_FILE_WRITE_FAILED;
	}

	close(fd);

	return res;
}



