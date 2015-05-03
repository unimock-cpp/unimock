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
   // We only store a function pointer so we set the other information to
   // uninitialized and nullptr.
   record_<FncParameters...>(
      typeid( decltype( functionPtr ) ),
      functionPtr,
      FiniteID(),
      static_cast<void(VoidType_::*)()>( nullptr ),
      std::forward<Parameters>( arguments )... );
}

template<class ConversionPolicy>
template<typename R, class T, typename... FncParameters, typename... Parameters>
void CallRecorder<ConversionPolicy>::record(
   const FiniteID& objectID,
   R(T::*methodPtr)(FncParameters...),
   Parameters&&... arguments )
{
   // We store a method pointer so we set the function pointer to nullptr.
   record_<FncParameters...>(
      typeid( decltype( methodPtr ) ),
      static_cast<void(*)()>( nullptr ),
      objectID,
      methodPtr,
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
      typeid( decltype( methodPtr ) ),
      static_cast<void(*)()>( nullptr ),
      objectID,
      methodPtr,
      std::forward<Parameters>( arguments )... );
}

template<class ConversionPolicy>
template<typename R, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(*functionPtr)(Parameters...) ) const
{
   // We only search for a function pointer so we set the other search criteria
   // to uninitialized and nullptr.
   return find_<Parameters...>(
      std::type_index( typeid( decltype( functionPtr ) ) ),
      functionPtr,
      FiniteID(),
      static_cast<void(VoidType_::*)()>( nullptr ) );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(T::*methodPtr)(Parameters...) ) const
{
   // We use an uninitialized FiniteID to say that we don't use the object ID as
   // a search criteria, i.e. we search for the method pointer in any object.
   return find_<Parameters...>(
      std::type_index( typeid( decltype( methodPtr ) ) ),
      static_cast<void(*)()>( nullptr ),
      FiniteID(),
      methodPtr );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   R(T::*methodPtr)(Parameters...) const ) const
{
   return find_<Parameters...>(
      std::type_index( typeid( decltype( methodPtr ) ) ),
      static_cast<void(*)()>( nullptr ),
      FiniteID(),
      methodPtr );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   const FiniteID& objectID,
   R(T::*methodPtr)(Parameters...) ) const
{
   return find_<Parameters...>(
      std::type_index( typeid( decltype( methodPtr ) ) ),
      static_cast<void(*)()>( nullptr ),
      objectID,
      methodPtr );
}

template<class ConversionPolicy>
template<typename R, class T, typename... Parameters>
auto CallRecorder<ConversionPolicy>::find(
   const FiniteID& objectID,
   R(T::*methodPtr)(Parameters...) const ) const
{
   return find_<Parameters...>(
      std::type_index( typeid( decltype( methodPtr ) ) ),
      static_cast<void(*)()>( nullptr ),
      objectID,
      methodPtr );
}

template<class ConversionPolicy>
template<
   typename... TupleParameters,
   class TypeIndex,
   class FunctionPtr,
   class MethodPtr,
   typename... Parameters>
void CallRecorder<ConversionPolicy>::record_(
   TypeIndex&& typeIndex,
   FunctionPtr functionPtr,
   const FiniteID& objectID,
   MethodPtr methodPtr,
   Parameters&&... arguments )
{
   // (1) We can't use std::make_unique here since we're using an interface.
   // (2) Deduce the tuple storage type by means of the conversion policy. The
   //     deduction uses the parameter types in the function that is recorded,
   //     not the actual arguments that could be temporaries passed in.
   callHistory_.emplace_back(
      std::forward<TypeIndex>( typeIndex ),
      reinterpret_cast<void(*)()>( functionPtr ),
      objectID,
      reinterpret_cast<void(VoidType_::*)()>( methodPtr ),
      std::unique_ptr<ArgumentTupleI>(    // (1)
         new ArgumentTuple<
            StorageT<ConversionPolicy, TupleParameters>...>(   // (2)
               this->convert( std::forward<Parameters>( arguments ) )... ) ) );
}

template<class ConversionPolicy>
template<typename... Parameters, class FunctionPtr, class MethodPtr>
auto CallRecorder<ConversionPolicy>::find_(
   const std::type_index& typeIndex,
   FunctionPtr functionPtr,
   const FiniteID& objectID,
   MethodPtr methodPtr ) const
{
   std::vector<std::tuple<StorageT<ConversionPolicy, Parameters>...>> resultSet;

   for( auto& call : callHistory_ )
   {
      // We must first make sure the type is the same. We use reinterpret_cast
      // below and [C++14, §5.2.10/10 Reinterpret cast] says that converting
      // the void pointers back to the types they were once created from shall
      // yield their original pointers.
      if( std::get<std::type_index>( call ) != typeIndex )
         continue;

      // Either we search for a function or an object/method combination.
      if( functionPtr )
      {
         if( reinterpret_cast<FunctionPtr>( std::get<void(*)()>( call ) ) !=
            functionPtr )
               continue;
      }
      else
      {
         // Object ID as search criteria.
         if( objectID && std::get<FiniteID>( call ) != objectID )
            continue;

         // Method pointer as search criteria.
         // WARNING! The != comparison is unspecified by the standard, see
         // [C++14, §5.10/3 Equality operators]. It works on many compilers
         // though and hopefully it will be specified in the future.
         if( methodPtr && reinterpret_cast<MethodPtr>(
            std::get<void(VoidType_::*)()>( call ) ) != methodPtr )
               continue;
      }

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

