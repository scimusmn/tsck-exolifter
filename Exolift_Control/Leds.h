#pragma once

#include <Arduino.h>
#include "SerialPrintf.h"


class Leds {
	protected:
		int led1, led2, led3;

	public:
		Leds(int ledPin1, int ledPin2, int ledPin3) {
			led1 = ledPin1;
			led2 = ledPin2;
			led3 = ledPin3;
			pinMode(led1, OUTPUT);
			pinMode(led2, OUTPUT);
			pinMode(led3, OUTPUT);
		}

		void set(bool state1, bool state2, bool state3) {
			SerialPrintf("LED state: [%d, %d, %d]\n", state1, state2, state3);
			digitalWrite(led1, state1);
			digitalWrite(led2, state2);
			digitalWrite(led3, state3);
		}
};
