// File FP_Helpers.hh
#ifndef FP_HELPERS_HH
#define FP_HELPERS_HH

#include "FP_Basics.hh"
#include <easylocal.hh>

using namespace EasyLocal::Core;

/***************************************************************************
 * State Manager 
 ***************************************************************************/

class FP_CostComponent1 : public CostComponent<FP_Input,FP_State> 
{
public:
  FP_CostComponent1(const FP_Input & in, int w, bool hard) :    CostComponent<FP_Input,FP_State>(in,w,hard,"FP_CostComponent1") 
  {}
  int ComputeCost(const FP_State& st) const;
  void PrintViolations(const FP_State& st, ostream& os = cout) const;
};

class  FP_CostComponent2: public CostComponent<FP_Input,FP_State> 
{
public:
  FP_CostComponent2(const FP_Input & in, int w, bool hard) : CostComponent<FP_Input,FP_State>(in,w,hard,"FP_CostComponent2") 
  {}
  int ComputeCost(const FP_State& st) const;
  void PrintViolations(const FP_State& st, ostream& os = cout) const;
};

class FP_StateManager : public StateManager<FP_Input,FP_State> 
{
public:
  FP_StateManager(const FP_Input &);
  void RandomState(FP_State&);   
  bool CheckConsistency(const FP_State& st) const;
protected:
}; 

/***************************************************************************
 * FP_Move Neighborhood Explorer:
 ***************************************************************************/

class FP_MoveDeltaCostComponent1
  : public DeltaCostComponent<FP_Input,FP_State,FP_Move>
{
public:
  FP_MoveDeltaCostComponent1(const FP_Input & in, FP_CostComponent1& cc) 
    : DeltaCostComponent<FP_Input,FP_State,FP_Move>(in,cc,"FP_MoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const FP_State& st, const FP_Move& mv) const;
};

class FP_MoveDeltaCostComponent2
  : public DeltaCostComponent<FP_Input,FP_State,FP_Move>
{
public:
  FP_MoveDeltaCostComponent2(const FP_Input & in, FP_CostComponent2& cc) 
    : DeltaCostComponent<FP_Input,FP_State,FP_Move>(in,cc,"FP_MoveDeltaCostComponent2") 
  {}
  int ComputeDeltaCost(const FP_State& st, const FP_Move& mv) const;
};

class FP_MoveNeighborhoodExplorer
  : public NeighborhoodExplorer<FP_Input,FP_State,FP_Move> 
{
public:
  FP_MoveNeighborhoodExplorer(const FP_Input & pin, StateManager<FP_Input,FP_State>& psm)  
    : NeighborhoodExplorer<FP_Input,FP_State,FP_Move>(pin, psm, "FP_MoveNeighborhoodExplorer") {} 
  void RandomMove(const FP_State&, FP_Move&) const;          
  bool FeasibleMove(const FP_State&, const FP_Move&) const;  
  void MakeMove(FP_State&, const FP_Move&) const;             
  void FirstMove(const FP_State&, FP_Move&) const;  
  bool NextMove(const FP_State&, FP_Move&) const;   
protected:
};

/***************************************************************************
 * Output Manager:
 ***************************************************************************/
class FP_OutputManager
  : public OutputManager<FP_Input,FP_Output,FP_State> 
{
public:
  FP_OutputManager(const FP_Input & pin)
    : OutputManager<FP_Input,FP_Output,FP_State>(pin,"XYZOutputManager") {}
  void InputState(FP_State&, const FP_Output&) const;  
  void OutputState(const FP_State&, FP_Output&) const; 
}; 
#endif
