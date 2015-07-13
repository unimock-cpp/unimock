/*

   TestFramework.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include "TestFramework.hh"

#include <iostream>


void test( std::string description )
{
    std::cout << "Test: " << description << std::endl;
}

void ensure( bool condition )
{
   if( !condition )
      std::cout << "Test failed!" << std::endl;
}

