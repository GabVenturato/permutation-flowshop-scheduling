#include "FP_Helpers.hh"

using namespace EasyLocal::Debug;

int main(int argc, const char* argv[]) {
  ParameterBox main_parameters("main", "Main Program options");

  // The set of arguments added are the following:

  Parameter<string> instance("instance", "Input instance", main_parameters);
  Parameter<int> seed("seed", "Random seed", main_parameters);
  Parameter<string> method("method", "Solution method (empty for tester)", main_parameters);
  Parameter<string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
  Parameter<string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);

  // 3rd parameter: false = do not check unregistered parameters
  // 4th parameter: true = silent
  CommandLineParameters::Parse(argc, argv, false, true);

  if (!instance.IsSet()) {
    cout << "Error: --main::instance filename option must always be set" << endl;
    return 1;
  }
  FP_Input in(instance);

  if (seed.IsSet()) Random::SetSeed(seed);

  // cost components: second parameter is the cost, third is the type (true -> hard, false -> soft)
  Makespan cc1(in, 1, false);
  Tardiness cc2(in, 1, false);

  SwapJobsDeltaMakespan dcc1(in, cc1);
  SwapJobsDeltaTardiness dcc2(in, cc2);

  MoveJobDeltaMakespan mj_dcc1(in, cc1);
  MoveJobDeltaTardiness mj_dcc2(in, cc2);

  // helpers
  FP_StateManager FP_sm(in);
  SwapJobsNeighborhoodExplorer FP_nhe(in, FP_sm);

  MoveJobNeighborhoodExplorer FP_mj_nhe(in, FP_sm);

  FP_OutputManager FP_om(in);

  // All cost components must be added to the state manager
  FP_sm.AddCostComponent(cc1);
  FP_sm.AddCostComponent(cc2);

  // All delta cost components must be added to the neighborhood explorer
  FP_nhe.AddDeltaCostComponent(dcc1);
  FP_nhe.AddDeltaCostComponent(dcc2);

  FP_mj_nhe.AddDeltaCostComponent(mj_dcc1);
  FP_mj_nhe.AddDeltaCostComponent(mj_dcc2);

  // runners
  HillClimbing<FP_Input, FP_State, SwapJobs> FP_hc(in, FP_sm, FP_nhe, "SwapJobsHillClimbing");
  SteepestDescent<FP_Input, FP_State, SwapJobs> FP_sd(in, FP_sm, FP_nhe, "SwapJobsSteepestDescent");
  SimulatedAnnealing<FP_Input, FP_State, SwapJobs> FP_sa(in, FP_sm, FP_nhe, "SwapJobsSimulatedAnnealing");
  TabuSearch<FP_Input, FP_State, SwapJobs> FP_ts(in, FP_sm, FP_nhe, "SwapJobsTabuSearch",
                                                   [](const SwapJobs& m1, const SwapJobs& m2)->bool
                                                   { return m1.j1 == m2.j1 && m1.j2 == m2.j2; });

  HillClimbing<FP_Input, FP_State, MoveJob> FP_mj_hc(in, FP_sm, FP_mj_nhe, "MoveJobHillClimbing");
  SteepestDescent<FP_Input, FP_State, MoveJob> FP_mj_sd(in, FP_sm, FP_mj_nhe, "MoveJobSteepestDescent");
  SimulatedAnnealing<FP_Input, FP_State, MoveJob> FP_mj_sa(in, FP_sm, FP_mj_nhe, "MoveJobSimulatedAnnealing");

  // tester
  Tester<FP_Input, FP_Output, FP_State> tester(in, FP_sm, FP_om);
  MoveTester<FP_Input, FP_Output, FP_State, SwapJobs> swap_move_test(in, FP_sm, FP_om, FP_nhe, "SwapJobs move", tester);
  MoveTester<FP_Input, FP_Output, FP_State, MoveJob> job_move_test(in, FP_sm, FP_om, FP_mj_nhe, "MoveJob move", tester);

  SimpleLocalSearch<FP_Input, FP_Output, FP_State> FP_solver(in, FP_sm, FP_om, "FP solver");
  if (!CommandLineParameters::Parse(argc, argv, true, false)) return 1;

  if (!method.IsSet()) {  // If no search method is set -> enter in the tester
    if (init_state.IsSet())
      tester.RunMainMenu(init_state);
    else
      tester.RunMainMenu();
  } else {
    if (method == string("SwapJobsSimulatedAnnealing")) {
      FP_solver.SetRunner(FP_sa);
    } else if (method == string("SwapJobsHillClimbing")) {
      FP_solver.SetRunner(FP_hc);
    } else if (method == string("SwapJobsSteepestDescent")) {
      FP_solver.SetRunner(FP_sd);
    } else if (method == string("SwapJobsTabuSearch")) {
      FP_solver.SetRunner(FP_ts);
    } else  if (method == string("MoveJobSimulatedAnnealing")) {
      FP_solver.SetRunner(FP_mj_sa);
    } else if (method == string("MoveJobHillClimbing")) {
      FP_solver.SetRunner(FP_mj_hc);
    } else {
      FP_solver.SetRunner(FP_mj_sd);
    }
    auto result = FP_solver.Solve();
    // result is a tuple: 0: solution, 1: number of violations, 2: total cost, 3: computing time
    FP_Output out = result.output;
    if (output_file.IsSet()) {  // write the output on the file passed in the command line
      ofstream os(static_cast<string>(output_file).c_str());
      os << out << endl;
      os << "Cost: " << result.cost.total << endl;
      os << "Time: " << result.running_time << "s " << endl;
      os.close();
    } else {  // write the solution in the standard output
      cout << out << endl;
      cout << "Cost: " << result.cost.total << endl;
      cout << "Time: " << result.running_time << "s " << endl;
    }
  }
  return 0;
}
