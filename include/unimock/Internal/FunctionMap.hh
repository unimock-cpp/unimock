/*

   FunctionMap.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <map>
#include <typeindex>
#include <memory>       // std::shared_ptr


namespace unimock
{

class FunctionMap
{
public:

   FunctionMap();

   template<typename R, typename... Parameters, class F>
   void set( R(*methodPtr)(Parameters...), F functor );

   template<typename R, class T, typename... Parameters, class F>
   void set( R(T::*methodPtr)(Parameters...), F functor );

   template<typename R, class T, typename... Parameters, class F>
   void set( R(T::*methodPtr)(Parameters...) const, F functor );

   template<typename R, typename... Parameters>
   auto get( R(*functionPtr)(Parameters...) ) const;

   template<typename R, class T, typename... Parameters>
   auto get( R(T::*methodPtr)(Parameters...) ) const;

   template<typename R, class T, typename... Parameters>
   auto get( R(T::*methodPtr)(Parameters...) const ) const;


private:

   // We use a simple type erasure technique with std::shared_ptr<void>. We also
   // need FunctionMap be copyable, and std::unique_ptr would make that less
   // easy. See
   // http://stackoverflow.com/a/6044720
   // https://akrzemi1.wordpress.com/2013/12/11/type-erasure-part-iii/

   std::map<std::type_index, std::shared_ptr<void>> functionMap_;

   template<typename R, typename... Parameters, class Index, class F>
   void set_( Index&& index, F functor );

   template<typename R, typename... Parameters>
   auto get_( const std::type_index& index ) const;


};


} // namespace


// Implementation.
#include "FunctionMap.tcc"


