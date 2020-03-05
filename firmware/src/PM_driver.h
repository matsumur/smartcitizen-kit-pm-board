#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <pins.h>

#define debug_PM

enum Clickcommands{
	CLICK_START,
	CLICK_STOP,
	CLICK_GET
};

static const uint8_t valuesSize = 2;

class ClickSensor {
public:

	ClickSensor() {

	}
	bool begin();
	bool stop();
	void reset();
	bool update();
	bool active = false;

	uint8_t values[valuesSize];
	// Transmission via I2C to SCK in 2 bytes:
	// 0:1->clicks 

	// Readings
	uint16_t buttonCount = 0;

private:
	bool currentButtonState = false;
	bool lastButtonState = false;
};

