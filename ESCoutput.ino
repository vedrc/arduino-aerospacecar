#include <Servo.h>
#define thrustPin 3 // thrust channel connected to pin 2
#define yawPin 2 // yaw channel connected to pin 3

byte servoPin = 9; // esc connected to pin 9
Servo servo; // establishing servo connection
int thrustValue,yawValue,totalValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(thrustPin, INPUT); // beginning reciever transmission
  pinMode(yawPin, INPUT);
  servo.attach(servoPin); // getting esc connection started 
  servo.writeMicroseconds(1000); // reset (nothing)
  delay(7000); // delay for ESC to startup fully

}

void loop() {
  // put your main code here, to run repeatedly:
  thrustValue = pulseIn(thrustPin, HIGH); // reading values from PWM reciever
  yawValue = pulseIn(yawPin, HIGH);
  
  if (thrustValue < 1000 || thrustValue > 2000) // making sure we dont send any invalid numbers to the esc
  {
    Serial.println("not valid");
  }
  else {
    totalValue = thrustValue + ((yawValue - 1500)/4); // combining yaw and thrust - yaw changes total value by factor of 1/4
    Serial.println(totalValue);
    servo.writeMicroseconds(totalValue); // send signal to ESC of whatever RC is at 



  }

}
