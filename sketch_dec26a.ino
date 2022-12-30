#include <Wire.h>

// declaring vars and address of mpu
int wireAddress = 0x68;
float accX,accY,accZ;

void setup() {
  Wire.begin(wireAddress);
  Serial.begin(9600);

  // setting up wire measurements with i2c initiation
  Wire.beginTransmission(wireAddress);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0x00);  // reset
  Wire.endTransmission(true);
  delay(100);

  
}

void loop() {
  // sending values
  Wire.beginTransmission(wireAddress);
  Wire.write(0x3B); // ACCEL_XOUT[15:8]
  Wire.endTransmission(false);


  Wire.requestFrom(wireAddress, 6, true); // reading accelerometer data (next 6 registers)

  // retrieving values - shifting 15:8 over 8 bits to convert back to original number
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0; // 16384.0 = sensitivity
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
  

  // printing values
  Serial.print("X: ");
  Serial.print(accX);
  Serial.print(" ");
  Serial.print("Y: ");
  Serial.print(accY);
  Serial.print(" ");
  Serial.print("Z: ");
  Serial.print(accZ);
  Serial.print(" ");
  Serial.print("\n");

  delay(500);

}
