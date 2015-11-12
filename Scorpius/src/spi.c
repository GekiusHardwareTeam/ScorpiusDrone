/*
 * spi.c
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Jhon_Qiki
 */
#include "include/spi.h"
#include "include/common.h"
#include "include/pin.h"
#include "include/gpio.h"
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/ioctl.h>

#define EDISON_DEFAULT_SPI_DEV 5
#define EDISON_DEFAULT_SPI_CS 1
#define SPI_DEV_PATH "/dev/spidev"

FUNC_RESULT set_spi_mode(spi_context_p dev, spi_mode mode)
{
	if ( dev == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	unsigned char _mode = mode;
	if ( ioctl(dev->fd, SPI_IOC_WR_MODE, &_mode) < 0 )
		return FUNC_RESULT_FAILED;
	dev->mode = mode;
	return FUNC_RESULT_SUCCESS;
}

void free_spi51_bus(pBus bus)
{
	if ( bus ==0 )
		return;

	int pinid = 0;
	for (;pinid<bus->pins_count; pinid++)
	{
		free(bus->_pins[pinid]);
	}

	free(bus);
}

FUNC_RESULT spi51_bus_init(spi_context_p dev)
{
	if ( dev == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	pBus bus = dev->spi_bus;
	bus->_id = 5;
	bus->bus_type = BUS_SPI;
	bus->pins_count = 4;

	bus->_pins = (p_bus_pin*)malloc(sizeof(p_bus_pin)*bus->pins_count);
	if ( bus->_pins < 1 )
		return FUNC_RESULT_FAILED;

	//	mosi pin
	p_bus_pin mosi = (p_bus_pin)malloc(sizeof(bus_pin));
	if ( mosi == 0 )
	{
		free(bus->_pins);
		return FUNC_RESULT_FAILED;
	}
	mosi->pin_type = BUS_PIN_SPI_MOSI;
	mosi->mux_id = 115;
	mosi->mode_value = 1;
	mosi->_fd = -1;

	//	miso pin
	p_bus_pin miso = (p_bus_pin)malloc(sizeof(bus_pin));
	if ( miso == 0 )
	{
		free(mosi);
		free(bus->_pins);
		return FUNC_RESULT_FAILED;
	}
	miso->pin_type = BUS_PIN_SPI_MISO;
	miso->mux_id = 114;
	miso->mode_value = 1;
	miso->_fd = -1;

	//	spi2 clk
	p_bus_pin clk = (p_bus_pin)malloc(sizeof(bus_pin));
	if ( clk == 0 )
	{
		free(miso);
		free(mosi);
		free(bus->_pins);
		return FUNC_RESULT_FAILED;
	}
	clk->pin_type = BUS_PIN_SPI_SCL;
	clk->mux_id = 109;
	clk->mode_value = 1;
	clk->_fd = -1;

	// spi2 cs
	p_bus_pin cs0 = (p_bus_pin)malloc(sizeof(bus_pin));
	if ( cs0 == 0 )
	{
		free(clk);
		free(miso);
		free(mosi);
		free(bus->_pins);
		return FUNC_RESULT_FAILED;
	}
	cs0->pin_type = BUS_PIN_SPI_CS;
	cs0->mode_value = 0;	// can not enable cs0, simulate gpio as cs
	cs0->mux_id = 110;
	cs0->_fd = -1;

	bus->_pins[0] = mosi;
	bus->_pins[1] = miso;
	bus->_pins[2] = clk;
	bus->_pins[3] = cs0;
	dev->cs_id = 3;

	return FUNC_RESULT_SUCCESS;
}

/*
 * configure multiplexer
 */
FUNC_RESULT enable_spi(spi_context_p spi)
{
	if ( spi == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;
	pBus spi_bus = spi->spi_bus;
	int pinid = 0;
	p_bus_pin current_pin = 0;
	for (; pinid<spi_bus->pins_count; pinid++)
	{
		current_pin = spi_bus->_pins[pinid];
		ChangePinMode(current_pin->mux_id, current_pin->mode_value);
	}

	//	edison doesn`t support hardware cs0, use gpio110 to simulate it
	FUNC_RESULT res = FUNC_RESULT_SUCCESS;
	p_bus_pin cs0_pin = spi_bus->_pins[spi->cs_id];
	gpio_context_p gp110 = gpio_init(cs0_pin->mux_id);
	res = gpio_direction(gp110, GPIO_DIR_OUT);
	gpio_release(gp110);

	return res;
}

spi_context_p spi_init()
{
	spi_context_p spi = (spi_context_p)malloc(sizeof(spi_context));
	memset(spi, 0, sizeof(spi_context));

	//	only one spi bus exist in edison
	char buffer[MAX_PATH_SIZE] = {0};
	snprintf( buffer, MAX_PATH_SIZE, SPI_DEV_PATH "%u.%u", EDISON_DEFAULT_SPI_DEV, EDISON_DEFAULT_SPI_CS );
	if ( (spi->fd=open( buffer, O_RDWR)) < 0 )
	{
		free(spi);
		return 0;
	}

	pBus spi_bus = (pBus)malloc(sizeof(bus));
	memset(spi_bus, 0, sizeof(bus));
	if ( spi_bus == 0 )
	{
		free(spi);
		return 0;
	}

	spi->spi_bus = spi_bus;
	if ( spi51_bus_init(spi) != FUNC_RESULT_SUCCESS )
	{
		free_spi51_bus(spi_bus);
		close(spi->fd);
		free(spi);
		return 0;
	}

	if ( enable_spi(spi) != FUNC_RESULT_SUCCESS )
	{
		free_spi51_bus(spi->spi_bus);
		close(spi->fd);
		free(spi);
		return 0;
	}
	return spi;
}

FUNC_RESULT set_spi_speed(spi_context_p dev, int speedinhz)
{
	if ( dev == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	int speed = 0;
	if ( ioctl( dev->fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed ) == -1 )
	{
		if ( speedinhz < speed )
			speed = speedinhz;
	}

	if ( ioctl( dev->fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1 )
		return FUNC_RESULT_FAILED;

	dev->speed = speed;

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT set_spi_lsbmode(spi_context_p dev, unsigned int lsbmode )
{
	if ( dev == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	if ( lsbmode == 0 )
	{
		if ( ioctl(dev->fd, SPI_IOC_WR_LSB_FIRST, &lsbmode) < 0 )
			return FUNC_RESULT_FAILED;
		if ( ioctl(dev->fd, SPI_IOC_RD_LSB_FIRST, &lsbmode) < 0 )
			return FUNC_RESULT_FAILED;
	}
	else
	{
		return FUNC_RESULT_SPI_LSB_NOT_SUPPORT;
	}

	dev->lsb = lsbmode;

	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT spi_per_word(spi_context_p dev, unsigned int bits)
{
	if ( dev == 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	if ( ioctl(dev->fd, SPI_IOC_WR_BITS_PER_WORD, &bits) <  0 )
		return FUNC_RESULT_SYSCALL_FAILED;

	dev->bpw = bits;
	return FUNC_RESULT_SUCCESS;
}

FUNC_RESULT spi_transfer_buffer(spi_context_p dev, uint8_t* data, uint8_t* rvdata, int length)
{
	struct spi_ioc_transfer msg;
	memset(&msg, 0, sizeof(msg));

	 msg.tx_buf = (__u64)data;
	 msg.rx_buf = (__u64)rvdata;
	 msg.speed_hz = dev->speed;
	 msg.bits_per_word = dev->bpw;
	 msg.delay_usecs = 0;
	 msg.len = length;
	 if ( ioctl(dev->fd, SPI_IOC_MESSAGE(1), &msg) == -1 )
	 {
		 return FUNC_RESULT_SYSCALL_FAILED;
	 }
	return FUNC_RESULT_SUCCESS;
}

uint8_t* spi_write_buffer(spi_context_p dev, uint8_t* data, int length)
{
	uint8_t *revbuffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
	if ( revbuffer == 0 )
		return 0;

	if ( spi_transfer_buffer(dev, data, revbuffer, length) != FUNC_RESULT_SUCCESS )
	{
		free(revbuffer);
		return 0;
	}

	return revbuffer;
}

gpio_context_p get_cs0_gpio_context(spi_context_p dev)
{
	if ( dev==0 )
		return 0;

	p_bus_pin cs0_pin = dev->spi_bus->_pins[dev->cs_id];
	gpio_context_p cs0 = gpio_init(cs0_pin->mux_id);

	return cs0;
}

FUNC_RESULT cs0_low(spi_context_p dev)
{
	if ( dev== 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	gpio_context_p cs0 = get_cs0_gpio_context(dev);
	if ( cs0 == 0 )
	{
		syslog(LOG_ERR, "spi cs0: initialization failed");
		return FUNC_RESULT_FAILED;
	}
	gpio_direction(cs0, GPIO_DIR_OUT);
	FUNC_RESULT res = gpio_write(cs0, 0);
	free(cs0);
	return res;
}

FUNC_RESULT cs0_high(spi_context_p dev)
{
	if ( dev== 0 )
		return FUNC_RESULT_INVALID_PARAMETER;

	gpio_context_p cs0 = get_cs0_gpio_context(dev);
	gpio_direction(cs0, GPIO_DIR_OUT);
	FUNC_RESULT res = gpio_write(cs0, 1);
	free(cs0);
	return res;
}
