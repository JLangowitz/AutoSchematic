
#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
Servo cutter;
 
int pos = 0;    // variable to store the servo position
 
void setup()
{
//  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  cutter.attach(10);
  cutter.write(55);
}
 
 
void loop()
{
//  for(pos = 150; pos < 165; pos += 1)  // goes from 0 degrees to 180 degrees
//  {                                  // in steps of 1 degree
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(5);                       // waits 15ms for the servo to reach the position
//  }
//  for(pos = 170; pos>=140; pos-=1)     // goes from 180 degrees to 0 degrees
//  {                                
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(5);                       // waits 15ms for the servo to reach the position
//  }
//  delay(5000);
}
