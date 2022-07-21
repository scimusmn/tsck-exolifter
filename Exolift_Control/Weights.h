#pragma once

class Weights {
	protected:
		int emLeft, emCenter, emRight;
		void select(bool left, bool center, bool right) {
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
