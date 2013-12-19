
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

int bend_dist = 50;

Servo stripper;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
int strip_pin = 9;
int s_closed = 120;
int s_open = 175;
int s_LED = 2;

Servo cutter;

int cut_pin = 10;
int c_closed = 120;
int c_open = 55;
int c_LED = 3;

Servo bender;

int bend_pin = 11;
int b_closed = 95;
int b_open = 10;
int b_LED = 4;

int pos = 0;    // variable to store the servo position

// Motor initializations
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *feeder = AFMS.getStepper(200, 2);

//Serial stuff
char command;
int length;
int serialState=0; //tells whether to read a command, length, or data

void setup() {
  Serial.begin(9600);
  Serial.flush();

  pinMode(s_LED, OUTPUT);
  stripper.attach(strip_pin); 
  stripper.write(s_open);
  
  //cutter servo
  pinMode(c_LED, OUTPUT);
  cutter.attach(cut_pin);
  cutter.write(c_open);
  
  //bender servo
  pinMode(b_LED, OUTPUT);
  bender.attach(bend_pin);
  bender.write(b_open);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  feeder->setSpeed(3);  // 3 steps/second 
  
  while(Serial.available() == 0) {
  // let server know arduino is ready
    Serial.println("ALIVE");
    delay(1000);
  }
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
  cut_wire(data);
  Serial.println(data);
  
  delay(1000);
}

void cut_wire(int steps) {
  if(steps < bend_dist) {
    //first strip-cut
    strip(1);
    dislodge();
    
    //feed steps
    feed(steps);
    
    //second strip-cut
    strip(1);
    dislodge();
    
    //feed 40 - steps
    feed(bend_dist-steps);
    
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
    feed(bend_dist);
    bend();
    
    //feed steps-40
    feed(steps-bend_dist);
    
    //strip-cut
    strip(1);
    dislodge();
    
    //feed to strip-cut, bend
    feed(bend_dist);
    bend();
    
    //cut
    cut();
    dispense(3); 
  }
}

void feed(int steps) {
  feeder->step(steps, BACKWARD, DOUBLE);
  //feeder->release();
  delay(1000);
}

void strip(int times) {
  digitalWrite(s_LED, HIGH);
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
  digitalWrite(s_LED, LOW);
  //i=0;
}

void cut() {
  digitalWrite(c_LED, HIGH);
  cutter.write(c_closed);
  delay(1000);
  
  cutter.write(c_open);    //open position
  delay(1500);
  digitalWrite(c_LED, LOW);
}

void bend() {
  digitalWrite(b_LED, HIGH);
  bender.write(b_closed);  //closed position
  delay(1000);
  
  bender.write(b_open);  //open position
  delay(1000);
  digitalWrite(b_LED, LOW);
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
  bender.write(b_closed-20);  //closed position
  delay(2000);
  
  bender.write(b_open);  //open position
  delay(500);
  
  //jiggles the bender a certain number of times
  int i;
  for(i=0; i < times; i++) {
    bender.write(b_closed-30);
    delay(100);
    bender.write(b_open);
    delay(1000);
  }
  
}
