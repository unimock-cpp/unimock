/*

   Mock.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <memory>       // std::shared_ptr

#include "unimock/FiniteID.hh"
#include "unimock/CallRecorder.hh"
#include "unimock/DefaultConversionPolicy.hh"
#include "Internal/FunctionMap.hh"


namespace unimock
{

template<class TI, class ConversionPolicy = DefaultConversionPolicy>
class Mock;


/// Mock to record call activity and provide stub functionality.
///
/// When doing unit testing or run-time analysis of software there's often a
/// need to monitor call activity to verify correct behavior. This mock class
/// is capable of following.
/// * Record method calls
/// * Convert input arguments before recording them (good for pointers)
/// * Override individual method functionality
/// * Forward the calls to a provided stub
///
/// #### See also ####
/// [Mocks](http://martinfowler.com/articles/mocksArentStubs.html)            \n
/// [Policy-based Design](http://en.wikipedia.org/wiki/Policy-based_design)
///
template<class TI, class ConversionPolicy>
class Mock : public TI
{
public:

   /// Default constructor.
   ///
   /// The default constructor constructs a mock with a default call recorder
   /// and without a stub.
   ///
   /// \exception Exception neutral.
   ///
   Mock();

   /// Constructor.
   ///
   /// This constructor constructs a mock with a provided call recorder and
   /// without a stub.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the method calls and their arguments.
   ///
   /// \exception Exception neutral.
   ///
   Mock( std::shared_ptr<CallRecorder<ConversionPolicy>> recorder );

   /// Constructor.
   ///
   /// This constructor constructs a mock with a default call recorder and a
   /// provided stub.
   ///
   /// \param[in] stub
   ///   The stub where to forward the mock's method calls.
   ///
   /// \exception Exception neutral.
   ///
   Mock( std::shared_ptr<TI> stub );

   /// Constructor.
   ///
   /// This constructor constructs a mock with a provided call recorder and a
   /// provided stub.
   ///
   /// \param[in] recorder
   ///   The call recorder where to record the method calls and their arguments.
   ///
   /// \param[in] stub
   ///   The stub where to forward the mock's method calls.
   ///
   /// \exception Exception neutral.
   ///
   Mock(
      std::shared_ptr<CallRecorder<ConversionPolicy>> recorder,
      std::shared_ptr<TI> stub );

   /// Sets a functor to be invoked for an interface method.
   ///
   /// This method sets a functor to be run when the provided interface method
   /// is called. If a stub was set in the constructor, this functor provided
   /// here will override the stub in the constructor.
   ///
   /// \param[in] methodPtr
   ///   The interface method that shall get the overriding function.
   ///
   /// \param[in] functor
   ///   The functor that shall be invoked when the interface method is called.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... Parameters, class F>
   void override( R(TI::*methodPtr)(Parameters...), F functor );

   /// Sets a functor to be invoked for an interface method.
   ///
   /// This method works the same as the other set method. The difference is
   /// that it takes a const method pointer.
   ///
   /// \param[in] methodPtr
   ///   The interface method that shall get the overriding function.
   ///
   /// \param[in] functor
   ///   The functor that shall be invoked when the interface method is called.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... Parameters, class F>
   void override( R(TI::*methodPtr)(Parameters...) const, F functor );

   /// Finds all recorded calls for the method provided.
   ///
   /// This find method looks up all recorded occasions of calls to the method
   /// provided and returns the result as an iterable container of tuples. The
   /// lookup will only be done for this mock. If this mock is connected to a
   /// recorder shared with other mocks of the same interface, this method will
   /// not find the recorded calls to those mocks.
   ///
   /// Note. This method isn't const correct. It should be declared const since
   /// it doesn't change anything in this class. We chose to declare it
   /// non-const as a convenience when using this method. This way there are
   /// less occasions we explicitly need to cast the provided method call when
   /// it's overloaded on const only. It's likely that when const methods in the
   /// interface are used, the whole interface is taken as a const argument. If
   /// so, we can now declare the mock const, making this find method find const
   /// methods by default.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided method, returned as an iterable
   ///   container of tuples where each tuple contains the arguments provided in
   ///   that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... Parameters>
   auto find( R(TI::*methodPtr)(Parameters...) );

   /// Finds all recorded calls for the method provided.
   ///
   /// This find method works the same as the other find method. The difference
   /// is that it takes a const method pointer.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided method, returned as an iterable
   ///   container of tuples where each tuple contains the arguments provided in
   ///   that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... Parameters>
   auto find( R(TI::*methodPtr)(Parameters...) const ) const;

   /// Gets the identifier of the theoretical object mocked.
   ///
   /// The identifier that is returned is not connected to the instance of this
   /// mock but to the theoretical object that this mock is mocking. When a stub
   /// is provided it could be seen as the (theoretical) object, but when a stub
   /// isn't provided, the object is just theoretical.
   ///
   /// In practice this means that if copies are made of the mock, each copy
   /// will get the same identifier. Roughly this Mock class could be compared
   /// with a shared pointer where this identifier would be the shared pointer's
   /// referenced raw pointer.
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


protected:

   /// Records and calls the provided method.
   ///
   /// This call method should be called in the methods implementing the
   /// interface we are mocking. It's very important that the provided method is
   /// exactly the same as the interface method we're calling this method from.
   /// The method should be a method pointer to the interface and not the mock.
   /// See example below.
   /// ~~~
   /// virtual Status setInt( int i ) { return call( &ISomeClass::setInt, i ); }
   /// ~~~
   ///
   /// This method will record the call and its argument, and then forward the
   /// call to either an overriding function if provided, a provided stub, or
   /// return a default value, in that order. If a conversion policy has been
   /// provided, it will convert the arguments before recorded. This is useful
   /// for pointers since the referenced objects may have been deleted by the
   /// time the recorded calls are analyzed.
   ///
   /// Note. This method isn't const correct. It should be declared const since
   /// it doesn't change anything in this class. We chose to declare it
   /// non-const as a convenience when using this method. This way we don't need
   /// to do an explicit cast of the provided method if it's overloaded on
   /// const.
   ///
   /// \param[in] methodPtr
   ///   The method to record and forward the call to.
   ///
   /// \param[in,out] arguments
   ///   The arguments to record and forward to the method that is recorded.
   ///
   /// \returns
   ///   The result of the method that is recorded. The result comes from a
   ///   method override, a stub, or a default value, in that order.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... FncParameters, typename... Parameters>
   R call(
      R(TI::*methodPtr)(FncParameters...),
      Parameters&&... arguments );

   /// Records and calls the provided method.
   ///
   /// This call method works the same as the other call method. The difference
   /// is that it takes a const method pointer.
   ///
   /// \param[in] methodPtr
   ///   The method to record and forward the call to.
   ///
   /// \param[in,out] arguments
   ///   The arguments to record and forward to the method that is recorded.
   ///
   /// \returns
   ///   The result of the method that is recorded. The result comes from a
   ///   method override, a stub, or a default value, in that order.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... FncParameters, typename... Parameters>
   R call(
      R(TI::*methodPtr)(FncParameters...) const,
      Parameters&&... arguments ) const;


private:

   FiniteID mockID_;

   std::shared_ptr<CallRecorder<ConversionPolicy>> recorder_;

   FunctionMap functionMap_;

   std::shared_ptr<TI> stub_;


};


} // namespace


// Implementation.
#include "Mock.tcc"

