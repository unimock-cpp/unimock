/*

   FunctorMock.tcc

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

template<typename R, typename... Parameters, class ConversionPolicy>
FunctorMock<R(Parameters...), ConversionPolicy>::FunctorMock()
:
   mockID_( FiniteID::generate() ),
   recorder_( std::make_shared<CallRecorder<ConversionPolicy>>() ),
   stub_()
{
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctorMock<R(Parameters...), ConversionPolicy>::FunctorMock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::move( recorder ) ),
   stub_()
{
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctorMock<R(Parameters...), ConversionPolicy>::FunctorMock(
   std::function<R(Parameters...)> stub )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::make_shared<CallRecorder<ConversionPolicy>>() ),
   stub_( std::move( stub ) )
{
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctorMock<R(Parameters...), ConversionPolicy>::FunctorMock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
   std::function<R(Parameters...)> stub )
:
   mockID_( FiniteID::generate() ),
   recorder_( std::move( recorder ) ),
   stub_( std::move( stub ) )
{
}

template<typename R, typename... Parameters, class ConversionPolicy>
R FunctorMock<R(Parameters...), ConversionPolicy>::operator()(
   Parameters... arguments )
{
   // We use std::forward just like in perfect forwarding. The parameters
   // Parameters... ain't taken with a universal reference T&& but the template
   // types passed to std::forward will be the same regardless of && due to the
   // rvalue reference collapsing rule. The arguments passed will irrespectively
   // of type always call the same overload of std::forward since named
   // variables invoke a function taking T&, and never T&&.
   recorder_->record(
      mockID_,
      &FunctorMock<R(Parameters...), ConversionPolicy>::operator(),
      std::forward<Parameters>( arguments )... );

   if( stub_ )
   {
      return static_cast<R>(
         stub_( std::forward<Parameters>( arguments )... ) );
   }

   // Without a stub we just return some default value.
   return static_cast<R>( R() );
}

template<typename R, typename... Parameters, class ConversionPolicy>
auto FunctorMock<R(Parameters...), ConversionPolicy>::find() const
{
   return recorder_->find(
      mockID_,
      &FunctorMock<R(Parameters...), ConversionPolicy>::operator() );
}

template<typename R, typename... Parameters, class ConversionPolicy>
FiniteID FunctorMock<R(Parameters...), ConversionPolicy>::getID() const
{
   return mockID_;
}


} // namespace

