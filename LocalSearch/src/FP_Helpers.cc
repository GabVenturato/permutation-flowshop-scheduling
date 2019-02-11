// File FP_Helpers.cc
#include "FP_Helpers.hh"

int FP_CostComponent1::ComputeCost(const FP_State& st) const
{
  unsigned cost = 0;
  // Insert the code that computes the cost for component 1 
  // of state st
	throw logic_error("FP_CostComponent1::ComputeCost not implemented yet");	
  return cost;
}
          
void FP_CostComponent1::PrintViolations(const FP_State& st, ostream& os) const
{
  // Insert the code that prints the violations of component 1 
  // of state st
	throw logic_error("FP_CostComponent1::PrintViolations not implemented yet");	
}

int FP_CostComponent2::ComputeCost(const FP_State& st) const
{ 
  unsigned cost = 0;
  // Insert the code that computes the cost for component 2
  // of state st
	throw logic_error("FP_CostComponent2::ComputeCost not implemented yet");	
  return cost;
}
 
void FP_CostComponent2::PrintViolations(const FP_State& st, ostream& os) const
{
  // Insert the code that prints the violations of component 1 
  // of state st
	throw logic_error("FP_CostComponent2::PrintViolations not implemented yet");	
}

// constructor
FP_StateManager::FP_StateManager(const FP_Input & pin) 
  : StateManager<FP_Input,FP_State>(pin, "XYZStateManager")  {} 

// initial state builder (random rooms)
void FP_StateManager::RandomState(FP_State& st) 
{
// Insert the code that creates a random state in object st
	throw logic_error("FP_StateManager::RandomState not implemented yet");	
} 

bool FP_StateManager::CheckConsistency(const FP_State& st) const
{
  // Insert the code that checks if state in object st is consistent
  // (for debugging purposes)
	throw logic_error("FP_StateManager::CheckConsistency not implemented yet");	
  return true;
}

/*****************************************************************************
 * Output Manager Methods
 *****************************************************************************/

void FP_OutputManager::InputState(FP_State& st, const FP_Output& XYZ) const 
{
  // Insert the code that "translates" an output object to a state object
	throw logic_error("FP_OutputManager::InputState not implemented yet");	
}

void FP_OutputManager::OutputState(const FP_State& st, FP_Output& XYZ) const 
{
  // Insert the code that "translates" a state object to an output object
	throw logic_error("FP_OutputManager::OutputState not implemented yet");	
}


/*****************************************************************************
 * FP_Move Neighborhood Explorer Methods
 *****************************************************************************/

// initial move builder
void FP_MoveNeighborhoodExplorer::RandomMove(const FP_State& st, FP_Move& mv) const
{
  // insert the code that writes a random move on mv in state st
	throw logic_error("FP_MoveNeighborhoodExplorer::RandomMove not implemented yet");	
} 

// check move feasibility
bool FP_MoveNeighborhoodExplorer::FeasibleMove(const FP_State& st, const FP_Move& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
	throw logic_error("FP_MoveNeighborhoodExplorer::FeasibleMove not implemented yet");	
  return true;
} 

// update the state according to the move 
void FP_MoveNeighborhoodExplorer::MakeMove(FP_State& st, const FP_Move& mv) const
{
  // Insert the code that modify the state st based on the application of move mv
	throw logic_error("FP_MoveNeighborhoodExplorer::MakeMove not implemented yet");	
}  

void FP_MoveNeighborhoodExplorer::FirstMove(const FP_State& st, FP_Move& mv) const
{
  // Insert the code the generate the first move in the neighborhood and store it in mv
	throw logic_error("FP_MoveNeighborhoodExplorer::FirstMove not implemented yet");	
}

bool FP_MoveNeighborhoodExplorer::NextMove(const FP_State& st, FP_Move& mv) const
{
  // Insert the code that generate the move that follows mv in the neighborhood, and writes
  // it back in mv. Return false if mv is already the last move. 
	throw logic_error("FP_MoveNeighborhoodExplorer::NextMove not implemented yet");	
  return true;
}

int FP_MoveDeltaCostComponent1::ComputeDeltaCost(const FP_State& st, const FP_Move& mv) const
{
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
	throw logic_error("FP_MoveDeltaCostComponent1::ComputeDeltaCost not implemented yet");	
  return cost;
}
          
int FP_MoveDeltaCostComponent2::ComputeDeltaCost(const FP_State& st, const FP_Move& mv) const
{
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
	throw logic_error("FP_MoveDeltaCostComponent2::ComputeDeltaCost not implemented yet");	
  return cost;
}

