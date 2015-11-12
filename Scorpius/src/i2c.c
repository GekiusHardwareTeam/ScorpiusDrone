/*
 * i2c.c
 *
 *  Created on: 2015Äê10ÔÂ16ÈÕ
 *      Author: Jhon_Qiki
 */
#include "include/i2c.h"
#include "include/pin.h"
#include "include/common.h"
#include "include/linux/i2c-dev.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

// come from mraa
typedef union i2c_smbus_data_union {
    uint8_t byte;        ///< data byte
    unsigned short word; ///< data short word
    uint8_t block[I2C_SMBUS_BLOCK_MAX + 2];
    ///< block[0] is used for length and one more for PEC
} i2c_smbus_data_t;

typedef struct i2c_smbus_ioctl_data_struct {
    uint8_t read_write;     ///< operation direction
    uint8_t command;        ///< ioctl command
    int size;               ///< data size
    i2c_smbus_data_t* data; ///< data
} i2c_smbus_ioctl_data_t;

int i2c_smbus_access(int fh, uint8_t read_write, uint8_t command, int size, i2c_smbus_data_t* data)
{
    i2c_smbus_ioctl_data_t args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;

    return ioctl(fh, I2C_SMBUS, &args);
}

pBus create_i2c1()
{
	pBus i2c1 = (pBus)malloc(sizeof(bus));
	if( i2c1 == 0 )
		return 0;

	i2c1->_id = 1;
	i2c1->bus_type = BUS_I2C;
	i2c1->pins_count = 2;

	//	i2c1 sda
	p_bus_pin i2c1_sda = (p_bus_pin)malloc(sizeof(bus_pin));
	if(i2c1_sda == 0)
	{
		free(i2c1);
		return 0;
	}
	i2c1_sda->pin_type=BUS_PIN_I2C_SDA;
	i2c1_sda->mux_id = 20;
	i2c1_sda->mode_value = 1;

	//	i2c1 scl
	p_bus_pin i2c1_scl = (p_bus_pin)malloc(sizeof(bus_pin));
	if( i2c1_scl == 0 )
	{
		free(i2c1_sda);
		free(i2c1);
		return 0;
	}
	i2c1_scl->pin_type = BUS_PIN_I2C_SCL;
	i2c1_scl->mux_id = 19;
	i2c1_scl->mode_value = 1;

	i2c1->_pins = (p_bus_pin*)malloc(sizeof(p_bus_pin)*i2c1->pins_count);
	if(i2c1->_pins == 0 )
	{
		free(i2c1_scl);
		free(i2c1_sda);
		free(i2c1);
		return 0;
	}
	i2c1->_pins[0] = i2c1_sda;
	i2c1->_pins[1] = i2c1_scl;

	return i2c1;
}

pBus create_i2c6()
{
	pBus i2c6 = (pBus)malloc(sizeof(bus));
	if( i2c6==0 )
		return 0;

	i2c6->_id = 6;				//	i2c bus 6
	i2c6->bus_type = BUS_I2C;
	i2c6->pins_count = 2;			//	sda and scl

	// i2c6 sda
	p_bus_pin i2c6_sda = (p_bus_pin)malloc(sizeof(bus_pin));
	if (i2c6_sda==0)
	{
		free(i2c6);
		return 0;
	}
	i2c6_sda->mux_id = 28;		//	mux with gpio28
	i2c6_sda->mode_value = 1;	//	when select the pin as i2c6 sda, you must set gpio28 mode to mode1

	// i2c6 scl
	p_bus_pin i2c6_scl = (p_bus_pin)malloc(sizeof(bus_pin));
	if ( i2c6_scl==0 )
	{
		free(i2c6_sda);
		free(i2c6);
		return 0;
	}
	i2c6_scl->mux_id = 27;
	i2c6_scl->mode_value = 1;

	//	store pins
	i2c6->_pins = (bus_pin**)malloc(sizeof(bus_pin)*i2c6->pins_count);
	if(i2c6->_pins==0)
	{
		free(i2c6_scl);
		free(i2c6_sda);
		free(i2c6);
		return 0;
	}
	i2c6->_pins[0] = i2c6_sda;
	i2c6->_pins[1] = i2c6_scl;

	return i2c6;
}

FUNC_RESULT set_i2c_mode(pBus bus)
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;

	if (bus==0)
		return FUNC_RESULT_NULLPOINTER;

	if ( bus->bus_type!=BUS_I2C )
		return FUNC_RESULT_FAILED;

	p_bus_pin bus_pin = 0;
	size_t pinid=0;
	for (; pinid<bus->pins_count; pinid++)
	{
		bus_pin = bus->_pins[pinid];
		res=ChangePinMode(bus_pin->mux_id, bus_pin->mode_value);
		if( res!=FUNC_RESULT_SUCCESS )
			break;
	}

	return res;
}

FUNC_RESULT open_i2c_dev( i2c_context_t i2c_dev )
{
	FUNC_RESULT res=FUNC_RESULT_SUCCESS;

	if(i2c_dev==0)
		return FUNC_RESULT_NULLPOINTER;

	char devpath[MAX_SIZE];
	snprintf(devpath, MAX_SIZE, I2C_DEV_PATH "%u", i2c_dev->_i2c_bus->_id);
	if( (i2c_dev->fd=open(devpath, O_RDWR)) < 1 )
		return FUNC_RESULT_FILE_OPEN_FAILED;

	if (ioctl(i2c_dev->fd, I2C_FUNCS, &i2c_dev->_funcs) < 0)
	{
		i2c_dev->_funcs = 0;
		res = FUNC_RESULT_FAILED;
	}

	return res;
}

i2c_context_t i2c_init(edison_i2c bus_id)
{
	i2c_context_t i2c = (i2c_context_t)malloc(sizeof(i2c_context));
	if( i2c==0 )
		return 0;

	if ( (i2c->_i2c_bus=bus_is_init(bus_id)) == 0 )
	{
		switch(bus_id)
		{
		case EDISON_I2C_1:
			i2c->_i2c_bus = create_i2c1();
			break;
		case EDISON_I2C_6:
			i2c->_i2c_bus = create_i2c6();
			break;
		default:
			break;
		}
	}

	set_i2c_mode(i2c->_i2c_bus);
	open_i2c_dev(i2c);

	return i2c;
}

uint8_t i2c_read_byte_data(i2c_context_t dev, uint8_t command)
{
    i2c_smbus_data_t data;

    if (i2c_smbus_access(dev->fd, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA, &data) < 0) {
        return 0;
    }
    return 0x0FF & data.byte;
}

int i2c_read_bytes(i2c_context_t dev, uint8_t command, uint8_t* data, int length)
{
	if ( length <= 0 )
		return -1;

	struct i2c_rdwr_ioctl_data d;
	struct i2c_msg m[2];

	m[0].addr = dev->_addr;
	m[0].flags = 0x00;
	m[0].len = 1;
	m[0].buf = &command;
	m[1].addr = dev->_addr;
	m[1].flags = I2C_M_RD;
	m[1].len = length;
	data[0] = 0;
	m[1].buf = data;

	d.msgs = m;
	d.nmsgs = 2;

	return ioctl(dev->fd, I2C_RDWR, &d) < 0 ? -1 : 0;
}

FUNC_RESULT i2c_set_frequency(i2c_context_t dev, i2c_speed_mode mode)
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;

	int fd = 0;

	switch (dev->_i2c_bus->_id) {
		case 1:
			fd = open("/sys/devices/pci0000:00/0000:00:08.0/i2c_dw_sysnode/mode", O_RDWR);
			break;
		case 6:
			fd = open("/sys/devices/pci0000:00/0000:00:09.1/i2c_dw_sysnode/mode", O_RDWR);
			break;
		default:
			return FUNC_RESULT_DEV_SPEED_CHANGE_NOT_SUPPORT;
	}

	if (fd == -1) {
		return FUNC_RESULT_FILE_OPEN_FAILED;
	}

	char mode_buffer[5];
	int length;
	switch (mode) {
		case I2C_STD:
			length = snprintf(mode_buffer, sizeof(mode_buffer), "std");
			break;
		case I2C_FAST:
			length = snprintf(mode_buffer, sizeof(mode_buffer), "fast");
			break;
		case I2C_HIGH:
			length = snprintf(mode_buffer, sizeof(mode_buffer), "high");
			break;
		default:
			close(fd);
			return FUNC_RESULT_INVALID_PARAMETER;
	}

	if (write(fd, mode_buffer, length * sizeof(char)) == -1)
	{
		close(fd);
		return FUNC_RESULT_FILE_WRITE_FAILED;
	}
	close(fd);

	return res;
}

FUNC_RESULT i2c_set_address(i2c_context_t dev, uint8_t addr)
{
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;
	if(ioctl(dev->fd, I2C_SLAVE_FORCE, addr) < 0)
		return FUNC_RESULT_FAILED;
	dev->_addr = addr;
	return res;
}

FUNC_RESULT i2c_write_bytes(i2c_context_t dev, uint8_t command, const uint8_t* data, int length)
{
	if ( dev == 0 || dev->fd==-1 )
		return FUNC_RESULT_NULLPOINTER;

	if (length>I2C_SMBUS_I2C_BLOCK_MAX )
		return FUNC_RESUTL_I2C_EXCCEED_BLOCK_SIZE;

	i2c_smbus_data_t smdata;
	smdata.block[0] = length;
	memcpy((void*)(smdata.block+1), (void*)data, length);
	int res = i2c_smbus_access( dev->fd, I2C_SMBUS_WRITE, command, I2C_SMBUS_I2C_BLOCK_DATA, &smdata);
	if ( res != 0 )
		return FUNC_RESULT_FAILED;
	return FUNC_RESULT_SUCCESS;
}
