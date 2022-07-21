#pragma once

#include <Arduino.h>
#include "Button.h"
#include "Weights.h"
#include "Leds.h"

#define BUTTON_LOW 6
#define BUTTON_MED 7
#define BUTTON_HI  8
#define BUTTON_LOCKOUT 9
#define BUTTON_HANDLE 10


/* button to lock other buttons */
class LockoutButton : public smm::Button {
	protected:
	bool locked;

	void onPress() {
		locked = true;
	}

	void onRelease() {
		locked = false;
	}

	public:
	LockoutButton(int pin) : Button(pin) { locked = false; }
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
		/* if interlock button is locked, do nothing */
		if (interlock.isLocked()) return;
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
		leds.set(1, 0, 0);
		weights.selectCenter();
	}
};


/* medium weight selector */
class ButtonMed : public InterlockedButton {
	public:
	ButtonMed(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: InterlockedButton(pin, interlock, leds, weights) 
	{}
	void onInterlockPress() {
		leds.set(0, 1, 0);
		weights.selectOuter();
	}
};


/* high weight selector */
class ButtonHi : public InterlockedButton {
	public:
	ButtonHi(int pin, LockoutButton& interlock, Leds& leds, Weights& weights) 
		: InterlockedButton(pin, interlock, leds, weights) 
	{}
	void onInterlockPress() {
		leds.set(0, 0, 1);
		weights.selectAll();
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
		Leds& leds, Weights& weights)
		: lockout(lockoutPin),
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
