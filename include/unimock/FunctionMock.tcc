/*

   FunctionMock.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>      // std::forward
#include <cassert>


namespace unimock
{

template<typename R, typename... Parameters, class ConversionPolicy>
std::weak_ptr<CallRecorder<ConversionPolicy>>
FunctionMock<R(Parameters...), ConversionPolicy>::recorderWPtr_;

template<typename R, typename... Parameters, class ConversionPolicy>
std::weak_ptr<std::function<R(Parameters...)>>
FunctionMock<R(Parameters...), ConversionPolicy>::stubWPtr_;


template<typename R, typename... Parameters, class ConversionPolicy>
FunctionMock<R(Parameters...), ConversionPolicy>::FunctionMock()
:
   recorder_( recorderWPtr_.lock() ),
   stub_( stubWPtr_.lock() )
{
   // If the static variable hasn't been initialized with a call recorder then
   // do that.
   if( !recorder_ )
   {
      recorder_ = std::make_shared<CallRecorder<ConversionPolicy>>();
      recorderWPtr_ = recorder_;
   }
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctionMock<R(Parameters...), ConversionPolicy>::FunctionMock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder )
:
   recorder_( recorderWPtr_.lock() ),
   stub_( stubWPtr_.lock() )
{
   assert( recorder_ == nullptr || recorder == recorder_ );

   if( !recorder_ )
   {
      recorder_ = recorder;
      recorderWPtr_ = recorder_;
   }
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctionMock<R(Parameters...), ConversionPolicy>::FunctionMock(
   std::function<R(Parameters...)> stub )
:
   FunctionMock()
{
   // Make sure we don't silently overwrite the static variable that others may
   // be using.
   assert( stub_ == nullptr );

   stub_ = std::make_shared<std::function<R(Parameters...)>>( stub );
   stubWPtr_ = stub_;
}

template<typename R, typename... Parameters, class ConversionPolicy>
FunctionMock<R(Parameters...), ConversionPolicy>::FunctionMock(
   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
   std::function<R(Parameters...)> stub )
:
   recorder_( recorderWPtr_.lock() ),
   stub_( stubWPtr_.lock() )
{
   assert( recorder_ == nullptr || recorder == recorder_ );
   assert( stub_ == nullptr );

   if( !recorder_ )
   {
      recorder_ = recorder;
      recorderWPtr_ = recorder_;
   }

   stub_ = std::make_shared<std::function<R(Parameters...)>>( stub );
   stubWPtr_ = stub_;
}

template<typename R, typename... Parameters, class ConversionPolicy>
R FunctionMock<R(Parameters...), ConversionPolicy>::function(
   Parameters... arguments )
{
   auto recorder = recorderWPtr_.lock();
   auto stub = stubWPtr_.lock();

   assert( recorder != nullptr );

   // We use std::forward just like in perfect forwarding. The parameters
   // Parameters... ain't taken with a universal reference T&& but the template
   // types passed to std::forward will be the same regardless of && due to the
   // rvalue reference collapsing rule. The arguments passed will irrespectively
   // of type always call the same overload of std::forward since named
   // variables invoke a function taking T&, and never T&&.
   recorder->record(
      &FunctionMock<R(Parameters...), ConversionPolicy>::function,
      std::forward<Parameters>( arguments )... );

   // Make sure we have a function pointer and that the function is initialized.
   if( stub && *stub )
   {
      return static_cast<R>(
         (*stub)( std::forward<Parameters>( arguments )... ) );
   }

   // Without a stub we just return some default value.
   return static_cast<R>( R() );
}

template<typename R, typename... Parameters, class ConversionPolicy>
auto FunctionMock<R(Parameters...), ConversionPolicy>::find() const
{
   return recorder_->find(
      &FunctionMock<R(Parameters...), ConversionPolicy>::function );
}


} // namespace

