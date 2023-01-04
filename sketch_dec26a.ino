#include <Wire.h>

// declaring vars and address of mpu
int wireAddress = 0x68;
float accX,accY,accZ;
float gyroX,gyroY,gyroZ;
float pitch,roll;

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
  // sending values for accelaration
  Wire.beginTransmission(wireAddress);
  Wire.write(0x3B); // ACCEL_XOUT[15:8]
  Wire.endTransmission(false);


  Wire.requestFrom(wireAddress, 6, true); // reading accelerometer data (next 6 registers)

  // retrieving values - shifting 15:8 over 8 bits to convert back to original number
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0; // 16384.0 = sensitivity
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  // converting to pitch/roll
  accX = accX * 9.8;
  accY = accY * 9.8;
  accZ = accZ * 9.8;

  pitch = asinf(accX / 9.8);
  roll = atan2f(accY, accZ);

  // rad to deg
  pitch = (pitch * (180/3.14));
  roll = (roll * (180/3.14));

  // reading for gyroscope
  Wire.beginTransmission(wireAddress);
  Wire.write(0x43); // ACCEL_XOUT[15:8]
  Wire.endTransmission(false);


  Wire.requestFrom(wireAddress, 6, true); // reading gyro data (next 6 registers)

  // retrieving values - shifting 15:8 over 8 bits to convert back to original number
  gyroX = (Wire.read() << 8 | Wire.read()) / 131;
  gyroY = (Wire.read() << 8 | Wire.read()) / 131;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 131;

  // printing values
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print("\n");

  Serial.print("gyro X: ");
  Serial.print(gyroX);
  Serial.print(" ");
  Serial.print("gyro Y: ");
  Serial.print(gyroY);
  Serial.print(" ");
  Serial.print("gyro Z: ");
  Serial.print(gyroZ);
  Serial.print(" ");
  Serial.print("\n");

  delay(100);
}
