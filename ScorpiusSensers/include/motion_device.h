#pragma once
#include "gpio.h"

class MotionDevice{
public:
	MotionDevice();
	~MotionDevice();

protected:
	gpio_context_p gpio_event;
};
