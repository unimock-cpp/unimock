/*

   DefaultConversionPolicy.tcc

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

// The conversion functions have two responsibilities; provide a storage type,
// and convert objects of a certain type (where the result can be of the same
// type as the input). The storage type will be deduced from the return type of
// the conversion function. The normal case and recommended way is to store the
// converted objects as copies, and the easiest way to make sure of that is to
// have a return type that is not a reference. A good way is to declare the
// return type as a plain auto since "plain auto never deduces to a reference".
// See http://open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3638.html

template<typename T>
auto DefaultConversionPolicy::convert( T&& object )
{
   return std::forward<T>( object );
}

template<typename T>
T DefaultConversionPolicy::convert( T* objectPtr )
{
   return *objectPtr;
}

inline std::string DefaultConversionPolicy::convert( const char* stringPtr )
{
   return stringPtr;
}

template<typename T>
T DefaultConversionPolicy::convert( const std::unique_ptr<T>& objectUPtr )
{
   return *objectUPtr;
}

template<typename T>
T DefaultConversionPolicy::convert( std::unique_ptr<T>&& objectUPtr )
{
   return *objectUPtr;
}


} // namespace

