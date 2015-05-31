/*

   Test.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include <iostream>
#include <cstdlib>


inline void test( std::string description )
{
    std::cout << "Test: " << description << std::endl;
}

inline void require( bool condition )
{
   if( !condition )
   {
      std::cout << "TEST FAILED!" << std::endl;
      std::exit( EXIT_FAILURE );
   }
}

