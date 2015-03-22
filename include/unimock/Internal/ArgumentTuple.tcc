/*

   ArgumentTuple.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>

namespace unimock
{

template<typename... TupleParameters>
template<typename... Parameters>
ArgumentTuple<TupleParameters...>::ArgumentTuple( Parameters&&... arguments )
:
   tuple( std::forward<Parameters>( arguments )... )
{
}


} // namespace

