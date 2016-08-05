/***************************************************
This is a library for our I2C LED Backpacks

Designed specifically to work with the Adafruit LED Matrix backpacks
----> http://www.adafruit.com/products/
----> http://www.adafruit.com/products/

These displays use I2C to communicate, 2 pins are required to
interface. There are multiple selectable I2C addresses. For backpacks
with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
with 3 Address Select pins: 0x70 thru 0x77

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
****************************************************/

#include "Adafruit_LEDBackpack.h"

void Adafruit_LEDBackpack::setBrightness(uint8_t b) {
	if (b > 15) b = 15;
	Wire.beginTransmission(i2c_addr);
	Wire.write(0xE0 | b);
	Wire.endTransmission();
}

void Adafruit_LEDBackpack::blinkRate(uint8_t b) {
	Wire.beginTransmission(i2c_addr);
	if (b > 3) b = 0; // turn off if not sure

	Wire.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
	Wire.endTransmission();
}

Adafruit_LEDBackpack::Adafruit_LEDBackpack(void) {
}

void Adafruit_LEDBackpack::begin(uint8_t _addr = 0x70) {
	i2c_addr = _addr;

	Wire.begin();

	Wire.beginTransmission(i2c_addr);
	Wire.write(0x21);  // turn on oscillator
	Wire.endTransmission();
	blinkRate(HT16K33_BLINK_OFF);

	setBrightness(15); // max brightness
}

void Adafruit_LEDBackpack::writeDisplay(void) {
	Wire.beginTransmission(i2c_addr);
	Wire.write((uint8_t)0x00); // start at address $00

	for (uint8_t i = 0; i<8; i++) {
		Wire.write(displaybuffer[i] & 0xFF);
		Wire.write(displaybuffer[i] >> 8);
	}
	Wire.endTransmission();
}

void Adafruit_LEDBackpack::clear(void) {
	for (uint8_t i = 0; i<8; i++) {
		displaybuffer[i] = 0;
	}
}