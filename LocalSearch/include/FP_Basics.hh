// File FP_Basics.hh
#ifndef FP_BASICS_HH
#define FP_BASICS_HH

#include "FP_Data.hh"

class FP_State
{  
  friend ostream& operator<<(ostream& os, const FP_State& st);
  friend bool operator==(const FP_State& st1, const FP_State& st2);
public:
  FP_State(const FP_Input &in);
  FP_State& operator=(const FP_State& s);
protected:
  const FP_Input & in;  
};

class FP_Move
{
  friend bool operator==(const FP_Move& m1, const FP_Move& m2);
  friend bool operator!=(const FP_Move& m1, const FP_Move& m2);
  friend bool operator<(const FP_Move& m1, const FP_Move& m2);
  friend ostream& operator<<(ostream& os, const FP_Move& c);
  friend istream& operator>>(istream& is, FP_Move& c);
 public:
  FP_Move();
};
#endif

