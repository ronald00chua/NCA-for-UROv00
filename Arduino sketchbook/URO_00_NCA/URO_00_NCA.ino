
/******************************************
 * Neural Cellular Automata (NCA)
 * by Ronald Chua
 * Contact: piezo.crystal@gmail.com
 * Version timestamp: 20141207 0253
 * This work is licensed under: 
 *    "Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License".
 *    http://creativecommons.org/licenses/by-nc-sa/3.0/
 ******************************************/

/****************************** 
   Libraries 
*/

#include <Wire.h> // For I2C protocol
#include <SimpleMPU6050.h> // My custom library for the MPU-6050 Six-Axis (Gyro + Accelerometer)
#include "mpr121.h" // Library for the MPR121 capacitive sensing 
#include <Servo.h> // Servo
#include <EEPROM.h> // Access built-on-chip flash storage memory


/****************************** 
   Inputs list 
*/

// MPU-6050 on GY-521 (sub-total: 7 input lines)
//// Line 1 - acceleration X
//// Line 2 - acceleration Y
//// Line 3 - acceleration Z
//// Line 4 - rotation X
//// Line 5 - rotation Y
//// Line 6 - rotation Z
//// Line 7 - temperature

// MPR121 (sub-total: 8 input lines , max possible: 12)
//// Line 1 - vertebra 6
//// Line 2 - vertebra 5
//// Line 3 - vertebra 4
//// Line 4 - vertebra 3
//// Line 5 - vertebra 2
//// Line 6 - vertebra 1
//// Line 7 - head side2
//// Line 8 - head side1

// LDR (sub-total: 2 input lines)
//// Line 1 - brightness LDR 1
//// Line 2 - brightness LDR 2

#define numOfInputLine     17 // total number of input lines (7 + 8 + 2 = 17)


/****************************** 
   Outputs list 
*/

// Servo (sub-total: 3 output lines)
//// Line 1 - servoA
//// Line 2 - servoB
//// Line 3 - servoC

// Speaker (sub-total: 1 output line)
//// Line 1 - speaker

// RGB LED (sub-total: 3 output line)
//// Line 1 - color R
//// Line 2 - color G
//// Line 3 - color B

#define numOfOutputLine    7 // total number of output lines ( 3 + 1 + 3 = 7 )


/****************************** 
   Pin numbers 
*/

// Input
#define irqpin            11  // IRQ pin on the MPR121 breakout board

// Output
#define pinAudioOut       3   // PWM
#define pinServo1         21 
#define pinServo2         22 
#define pinServo3         23 
#define pinLED1           12  // PWM
#define pinLED2           13  // PWM
#define pinLED3           14  // PWM


/****************************** 
   NCA grid world configuration and other variables 
*/

#define gridWorldSizeX 16 // Keep this at 16, easy conversion of grid world values as 16bit numbers.
#define gridWorldSizeY 16
#define gridWorldMaxNumOfCell 256 // gridWorldSizeX x gridWorldSizeY = 16 x 16 = 256
#define numOfNCA 256 // Hardware memory and speed limitation, max = 384 on ATMEGA328
#define caAgentMaxActionPotential 7 // Max value is 7; 2^3 = 8 will cause binary overflow
// Above settings are the maximum safe limit for ATMEGA328

/* Create container global variables */
unsigned int arrayInputValues[numOfInputLine];
unsigned int arrayLayerInput[gridWorldSizeY];
unsigned int arrayLayerOutput[gridWorldSizeY];
unsigned int arrayOutputValues[numOfOutputLine];

/* Some pre-defined maths */
#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] )) // For getting 1D array length

/* Create hardware instance */
SimpleMPU6050 myMPU6050; // Instantiate my MPU-6050 on the GY-521 breakout board using my custom library "SimpleMPU6050"
Servo servo1, servo2, servo3;

/* Marker variables */ 
boolean similarPhaseAgentHasBeenExcited = false; // Mark if CA Agent which fired has excited other agents in similar phase
unsigned long timeStart; // Mark time between saves

/* Array to store touch states */
boolean touchStates[12]; // Max total of 12, using only first 8

/****************************** 
   SETUP 
*/

void setup()
{
  /* Pin modes */
  pinMode(irqpin,INPUT);
  pinMode(pinAudioOut,OUTPUT);
  pinMode(pinServo1,OUTPUT);
  pinMode(pinServo2,OUTPUT);
  pinMode(pinServo3,OUTPUT);
  pinMode(pinLED1,OUTPUT);
  pinMode(pinLED2,OUTPUT);
  pinMode(pinLED3,OUTPUT);

  digitalWrite(irqpin, HIGH);  // Enable pullup resistor

  /* Check if memory exist, else create new one */
  // ...

  /* Miscellaneous */
  timeStart = millis(); // Record the time the entire system is started up
  myMPU6050.initializeInstance(); // Actual SimpleMPU6050 instance is empty, this fully initializes it
  
  Serial.begin(9600);
  Wire.begin();
  
  mpr121_setup();
  
  delay(1000); // Wait for things to settle a bit
  Serial.println("Setup completed.");
}


/****************************** 
   LOOP 
*/

void loop()
{  
  unsigned long timeStartLoop = millis(); // get start time

  /* Begin pseudo-parallel routines */
  sensorReadAll(); //Serial.println("All sensors read.");
  inputToGridWorld(); //Serial.println("Input to gw done.");
  populateLayerNCA(); //Serial.println("NCA layer populated.");
  activateLayerNCA(); //Serial.println("NCA layer active.");
  gridWorldToOutput(); //Serial.println("Gw to output done.");

  /* Store NCA layer state about every 30 minutes (1800000 milliseconds) into EEPROM */
  unsigned long timeLapsed = millis()-timeStart;
  if (timeLapsed/1800000 >= 1) 
  {
    // Convert NCA layer to storable data type then store
    // ...
    timeStart = millis(); // Record last save time
  }
  
}


/****************************** 
   Debug stuffs 
*/

/*

  unsigned long timeTaken = millis() - timeStartLoop; // calculate time take to complete single loop
  
  Serial.print("Time taken: "); Serial.println(timeTaken); // print out the time taken
  
  delay(1000); // Wait 1s, don't flood serial
  
  Serial.println("");
  Serial.println("IN");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayInputValues ); i++)
  {
    Serial.println(arrayInputValues[i]);
  }
  Serial.println("");
  
  delay(1000); // Wait 1s, don't flood serial
  
  Serial.println("");
  Serial.println("L INPUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayLayerInput ); i++)
  {
    Serial.println(arrayLayerInput[i], BIN);
  }
  Serial.println("");
  
  delay(1000); // Wait 1s, don't flood serial
  
  Serial.println("");
  Serial.println("L OUTPUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayLayerOutput ); i++)
  {
    Serial.println(arrayLayerOutput[i], BIN);
  }
  Serial.println("");
  
  delay(1000); // Wait 1s, don't flood serial
  
  Serial.println("");
  Serial.println("OUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayOutputValues ); i++)
  {
    Serial.println(arrayOutputValues[i]);
  }
  Serial.println("");
  
  delay(1000); // Wait 1s, don't flood serial

*/
