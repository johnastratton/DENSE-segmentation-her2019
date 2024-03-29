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
#include "init_mutants.hpp"

//include her2019 files
#include "her2019_utility.hpp"
#include "cell_growth.hpp"

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
using dense::stochastic::Next_Reaction_Simulation;
using dense::Sim_Builder;
using dense::parse_static_args;
using dense::parse_analysis_entries;
using dense::Static_Args;
using dense::CSV_Streamed_Simulation;
using dense::Details;

namespace dense{

#ifndef __cpp_concepts
template <typename Simulation>
#else
template <Simulation_Concept Simulation>
#endif
void run_and_modify_simulation(
  bool show_cells,
  int width,
  int num_param_sets,
  mutant_data mutants,
  std::chrono::duration<Real, std::chrono::minutes::period> duration,
  std::chrono::duration<Real, std::chrono::minutes::period> notify_interval,
  std::vector<Simulation> simulations,
  std::vector<std::pair<std::string, std::unique_ptr<Analysis<Simulation>>>> analysis_entries);

#ifndef __cpp_concepts
template <typename Simulation>
#else
template <Simulation_Concept Simulation>
#endif
void run_and_modify_simulation(
  bool show_cells,
  int width,
  int num_param_sets,
  mutant_data mutants,
  std::chrono::duration<Real, std::chrono::minutes::period> duration,
  std::chrono::duration<Real, std::chrono::minutes::period> notify_interval,
  std::vector<Simulation> simulations,
  std::vector<std::pair<std::string, std::unique_ptr<Analysis<Simulation>>>> analysis_entries){
  
  struct Callback {
    Callback(
    std::unique_ptr<Analysis<Simulation>> analysis,
    Simulation & simulation,
    csvw log
    ):
    analysis   { std::move(analysis) },
    simulation { std::addressof(simulation) },
    log        { std::move(log) }
    {
    }

    void operator()() {
      return analysis->when_updated_by(*simulation, log.stream());
    }

    std::unique_ptr<Analysis<Simulation>> analysis;
    Simulation* simulation;
    csvw log;

  };
  
  std::vector<Callback> callbacks;
    
      // If multiple sets, set file name to "x_####.y"
  int mt = 0;
  for (std::size_t i = 0; i < simulations.size(); ++i) {
    if((i % num_param_sets == 0) && (i != 0)){
      mt++;
    }
    for (auto& name_and_analysis : analysis_entries) {
      auto& out_file = name_and_analysis.first;
      callbacks.emplace_back(
        std::unique_ptr<Analysis<Simulation>>(name_and_analysis.second->clone()),
        simulations[i],
        csvw(simulations.size() == 1 ? out_file : file_add_mutant(out_file, mutants[mt], i % num_param_sets)));
    }
  }
  
  //show cells in out file
  if(show_cells){
    for (auto& callback : callbacks) {
      callback.analysis->show_cells();
    }
  }
  
  cell_growth<Simulation, Callback> cg(&simulations, &mutants, width, &callbacks);

  // End all observer preparation
  // ========================= RUN THE SHOW =========================

  Real analysis_chunks = duration / notify_interval;
  int notifications_per_min = decltype(duration)(1.0) / notify_interval;

  for (dense::Natural a = 0; a < analysis_chunks; a++) {
      //handle cell growth
      if(a % notifications_per_min == 0 && a != 0){
        cg.grow_cells();
      }
    
      std::vector<Simulation const*> bad_simulations;
      for (auto& callback : callbacks) {
        try {
            callback();
        }
        catch (Bad_Simulation_Error<Simulation>& error) {
            bad_simulations.push_back(std::addressof(error.simulation()));
        }
      }
      for (auto& bad_simulation : bad_simulations) {
        auto has_bad_simulation = [=](Callback const& callback) {
            return callback.simulation == bad_simulation;
        };
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(), has_bad_simulation),
            callbacks.end());
        using std::swap;
        swap(simulations[bad_simulation - simulations.data()], simulations.back());
        simulations.pop_back();
      }
    
      Minutes age;
    
      for (auto & simulation : simulations) {
        age = simulation.age_by(notify_interval);
      }
      if (a % notifications_per_min == 0) {
          std::cout << "Time: " << age / Minutes{1} << '\n';
      }
  }

  for (auto& callback : callbacks) {
      callback.analysis->finalize();
      callback.analysis->show(&callback.log);
  }
}

}