/*

   DefaultConversionPolicy.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <string>
#include <memory>       // std::unique_ptr


namespace unimock
{

/// DefaultConversionPolicy to convert from one type to another.
///
/// This conversion policy passes along all types unchanged except for a couple
/// of pointer types. Raw pointers and std::unique_ptr are dereferenced. String
/// literals and const char* are converted to std::string.
///
/// Note! Arguments of type std::shared_ptr are not dereferenced and may
/// therefore increase their reference counts if copied by the caller of the
/// conversion function.
///
class DefaultConversionPolicy
{
public:

   template<typename T>
   static auto convert( T&& object );

   template<typename T>
   static T convert( T* objectPtr );

   static std::string convert( const char* stringPtr );

   template<typename T>
   static T convert( const std::unique_ptr<T>& objectUPtr );

   template<typename T>
   static T convert( std::unique_ptr<T>&& objectUPtr );

};


} // namespace


// Implementation.
#include "DefaultConversionPolicy.tcc"

