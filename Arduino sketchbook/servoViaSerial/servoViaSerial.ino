#include <Servo.h>

Servo servoA; 
Servo servoB;
Servo servoC;

void setup() {
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  
  servoA.attach(8);
  servoB.attach(12);
  servoC.attach(13);
  
  Serial.begin(9600);
  Serial.println("Ready");
  Serial.flush();
}

void loop() {

  static int v = 0;

  if(Serial.available()) {
    char ch = Serial.read();

    switch(ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 'a':
        servoA.write(v);
        v = 0;
        break;
      case 'b':
        servoB.write(v);
        v = 0;
        break;
      case 'c':
        servoC.write(v);
        v = 0;
        break;        
    }
  }

} 
