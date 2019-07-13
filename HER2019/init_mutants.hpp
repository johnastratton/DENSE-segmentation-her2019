#ifndef INIT_MUTANTS_H
#define INIT_MUTANTS_H

#include "../DENSE/source/core/parameter_set.hpp"

class mutant{
 public:
  mutant(std::string type, int analysis_type, bool cell_growth, int knockouts[2]) :
    name_{type},
    analysis_type_{analysis_type},
    cell_growth_{cell_growth}{
      std::cout << type << ": " <<  knockouts[0] << " " << knockouts[1] << std::endl;
      knockouts_[0] = knockouts[0]; 
      knockouts_[1] = knockouts[1]; 
    }
  
  void fill_paramsets(std::vector<Parameter_Set> paramsets){
    for(auto paramset : paramsets){
      int i = 0;
      Parameter_Set p;
      for(auto it = p.begin(); it != p.end(); it++){
        if(knockouts_[0] == i){
          *it = 0;
//          std::cout << "NO 0 " << i << std::endl;
        } else if(knockouts_[1] == i){ 
          *it = 0;
//          std::cout << "NO 0 " << i << std::endl;
        } else {
//          std::cout << paramset.data()[i] << " YES\n";
          *it = paramset.data()[i];
        }
        i++;
      }
      param_sets_.emplace_back(std::move(p));
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
  mutant_data(std::vector<Parameter_Set> paramsets, int num_active_mutants):
    Wild_type(std::string("Wild_type"), 1, true, knockouts[0]),
    Her7(std::string("Her7"), 1, true,knockouts[1]),
    Her13(std::string("Her13"), 1, true,knockouts[2]),
    Delta(std::string("Delta"), 1, true,knockouts[3]),
    Her7_Her13(std::string("Her7_Her13"), 1, true,knockouts[4]),
    Her1(std::string("Her1"), 1, true,knockouts[5]),
    Her7_Delta(std::string("Her7_Delta"), 1, true,knockouts[6]),
    Her1_Delta(std::string("Her1_Delta"), 1, true,knockouts[7]),
    Her7_Overexpressed(std::string("Her7_Overexpressed"), 1, true,knockouts[8]),
    Her1_Overexpressed(std::string("Her1_Overexpressed"), 1, true,knockouts[9]),
    Delta_Overexpressed(std::string("Delta_Overexpressed"), 1, true,knockouts[10]) 
    {
      for(int m = 0; m < num_active_mutants; m++){
        mutant mt = mutants[m];
        mt.fill_paramsets(paramsets);
      }
      for(auto i : knockouts){
        if( i[0] >= 0 ){
          std::cout << i[0] << " ";
        }
        if( i[1] >= 0 ){
          std::cout << i[1] << " ";
        }
        std::cout << std::endl;
      }
    }
  
  std::vector<Parameter_Set> get_sets(int num_active_mutants){
    std::vector<Parameter_Set> param_sets;
    for(int m = 0; m < num_active_mutants; m++){
      mutant mt = mutants[m];
      std::cout << mt.name_ << " " << mt.param_sets_.size() << " ";
      for(Parameter_Set set : mt.param_sets_){
        param_sets.push_back(set);
        std::cout << "set, ";
      }
      std::cout << std::endl;
    }
    return param_sets;
  }
  
  std::vector<mutant> get_mutants(){
    return mutants;
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
  
  int knockouts[11][2] = {{-1,-1}, {ph7_synthesis,-1}, {ph13_synthesis,-1}, {-222,-1}, {ph7_synthesis,ph13_synthesis}, {ph1_synthesis,-1}, {ph7_synthesis,-222}, {ph1_synthesis,-222}, {-1,-1}, {-1,-1}, {-1,-1}};
  std::vector<mutant> mutants = {Wild_type, Her7, Her13, Delta, Her7_Her13, Her1, Her7_Delta, Her1_Delta, Her7_Overexpressed, Her1_Overexpressed, Delta_Overexpressed};
};

#endif