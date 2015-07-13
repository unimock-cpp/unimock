/*

   TestFramework.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <string>


/// Prints a test description.
///
/// \param[in] description
///   The description to print out.
///
/// \exception Exception neutral.
///
void test( std::string description );

/// Checks the condition.
///
/// Checks the input condition and prints an error message if it evaluates
/// false.
///
/// \param[in] condition
///   The condition to check.
///
/// \exception Exception neutral.
///
void ensure( bool condition );

