//include dense packages
#include "io/arg_parse.hpp"
#include "measurement/oscillation.hpp"
#include "measurement/basic.hpp"
#include "measurement/bad_simulation_error.hpp"
#include "utility/style.hpp"
#include "utility/common_utils.hpp"
#include "io/csvr_sim.hpp"
#include "io/csvw_sim.hpp"
#include "sim/determ/determ.hpp"
#include "sim/stoch/fast_gillespie_direct_simulation.hpp"
#include "sim/stoch/next_reaction_simulation.hpp"
#include "model_impl.hpp"
#include "io/ezxml/ezxml.h"
#include "Sim_Builder.hpp"
#include "run_simulation.hpp"
#include "arg_parse.hpp"
#include "parse_analysis_entries.hpp"

//include her2019 files
#include "run_and_modify_simulation.hpp"
#include "init_mutants.hpp"


using style::Color;

#include <chrono>
#include <cstdlib>
#include <cassert>
#include <random>
#include <memory>
#include <iterator>
#include <algorithm>
#include <functional>
#include <exception>
#include <iostream>

using dense::csvw_sim;
using dense::Deterministic_Simulation;
using dense::Fast_Gillespie_Direct_Simulation;
using dense::Sim_Builder;
using dense::parse_static_args;
using dense::parse_analysis_entries;
using dense::Static_Args;
using dense::CSV_Streamed_Simulation;
using dense::Details;


int main(int argc, char* argv[]){
  int ac = argc;
  char** av = argv;

  Static_Args args = parse_static_args(argc, argv);
  if(args.help == 1){
    return EXIT_SUCCESS;
  }
  if(args.help == 2){
    return EXIT_FAILURE;
  }
  int num_param_sets = args.param_sets.size();
  int num_active_mutants = 10;
  mutant_data mtd(args.param_sets, num_active_mutants);
  
  using Simulation = Fast_Gillespie_Direct_Simulation;
  Sim_Builder<Simulation> sim = Sim_Builder<Simulation>(args.perturbation_factors, args.gradient_factors, args.adj_graph, ac, av);

  std::vector<Parameter_Set> param_sets = mtd.get_sets(num_active_mutants);
  std::vector<Simulation> all_sims = sim.get_simulations(param_sets);
  
//  dense::Natural width = args.
  
  dense::run_and_modify_simulation<Simulation>(true, num_active_mutants, num_param_sets, mtd, args.simulation_duration, args.analysis_interval, std::move(all_sims), parse_analysis_entries<Simulation>(argc, argv, args.adj_graph.num_vertices()));
}