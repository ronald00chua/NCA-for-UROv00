// Required libraries
#include <Wire.h>
#include <SimpleMPU6050.h>

// Instantiate the MPU-6050 on the GY-521 breakout board
SimpleMPU6050 myMPU6050;

void setup()
{
  // Opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
 
  // Actual SimpleMPU6050 instance is empty, this fully initializes it
  myMPU6050.initializeInstance(); 
  
  // Wait for things to settle a bit
  delay(1000);
}

void loop()
{
  // Read in all raw values from the MPU-6050
  myMPU6050.initiateRead(); 
  
  // Print out all raw values via serial
  // Raw values are signed 16 bits, thus its range is -32768 to +32768
  Serial.print("Accel X:"); Serial.println(myMPU6050.getAccelX());
  Serial.print("Accel Y:"); Serial.println(myMPU6050.getAccelY());
  Serial.print("Accel Z:"); Serial.println(myMPU6050.getAccelZ());
  Serial.print("Gyro X:"); Serial.println(myMPU6050.getGyroX());
  Serial.print("Gyro Y:"); Serial.println(myMPU6050.getGyroY());
  Serial.print("Gyro Z:"); Serial.println(myMPU6050.getGyroZ());
  Serial.print("Temperature:"); Serial.println(myMPU6050.getTemperature());
  Serial.println("");
 
  // Wait for 1 second before reading again
  delay(1000);
}
