

#include <Servo.h>
 
Servo bender;

int pos = 0;    // variable to store the servo position
int i = 0;

void setup() {
  //bender servo
  bender.attach(11); 
  bender.write(10);  //open position
  
  delay(1000);
  
  bender.write(55);
  
  Serial.begin(9600);

}

void loop() {
    bender.write(85);
}


