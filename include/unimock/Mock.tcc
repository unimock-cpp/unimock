/*

   Mock.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>      // std::move, std::forward


namespace unimock
{

template<class TI, class ConversionPolicy>
Mock<TI, ConversionPolicy>::Mock()
:
   mockID_( FiniteID::generate() ),
   recorder_( std::make_shared<CallRecorder<ConversionPolicy>>() ),
   functionMap_(),
   stub_()
{
}

template<class TI, class ConversionPolicy>
Mock<TI, ConversionPolicy>::Mock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::move( recorder ) ),
   functionMap_(),
   stub_()
{
}

template<class TI, class ConversionPolicy>
Mock<TI, ConversionPolicy>::Mock( std::shared_ptr<TI> stub )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::make_shared<CallRecorder<ConversionPolicy>>() ),
   functionMap_(),
   stub_( std::move( stub ) )
{
}

template<class TI, class ConversionPolicy>
Mock<TI, ConversionPolicy>::Mock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
   std::shared_ptr<TI> stub )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::move( recorder ) ),
   functionMap_(),
   stub_( std::move( stub ) )
{
}

template<class TI, class ConversionPolicy>
template<typename R, typename... Parameters, class F>
void Mock<TI, ConversionPolicy>::set(
   R(TI::*methodPtr)(Parameters...), F functor )
{
   functionMap_.set( methodPtr, std::move( functor ) );
}

template<class TI, class ConversionPolicy>
template<typename R, typename... Parameters, class F>
void Mock<TI, ConversionPolicy>::set(
   R(TI::*methodPtr)(Parameters...) const, F functor )
{
   functionMap_.set( methodPtr, std::move( functor ) );
}

template<class TI, class ConversionPolicy>
template<typename R, typename... Parameters>
auto Mock<TI, ConversionPolicy>::find(
   R(TI::*methodPtr)(Parameters...) )
{
   return recorder_->find( mockID_, methodPtr );
}

template<class TI, class ConversionPolicy>
template<typename R, typename... Parameters>
auto Mock<TI, ConversionPolicy>::find(
   R(TI::*methodPtr)(Parameters...) const ) const
{
   return recorder_->find( mockID_, methodPtr );
}

template<class TI, class ConversionPolicy>
FiniteID Mock<TI, ConversionPolicy>::getID() const
{
   return mockID_;
}

template<class TI, class ConversionPolicy>
template<typename R, typename... FncParameters, typename... Parameters>
R Mock<TI, ConversionPolicy>::call(
   R(TI::*methodPtr)(FncParameters...),
   Parameters&&... arguments )
{
   recorder_->record(
      mockID_, methodPtr, std::forward<Parameters>( arguments )... );

   auto overridingFunctor = functionMap_.get( methodPtr );
   if( overridingFunctor )
   {
      return static_cast<R>(
         overridingFunctor( std::forward<Parameters>( arguments )... ) );
   }

   if( stub_ )
   {
      return static_cast<R>(
         ((*stub_).*methodPtr)( std::forward<Parameters>( arguments )... ) );
   }

   // Without an overriding functor or a stub we just return some default value.
   return static_cast<R>( R() );
}

template<class TI, class ConversionPolicy>
template<typename R, typename... FncParameters, typename... Parameters>
R Mock<TI, ConversionPolicy>::call(
   R(TI::*methodPtr)(FncParameters...) const,
   Parameters&&... arguments ) const
{
   recorder_->record(
      mockID_, methodPtr, std::forward<Parameters>( arguments )... );

   auto overridingFunctor = functionMap_.get( methodPtr );
   if( overridingFunctor )
   {
      return static_cast<R>(
         overridingFunctor( std::forward<Parameters>( arguments )... ) );
   }

   if( stub_ )
   {
      return static_cast<R>(
         ((*stub_).*methodPtr)( std::forward<Parameters>( arguments )... ) );
   }

   return static_cast<R>( R() );
}


} // namespace

