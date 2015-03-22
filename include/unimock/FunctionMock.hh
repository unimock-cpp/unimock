/*

   FunctionMock.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <memory>       // std::weak_ptr, std::shared_ptr
#include <functional>

#include "unimock/CallRecorder.hh"
#include "unimock/DefaultConversionPolicy.hh"


namespace unimock
{

template<class Function, class ConversionPolicy = DefaultConversionPolicy>
class FunctionMock;


/// FunctionMock to record call activity for functions.
///
/// Note! If possible, use the FunctorMock instead.
///
/// This function mock is provided to record call activity for non-member and
/// static member functions. This is the only mock that can do that since a
/// functor like std::function or FunctorMock can't be provided to an interface
/// taking a non-member or static member function pointer. The opposite is true
/// though, so it's possible to provide this function mock to an interface
/// taking a functor but don't do that. Use the FunctorMock instead.
///
/// Since this function mock relies on a static member function, it brings a
/// couple of drawbacks.
/// 1. This function mock can only have one call recorder and one stub
///    registered at a time for each function signature since the mock function
///    has to be static. This limitation may be acceptable for unit tests
///    though.
/// 2. It's less safe. If a new call recorder or stub is provided for a
///    particular function signature while it's in use by another function mock,
///    a precondition is violated in run-time.
/// 3. It costs static storage memory for each function signature.
///
template<typename R, typename... Parameters, class ConversionPolicy>
class FunctionMock<R(Parameters...), ConversionPolicy>
{
public:

   /// Default constructor.
   ///
   /// All function mocks are dependent on a class variable holding a common
   /// call recorder. If the call recorder hasn't been initialized by any other
   /// function mock then this constructor will initialize it.
   ///
   /// \exception Exception neutral.
   ///
   FunctionMock();

   /// Constructor.
   ///
   /// This constructor can be used to provide a custom call recorder. The call
   /// recorder is held by a class variable common to all function mocks. If the
   /// call recorder has been initialized already and is in use by another
   /// function mock, the precondition is violated.
   ///
   /// \pre The common class variable call recorder must not be in use.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the function calls and their
   ///   arguments.
   ///
   /// \exception Exception neutral.
   ///
   FunctionMock( std::shared_ptr<CallRecorder<ConversionPolicy>> recorder );

   /// Constructor.
   ///
   /// This constructor can be used to provide a custom stub. The stub is held
   /// by a class variable common to all function mocks. If the stub has been
   /// initialized already and is in use by another function mock, the
   /// precondition is violated.
   ///
   /// This constructor will initialize the call recorder the same way as the
   /// default constructor.
   ///
   /// \pre The common class variable stub must not be in use.
   ///
   /// \param[in] stub
   ///   The stub where to forward the mock's method calls.
   ///
   /// \exception Exception neutral.
   ///
   FunctionMock( std::function<R(Parameters...)> stub );

   /// Constructor.
   ///
   /// This constructor can be used to provide a custom call recorder and a
   /// custom stub. Both the call recorder and the stub are held by class
   /// variables common to all function mocks. If any of them has been
   /// initialized already and is in use by another function mock, the
   /// precondition is violated.
   ///
   /// \pre The common class variable call recorder must not be in use.
   ///
   /// \pre The common class variable stub must not be in use.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the function calls and their
   ///   arguments.
   ///
   /// \exception Exception neutral.
   ///
   FunctionMock(
      std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
      std::function<R(Parameters...)> stub );

   /// Records and forwards the function call.
   ///
   /// This static member function will record the call and its argument, and
   /// then forward the call to a provided stub or return a default value, in
   /// that order. If a conversion policy has been provided, it will convert the
   /// arguments before recorded. This is useful for pointers since the
   /// referenced objects may have been deleted by the time the recorded calls
   /// are analyzed.
   ///
   /// \param[in,out] arguments
   ///   The arguments to record and forward to the stub.
   ///
   /// \returns
   ///   The result of the function that is recorded. The result comes from a
   ///   stub or a default value, in that order.
   ///
   /// \exception Exception neutral.
   ///
   static R function( Parameters... arguments );

   /// Finds all recorded calls for the function.
   ///
   /// This find method looks up all recorded occasions of calls to the static
   /// member function in this class and returns the result as an iterable
   /// container of tuples.
   ///
   /// Note! This method will find all function mocks' static member functions
   /// with the same signature. Since the static member function is static, it
   /// can't record to which function mock instance it belongs. Currently there
   /// is no workaround for this deficiency.
   ///
   /// \returns
   ///   The recorded calls for this mock's static member function, returned as
   ///   an iterable container of tuples where each tuple contains the arguments
   ///   provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   auto find() const;


private:

   static std::weak_ptr<CallRecorder<ConversionPolicy>> recorderWPtr_;

   static std::weak_ptr<std::function<R(Parameters...)>> stubWPtr_;

   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder_;

   std::shared_ptr<std::function<R(Parameters...)>> stub_;


};


} // namespace


// Implementation.
#include "FunctionMock.tcc"

