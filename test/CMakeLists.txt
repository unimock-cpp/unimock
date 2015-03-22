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

# We exclude the test runner from being built during installation.
add_executable( TestRunner EXCLUDE_FROM_ALL
   CallRecorderTest.cc
   FunctionMockTest.cc
   FunctorMockTest.cc
   MockTest.cc
   TestMain.cc )

add_test( build_test_runner ${CMAKE_BUILD_TOOL} TestRunner )
add_test( all_tests TestRunner )

