
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

// Motor initializations
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *feeder = AFMS.getStepper(200, 2);

Servo stripper; 
Servo cutter;
Servo bender;

int pos = 0;    // variable to store the servo position
int i = 0;

//Serial stuff
char command;
int length;
int serialState=0; //tells whether to read a command, length, or data

void setup() {
  Serial.begin(9600);
  Serial.flush();

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
  cut_wire(5);
  
  delay(1000);
  
  cut_wire(10);
  
  delay(1000);
  
  Serial.println(data);
}

void cut_wire(int steps) {
  //first strip-cut
  //feed(15);
  strip(1);
  delay(500);
  
  //dislodge wire from stripper
  dislodge();
  
  //bend at the strip-cut
  feed(40);
  bend();
  delay(500);

  //feed out length of wire, then strip-cut
  feed(steps);
  strip(1);
  delay(500);
  
  //dislodge wire from stripper
  dislodge();
  
  //bend at the strip cut
  feed(40);
  bend();
  delay(500);
  
  //final feed/cut
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
  cutter.write(120);    //closed position
  delay(1000);
  
  cutter.write(55);    //open position
  delay(500);
  
}

void bend() {
  bender.write(85);  //closed position
  delay(1000);
  
  bender.write(10);  //open position
  delay(500);
}

void dislodge() {
  //dislodge wire from stripper
  feeder->setSpeed(10);  // 10 rpm   
  feeder->step(5, FORWARD, DOUBLE);
  delay(1000);
  feeder->step(5, BACKWARD, DOUBLE);
  delay(500);
  feeder->setSpeed(3);  // 3 rpm  
}
