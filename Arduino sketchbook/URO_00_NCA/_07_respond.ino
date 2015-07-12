void respond()
{
  signalToServo();
  audioOut();
  ledControl();
}

void signalToServo()
{
  byte signalServo1 = map(arrayOutputValues[7], 0, 65535, 15, 100); // Tested safe range for Turnigy TGY-R5180MG: 15' - 100'
  byte signalServo2 = map(arrayOutputValues[6], 0, 65535, 15, 100); // Tested safe range for Turnigy TGY-R5180MG: 15' - 100'
  byte signalServo3 = map(arrayOutputValues[5], 0, 65535, 15, 100); // Tested safe range for Turnigy TGY-R5180MG: 15' - 100'

  byte firstValue = 0;
  byte secondValue = 0;
  Servo servoToActivate1;
  Servo servoToActivate2;
  Servo servoToReset; 
  //servo1.attach(pinServo1);
  //servo2.attach(pinServo2);
  //servo3.attach(pinServo3);  
  for (byte i = 0; i < 3; i++) 
  {
    if (signalServo1 > firstValue) 
    {
      secondValue = firstValue;
      firstValue = signalServo1;
      servoToReset = servoToActivate2;
      servoToActivate2 = servoToActivate1;
      servoToActivate1 = servo1; 
    }
    if (signalServo2 > firstValue) 
    {
      secondValue = firstValue;
      firstValue = signalServo2;
      servoToReset = servoToActivate2;
      servoToActivate2 = servoToActivate1;
      servoToActivate1 = servo2;  
    }
    if (signalServo3 > firstValue) 
    {
      secondValue = firstValue;
      firstValue = signalServo3;
      servoToReset = servoToActivate2;
      servoToActivate2 = servoToActivate1;
      servoToActivate1 = servo3; 
    }
  }
  unsigned int combinedTwoServoAngleMax = 300;
  unsigned int combinedTwoServoAngle = firstValue + secondValue; 
  if (combinedTwoServoAngle < combinedTwoServoAngleMax)
  {
      servoToActivate1.write(firstValue);
      servoToActivate2.write(secondValue);
  }
  else
  {
      servoToActivate1.write(firstValue);
      servoToActivate2.write(secondValue - (combinedTwoServoAngle-combinedTwoServoAngleMax));
  }
  servoToReset.write(0);
  //delay(1000);
  //servo1.detach();
  //servo2.detach();
  //servo3.detach();
}

void audioOut()
{
  unsigned int frequency = (unsigned int)map(arrayOutputValues[3], 0, 65535, 0, 20000);
  unsigned long duration = (unsigned int)map(arrayOutputValues[4], 0, 65535, 0, 10000);
  if (duration >= 1) 
  {
    tone(pinAudioOut, frequency, duration);
  }
}

void ledControl()
{
  unsigned int ledValue1 = (unsigned int)map(arrayOutputValues[0], 0, 65535, 0, 255);
  unsigned int ledValue2 = (unsigned int)map(arrayOutputValues[1], 0, 65535, 0, 255);
  unsigned int ledValue3 = (unsigned int)map(arrayOutputValues[2], 0, 65535, 0, 255);
  
  analogWrite(pinLED1, ledValue1);
  analogWrite(pinLED2, ledValue2);
  analogWrite(pinLED3, ledValue3);
}

