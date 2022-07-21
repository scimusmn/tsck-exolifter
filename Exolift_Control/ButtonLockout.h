#pragma once

#include <Arduino.h>
#include "Button.h"
#include "Weights.h"
#include "Leds.h"
#include "SerialPrintf.h"
#include "IntervalTimer.h"

#define BUTTON_LOW 6
#define BUTTON_MED 7
#define BUTTON_HI  8
#define BUTTON_LOCKOUT 9
#define BUTTON_HANDLE 10


/* button to lock other buttons */
class LockoutButton : public smm::Button {
	protected:
	bool locked;
	IntervalTimer& timer;

	void onPress() {
		SerialPrintf("lockout: locked\n");
		locked = true;
		timer.stop();
	}

	void onRelease() {
		SerialPrintf("lockout: unlocked\n");
		locked = false;
		timer.start();
	}

	public:
	LockoutButton(int pin, IntervalTimer& timer) : Button(pin), timer(timer) { locked = false; }
	bool isLocked() { return locked; }
};


/* base class for buttons to disable with the interlock */
class InterlockedButton : public smm::Button {
	protected:
	LockoutButton& interlock;
	Leds& leds;
	Weights& weights;

	public:
	InterlockedButton(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: Button(pin), interlock(interlock), leds(leds), weights(weights) {}

	/* this function triggers only if the interlock is unlocked */
	virtual void onInterlockPress() {}

	void onPress() {
		SerialPrintf("Button on pin %d pressed\n", buttonPin);
		/* if interlock button is locked, do nothing */
		if (interlock.isLocked()) {
			SerialPrintf("Interlock enabled; press on %d has no effect\n", buttonPin);
			return;
		}
		onInterlockPress();
	}
};


/* low weight selector */
class ButtonLow : public InterlockedButton {
	public:
	ButtonLow(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: InterlockedButton(pin, interlock, leds, weights) 
	{}
	void onInterlockPress() {
		SerialPrintf("Select low weight\n");
		leds.set(1, 0, 0);
		weights.selectCenter();
		Serial.println();
	}
};


/* medium weight selector */
class ButtonMed : public InterlockedButton {
	public:
	ButtonMed(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: InterlockedButton(pin, interlock, leds, weights) 
	{}
	void onInterlockPress() {
		SerialPrintf("Select medium weight\n");
		leds.set(0, 1, 0);
		weights.selectOuter();
		Serial.println();
	}
};


/* high weight selector */
class ButtonHi : public InterlockedButton {
	public:
	ButtonHi(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: InterlockedButton(pin, interlock, leds, weights) 
	{}
	void onInterlockPress() {
		SerialPrintf("Select high weight\n");
		leds.set(0, 0, 1);
		weights.selectAll();
		Serial.println();
	}
};


/* group weights together */
class Buttons {
	protected:
	LockoutButton lockout;
	ButtonLow low;
	ButtonMed med;
	ButtonHi hi;

	public:
	Buttons(int lowPin, int medPin, int hiPin, int lockoutPin, 
		IntervalTimer& timer, Leds& leds, Weights& weights)
		: lockout(lockoutPin, timer),
		  low(lowPin, lockout, leds, weights),
		  med(medPin, lockout, leds, weights),
		  hi (hiPin,  lockout, leds, weights)
	{}

	void update() {
		lockout.update();
		low.update();
		med.update();
		hi.update();
	}
};
