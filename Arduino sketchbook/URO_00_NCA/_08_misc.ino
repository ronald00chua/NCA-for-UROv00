void printAgent()
{
  for (unsigned int i = 0; i < numOfNCA; i++)
  {
    Serial.print(i);
    Serial.print("\t");Serial.print(layerNCA[i].caAgentLocationCellID);
    Serial.print("\t");Serial.print(layerNCA[i].caAgentActionPotential);
    Serial.print("\t");Serial.print(layerNCA[i].caAgentFiringStatus);
    Serial.print("\t");Serial.print(layerNCA[i].caAgentFixedState);
    Serial.print("\t");Serial.print(layerNCA[i].caAgentInertiaDirection);
    Serial.print("\n");
    delay(100);
  }
}


void printBinary(unsigned int value, byte numOfPlaces)
{
  for (byte i = 0; i < numOfPlaces; i++)
  {
    if( ((i % 4) == 0) && (i != 0))
    {
      Serial.print(" ");
    }
    Serial.print(bitRead(value,numOfPlaces-i-1));
  }
}


void debugDisplay() 
{
  unsigned long timeTaken = millis() - timeStart; // calculate time take to complete single loop
  Serial.print("Time taken: "); Serial.println(timeTaken); // print out the time taken
  
  Serial.println("");
  Serial.println("IN");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayInputValues ); i++)
  {
    Serial.println(arrayInputValues[i]);
  }
  Serial.println("");

  //delay(100); // Wait, don't flood serial

  Serial.println("");
  Serial.println("L INPUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayLayerInput ); i++)
  {
    //Serial.println(arrayLayerInput[i],BIN);
    printBinary(arrayLayerInput[i], 16); 
    Serial.println();
  }
  Serial.println("");

  //delay(100); // Wait, don't flood serial

  Serial.println("");
  Serial.println("L OUTPUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayLayerOutput ); i++)
  {
    //Serial.println(arrayLayerOutput[i],BIN);
    printBinary(arrayLayerOutput[i], 16);
    Serial.println();
  }
  Serial.println("");

  //delay(100); // Wait, don't flood serial

  Serial.println("");
  Serial.println("OUT");
  for (unsigned int i = 0; i < SIZEOF_ARRAY( arrayOutputValues ); i++)
  {
    Serial.println(arrayOutputValues[i]);
  }
  Serial.println("");

  //delay(100); // Wait, don't flood serial
}
