/*

   FunctorMock.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <memory>       // std::shared_ptr
#include <functional>

#include "unimock/FiniteID.hh"
#include "unimock/CallRecorder.hh"
#include "unimock/DefaultConversionPolicy.hh"


namespace unimock
{

template<class Function, class ConversionPolicy = DefaultConversionPolicy>
class FunctorMock;


/// FunctorMock to record call activity for functors.
///
/// This functor mock is provided to record call activity for functors like
/// std::function. It can typically be used with callback functors.
///
template<typename R, typename... Parameters, class ConversionPolicy>
class FunctorMock<R(Parameters...), ConversionPolicy>
{
public:

   /// Default constructor.
   ///
   /// The default constructor constructs a mock with a default call recorder
   /// and without a stub.
   ///
   /// \exception Exception neutral.
   ///
   FunctorMock();

   /// Constructor.
   ///
   /// This constructor constructs a mock with a provided call recorder and
   /// without a stub.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the functor calls and their
   ///   arguments.
   ///
   /// \exception Exception neutral.
   ///
   FunctorMock( std::shared_ptr<CallRecorder<ConversionPolicy>> recorder );

   /// Constructor.
   ///
   /// This constructor constructs a mock with a default call recorder and a
   /// provided stub.
   ///
   /// \param[in] stub
   ///   The stub where to forward the mock's functor calls.
   ///
   /// \exception Exception neutral.
   ///
   FunctorMock( std::function<R(Parameters...)> stub );

   /// Constructor.
   ///
   /// This constructor constructs a mock with a provided call recorder and a
   /// provided stub.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the functor calls and their
   ///   arguments.
   ///
   /// \param[in] stub
   ///   The stub where to forward the mock's functor calls.
   ///
   /// \exception Exception neutral.
   ///
   FunctorMock(
      std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
      std::function<R(Parameters...)> stub );

   /// Records and forwards the functor call.
   ///
   /// This functor will record the call and its argument, and then forward the
   /// call to a provided stub or return a default value, in that order. If a
   /// conversion policy has been provided, it will convert the arguments before
   /// recorded. This is useful for pointers since the referenced objects may
   /// have been deleted by the time the recorded calls are analyzed.
   ///
   /// \param[in,out] arguments
   ///   The arguments to record and forward to the stub.
   ///
   /// \returns
   ///   The result of the functor that is recorded. The result comes from a
   ///   stub or a default value, in that order.
   ///
   /// \exception Exception neutral.
   ///
   R operator()( Parameters... arguments );

   /// Finds all recorded calls for the functor.
   ///
   /// This find method looks up all recorded occasions of calls to this mock's
   /// functor and returns the result as an iterable container of tuples. The
   /// lookup will only be done for this functor mock. If this mock is connected
   /// to a recorder shared with other functor mocks with the same function
   /// signature, this method will not find the recorded calls to those mocks.
   ///
   /// \returns
   ///   The recorded calls for this mock's functor, returned as an iterable
   ///   container of tuples where each tuple contains the arguments provided in
   ///   that particular call.
   ///
   /// \exception Exception neutral.
   ///
   auto find() const;

   /// Gets the identifier of the theoretical functor mocked.
   ///
   /// The identifier that is returned is not connected to the instance of this
   /// mock but to the theoretical functor that this mock is mocking. When a
   /// stub is provided it could be seen as the (theoretical) functor, but when
   /// a stub isn't provided, the functor is just theoretical.
   ///
   /// In practice this means that if copies are made of the mock, each copy
   /// will get the same identifier. Roughly this FunctorMock class could be
   /// compared with a shared pointer where this identifier would be the shared
   /// pointer's referenced raw pointer.
   ///
   /// This method is usually only needed when working with shared call
   /// recorders.
   ///
   /// \returns
   ///   A unique identifier.
   ///
   /// \exception Exception neutral.
   ///
   FiniteID getID() const;


private:

   FiniteID mockID_;

   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder_;

   std::function<R(Parameters...)> stub_;


};


} // namespace


// Implementation.
#include "FunctorMock.tcc"

