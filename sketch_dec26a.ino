#include <Wire.h>

int wireAddress = byte(0x68);
int X,Y,Z;

void setup() {
  Wire.begin(wireAddress);
  Serial.begin(9600);
  delay(100);

  // setting up wire measurements
  Wire.beginTransmission(wireAddress);
  Wire.write(byte(0x6B));
  Wire.write(8);  
  Wire.endTransmission();

}

void loop() {
  // sending values
  Wire.beginTransmission(wireAddress);
  Wire.write(67); // GYRO_XOUT_H
  Wire.write(69); // GYRO_YOUT_H
  Wire.write(71); // GYRO_ZOUT_H
  Wire.endTransmission();


  Wire.requestFrom(wireAddress, 32);

  Serial.print(Wire.available()); // still need to figure out why this returns 32 bits when im only sending 24 (8*3)
  Serial.print("\n");

  // retrieving values
  if (Wire.available() <= 32) {
    X = Wire.read();
    Y = Wire.read();
    Z = Wire.read();
  }

  // printing values
  Serial.print("X: ");
  Serial.print(X);
  Serial.print(" ");
  Serial.print("Y: ");
  Serial.print(Y);
  Serial.print(" ");
  Serial.print("Z: ");
  Serial.print(Z);
  Serial.print(" ");
  Serial.print("\n");

}
