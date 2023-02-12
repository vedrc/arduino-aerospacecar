#include <Servo.h>
#define thrustPin 3 // thrust channel connected to pin 2
#define yawPin 2 // yaw channel connected to pin 3

byte ccwPin = 9; // esc connected to pin 9
byte cwPin = 10;
Servo servoCcw;
Servo servoCw; // establishing servo connection
int thrustValue,yawValue,totalValueCw,totalValueCcw;

void setup() {
  Serial.begin(9600);
  pinMode(thrustPin, INPUT); // beginning reciever transmission
  pinMode(yawPin, INPUT);
  servoCw.attach(cwPin); // getting esc connection started
  servoCcw.attach(ccwPin); 
  servoCw.writeMicroseconds(1000); // reset (nothing)
  servoCcw.writeMicroseconds(1000);
  delay(7000); // delay for ESC to startup fully

}

void loop() {
  thrustValue = pulseIn(thrustPin, HIGH); // reading values from PWM reciever
  yawValue = pulseIn(yawPin, HIGH);
  
  if (thrustValue < 1000 || thrustValue > 2000) // making sure we dont send any invalid numbers to the esc
  {
    Serial.println("not valid");
  }
  else {
    totalValueCw = thrustValue - ((yawValue - 1500)/4); // combining yaw and thrust - yaw changes total value by factor of 1/4
    totalValueCcw = thrustValue + ((yawValue - 1500)/4); // combining yaw and thrust - yaw changes total value by factor of 1/4
    Serial.println(totalValueCw);
    Serial.println(totalValueCcw);
    servoCcw.writeMicroseconds(totalValueCcw); // send signal to ESC of whatever RC is at 
    servoCw.writeMicroseconds(totalValueCw);



  }

}
