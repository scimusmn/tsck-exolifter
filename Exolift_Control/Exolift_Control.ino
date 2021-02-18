//This code monitors the high/low states of three pushbuttons and one lockout switch.
//When one of the pushbuttons goes "low", a corrresponding LED turns "on", and the other LEDs turn "off".
//The pushbuttons are interlocked, so that while one is being pushed, the other two are ignored.
//If the lockout switch is set "low", all three pushbuttons are ignored, but the last LED that was turned on stays "on".

//included libraries
#include "Button.h"

//definitions
#define LED1 10
#define LED2 11
#define LED3 12

//variables
const int button1pin = 2; //center weight pushbutton
const int button2pin = 3; //outer weight pushbutton
const int button3pin = 4; //all-weights pushbutton
const int button4pin = 5; //lockout switch

//Button class instances
Button centerStackButton(button1pin, &centerStackPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button outerStacksButton(button2pin, &outerStacksPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button allStacksButton(button3pin, &allStacksPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button lockoutButton(button4pin,&lockoutSwitched, 20); //pin, no function needed here, (optional) debounce in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

//functions tied to button class instances
void lockoutSwitched(){ //no tasks here, just placeholder function for creating lockoutButton instance
}

void centerStackPushed(int state) {
  if (state && lockoutButton.getState() && outerStacksButton.getState() && allStacksButton.getState()) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }
}

void outerStacksPushed(int state) {
  if (state && lockoutButton.getState() && centerStackButton.getState() && allStacksButton.getState()) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  }
}

void allStacksPushed(int state) {
  if (state && lockoutButton.getState() && centerStackButton.getState() && outerStacksButton.getState()) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }
}

//main loop
void loop() {
  lockoutButton.update();
  centerStackButton.update();
  outerStacksButton.update();
  allStacksButton.update();
}
