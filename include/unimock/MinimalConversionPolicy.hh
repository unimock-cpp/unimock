/*

   MinimalConversionPolicy.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <memory>       // std::unique_ptr


namespace unimock
{

/// MinimalConversionPolicy to convert from one type to another.
///
/// Please be well aware of the behavior of this policy before use.
///
/// This conversion policy passes along all types unchanged except for
/// std::unique_ptr. The usual scenario is that the caller of the conversion
/// functions copies the result from the conversion, but this won't work for
/// std::unique_ptr since it can't be copied. Instead the pointer to its managed
/// object is extracted and returned, making the conversion look similar to how
/// a raw pointer is just passed unchanged.
///
class MinimalConversionPolicy
{
public:

   template<typename T>
   static auto convert( T&& object );

   template<typename T>
   static T* convert( const std::unique_ptr<T>& objectUPtr );

   template<typename T>
   static T* convert( std::unique_ptr<T>&& objectUPtr );

};


} // namespace


// Implementation.
#include "MinimalConversionPolicy.tcc"

