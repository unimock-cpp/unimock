/*

   ResultSetFactory.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <memory>    // std::shared_ptr

#include "unimock/FunctionMock.hh"
#include "unimock/FunctorMock.hh"
#include "unimock/Mock.hh"
#include "unimock/CallRecorder.hh"


namespace unimock
{

/// Creates a ResultSet from a function mock.
///
/// Since the constructor of ResultSet can't be a template method, this
/// non-member template function is provided as a convenience function to remove
/// the need to provide the tuple parameter types.
///
/// Note! This function will find all function mocks' static member functions
/// with the same signature. Since the static member function is static, it
/// can't record to which function mock instance it belongs. Currently there is
/// no workaround for this deficiency.
///
/// \param[in] functionMock
///   The function mock to get the call history from.
///
/// \exception Exception neutral.
///
template<typename R, typename... Parameters, class ConversionPolicy>
auto makeResultSet(
   const FunctionMock<R(Parameters...), ConversionPolicy>& functionMock );

/// Creates a ResultSet from a functor mock.
///
/// Since the constructor of ResultSet can't be a template method, this
/// non-member template function is provided as a convenience function to remove
/// the need to provide the tuple parameter types.
///
/// \param[in] functorMock
///   The functor mock to get the call history from.
///
/// \exception Exception neutral.
///
template<typename R, typename... Parameters, class ConversionPolicy>
auto makeResultSet(
   const FunctorMock<R(Parameters...), ConversionPolicy>& functorMock );

/// Creates a ResultSet from a mock and a method in its interface.
///
/// Since the constructor of ResultSet can't be a template method, this
/// non-member template function is provided as a convenience function to remove
/// the need to provide the tuple parameter types.
///
/// Note. This function isn't const correct. It should take the mock by a const
/// reference since this function doesn't change anything in the mock. See an
/// explanation in Mock and its find method.
///
/// \param[in] mock
///   The mock to get the call history from.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class TI, class ConversionPolicy, typename R, typename... Parameters>
auto makeResultSet(
   Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) );

/// Creates a ResultSet from a mock and a method in its interface.
///
/// This function works the same as the other factory function. The difference
/// is that it takes a const method pointer.
///
/// \param[in] mock
///   The mock to get the call history from.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class TI, class ConversionPolicy, typename R, typename... Parameters>
auto makeResultSet(
   const Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) const );

/// Creates a ResultSet from a call recorder and a method.
///
/// This function works the same as the other factory function.
///
/// \param[in] callRecorder
///   The call recorder to get the call history from.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   R(TI::*methodPtr)(Parameters...) );

/// Creates a ResultSet from a call recorder and a method.
///
/// This function works the same as the other factory function. The difference
/// is that it takes a const method pointer.
///
/// \param[in] callRecorder
///   The call recorder to get the call history from.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   R(TI::*methodPtr)(Parameters...) const );

/// Creates a ResultSet from a call recorder, a mock, and a method.
///
/// This function works the same as the other factory function.
///
/// Note. This function isn't const correct. It should take the mock by a const
/// reference since this function doesn't change anything in the mock. See an
/// explanation in Mock and its find method.
///
/// \param[in] callRecorder
///   The call recorder to get the call history from.
///
/// \param[in] mock
///   The mock that shall be looked up.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) );

/// Creates a ResultSet from a call recorder, a mock, and a method.
///
/// This function works the same as the other factory function. The difference
/// is that it takes a const method pointer.
///
/// \param[in] callRecorder
///   The call recorder to get the call history from.
///
/// \param[in] mock
///   The mock that shall be looked up.
///
/// \param[in] methodPtr
///   The method who's recorded calls shall be looked up.
///
/// \exception Exception neutral.
///
template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   const Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) const );


} // namespace


// Implementation.
#include "ResultSetFactory.tcc"

