
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
int i = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  myservo.attach(9); 
  myservo.write(170);
  
  Serial.begin(9600);

  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(10);  // 10 rpm   
}

void loop() {

  Serial.println("Double coil steps");
  myMotor->step(100, BACKWARD, DOUBLE); 
  
  delay(6000);
  
  for(i = 0; i < 3; i +=1)
  {
    for(pos = 170; pos>=140; pos-=1)     // goes from 180 degrees to 0 degrees
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    delay(5000);
    for(pos = 150; pos < 170; pos += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
  }
  delay(500);
  i = 0;
  
  
}
