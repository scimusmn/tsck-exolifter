//This code monitors the high/low states of three pushbuttons and one lockout switch.
//When one of the pushbuttons goes "low", a corresponding LED turns "on", and the other LEDs turn "off".
//The pushbuttons are interlocked, so that while one is being pushed, the other two are ignored.
//If the lockout switch is set "low", all three pushbuttons are ignored, but the last LED that was turned on stays "on".

//included libraries
#include "Button.h"


//definitions 
#define LED1 A0
#define LED2 A1
#define LED3 A2


//variables

const int button1pin = 6; //center weight pushbutton 
const int button2pin = 7; //outer weight pushbutton
const int button3pin = 8; //all-weights pushbutton
const int button4pin = 9; //lockout switch
const int handle = 10; // handle switch
const int centerStack = 3; // weight stack electromagnet FET
const int leftStack = 2;
const int rightStack = 4;
const int valve = 5; //spool valve FET
unsigned long timeNow;


//Button class instances
Button centerStackButton(button1pin, &centerStackPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button outerStacksButton(button2pin, &outerStacksPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button allStacksButton(button3pin, &allStacksPushed, 20); // pin, function reference, (optional)debounce in milliseconds.
Button lockoutButton(button4pin,&lockoutSwitched, 20); //pin, no function needed here, (optional) debounce in milliseconds
Button handleButton(handle,&handleSwitched, 20);


void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(centerStack, OUTPUT);
  pinMode(leftStack, OUTPUT);
  pinMode(rightStack, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(button1pin, INPUT);  
  pinMode(button2pin, INPUT); 
  pinMode(button3pin, INPUT); 
  pinMode(button4pin, INPUT); 
  pinMode(handle, INPUT); 
}

//functions tied to button class instances
void lockoutSwitched(){ //no tasks here, just placeholder function needed for creating lockoutButton instance
}
void handleSwitched(int state){
  if (state == LOW){
    digitalWrite(valve, HIGH);
    Serial.write("Handle on -- ");
    timeNow=millis();
  }
  else {
    digitalWrite(valve,LOW);
    Serial.write("Handle off -- ");
  }
}

void centerStackPushed(int state) {
  if (!state && lockoutButton.getState() && outerStacksButton.getState() && allStacksButton.getState()) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(leftStack, LOW);
    digitalWrite(centerStack, HIGH);
    digitalWrite(rightStack, LOW);  
    Serial.write("Center -- ");
    }
}

void outerStacksPushed(int state) {
  if (!state && lockoutButton.getState() && centerStackButton.getState() && allStacksButton.getState()) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);    
    digitalWrite(leftStack, HIGH);
    digitalWrite(centerStack, LOW);
    digitalWrite(rightStack, HIGH);
    Serial.write("Outside -- ");

  }
}

void allStacksPushed(int state) {
  if (!state && lockoutButton.getState() && centerStackButton.getState() && outerStacksButton.getState()) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);    
    digitalWrite(leftStack, HIGH);
    digitalWrite(centerStack, HIGH);
    digitalWrite(rightStack, HIGH);
    Serial.write("All -- ");

  }
}

//main loop
void loop() {
  lockoutButton.update();
  centerStackButton.update();
  outerStacksButton.update();
  allStacksButton.update();
  handleButton.update();
  if (lockoutButton.getState() && millis()>=timeNow+60000){
   digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(leftStack, LOW);
    digitalWrite(centerStack, LOW);
    digitalWrite(rightStack, LOW);  
    Serial.write("Timeout -- ");
    timeNow=millis();
  }
}
