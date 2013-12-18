
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Servo.h>


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *feeder = AFMS.getStepper(200, 2);


Servo stripper;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
int strip_pin = 9;
int s_closed = 120;
int s_open = 180;

Servo cutter;

int cut_pin = 10;
int c_closed = 120;
int c_open = 55;

Servo bender;

int bend_pin = 11;
int b_closed = 95;
int b_open = 10;

int pos = 0;    // variable to store the servo position

void setup() {
  //stripper servo

  stripper.attach(strip_pin); 
  stripper.write(s_open);
  
  //cutter servo

  cutter.attach(cut_pin);
  cutter.write(c_open);
  
  //bender servo

  bender.attach(bend_pin);
  bender.write(b_open);
  
  Serial.begin(9600);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  feeder->setSpeed(3);  // 3 rpm   
  
  int i;
  for(i=10; i<=50; i+=10){
   cut_wire(i); 
   cut_wire(i);
   cut_wire(i);
  }

}

void loop() {
}

void cut_wire(int steps) {
  feed(steps);
  delay(1000);
  
  cut();
  delay(1000);
  
  dispense(3);
  delay(1000);
}


void feed(int steps) {
  feeder->step(steps, BACKWARD, DOUBLE);
  //feeder->release();
  delay(1000);
}

void cut() {

  cutter.write(c_closed);
  delay(1000);
  
  cutter.write(c_open);    //open position
  delay(1500);

}


void dislodge() {
  //dislodge wire from stripper
  feeder->setSpeed(10);  // 10 rpm   
  feeder->step(5, FORWARD, DOUBLE);
  delay(1000);
  feeder->step(5, BACKWARD, DOUBLE);
  delay(500);
  //feeder->release();
  feeder->setSpeed(3);  // 3 rpm  
}

void dispense(int times) {
  bender.write(b_closed);  //closed position
  delay(2000);
  
  bender.write(b_open);  //open position
  delay(500);
  
  //jiggles the bender a certain number of times
  int i;
  for(i=0; i < times; i++) {
    bender.write(b_closed-20);
    delay(100);
    bender.write(b_open);
    delay(500);
  }
  
}
