/*******
 * Define the basic structure and internal properties of a Neural Cellular Automaton
 *******/
 
struct nca // Define internal properties of a neural cellular automaton
{
  byte caAgentLocationCellID; // Index of location in grid world
  byte caAgentActionPotential:3;
  byte caAgentFiringStatus:1; // Is the CA agent firing? Either '1' (true) or '0' (false)
  byte caAgentFixedState:1; // Is the CA agent moving? Either '1' (true) or '0' (false)
  byte caAgentInertiaDirection:3; // Max value of 2^3 = 8, see illustration below 
  /* NW  N  NE       7 0 1
      W  C  E        6   2
     SW  S  SE       5 4 3 */
  // Total memory usage per cellular automaton = 8+3+1+1+3 = 16 bits (2 bytes)
};


struct nca layerNCA[numOfNCA]; // Create an array of neural cellular automata


void populateLayerNCA() // Populate the NCA layer in case it's empty
{
  for (unsigned int i = 0; i < numOfNCA; i++)
  {
    layerNCA[i].caAgentLocationCellID = (unsigned int)random(0,gridWorldMaxNumOfCell); // Randomly locate CA agent at start
    layerNCA[i].caAgentActionPotential = (byte)random(0,8); // Initially all CA agents have no action potential
    layerNCA[i].caAgentFiringStatus = 0;
    layerNCA[i].caAgentFixedState = 0;
    layerNCA[i].caAgentInertiaDirection = (byte)random(0,8); // Upper bound is exclusive
  }
}


void storeLayerNCA()
{
  // Convert to storable data type
  // ...
  // Write to EEPROM
  // ...
}

