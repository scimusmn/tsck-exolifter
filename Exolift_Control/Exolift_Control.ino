//This code monitors the high/low states of three pushbuttons and one lockout switch.
//When one of the pushbuttons goes "low", a corresponding LED turns "on", and the other LEDs turn "off".
//The pushbuttons are interlocked, so that while one is being pushed, the other two are ignored.
//If the lockout switch is set "low", all three pushbuttons are ignored, but the last LED that was turned on stays "on".

//included libraries
#include "Leds.h"
#include "Weights.h"
#include "Button.h"
#include "ButtonLockout.h"
#include "IntervalTimer.h"

// Leds
#define LED1 A0
#define LED2 A1
#define LED3 A2
Leds leds(LED1, LED2, LED3);

// Weights
#define EM_LEFT 2
#define EM_CENTER 3
#define EM_RIGHT 4
Weights weights(EM_LEFT, EM_CENTER, EM_RIGHT);


// Buttons
#define BUTTON_LOW 6
#define BUTTON_MED 7
#define BUTTON_HI  8
#define BUTTON_LOCKOUT 9
Buttons buttons(BUTTON_LOW, BUTTON_MED, BUTTON_HI, BUTTON_LOCKOUT, leds, weights);


// Valve
#define VALVE_PIN 5
#define BUTTON_HANDLE 10

IntervalTimer weightTimeout(60000);
class HandleButton : public smm::Button {
	protected:
	int valvePin;
	IntervalTimer& timeout;

	public:
	HandleButton(int pin, int valvePin, IntervalTimer& timeout) : Button(pin), timeout(timeout) {
		pinMode(valvePin, OUTPUT);
	}
	void onPress() {
		timeout.stop();
		digitalWrite(valvePin, 1);
	}

	void onRelease() {
		timeout.start();
		digitalWrite(valvePin, 0);
	}
} handle(BUTTON_HANDLE, VALVE_PIN, weightTimeout);


void setup() {
	Serial.begin(9600);
}


void loop() {
	buttons.update();
	handle.update();
	if (weightTimeout.triggered()) {
		leds.set(0, 0, 0);
		weights.selectNone();
	}
}
