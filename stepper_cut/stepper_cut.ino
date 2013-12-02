/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  myMotor->setSpeed(10);  // 10 rpm
  
  //short wire
  Serial.println("Double coil steps");
  myMotor->step(25, BACKWARD, DOUBLE); 
  delay(3000);
  
  myservo.attach(9);
  myservo.write(180);    //closed position
  delay(1000);
  
  myservo.write(109);    //open position
  delay(500);
  
  //medium wire
  myMotor->step(50, BACKWARD, DOUBLE); 
  delay(3000);
  
  myservo.attach(9);
  myservo.write(180);    //closed position
  delay(1000);
  
  myservo.write(109);    //open position
  delay(500);
  
  
//  //long wire
//  myMotor->step(100, BACKWARD, DOUBLE); 
//  delay(3000);
//  
//  myservo.attach(9);
//  myservo.write(180);    //closed position
//  delay(1000);
//  
//  myservo.write(109);    //open position
//  delay(500);

   
}

void loop() {
//  Serial.println("Double coil steps");
//  myMotor->step(100, BACKWARD, DOUBLE); 
//  delay(6000);
//  
//  myservo.attach(9);
//  myservo.write(180);    //closed position
//  delay(1000);
//  
//  myservo.write(109);    //open position
//  delay(50);
//  Serial.println("Double coil steps");
//  myMotor->step(100, BACKWARD, DOUBLE); 
//  
//  myservo.write(180);    //closed position
//  delay(1000);
//  myservo.write(109);    //open position
//  delay(50);
//
//  delay(5000);
//  
}
