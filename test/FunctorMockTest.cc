/*

   FunctorMockTest.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include <functional>

#include "Test.hh"

#include "unimock/FunctorMock.hh"
#include "unimock/CallRecorder.hh"
#include "unimock/ResultSet.hh"


namespace
{

void setIntStr( int i, std::string s ) {}
void setConstRefUPtr( const std::unique_ptr<int>& uip ) {}
void setUPtr( std::unique_ptr<int> uip ) {}
void setFunction( std::function<void(int, std::string)> f )
   { f( 45, "fortyfive" ); }

} // unnamed namespace


void testFunctorMock()
{
   using namespace unimock;

   test( "Call a mock functor" );
   {
      FunctorMock<void(int, std::string)> mock;

      mock( 10, "ten" );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 10 );
      require( resultSet.get<0, 1>() == "ten" );
   }

   test( "Call a stubbed mock functor" );
   {
      FunctorMock<void(int, std::string)> mock( &setIntStr );
      int i = 15;
      std::string str = "fifteen";

      mock( i, str );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == i );
      require( resultSet.get<0, 1>() == str );
   }

   test( "Call a mock functor with a const unique_ptr&" );
   {
      FunctorMock<void(const std::unique_ptr<int>&)> mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock( uip );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a mock functor with a unique_ptr" );
   {
      FunctorMock<void(std::unique_ptr<int>)> mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock( std::move( uip ) );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a stubbed mock functor with a const unique_ptr&" );
   {
      FunctorMock<void(const std::unique_ptr<int>&)> mock( &setConstRefUPtr );
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock( uip );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a stubbed mock functor with a unique_ptr" );
   {
      FunctorMock<void(std::unique_ptr<int>)> mock( &setUPtr );
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock( std::move( uip ) );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a function with a mock functor" );
   {
      FunctorMock<void(int, std::string)> mock;

      setFunction( mock );

      auto resultSet = makeResultSet( mock.find() );
      require( resultSet.get<0, 0>() == 45 );
      require( resultSet.get<0, 1>() == "fortyfive" );
   }

   test(
      "Call two functions with two mocks connected to the same call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      FunctorMock<void(int, std::string)> mock1( recorder );
      FunctorMock<void(int, std::string)> mock2( recorder );

      setFunction( mock1 );
      setFunction( mock2 );

      auto resultSet = makeResultSet( mock1.find() );
      require( resultSet.size() == 1 );
      require( resultSet.get<0, 0>() == 45 );
      require( resultSet.get<0, 1>() == "fortyfive" );
      auto resultSet2 = makeResultSet( mock2.find() );
      require( resultSet2.size() == 1 );
      require( resultSet2.get<0, 0>() == 45 );
      require( resultSet2.get<0, 1>() == "fortyfive" );
   }

}

