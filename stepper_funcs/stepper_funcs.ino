

#include <Servo.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

Servo stripServo;
Servo bendServo;
Servo cutServo;

int servoPin1 = 9;
int servoPin2 = 10;
int servoPin3 = 11;

int stripClosed = 0;
int bendClosed = 0;
int cutClosed = 0;

int stripOpen = 0;
int bendOpen = 0;
int cutOpen = 0;



void setup() {
  Serial.begin(9600);
  
  stripServo.attach(servoPin1);
  bendServo.attach(servoPin2);
  cutServo.attach(servoPin3);
  
  stripServo.write(stripOpen);
  bendServo.write(bendOpen);
  cutServo.write(cutOpen);
  
  AFMS.begin();  // create with the default frequency 1.6KHz

  myMotor->setSpeed(10);  // 10 rpm
  
  
}

void loop() {
//incorporate serial comm
  int data = 10;    //arbitrarily chosen number, for testing
  int steps = process_data(data);
  process_wire(steps);

}

int process_data(int data) {
  //200 step stepper, wheel has radius ~2cm
  //distance-to-step function: data*200/(4pi)
  //do this calculation in Python! 
  int random_factor = 1;
  return data * random_factor;
}

void process_wire(int steps) {
  //feeding wire for initial strip/bend
  myMotor->step(50, FORWARD, DOUBLE);
  
  //strips wire
  stripServo.write(stripClosed);
  delay(100);
  myMotor->step(50, BACKWARD, DOUBLE);    //removes insulation
  myMotor->step(50, FORWARD, DOUBLE);
  
  //bends wire
  bendServo.write(bendClosed);
  delay(100);
  stripServo.write(stripOpen);
  delay(100);
  
  //feeding wire for final strip
  myMotor->step(steps, FORWARD, DOUBLE); 
  
  //strip-cut
  stripServo.write(stripClosed);
  delay(100);
  
  //cuts wire
  cutServo.write(cutClosed);
  delay(100);
  cutServo.write(cutOpen);
  delay(100);
  
  //bend...how will the final bend work?
  bendServo.write(bendClosed);
  delay(100);
  stripServo.write(stripOpen);
  delay(100);
  
}

