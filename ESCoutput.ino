#include <Servo.h>
#define RCPin 2 //receiver connected to pin 2

byte servoPin = 9; // esc connected to pin 9
Servo servo; // establishing servo connection
int RCValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RCPin, INPUT); // beginning reciever transmission
  servo.attach(servoPin); // getting esc connection started 
  servo.writeMicroseconds(1000); // reset (nothing)
  delay(7000); // delay for ESC to startup fully

}

void loop() {
  // put your main code here, to run repeatedly:
  RCValue = pulseIn(RCPin, HIGH); // reading values from PWM reciever
  Serial.println(RCValue);
  
  if(RCValue < 1000 || RCValue > 2000) // making sure we dont send any invalid numbers to the esc
  {
    Serial.println("not valid");
  }
  else
  {
    servo.writeMicroseconds(RCValue); // send signal to ESC of whatever RC is at 
  }

}
