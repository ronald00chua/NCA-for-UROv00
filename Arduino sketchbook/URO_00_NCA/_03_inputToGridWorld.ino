/* Converting the input values into usable input grid world layer */

void inputToGridWorld() 
{
  byte sizeArrayInputValues = SIZEOF_ARRAY( arrayInputValues );
  byte sizeArrayLayerInput = SIZEOF_ARRAY( arrayLayerInput );
  /* Adapt number of input lines to grid world size */
  if (numOfInputLine < gridWorldSizeY)
  {
    applyRepeat(arrayInputValues, sizeArrayInputValues, arrayLayerInput, sizeArrayLayerInput);
  }
  else if (numOfInputLine > gridWorldSizeY)
  {
    applySelection(arrayInputValues, sizeArrayInputValues, arrayLayerInput, sizeArrayLayerInput);
  }
  else 
  { 
    applyCopy(arrayInputValues, sizeArrayInputValues, arrayLayerInput, sizeArrayLayerInput); 
  }
}


/*
 * Common functions for Input-Output-GridWorld inter-conversions
 */

void applyRepeat(unsigned int inputArray[], byte inputArrayLength, unsigned int outputArray[], byte outputArrayLength)
{
  byte numOfRepeat = (unsigned int)(outputArrayLength - inputArrayLength)/inputArrayLength;
  byte indexArrayOutput = 0;
  byte indexArrayInput = 0;

  /* Copy whole set of the input values as many times as possible */
  for (byte repeatCount = 0; repeatCount <= numOfRepeat; repeatCount++)
  {
    for (indexArrayInput = 0; indexArrayInput < inputArrayLength; indexArrayInput++)
    {
      outputArray[indexArrayOutput] = inputArray[indexArrayInput];
      indexArrayOutput++;
    }
  }

  /* For remaining available slots, select random input values */
  for (indexArrayOutput = indexArrayOutput; indexArrayOutput < outputArrayLength; indexArrayOutput++)
  {
    outputArray[indexArrayOutput] = inputArray[(byte)random(0,inputArrayLength)];
  }
}


void applySelection(unsigned int inputArray[], byte inputArrayLength, unsigned int outputArray[], byte outputArrayLength)
{
  //byte inputArrayLength = SIZEOF_ARRAY( inputArray );
  //byte outputArrayLength = SIZEOF_ARRAY( outputArray );
  //byte indexOutputArray = 0;
  unsigned int arrayTemporary[inputArrayLength];
  boolean selectedValuesIndex[inputArrayLength];

  // Copy array
  for (byte i = 0; i < inputArrayLength; i++)
  {
    arrayTemporary[i] = inputArray[i];
  }

  // Initialize boolean array to default 'false'
  for (byte i = 0; i < inputArrayLength; i++)
  {
    selectedValuesIndex[i] = false;
  }

  // Find biggest values in the array of input values and mark the index
  for (byte i = 0; i < outputArrayLength; i++)
  {
    unsigned int biggestValue = 0;
    byte currentIndex = 0;
    for (byte i1 = 0; i1 < inputArrayLength; i1++)
    {
      if (arrayTemporary[i1] > biggestValue)
      {
        biggestValue = arrayTemporary[i1];
        currentIndex = i1;
      }
    }
    arrayTemporary[currentIndex] = 0;
    selectedValuesIndex[currentIndex] = true;
  }

  // Copy values with marked index into the matrix for grid world input
  byte i = 0;
  for (byte i1 = 0; i1 < inputArrayLength; i1++)
  {
    if (selectedValuesIndex[i1] == true)
    {
      outputArray[i] = inputArray[i1];
      i++;
    }
  }
}


void applyCopy(unsigned int inputArray[], byte inputArrayLength, unsigned int outputArray[], byte outputArrayLength) 
{
  //byte inputArrayLength = SIZEOF_ARRAY( inputArray );
  // Copy array
  for (byte i = 0; i < inputArrayLength; i++)
  {
    outputArray[i] = inputArray[i];
  }
}
