/*

   FunctionMockTest.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include <functional>

#include "Test.hh"

#include "unimock/FunctionMock.hh"
#include "unimock/CallRecorder.hh"
#include "unimock/ResultSet.hh"


namespace
{

void setIntStr( int i, std::string s ) {}
void setConstRefUPtr( const std::unique_ptr<int>& uip ) {}
void setUPtr( std::unique_ptr<int> uip ) {}
void setFnc( void(*functionPtr)(int, std::string) )
   { (*functionPtr)( 44, "fortyfour" ); }
void setFunction( std::function<void(int, std::string)> f )
   { f( 45, "fortyfive" ); }

} // unnamed namespace


void testFunctionMock()
{
   using namespace unimock;

   test( "Provide a mock function" );
   {
      FunctionMock<void(int, std::string)> mock;

      setFnc( &mock.function );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 44 );
      require( resultSet.get<0, 1>() == "fortyfour" );
   }

   test( "Provide a stubbed mock function" );
   {
      FunctionMock<void(int, std::string)> mock( &setIntStr );

      setFnc( &mock.function );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 44 );
      require( resultSet.get<0, 1>() == "fortyfour" );
   }

   test( "Call a mock function with a const unique_ptr&" );
   {
      FunctionMock<void(const std::unique_ptr<int>&)> mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock.function( uip );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a mock function with a unique_ptr" );
   {
      FunctionMock<void(std::unique_ptr<int>)> mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock.function( std::move( uip ) );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a stubbed mock function with a const unique_ptr&" );
   {
      FunctionMock<void(const std::unique_ptr<int>&)> mock( &setConstRefUPtr );
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock.function( uip );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a stubbed mock function with a unique_ptr" );
   {
      FunctionMock<void(std::unique_ptr<int>)> mock( &setUPtr );
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock.function( std::move( uip ) );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a function with a mock function" );
   {
      FunctionMock<void(int, std::string)> mock;

      setFunction( mock.function );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 45 );
      require( resultSet.get<0, 1>() == "fortyfive" );
   }

   test(
      "Call two functions with two mocks connected to the same call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      FunctionMock<void(int, std::string)> mock1( recorder );
      FunctionMock<void(int, std::string)> mock2( recorder );

      setFunction( mock1.function );
      setFunction( mock2.function );

      // NOTE! The call recorder can't differentiate between different mocks
      // since the static member function doesn't know to which object it
      // belongs.
      auto resultSet = makeResultSet( mock1.find() );
      require( resultSet.get<0, 0>() == 45 );
      require( resultSet.get<0, 1>() == "fortyfive" );
      require( resultSet.get<1, 0>() == 45 );
      require( resultSet.get<1, 1>() == "fortyfive" );
   }

}

