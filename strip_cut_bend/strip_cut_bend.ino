
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Servo.h>
 
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
//int i = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *feeder = AFMS.getStepper(200, 2);


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

}

void loop() {
  cut_wire(20);
  
  delay(1000);
  
  cut_wire(50);
  
  delay(1000);
  
}

void cut_wire(int steps) {
  if(steps < 40) {
    //first strip-cut
    strip(1);
    dislodge();
    
    //feed steps
    feed(steps);
    
    //second strip-cut
    strip(1);
    dislodge();
    
    //feed 40 - steps
    feed(40-steps);
    
    //first bend
    bend();
    
    //feed steps
    feed(steps);
    
    //second bend
    bend();
    
    //cut
    cut();
    dispense(3);
    
  }
  else {
    //first strip-cut
    strip(1);
    dislodge();
    
    //feed to first strip-cut, bend
    feed(40);
    bend();
    
    //feed steps-40
    feed(steps-40);
    
    //strip-cut
    strip(1);
    dislodge();
    
    //feed to strip-cut, bend
    feed(40);
    bend();
    
    //cut
    cut();
    dispense(3);
    
    
  }
  
  
//  //first strip-cut
//  strip(1);
//  
//  //dislodge wire from stripper
//  dislodge();
//  
//  //bend at the strip-cut
//  feed(50);
//  bend();
//
//  //feed out length of wire, then strip-cut
//  //feed(steps - 40);
//  feed(steps);
//  strip(1);
//  
//  //dislodge wire from stripperx
//  dislodge();
//  
//  //bend at the strip cut
//  feed(50);
//  bend();
//  
//  //final feed/cut
//  cut();
//  delay(500);
//  
//  //drop wire into tray by raising the bending table
//  dispense(3);
//  delay(1000);
  
  
}

void feed(int steps) {
  feeder->step(steps, BACKWARD, DOUBLE);
  //feeder->release();
  delay(1000);
}

void strip(int times) {
  int i;
  for(i=0; i<times; i++) {
    for(pos = s_open; pos >= s_closed; pos-=1) { //closes stripper
      stripper.write(pos);
      delay(15);  
    }
    delay(1000);
    for(pos = s_closed; pos< s_open; pos+=1) {  //opens stripper
      stripper.write(pos);
      delay(15); 
    }
    delay(500);
  }
  delay(2500);
  //i=0;
}

void cut() {
  cutter.write(c_closed);
  delay(1000);
  
  cutter.write(c_open);    //open position
  delay(1500);
}

void bend() {
  bender.write(b_closed);  //closed position
  delay(1000);
  
  bender.write(b_open);  //open position
  delay(1000);
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
