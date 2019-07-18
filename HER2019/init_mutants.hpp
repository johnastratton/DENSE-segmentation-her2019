#ifndef INIT_MUTANTS_H
#define INIT_MUTANTS_H

#include "../DENSE/source/core/parameter_set.hpp"

class mutant{
 public:
  mutant(): name_{""},
    analysis_type_{0},
    param_sets_{3},
    cell_growth_{false}{
      knockouts_[0] = -1;
      knockouts_[1] = -1;
  }
  
  void update(std::string type, int analysis_type, bool cell_growth, int const knockout[2]){
    name_ = type;
    analysis_type_ = analysis_type;
    cell_growth_ = cell_growth;
    knockouts_[0] = knockout[0];
    knockouts_[1] = knockout[1];
  }
  
  void fill_paramsets(std::vector<Parameter_Set> paramsets){
    int j = 0;
    for(auto paramset : paramsets){
      Parameter_Set p = paramset;
      if(knockouts_[0] >= 0){
        p.data()[knockouts_[0]] = 0;
      } if(knockouts_[1] >= 0){
        p.data()[knockouts_[1]] = 0;
      }
      param_sets_[j] = p;
      j++;
    }
  }
  
  std::string name_;
  int analysis_type_;
  bool cell_growth_;
  int knockouts_[2];
  std::vector<Parameter_Set> param_sets_;
};

class mutant_data{
 public:
  mutant_data(std::vector<Parameter_Set> paramsets, int num_active_mutants){
    mutants[0].update(std::string("Wild_type"), 1, true, knockouts[0]);
    mutants[1].update(std::string("Her7"), 1, true, knockouts[1]);
    mutants[2].update(std::string("Her13"), 1, true, knockouts[2]);
    mutants[3].update(std::string("Delta"), 1, true, knockouts[3]);
    mutants[4].update(std::string("Her7_Her13"), 1, true, knockouts[4]);
    mutants[5].update(std::string("Her1"), 1, true, knockouts[5]);
    mutants[6].update(std::string("Her7_Delta"), 1, true, knockouts[6]);
    mutants[7].update(std::string("Her1_Delta"), 1, true, knockouts[7]);
    mutants[8].update(std::string("Her7_Overexpressed"), 1, true, knockouts[8]);
    mutants[9].update(std::string("Her1_Overexpressed"), 1, true, knockouts[9]);
    mutants[10].update(std::string("Delta_Overexpressed"), 1, true, knockouts[10]);

    for(int m = 0; m < num_active_mutants; m++){
      mutants[m].fill_paramsets(paramsets);
    }
  }
  
  std::vector<Parameter_Set> get_sets(int num_active_mutants){
    std::vector<Parameter_Set> param_sets;
    for(int m = 0; m < num_active_mutants; m++){
      for(Parameter_Set set : mutants[m].param_sets_){
        param_sets.push_back(set);
      }
    }
    return param_sets;
  }
  
  mutant& operator[](int type){
    return mutants[type];
  }
  
 private:
  mutant Wild_type;
  mutant Her7;
  mutant Her13;
  mutant Delta;
  mutant Her7_Her13;
  mutant Her1;
  mutant Her7_Delta;
  mutant Her1_Delta;
  mutant Her7_Overexpressed;
  mutant Her1_Overexpressed;
  mutant Delta_Overexpressed;
  
  mutant mutants[11] = {Wild_type, Her7, Her13, Delta, Her7_Her13, Her1, Her7_Delta, Her1_Delta, Her7_Overexpressed, Her1_Overexpressed, Delta_Overexpressed};
  
  int const knockouts[11][2] = {{-1,-1}, {ph7_synthesis,-1}, {ph13_synthesis,-1}, {-222,-1}, {ph7_synthesis,ph13_synthesis}, {ph1_synthesis,-1}, {ph7_synthesis,-222}, {ph1_synthesis,-222}, {-1,-1}, {-1,-1}, {-1,-1}};
};

#endif