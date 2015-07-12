/* Read values from all the sensors */

void sensorReadAll()
{
  sensorReadMPU6050();
  sensorReadMPR121();
  sensorReadLDR();
}


/******************************  
  Read values from the MPU-6050 
*/
void sensorReadMPU6050() 
{
  // Start read from the MPU-6050
  myMPU6050.initiateRead();
  // Convert each MPU-6050 raw value to unsigned 16 bit and store in Input binary matrix
  arrayInputValues[0] = (unsigned int)map(myMPU6050.getAccelX(), -32765, 32765, 0, 65535);
  arrayInputValues[1] = (unsigned int)map(myMPU6050.getAccelY(), -32765, 32765, 0, 65535);
  arrayInputValues[2] = (unsigned int)map(myMPU6050.getAccelZ(), -32765, 32765, 0, 65535);
  arrayInputValues[3] = (unsigned int)map(myMPU6050.getGyroX(), -32765, 32765, 0, 65535);
  arrayInputValues[4] = (unsigned int)map(myMPU6050.getGyroY(), -32765, 32765, 0, 65535);
  arrayInputValues[5] = (unsigned int)map(myMPU6050.getGyroZ(), -32765, 32765, 0, 65535);
  arrayInputValues[6] = (unsigned int)map(myMPU6050.getTemperature(), -32765, 32765, 0, 65535);
}


/****************************** 
  Read values from the MPR121
*/

void sensorReadMPR121()
{
  if(!checkInterrupt())
  {  
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    for (int i=0; i < 8; i++){  // Check what electrodes were pressed
      if(touched & (1<<i)){
        if(touchStates[i] == 0){
          // Convert touch to incremental value in the Input binary matrix
          if (i == 7) {
            arrayInputValues[i+7] = (i) * (65535/8); // Both sides of head same touch effect, avoid max affect
          }
          else {
            arrayInputValues[i+7] = (i+1) * (65535/8); // Divide touch effect to portion of 8, increment effect nearer to head
          }
        }else if(touchStates[i] == 1){
          //pin i is still being touched
          // Convert touch to incremental value in the Input binary matrix
          if (i == 7) {
            arrayInputValues[i+7] = (i) * (65535/8); // Both sides of head same touch effect, avoid max affect
          }
          else {
            arrayInputValues[i+7] = (i+1) * (65535/8); // Divide touch effect to portion of 8, increment effect nearer to head
          }
        }  
        touchStates[i] = 1;      
      }else{
        if(touchStates[i] == 1){
          //pin i is no longer being touched
          // Null touch values in the Input binary matrix
          arrayInputValues[i+7] = 0; // Both sides of head same touch effect, avoid max affect
        }
        touchStates[i] = 0;
      }
    }
    
    // NOTE: Input binary matrix used up to -> arrayInputValues[14] 
  }
}

void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);
 
  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);
  
  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);
  
  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);
  
  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);
  
  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);
  
  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}


boolean checkInterrupt(void){
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}


/****************************** 
  Read values from the LDR
*/

void sensorReadLDR()
{
  // Convert each LDR raw value to unsigned 16 bit and store in Input binary matrix
  arrayInputValues[15] = (unsigned int)map(analogRead(A0), 0, 1023, 0, 65535);  // LDR 1
  arrayInputValues[16] = (unsigned int)map(analogRead(A1), 0, 1023, 0, 65535);  // LDR 2
}
