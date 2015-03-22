/*

   FunctionMap.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>      // std::move, std::forward
#include <type_traits>  // std::result_of_t


namespace unimock
{

inline FunctionMap::FunctionMap()
:
   functionMap_()
{
}

template<typename R, typename... Parameters, class F>
void FunctionMap::set( R(*functionPtr)(Parameters...), F functor )
{
   set_<R, Parameters...>(
      typeid( decltype( functionPtr ) ), std::move( functor ) );
}

template<typename R, class T, typename... Parameters, class F>
void FunctionMap::set( R(T::*methodPtr)(Parameters...), F functor )
{
   set_<R, Parameters...>(
      typeid( decltype( methodPtr ) ), std::move( functor ) );
}

template<typename R, class T, typename... Parameters, class F>
void FunctionMap::set( R(T::*methodPtr)(Parameters...) const, F functor )
{
   set_<R, Parameters...>(
      typeid( decltype( methodPtr ) ), std::move( functor ) );
}

template<typename R, typename... Parameters>
auto FunctionMap::get( R(*functionPtr)(Parameters...) ) const
{
   return get_<R, Parameters...>( typeid( decltype( functionPtr ) ) );
}

template<typename R, class T, typename... Parameters>
auto FunctionMap::get( R(T::*methodPtr)(Parameters...) ) const
{
   return get_<R, Parameters...>( typeid( decltype( methodPtr ) ) );
}

template<typename R, class T, typename... Parameters>
auto FunctionMap::get( R(T::*methodPtr)(Parameters...) const ) const
{
   return get_<R, Parameters...>( typeid( decltype( methodPtr ) ) );
}

template<typename R, typename... Parameters, class Index, class F>
void FunctionMap::set_( Index&& index, F functor )
{
   static_assert(
      std::is_same<std::result_of_t<F(Parameters...)>, R>::value,
      "Functor type F doesn't match the provided function type" );

   functionMap_.emplace(
      std::forward<Index>( index ),
      std::shared_ptr<void>(
         new std::function<R(Parameters...)>( std::move( functor ) ) ) );
}

template<typename R, typename... Parameters>
auto FunctionMap::get_( const std::type_index& index ) const
{
   auto element = functionMap_.find( index );

   if( element == functionMap_.end() )
      return std::function<R(Parameters...)>();

   // We cast the stored void* to the correct functor, since what went in with a
   // certain function pointer key should come out using the same key.
   // Unfortunately we can't assert correctness since dynamic_cast doesn't
   // allow void*.
   auto functorPtr =
      static_cast<std::function<R(Parameters...)>*>( element->second.get() );

   return *functorPtr;
}


} // namespace

