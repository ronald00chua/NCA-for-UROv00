/* Convert NCA agent layer interaction response with the input layer to an output layer */

void gridWorldToOutput()
{
  byte sizeArrayLayerOutput = SIZEOF_ARRAY( arrayLayerOutput );
  byte sizeArrayOutputValues = SIZEOF_ARRAY( arrayOutputValues );

  populateLayerOuput(); // Obtain the output layer from the NCA layer
  /* Adapt grid world size Y to number of output lines */
  // Use common functions for Input-Output-GridWorld inter-conversions
  if (gridWorldSizeY < numOfOutputLine)
  {
    applyRepeat(arrayLayerOutput, sizeArrayLayerOutput, arrayOutputValues, sizeArrayOutputValues);
  }
  else if (gridWorldSizeY > numOfOutputLine)
  {
    applySelection(arrayLayerOutput, sizeArrayLayerOutput, arrayOutputValues, sizeArrayOutputValues);
  }
  else 
  { 
    applyCopy(arrayLayerOutput, sizeArrayLayerOutput, arrayOutputValues, sizeArrayOutputValues); 
  }
}


void populateLayerOuput()
{
  // Reset layer output before re-populating
  for (byte i = 0; i < gridWorldSizeY; i++) 
  {
    arrayLayerOutput[i] = 0;
  } 
  
  for (unsigned int agentID = 0; agentID < numOfNCA; agentID++) 
  {
    if (layerNCA[agentID].caAgentFiringStatus == 1) 
    {
      byte agentLocationCellID = layerNCA[agentID].caAgentLocationCellID;
      byte arrayLayerOutputIndexToRead = (unsigned int)agentLocationCellID / gridWorldSizeX;
      byte arrayLayerOutputBitToRead = gridWorldSizeX - (agentLocationCellID % gridWorldSizeX) - 1;
      if ( bitRead(arrayLayerOutput[arrayLayerOutputIndexToRead], arrayLayerOutputBitToRead) == 0 ) 
      {
        bitSet(arrayLayerOutput[arrayLayerOutputIndexToRead], arrayLayerOutputBitToRead );
      } 
      else 
      {
        boolean locationPushed = false;
        unsigned int pushTarget = agentLocationCellID;
        byte pushTryCount = 0;
        while (locationPushed == false) 
        {
          pushTarget++;
          if (pushTarget == gridWorldMaxNumOfCell) // Edge-wrapping
          {
            pushTarget = 0;
            pushTryCount++;
          } 
          else 
          {
            // Determines how many times to try pushing bit location
            if (pushTryCount > 1) 
            {
              // Stop trying and ignore bit once limit reached
              // Prevents flooding of the output layer
              break;
            } 
          }
          byte arrayLayerOutputIndexToRead = (byte)pushTarget/gridWorldSizeX;
          byte arrayLayerOutputBitToRead = gridWorldSizeX - (pushTarget % gridWorldSizeX) - 1;
          if ( bitRead(arrayLayerOutput[arrayLayerOutputIndexToRead], arrayLayerOutputBitToRead) == 0 )
          {
            bitSet(arrayLayerOutput[arrayLayerOutputIndexToRead], arrayLayerOutputBitToRead );
            locationPushed = true;
          } 
          else 
          {
            locationPushed = false;
          }
        }
      }
    } 
  }
}

