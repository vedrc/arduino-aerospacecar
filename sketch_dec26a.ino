#include <Wire.h>

// declaring vars and address of mpu (0x68)
int mpuAddress = 0x68;
float accX,accY,accZ;
float gyroX,gyroY,gyroZ;
float pitch,roll;
float accErrorX,accErrorY,accErrorZ;
float gyroErrorX,gyroErrorY,gyroErrorZ;

int c = 0;

void setup() {
  Wire.begin(mpuAddress);

  // 9600 baud set in serial monitor
  Serial.begin(9600);

  // setting up wire measurements with i2c initiation
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0x00);  // reset
  Wire.endTransmission(true);

  // recommended delay for optimal performance
  delay(100);

  calculateError();

  
}

void loop() {
  // requesting an i2c transmission from 0x3B to get the current data for that register
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x3B); // ACCEL_XOUT[15:8]
  Wire.endTransmission(false);


  Wire.requestFrom(mpuAddress, 6, true); // reading accelerometer data (next 6 registers)

  // retrieving values - shifting 15:8 over 8 bits to convert back to original number
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0; // 16384.0 = sensitivity
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  // correcting with inputs from below
  accX = accX - (-0.02);
  accY = accY;
  accZ = accZ - (-2);

  
  // converting to pitch/roll
  // not sure of the exact physics yet but this could be useful to determine the current positioning of the car
  // also a more testable value that can be easily looked at to see if its working
  accX = accX * 9.8;
  accY = accY * 9.8;
  accZ = accZ * 9.8;

  pitch = asinf(accX / 9.8);
  roll = atan2f(accY, accZ);
  
  // converting from rad to deg
  pitch = (pitch * (180/3.14));
  roll = (roll * (180/3.14));
  

  // reading register value (0x43) for gyroscopic data
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x43); // GYRO_XOUT[15:8]
  Wire.endTransmission(false);


  Wire.requestFrom(mpuAddress, 6, true); // reading gyro data (next 6 registers)

  // retrieving values - shifting 15:8 over 8 bits to convert back to original number
  gyroX = (Wire.read() << 8 | Wire.read()) / 131; // 131 = gyro sensitivity
  gyroY = (Wire.read() << 8 | Wire.read()) / 131;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 131;

  // correcting with gyro error
  gyroX = gyroX - (-9);
  gyroY = gyroY;
  gyroZ = gyroZ - (-1000);

  
  // printing values

  Serial.print("pitch: ");
  Serial.print(pitch);
  Serial.print(" roll: ");
  Serial.print(roll);
  Serial.print("\n");
  
  /*
  Serial.print("Acc X: ");
  Serial.print(accX);
  Serial.print(" ");
  Serial.print("Acc Y: ");
  Serial.print(accY);
  Serial.print("Acc Z: ");
  Serial.print(accZ);
  Serial.print("\n");
  */

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

  delay(500);

}

// will update with error code in the future
void calculateError() {
  // calculating error value by finding the different values it gets for one position, then averaging it out
  // accelerometer averaging
  while (c < 500) {
    Wire.beginTransmission(mpuAddress);
    Wire.write(0x3B); // ACCEL_XOUT[15:8]
    Wire.endTransmission(false);
    Wire.requestFrom(mpuAddress, 6, true);
    accX = (Wire.read() << 8 | Wire.read()) / 16384.0; 
    accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
    accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

    accErrorX = accErrorX + accX;
    accErrorY = accErrorY + accY;
    accErrorZ = accErrorZ + accZ;

    c++;
  }

  accErrorX = accErrorX / 500;
  accErrorY = accErrorY / 500;
  accErrorZ = accErrorZ / 500;
  c = 0;

  // gyro averaging
  while (c < 500) {
    Wire.beginTransmission(mpuAddress);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(mpuAddress, 6, true);
    gyroX = (Wire.read() << 8 | Wire.read()) / 131;
    gyroY = (Wire.read() << 8 | Wire.read()) / 131;
    gyroZ = (Wire.read() << 8 | Wire.read()) / 131;

    gyroErrorX = gyroErrorX + gyroX;
    gyroErrorY = gyroErrorY + gyroY;
    gyroErrorZ = gyroErrorZ + gyroZ;

    c++;
  }

  gyroErrorX = gyroErrorX / 500;
  gyroErrorY = gyroErrorY / 500;
  accErrorZ = gyroErrorZ / 500;
  c = 0;


  
}
