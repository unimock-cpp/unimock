/*

   CallRecorder.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <cassert>

#include "Internal/ArgumentTuple.hh"


namespace unimock
{

namespace
{
// Check the return type of the conversion to see what storage type we will use
// for a certain type to convert.
template<class ConversionPolicy, typename T>
using StorageT =
   decltype( std::declval<ConversionPolicy>().convert( std::declval<T>() ) );

} // unnamed namespace


template<class ConversionPolicy>
CallRecorder<ConversionPolicy>::CallRecorder()
:
   ConversionPolicy(),
   callHistory_()
{
}

template<class ConversionPolicy>
template<typename R, typename... FncParameters, typename... Parameters>
void CallRecorder<ConversionPolicy>::record(
   R(*functionPtr)(FncParameters...),
   Parameters&&... arguments )
{
   // We must use an uninitialized FiniteID since non-member functions don't
   // belong to any object.
   record_<FncParameters...>(
      FiniteID(),
      typeid( decltype( functionPtr ) ),
      std::forward<Parameters>( arguments )... );
}

template<class ConversionPolicy>
template<typename R, class T, typename... FncParameters, typename... Parameters>
void CallRecorder<ConversionPolicy>::record(
   const FiniteID& objectID,
   R(T::*methodPtr)(FncParameters...),
   Parameters&&... arguments )
{
   record_<FncParameters...>(
      objectID,
      typeid( decltype( methodPtr ) ),
      std::forward<Parameters>( arguments )... );
}

template<class ConversionPolicy>
template<typename R, class T, typename... FncParameters, typename... Parameters>
void CallRecorder<ConversionPolicy>::record(
   const FiniteID& objectID,
   R(T::*methodPtr)(FncParameters...) const,
   Parameters&&... arguments )
{
   record_<FncParameters...>(
      objectID,
      typeid( decltype( methodPtr ) ),
      std::forward<Parameters>( arguments )... );
}

template<class ConversionPolicy>
template<typename R, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(*functionPtr)(Parameters...) ) const
{
   // Non-member functions don't belong to an object so we use an uninitialized
   // FiniteID to say that we don't use object ID as a search criteria.
   return find_<Parameters...>(
      FiniteID(),
      std::type_index( typeid( decltype( functionPtr ) ) ) );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(T::*methodPtr)(Parameters...) ) const
{
   // We use an uninitialized FiniteID to say that we don't search with the
   // object ID as a search criteria.
   return find_<Parameters...>(
      FiniteID(),
      std::type_index( typeid( decltype( methodPtr ) ) ) );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(T::*methodPtr)(Parameters...) const ) const
{
   return find_<Parameters...>(
      FiniteID(),
      std::type_index( typeid( decltype( methodPtr ) ) ) );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   const FiniteID& objectID,
   R(T::*methodPtr)(Parameters...) ) const
{
   return find_<Parameters...>(
      objectID,
      std::type_index( typeid( decltype( methodPtr ) ) ) );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   const FiniteID& objectID,
   R(T::*methodPtr)(Parameters...) const ) const
{
   return find_<Parameters...>(
      objectID,
      std::type_index( typeid( decltype( methodPtr ) ) ) );
}

template<class ConversionPolicy>
template<typename... TupleParameters, class Index, typename... Parameters>
void CallRecorder<ConversionPolicy>::record_(
   const FiniteID& objectID, Index&& index, Parameters&&... arguments )
{
   // (1) We can't use std::make_unique here since we're using an interface.
   // (2) Deduce the tuple storage type by means of the conversion policy. The
   //     deduction uses the parameter types in the function that is recorded,
   //     not the actual arguments that could be temporaries passed in.
   callHistory_.emplace_back(
      objectID,
      std::forward<Index>( index ),
      std::unique_ptr<ArgumentTupleI>(    // (1)
         new ArgumentTuple<
            StorageT<ConversionPolicy, TupleParameters>...>(   // (2)
               this->convert( std::forward<Parameters>( arguments ) )... ) ) );
}

template<class ConversionPolicy>
template<typename... Parameters>
auto CallRecorder<ConversionPolicy>::find_(
   const FiniteID& objectID,
   const std::type_index& index ) const
{
   std::vector<std::tuple<StorageT<ConversionPolicy, Parameters>...>> resultSet;

   for( auto& call : callHistory_ )
   {
      if( objectID && std::get<FiniteID>( call ) != objectID )
         continue;

      if( std::get<std::type_index>( call ) != index )
         continue;

      // We cast the stored pointer to the correct tuple of arguments, since
      // what went in with a certain function pointer key should come out using
      // the same key.
      auto tuplePtr = static_cast<ArgumentTuple<
         StorageT<ConversionPolicy, Parameters>...>*>(
            std::get<std::unique_ptr<ArgumentTupleI>>( call ).get() );

      // The cast should never fail but we check the invariant just in case.
      // Remove or replace with your favorite assert function.
#ifndef NDEBUG
      auto dynamicTuplePtr = dynamic_cast<ArgumentTuple<
         StorageT<ConversionPolicy, Parameters>...>*>(
            std::get<std::unique_ptr<ArgumentTupleI>>( call ).get() );
      assert( dynamicTuplePtr == tuplePtr );
#endif

      resultSet.emplace_back( tuplePtr->tuple );
   }

   return resultSet;
}


} // namespace

