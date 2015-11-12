/*
 * gpio.c
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Jhon_Qiki
 */
#include "include/gpio.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>

#define SYSFS_CLASS_GPIO "/sys/class/gpio"
//static sigset_t sigset;

FUNC_RESULT gpio_create_value_fd(gpio_context_p gpio, int property)
{
	char buffer[MAX_PATH_SIZE] = {0};
	snprintf(buffer, MAX_PATH_SIZE, SYSFS_CLASS_GPIO "/gpio%u/value", gpio->id );
	if ( gpio->value_fd <1 )
	{
		if ( (gpio->value_fd=open(buffer, property)) < 1 )
			return FUNC_RESULT_FILE_OPEN_FAILED;
	}

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT gpio_release(gpio_context_p gpio)
{
	if ( gpio == 0 )
		return FUNC_RESULT_NULLPOINTER;

	if ( gpio->value_fd > 0 )
		close(gpio->value_fd);

	free(gpio);

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT gpio_active_low(gpio_context_p gpio, int value)
{
	char path[MAX_PATH_SIZE] = {0};
	int length = snprintf(path, MAX_PATH_SIZE, SYSFS_CLASS_GPIO "/gpio%u/active_low", gpio->id);

	int fd = open(path, O_RDWR);
	if ( fd < 1 )
		return FUNC_RESULT_FILE_OPEN_FAILED;
	char valuebuffer[MAX_SIZE] = {0};
	length = snprintf(valuebuffer, MAX_SIZE, "%d", value);
	if ( write(fd, valuebuffer, length) == -1 )
		return FUNC_RESULT_FILE_WRITE_FAILED;

	close(fd);

	return FUNC_RESULT_SUCCESS;
}

gpio_context_p gpio_init(unsigned int id)
{
	gpio_context_p gpio = (gpio_context_p)malloc(sizeof(gpio_context));
	if (gpio == 0)
	{
		return 0;
	}
	gpio->id = id;
	gpio->value_fd = -1;

	//	check if gpio is exported
	char path[MAX_PATH_SIZE] = {0};
	char gpioid[MAX_SIZE] = {0};
	snprintf( path, MAX_PATH_SIZE,  SYSFS_CLASS_GPIO "/gpio%u/", id );
	struct stat dirstate;
	if ( stat(path, &dirstate)==-1 || !S_ISDIR(dirstate.st_mode) )//&& errno==ENOENT )//maybe incorrect in concurrent environment
	{
		int exportfd = open(SYSFS_CLASS_GPIO "/export", O_WRONLY);
		if (exportfd < 1 )
		{
			free(gpio);
			return 0;
		}

		int length = snprintf(gpioid, MAX_SIZE, "%u", id);
		if ( write(exportfd, gpioid, length) < 0  )
		{
			close(exportfd);
			free(gpio);
			return 0;
		}

		close(exportfd);
	}

	if ( gpio_create_value_fd(gpio, O_RDWR) != FUNC_RESULT_SUCCESS )
	{
		gpio_release(gpio);
		return 0;
	}

	if ( gpio_active_low(gpio, 0) != FUNC_RESULT_SUCCESS )
	{
		gpio_release(gpio);
		return 0;
	}
	return gpio;
}

FUNC_RESULT gpio_write(gpio_context_p gpio, int value)
{
	if ( gpio==0 || gpio->value_fd<1 )
		return FUNC_RESULT_FAILED;

	if ( lseek(gpio->value_fd, 0, SEEK_SET) == -1 )
		return FUNC_RESULT_FAILED;

	char buffer[MAX_SIZE] = {0};
	int length = snprintf( buffer, MAX_SIZE, "%d", value);
	if ( write(gpio->value_fd, buffer, length) ==-1 )
		return FUNC_RESULT_FILE_WRITE_FAILED;

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT gpio_direction(gpio_context_p gpio, gpio_dir dir)
{
	if ( gpio == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	char buffer[MAX_PATH_SIZE] = {0};
	snprintf(buffer, MAX_PATH_SIZE, SYSFS_CLASS_GPIO "/gpio%u/direction", gpio->id);

	int fd = open(buffer, O_RDWR);
	//	failed to open direction file, try to set high or low if passed
	if ( fd == -1 )
	{
		switch(dir)
		{
		case GPIO_DIR_HIGH:
			return gpio_write(gpio, 1);
		case GPIO_DIR_LOW:
			return gpio_write(gpio, 0);
		default:
			return FUNC_RESULT_FAILED;
		}
	}

	char valuebuffer[MAX_SIZE] = {0};
	int length = 0;
	switch(dir)
	{
	case GPIO_DIR_IN:
		length = snprintf(valuebuffer, MAX_SIZE, "in");
		break;
	case GPIO_DIR_OUT:
		length = snprintf(valuebuffer, MAX_SIZE, "out");
		break;
	case GPIO_DIR_HIGH:
		length = snprintf(valuebuffer, MAX_SIZE, "high");
		break;
	case GPIO_DIR_LOW:
		length = snprintf(valuebuffer, MAX_SIZE, "low");
		break;
	default:
		close(fd);
		return FUNC_RESULT_INVALID_PARAMETER;
	}

	if ( write(fd, valuebuffer, length) < 0 )
	{
		close(fd);
		return FUNC_RESULT_FILE_WRITE_FAILED;
	}
	close(fd);

	return FUNC_RESULT_SUCCESS;
}


FUNC_RESULT gpio_ir_edge_mode(gpio_context_p gpio, gpio_interrupt_edge mode)
{
	if( gpio->value_fd != -1)
	{
		close(gpio->value_fd);
		gpio->value_fd = -1;
	}

	char path[MAX_PATH_SIZE] = {0};
	snprintf( path, MAX_PATH_SIZE, SYSFS_CLASS_GPIO "/gpio%u/edge", gpio->id);
	int fd = open(path, O_RDWR);
	if ( fd==-1 )
	{
		return FUNC_RESULT_FILE_OPEN_FAILED;
	}

	char c_mode[MAX_SIZE] = {0};
	int length = 0;
	switch( mode )
	{
	case GPIO_IR_EDGE_NONE:
		length = snprintf( c_mode, MAX_SIZE, "none");
		break;
	case GPIO_IR_EDGE_BOTH:
		length = snprintf( c_mode, MAX_SIZE, "both");
		break;
	case GPIO_IR_EDGE_RISING:
		length = snprintf( c_mode, MAX_SIZE, "rising");
		break;
	case GPIO_IR_EDGE_FALLING:
		length = snprintf( c_mode, MAX_SIZE, "falling");
		break;
	default:
		close(fd);
		return FUNC_RESULT_FAILED;
	}

	if ( write(fd, c_mode, length) == -1 )
	{
		close(fd);
		return FUNC_RESULT_FILE_WRITE_FAILED;
	}

	close(fd);
	return FUNC_RESULT_SUCCESS;
}

static FUNC_RESULT gpio_ir_wait(gpio_context_p dev)
{
	if ( dev <= 0 )
	{
		return FUNC_RESULT_NULLPOINTER;
	}

	struct pollfd pfd;
	pfd.fd = dev->value_fd;
	pfd.events = POLLPRI;
	//	clear interrupt
	unsigned char itr_value;
	lseek(pfd.fd, 0, SEEK_SET);
	read( pfd.fd, &itr_value, 1);
	poll( &pfd, 1, -1);//0, &sigset);

	return FUNC_RESULT_SUCCESS;
}

static void* gpio_ir_cb_distributer(void *arg)
{
	if ( arg <= 0 )
		return 0;

	gpio_context_p dev = (gpio_context_p)arg;

	if ( gpio_create_value_fd(dev, O_RDONLY) != FUNC_RESULT_SUCCESS )
	{
		return 0;
	}

	while(1)
	{
		if ( gpio_ir_wait(dev) == FUNC_RESULT_SUCCESS )
		{
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, 0);
			dev->ir_handler(dev->ir_args);
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
		}
		else
		{
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, 0);
			close(dev->value_fd);
			dev->value_fd = -1;
			return 0;
		}
	}
}

FUNC_RESULT gpio_register_ir_handler(gpio_context_p gpio, gpio_interrupt_edge mode, fn_ir fn, void* args)
{
	if (gpio==0 || fn ==0 )
		return FUNC_RESULT_NULLPOINTER;

	if ( gpio_ir_edge_mode(gpio, mode) != FUNC_RESULT_SUCCESS )
		return FUNC_RESULT_FAILED;

	gpio->ir_handler = fn;
	gpio->ir_args = args;
	pthread_create(&gpio->thread_id, 0, gpio_ir_cb_distributer, gpio );

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT gpio_unregister_ir_handler(gpio_context_p gpio)
{
	if( gpio == 0)
		return FUNC_RESULT_NULLPOINTER;

	/// no running handler
	if ( gpio->thread_id==0  )
		return FUNC_RESULT_SUCCESS;

	if ( gpio_ir_edge_mode(gpio, GPIO_IR_EDGE_NONE) != FUNC_RESULT_SUCCESS )
		return FUNC_RESULT_FAILED;

	//	stop thread
	if ( gpio->thread_id != 0 )
	{
		if ( (pthread_cancel(gpio->thread_id)!=0) || (pthread_join(gpio->thread_id, 0)!=0) )
		{
			return FUNC_RESULT_FAILED;
		}

		gpio->thread_id = 0;
	}

	//	close edge value reading
	if ( gpio->value_fd != -1 )
	{
		if ( close(gpio->value_fd) != 0 )
			return FUNC_RESULT_FAILED;
		gpio->value_fd = -1;

	}
	return FUNC_RESULT_SUCCESS;
}
