#ifndef HER2019_UTILITY_H
#define HER2019_UTILITY_H

#include "init_mutants.hpp"

std::string file_add_mutant(std::string file, mutant mt, int param_set){
  file.replace(file.find(".csv"), string::npos, "");
  file.append("_" + mt.name_ + "_" + std::to_string(param_set) + ".csv");
  return file;
}

namespace dense{
  

  
  
}

#endif