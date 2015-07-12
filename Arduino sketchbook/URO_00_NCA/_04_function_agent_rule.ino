/*******
 * NCA agent behavior rules
 *******/


void activateLayerNCA()
{
  for (unsigned int agentID = 0; agentID < numOfNCA; agentID++)
  {
    executeAgentRulesChain(agentID);
  }
}


// Execute the chain of agent behavior rules
void executeAgentRulesChain(unsigned int agentID)
{
  /* 
   * Rule 1: Move 
   */

  // Agent moves based on its Inertia direction if it is not in a Fixed state
  if (layerNCA[agentID].caAgentFixedState == 0)
  {
    moveAgent(agentID);
  }
  
  // If after movement Agent is above an active Input cell its Action Potential increases
  if (checkForActiveInputCellBelow(agentID) == true) 
  {
    increaseAgentActionPotential(agentID);
  } 
  else 
  { // its Action Potential decreases
    decreaseAgentActionPotential(agentID);
  } // end if else

  /* 
   * Rule 2: Action Potential 
   */
  
  // Check if Agent is already firing, if yes reset its Action Potential
  if (layerNCA[agentID].caAgentFiringStatus == 1) 
  {
    resetAgentActionPotential(agentID);
  } 
  else 
  { 
    // do nothing
  }
  
  // When Agent is at its maximum Action Potential, Agent "fires" up all its Action Potential
  boolean caAgentAtMaxActionPotential = checkIfAgentAtMaxActionPotential(agentID);
  if (caAgentAtMaxActionPotential == true) 
  {
    markAgentAsFiring(agentID);
    if (similarPhaseAgentHasBeenExcited == false) 
    {
      // When an agent fires, it excites other Agents in similar firing phase
      exciteSimilarPhaseAgent();
      similarPhaseAgentHasBeenExcited = true;
    } 
    else 
    { 
      // do nothing
    }
  } 
  else 
  {
    markAgentAsNotFiring(agentID);
  }
}


/*
 * "Move" functions
 */


// Move NCA agent in the directions its Inertia value indicates
void moveAgent(unsigned int agentID)
{
  byte agentLocationCellID = layerNCA[agentID].caAgentLocationCellID;
  byte neighborN, neighborNE, neighborE, neighborSE, neighborS, neighborSW, neighborW, neighborNW;
  byte agentFixedState = layerNCA[agentID].caAgentFixedState;

  // Directional coding scheme
  /* NW  N  NE       7 0 1
      W  c  E        6   2
     SW  S  SE       5 4 3 */

  // First get neighbor Cell ID values with top-bottom left-right border wrapping, illustrated below
  /* 35   30 31 32 33 34 35   30
   
      5    0  1  2  3  4  5    0
     11    6  7  8  9 10 11    6
     17   12 13 14 15 16 17   12
     23   18 19 20 21 22 23   18
     29   24 25 26 27 28 29   24
     35   30 31 32 33 34 35   30
   
      5    0  1  2  3  4  5    0 */

  // Get grid cell ID for Neighbor N (North), with border wrap top-to-bottom
  if ((0 <= agentLocationCellID) && (agentLocationCellID < gridWorldSizeX)) 
  {
    neighborN = agentLocationCellID + gridWorldMaxNumOfCell - gridWorldSizeX;
  } 
  else 
  {
    neighborN = agentLocationCellID - gridWorldSizeX;
  }

  // Get grid cell ID for Neighbor S (South), with border wrap bottom-to-top
  if (((gridWorldMaxNumOfCell - gridWorldSizeX) <= agentLocationCellID) && (agentLocationCellID < gridWorldMaxNumOfCell)) 
  {
    neighborS = agentLocationCellID % gridWorldSizeX;
  } 
  else 
  {
    neighborS = agentLocationCellID + gridWorldSizeX;
  }

  // Get grid cell ID for Neighbor W (West), with border wrap left-to-right
  if ((agentLocationCellID % gridWorldSizeX) == 0) 
  {
    neighborW = agentLocationCellID + gridWorldSizeX - 1;
  } 
  else 
  {
    neighborW = agentLocationCellID -1;
  }

  // Get grid cell ID for Neighbor E (East), with border wrap right-to-left
  if ((agentLocationCellID % gridWorldSizeX) == (gridWorldSizeX - 1)) 
  {
    neighborE = agentLocationCellID - gridWorldSizeX + 1;
  } 
  else 
  {
    neighborE = agentLocationCellID +1;
  }

  // Get grid cell ID for Neighbor NW (North-West), with border wrap NW corner
  if (agentLocationCellID == 0) 
  {
    neighborNW = gridWorldMaxNumOfCell - 1;
  } 
  else 
  {
    neighborNW = neighborN - 1;
    if ((neighborNW < 0) || (neighborNW > (gridWorldMaxNumOfCell - 1))) 
    {
      neighborNW = neighborW - gridWorldSizeX;
    } 
    else 
    { 
      // do nothing
    }
  } 

  // Get grid cell ID for Neighbor NE (North-East) with border wrap NE corner
  if (agentLocationCellID == (gridWorldSizeX - 1)) 
  {
    neighborNE = gridWorldMaxNumOfCell - gridWorldSizeX;
  } 
  else {
    neighborNE = neighborN + 1;
    if ((neighborNE < 0) || (neighborNE > ( - 1))) 
    {
      neighborNE = neighborE - gridWorldSizeX;
    } 
    else 
    { 
      // do nothing
    } 
  } 

  // Get grid cell ID for Neighbor SW (South-West), with border wrap SW corner
  if (agentLocationCellID == (gridWorldMaxNumOfCell - gridWorldSizeX)) 
  {
    neighborSW = gridWorldSizeX - 1;
  } 
  else 
  {
    neighborSW = neighborS - 1;
    if ((neighborSW < 0) || (neighborSW > (gridWorldMaxNumOfCell - 1))) 
    {
      neighborSW = neighborW + gridWorldSizeX;
    } 
    else 
    { 
      // do nothing
    } 
  }  

  // Get grid cell ID for Neighbor SE (South-East), with border wrap SE corner
  if (agentLocationCellID == (gridWorldMaxNumOfCell - 1)) 
  {
    neighborSE = 0;
  } 
  else 
  {
    neighborSE = neighborS + 1;
    if ((neighborSE < 0) || (neighborSE > (gridWorldMaxNumOfCell - 1))) 
    {
      neighborSE = neighborE + gridWorldSizeX;
    } 
    else 
    { 
      // do nothing
    }
  } 

  // Then move Agent based on its Inertia direction 
  if (layerNCA[agentID].caAgentInertiaDirection == 0) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborN;
    //Serial.println("AgentMovedN");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 1) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborNE;
    //Serial.println("AgentMovedNE");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 2) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborE;
    //Serial.println("AgentMovedE");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 3) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborSE;
    //Serial.println("AgentMoveSE");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 4) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborS;
    //Serial.println("AgentMovedS");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 5) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborSW;
    //Serial.println("AgentMovedSW");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 6) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborW;
    //Serial.println("AgentMovedW");
  } 
  else if (layerNCA[agentID].caAgentInertiaDirection == 7) 
  {    
    layerNCA[agentID].caAgentLocationCellID = neighborNW;
    //Serial.println("AgentMovedNW");
  } 
  else 
  { 
    // do nothing
  }

}


// Check for active Input cell on the Input layer below
boolean checkForActiveInputCellBelow (unsigned int agentID) 
{
  byte agentLocationCellID = layerNCA[agentID].caAgentLocationCellID;
  byte arrayLayerInputIndexToRead = (unsigned int)agentLocationCellID / gridWorldSizeX;
  byte arrayLayerInputBitToRead = gridWorldSizeX - (agentLocationCellID % gridWorldSizeX) - 1;
  if (bitRead(arrayLayerInput[arrayLayerInputIndexToRead], arrayLayerInputBitToRead ) == 1) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
} // end checkForActiveInputCellBelow


/* 
 * "Action Potential" functions 
 */
 
 
// Agent Action Potential increases when "touching" active cell below on Input layer
void increaseAgentActionPotential (unsigned int agentID) 
{
  // If Agent Action potential is not full yet, add to it
  if (layerNCA[agentID].caAgentActionPotential < caAgentMaxActionPotential) 
  {
    layerNCA[agentID].caAgentActionPotential++;
  } 
}


// At each turn, if Agent does not touch an active Input cell, its Action Potential leaks away
void decreaseAgentActionPotential (unsigned int agentID) 
{
  // If Agent Action potential is not already empty, decrease it
  if (layerNCA[agentID].caAgentActionPotential > 0) 
  {
    layerNCA[agentID].caAgentActionPotential--;
  } 
}


// Check if Agent at maximum Action Patential
boolean checkIfAgentAtMaxActionPotential (unsigned int agentID) 
{
  boolean agentAtMaxActionPotential = false;
  // If Agent Action Potential is equal to given max potential, agent fires
  if (layerNCA[agentID].caAgentActionPotential == caAgentMaxActionPotential) 
  {
    agentAtMaxActionPotential = true;
    //Serial.print(layerNCA[agentID].caAgentActionPotential);Serial.print("\t");Serial.print(caAgentMaxActionPotential);Serial.print("\t");Serial.println("AgentAtMaxAP=TRUE");
  } 
  else 
  {
    agentAtMaxActionPotential = false;
    //Serial.print(layerNCA[agentID].caAgentActionPotential);Serial.print("\t");Serial.print(caAgentMaxActionPotential);Serial.print("\t");Serial.println("AgentAtMaxAP=FALSE");
  }
  return agentAtMaxActionPotential;
} 


// Mark Agent as firing
void markAgentAsFiring (unsigned int agentID) 
{
  layerNCA[agentID].caAgentFiringStatus = 1;
}


// Mark Agent as not firing
void markAgentAsNotFiring (unsigned int agentID) 
{
  layerNCA[agentID].caAgentFiringStatus = 0;
}


// Reset Agent Action Potential after it fires
void resetAgentActionPotential (unsigned int agentID) 
{
  layerNCA[agentID].caAgentActionPotential = 0;
}


// Give other Agent in similar Action Potential excitation phase a boost
void exciteSimilarPhaseAgent() 
{
  for (unsigned int agentID = 0; agentID < numOfNCA; agentID++) 
  {
    if (layerNCA[agentID].caAgentActionPotential == caAgentMaxActionPotential - 1 || layerNCA[agentID].caAgentActionPotential == 0) 
    {
      layerNCA[agentID].caAgentActionPotential++;
    } 
    else 
    { 
      // do nothing
    }
  }
}
