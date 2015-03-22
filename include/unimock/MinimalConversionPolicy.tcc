/*

   MinimalConversionPolicy.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>      // std::forward


namespace unimock
{

template<typename T>
auto MinimalConversionPolicy::convert( T&& object )
{
   return std::forward<T>( object );
}

template<typename T>
T* MinimalConversionPolicy::convert( const std::unique_ptr<T>& objectUPtr )
{
   return objectUPtr.get();
}

template<typename T>
T* MinimalConversionPolicy::convert( std::unique_ptr<T>&& objectUPtr )
{
   return objectUPtr.get();
}


} // namespace

