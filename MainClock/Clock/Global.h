/*
	NOTE:	This file 'Global.h' (or 'application.h') must be included
			in every .h file you added in this solution.
*/
#pragma once
#include <application.h>
#include "PinMappings.h"

/* Global Variables and Objects */
unsigned long Time1;

/* Global Function Declaration */
int ledToggle(String command) {

	if (command == "on") {
		LED_SetRGBColor(RGB_COLOR_RED);
		digitalWrite(BuiltInLED, HIGH);
		return 1;
	}
	else if (command == "off") {
		LED_SetRGBColor(RGB_COLOR_GREEN);
		digitalWrite(BuiltInLED, LOW);
		return 0;
	}
	else {
		return -1;
	}
}