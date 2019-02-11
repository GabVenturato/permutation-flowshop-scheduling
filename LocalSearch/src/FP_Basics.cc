// File FP_Basics.cc
#include "FP_Basics.hh"

FP_State::FP_State(const FP_Input &my_in) 
  : in(my_in)
{
  // Insert the code that initialized the data structure of the
  // state object based on the input object	
  cerr << "FP_State::FP_State(const FP_Input &) not implemented yet" << endl;
}

FP_State& FP_State::operator=(const FP_State& st)
{
  // Insert the code that copies all data structures of the
  // state object from the ones of the parameter out
  // (excluding the reference to the input object, that is constant)
  throw logic_error("FP_State::operator= not implemented yet");
  return *this;
}
  
bool operator==(const FP_State& st1, const FP_State& st2)
{
  // Insert the code that compares two states (this is used by the tester)
  throw logic_error("operator==(const FP_State& st1, const FP_State& st2) not implemented yet");
  return true;
}


ostream& operator<<(ostream& os, const FP_State& st)
{
  // Insert the code that writes the state object (for debugging)
  throw logic_error("operator<<(ostream& os, const FP_State& as) not implemented yet");
  return os;
}

FP_Move::FP_Move()
{
  // Insert the code that initialize the move
  cerr << "FP_Move::FP_Move() not implemented yet" << endl;
}

bool operator==(const FP_Move& mv1, const FP_Move& mv2)
{
  // Insert the code that checks if two moves are identical
  throw logic_error("operator==(const FP_Move& mv1, const FP_Move& mv2) not implemented yet");	
  return true;
}

bool operator!=(const FP_Move& mv1, const FP_Move& mv2)
{
  // Insert the code that checks if two moves are different
  throw logic_error("operator!=(const FP_Move& mv1, const FP_Move& mv2) not implemented yet");	
  return true;
}

bool operator<(const FP_Move& mv1, const FP_Move& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  throw logic_error("operator<(const FP_Move& mv1, const FP_Move& mv2) not implemented yet");	
  return true;
}

istream& operator>>(istream& is, FP_Move& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, FP_Move& mv) not implemented yet");	
  return is;
}

ostream& operator<<(ostream& os, const FP_Move& mv)
{
  // Insert the code that writes a move
  throw logic_error("operator<<(ostream& os, const FP_Move& mv) not implemented yet");	
  return os;
}
