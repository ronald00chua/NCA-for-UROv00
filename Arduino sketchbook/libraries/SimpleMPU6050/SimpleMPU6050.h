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


#ifndef SimpleMPU6050_h
#define SimpleMPU6050_h

#include "Arduino.h"
#include "Wire.h"

class SimpleMPU6050
{
  public:
    SimpleMPU6050();
    void initializeInstance();
    void initiateRead();
    int getAccelX();
    int getAccelY();
    int getAccelZ();
    int getGyroX();
    int getGyroY();
    int getGyroZ();
    int getTemperature();
  private:
    int MPU6050_read(int start, uint8_t *buffer, int size);
    int MPU6050_write(int start, const uint8_t *pData, int size);
    int MPU6050_write_reg(int reg, uint8_t data);
};

#endif
