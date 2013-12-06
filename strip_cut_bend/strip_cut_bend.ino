
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Servo.h>
 
Servo stripper;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
Servo cutter;

Servo bender;

int pos = 0;    // variable to store the servo position
//int i = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *feeder = AFMS.getStepper(200, 2);


void setup() {
  //stripper servo
  stripper.attach(9); 
  stripper.write(180);  //open position
  
  //cutter servo
  cutter.attach(10);
  cutter.write(55);  //open position
  
  //bender servo
  bender.attach(11);
  bender.write(10); //open position
  
  Serial.begin(9600);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  feeder->setSpeed(3);  // 3 rpm   

}

void loop() {
  //first feed/strip-cut
  feed(15);
  strip(1);
  delay(500);
  
  //bend at the strip-cut
  bend();
  delay(500);
  
  //dislodge wire from stripper
  feeder->setSpeed(10);  // 10 rpm   
  feeder->step(5, FORWARD, DOUBLE);
  delay(1000);
  feeder->step(5, BACKWARD, DOUBLE);
  delay(500);
  feeder->setSpeed(3);  // 3 rpm   
  
  //feed out length of wire, then strip-cut
  feed(20);
  strip(1);
  delay(500);
  
  //bend at the strip cut
  feed(15);
  bend();
  delay(500);
  
  //final feed/cut
  feed(20);
  cut();
  delay(1000);
  
}

void feed(int steps) {
  feeder->step(steps, BACKWARD, DOUBLE);
  delay(1000);
}

void strip(int times) {
  int i;
  for(i=0; i<times; i++) {
    for(pos = 180; pos >=120; pos-=1) { //closes stripper
      stripper.write(pos);
      delay(15);  
    }
    delay(1000);
    for(pos = 120; pos<180; pos+=1) {  //opens stripper
      stripper.write(pos);
      delay(15); 
    }
    delay(500);
  }
  delay(2000);
  //i=0;
}


void cut() {
  cutter.write(110);    //closed position
  delay(1000);
  
  cutter.write(55);    //open position
  delay(500);
  
}

void bend() {
  bender.write(90);  //closed position
  delay(500);
  
  bender.write(10);  //open position
  delay(500);
}
