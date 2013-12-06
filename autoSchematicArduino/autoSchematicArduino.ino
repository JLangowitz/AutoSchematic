
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

// Motor initializations
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

//Serial stuff
char command;
int length;
int serialState=0; //tells whether to read a command, length, or data

void setup() {
  Serial.begin(9600);
  Serial.flush();

  
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  myMotor->setSpeed(10);  // 10 rpm 
  
  // let server know arduino is ready
  Serial.println("ALIVE");
}

void loop() {
  //Serial.print(Serial.available());
  getCommand();
}

//Uses serial port to get commands from python and then execute them
void getCommand(){
  //Serial.print(serialState);

  switch (serialState){
    case 0:
      if (Serial.available()>0) {
        command=Serial.read();
        Serial.print(command);
        serialState++;
      }
      break;
    case 1:
      if (Serial.available()>0) {
        length=Serial.read();
        serialState++;
        Serial.print(length);
      }
      break;
    case 2:
      if (Serial.available()>=length) {
        //char data[length-2];
        //Serial.readBytes(data, length);
        char data=Serial.read();
        executeCommand(command, data);
        serialState=0;
        cutWire(data);
      }
      break;
    default:
      break;
  }
}

void executeCommand(char command, int data){
  switch (command){
    case 'W':
      cutWire(data);
//      Serial.write('d');
      break;
    default:
      break;  
  }
}

void cutWire(int data){
  //do some wire logic
  Serial.println(data);
}
