#ifndef CELL_GROWTH_H
#define CELL_GROWTH_H

#include "../DENSE/source/sim/base.hpp"

class cell_growth{
  
  cell_growth(std::vector<Simulation*> sim, mutant mt, dense::Natural w):
    mts{mt},
    max_physical_cell{simulation},
    min_physical_cell{0},
    simulations{sim},
    width{w}{}

  template<class Simulation>
  void grow_cells(){
    //  int parents[4];
    //  for (int i = 0; i < 4; i++) {
    //    parents[i] = -1;
    //  }
    //  for (int i = 0; i < 4; i++) {
    //    int index;
    //    bool dup = true;
    //    while (dup) { // Ensure each parent produces exactly one child
    //     dup = false;
    //     index = random_int(std::make_pair(0, 4 - 1));
    //     for (int j = 0; j < i; j++) {
    //      if (index == parents[j]) {
    //       dup = true;
    //       break;
    //      }
    //     }
    //    };
    //    parents[i] = simulation->cell_count() - index;
    //  }

      dense::Natural parent_start = max_physical_cell();
      dense::Natural remove_start = min_physical_cell();
      for(dense::Natural i = 0; i < width; i++){
        simulation->remove_cell(remove_start + i);
        dense::Natural parent = parent_start - i;
        dense::Natural new_cell = simulation->max_physical_cell() + 1;
    //    std::cout << "New cell: " << new_cell << " parent: " << parent << " New max_physical_cell: " << simulation->max_physical_cell() << " New min_physical_cell: " << simulation->min_physical_cell() << std::endl;
    //    simulation->add_cell(new_cell, parent);

    }

    return;
  }
  
 private:
  mutant mts;
  dense::Natural max_physical_cell;
  dense::Natural min_physical_cell;
  std::vector<Simulation*> simulations;
  dense::Natural width;
}


#endif