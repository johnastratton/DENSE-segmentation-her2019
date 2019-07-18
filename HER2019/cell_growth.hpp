#ifndef CELL_GROWTH_H
#define CELL_GROWTH_H

#include "../DENSE/source/sim/base.hpp"

template<class Sim, class Callback>
class cell_growth{
 public:
  cell_growth(std::vector<Sim>* sim, mutant_data* mt, dense::Natural w, std::vector<Callback>* callb):
    used_parents{std::vector<dense::Natural>(w)},
    mts{mt},
    simulations{sim},
    max_physical_cell{(*simulations)[0].cell_count()-1},
    min_physical_cell{0},
    width{w},
    callbacks{callb}{}

  void grow_cells(){
    bool increment = false;
    dense::Natural parent_start = max_physical_cell;
    dense::Natural remove_start = min_physical_cell;
    for(int i = 0; i < simulations->size(); i++){
      dense::Natural new_cell = parent_start + 1;
      if((*mts)[i].cell_growth_){ /*&& mts[i].ANT_SIM*/ //if the mutant should have cell_growth
        for(dense::Natural w = 0; w < width; w++){ //walk through each cell in the simulations width
          (*simulations)[i].remove_cell(remove_start + w); //start by removing cells to open up space for the new cells
          dense::Natural parent = randomize_parent(parent_start - width + 1, parent_start, w); //find the parent index
          (*simulations)[i].add_cell(new_cell, parent);
          increment = true;
          new_cell++;
        }
        for (auto& callback : *callbacks) { //update the analysis objects
          callback.analysis->update_cell_range(0, (*simulations)[i].cell_count(), (*simulations)[i].physical_cells_id());
        }
      }
      used_parents = std::vector<dense::Natural>(width);
    }
    if(increment){
      max_physical_cell += width;
      min_physical_cell += width;
    }
  }
  
 private:
 
  dense::Natural find_neighbors{}
 
 
  dense::Natural randomize_parent(dense::Natural start, dense::Natural end, int w){
     dense::Natural parent = start + (rand() % ((end) - start + 1));
     for(dense::Natural p : used_parents){
        if(parent == p){
          return randomize_parent(start, end, w);
        }
     }
     used_parents[w] = parent;
     return parent;
  }
  
  
  std::vector<dense::Natural> used_parents;
  mutant_data* mts;
  std::vector<Sim>* simulations;
  dense::Natural max_physical_cell;
  dense::Natural min_physical_cell;
  dense::Natural width;
  std::vector<Callback>* callbacks;
};


#endif