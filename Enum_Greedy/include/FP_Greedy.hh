#ifndef FP_GREEDY_HH
#define FP_FREEDY_HH
#include "FP_Data.hh"

int random(int min, int max);
void greedyWSolver(const FP_Input& in, FP_Output& out, bool rand = false);
void greedyRandomWSolver(const FP_Input& in, FP_Output& out);
void greedyDynamicWSolver(const FP_Input& in, FP_Output& out, bool rand = false);
void greedyDynamicRandomWSolver(const FP_Input& in, FP_Output& out);

#endif
