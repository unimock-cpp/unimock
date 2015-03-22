/*

   ArgumentTuple.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <tuple>

#include "ArgumentTupleI.hh"


namespace unimock
{

template<typename... TupleParameters>
struct ArgumentTuple : public ArgumentTupleI
{
   template<typename... Parameters>
   ArgumentTuple( Parameters&&... arguments );

   std::tuple<TupleParameters...> tuple;

};


} // namespace


// Implementation.
#include "ArgumentTuple.tcc"

