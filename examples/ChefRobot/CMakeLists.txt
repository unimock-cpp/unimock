#
#   CMakeLists.txt
#
#   Copyright (c) 2015 Daniel Markus
#
#   This file is part of the Unimock library that is distributed under the
#   University of Illinois/NCSA Open Source License (NCSA). See accompanying
#   file LICENSE.TXT for details.
#
#_______________________________________________________________________________
 

include_directories( ${PROJECT_SOURCE_DIR}/include )

if(
   ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
   ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )

   add_compile_options(
      -std=c++1y
      -Wall -Wextra -Weffc++ -Werror -Wno-unused-parameter )

endif()

# We exclude this example from being built during installation.
add_executable( ChefRobot EXCLUDE_FROM_ALL
   TestFramework.cc
   ChefRobot.cc
   CoolFood.cc
   GasStove3000.cc
   Dish.cc
   Main.cc )

