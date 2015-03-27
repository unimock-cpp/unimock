/*

   ResultSetFactory.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include "unimock/ResultSet.hh"


namespace unimock
{

template<typename R, typename... Parameters, class ConversionPolicy>
auto makeResultSet(
   const FunctionMock<R(Parameters...), ConversionPolicy>& functionMock )
{
   return makeResultSet( functionMock.find() );
}

template<typename R, typename... Parameters, class ConversionPolicy>
auto makeResultSet(
   const FunctorMock<R(Parameters...), ConversionPolicy>& functorMock )
{
   return makeResultSet( functorMock.find() );
}

template<class TI, class ConversionPolicy, typename R, typename... Parameters>
auto makeResultSet(
   Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) )
{
   return makeResultSet( mock.find( methodPtr ) );
}

template<class TI, class ConversionPolicy, typename R, typename... Parameters>
auto makeResultSet(
   const Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) const )
{
   return makeResultSet( mock.find( methodPtr ) );
}

template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   R(TI::*methodPtr)(Parameters...) )
{
   return makeResultSet( callRecorder->find( methodPtr ) );
}

template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   R(TI::*methodPtr)(Parameters...) const )
{
   return makeResultSet( callRecorder->find( methodPtr ) );
}

template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) )
{
   return makeResultSet( callRecorder->find( mock.getID(), methodPtr ) );
}

template<class ConversionPolicy, typename R, class TI, typename... Parameters>
auto makeResultSet(
   const std::shared_ptr<CallRecorder<ConversionPolicy>>& callRecorder,
   const Mock<TI, ConversionPolicy>& mock,
   R(TI::*methodPtr)(Parameters...) const )
{
   return makeResultSet( callRecorder->find( mock.getID(), methodPtr ) );
}


} // namespace

