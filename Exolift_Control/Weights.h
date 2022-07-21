#pragma once

#include <Arduino.h>
#include "SerialPrintf.h"

class Weights {
	protected:
		int emLeft, emCenter, emRight;
		void select(bool left, bool center, bool right) {
			SerialPrintf("Magnet state: [%d, %d, %d]\n", left, center, right);
			digitalWrite(emLeft, left);
			digitalWrite(emCenter, center);
			digitalWrite(emRight, right);
		}
	
	public:
		Weights(int emLeftPin, int emCenterPin, int emRightPin) {
			emLeft = emLeftPin;
			pinMode(emLeft, OUTPUT);
			emCenter = emCenterPin;
			pinMode(emCenter, OUTPUT);
			emRight = emRightPin;
			pinMode(emRight, OUTPUT);
		}

		void selectNone() {
			select(0, 0, 0);
		}

		void selectCenter() {
			select(0, 1, 0);
		}

		void selectOuter() {
			select(1, 0, 1);
		}

		void selectAll() {
			select(1, 1, 1);
		}
};
