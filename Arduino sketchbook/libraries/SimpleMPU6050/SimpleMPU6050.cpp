/*
 * SimpleMPU6050.cpp - Library for reading raw values from an MPU-6050 (breakout board:GY-521)
 * Created by Ronald Chua
 * Version timestamp: 20121130 1830
 * Info:
 *    This is an adapted barebone version of the code by arduino.cc user "Krodal"
 *    which can be found at http://arduino.cc/playground/Main/MPU-6050
 *    I use this only to read raw values from the MPU-6050 on my GY-521 breakout board.
 * Released into the public domain.
*/


#include "Arduino.h"
#include "Wire.h"
#include "SimpleMPU6050.h"

// Register names
#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_PWR_MGMT_2         0x6C   // R/W
#define MPU6050_WHO_AM_I           0x75   // R

// Default I2C address of MPU-6050 on GY-521 
#define MPU6050_I2C_ADDRESS 0x68

// Declaring an union for the registers and the axis values.
// The byte order does not match the byte order of the compiler and AVR chip.
// The AVR chip (on the Arduino board) has the Low Byte at the lower address.
// But the MPU-6050 has a different order: High Byte at lower address, so that has to be corrected.
// The register part "reg" is only used internally, and are swapped in code.
typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } 
  reg;
  struct 
  {
    int x_accel;
    int y_accel;
    int z_accel;
    int temperature;
    int x_gyro;
    int y_gyro;
    int z_gyro;
  } 
  value;
};

accel_t_gyro_union accel_t_gyro;

SimpleMPU6050::SimpleMPU6050() 
{
  // Empty constructor
}

void SimpleMPU6050::initializeInstance() 
{
  /* Part of "void setup()" in a general Arduino sketch */
  uint8_t c;
  Wire.begin();                               // Initialize the 'Wire' class for the I2C-bus.
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);  // Clear the 'sleep' bit to start the sensor.
}

void SimpleMPU6050::initiateRead()
{
  // Read all raw values at once (accelerometer, gyroscope, temperature)
  MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));

  // Swap all high and low bytes.
  // After this, the registers values are swapped, 
  // so the structure name like x_accel_l does no 
  // longer contain the lower byte.
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
  SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
  SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
  SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
  SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
  SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
  SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
}

// Return accelerometer raw X value
int SimpleMPU6050::getAccelX()
{ return accel_t_gyro.value.x_accel; }

// Return accelerometer raw Y value
int SimpleMPU6050::getAccelY()
{ return accel_t_gyro.value.y_accel; }

// Return accelerometer raw Z value
int SimpleMPU6050::getAccelZ()
{ return accel_t_gyro.value.z_accel; }

// Return gyroscope raw X value
int SimpleMPU6050::getGyroX()
{ return accel_t_gyro.value.x_gyro; }

// Return gyroscope raw Y value
int SimpleMPU6050::getGyroY()
{ return accel_t_gyro.value.y_gyro; }

// Return gyroscope raw Z value
int SimpleMPU6050::getGyroZ()
{ return accel_t_gyro.value.z_gyro; }

// Return temperature raw value
int SimpleMPU6050::getTemperature()
{ return accel_t_gyro.value.temperature; }

/*
 * MPU6050_read
 * This is a common function to read multiple bytes from an I2C device.
 * It uses the boolean parameter for Wire.endTransMission() to be able to hold or release the I2C-bus. 
 * This is implemented in Arduino 1.0.1.
 * Only this function is used to read. 
 * There is no function for a single byte.
*/
int SimpleMPU6050::MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}


/*
 * MPU6050_write
 * This is a common function to write multiple bytes to an I2C device.
 * If only a single register is written, use the function MPU_6050_write_reg().
 * Parameters:
 *    start : Start address, use a define for the register
 *    pData : A pointer to the data to write.
 *    size  : The number of bytes to write.
 * If only a single register is written, a pointer to the data has to be used, and the size is a single byte:
 *    int data = 0;        // the data to write
 *    MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
*/
int SimpleMPU6050::MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}


/*
 * MPU6050_write_reg
 * An extra function to write a single register.
 * It is just a wrapper around the MPU_6050_write() function, and it is only a convenient 
 * function to make it easier to write a single register.
*/
int SimpleMPU6050::MPU6050_write_reg(int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}
