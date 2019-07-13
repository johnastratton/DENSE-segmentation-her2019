cmake_minimum_required(VERSION 2.8)

add_subdirectory(DENSE)

function(HER2019 localname simdir)
  SIMULATION(${localname} ${simdir} HER2019/her2019_main.cpp)
endfunction(HER2019 localname simdir)

HER2019(her2019 ${PROJECT_BINARY_DIR})
set_target_properties(her2019 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
