/*

   ResultSetFactoryTest.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include "Test.hh"

#include "unimock/FunctionMock.hh"
#include "unimock/FunctorMock.hh"
#include "unimock/Mock.hh"
#include "unimock/CallRecorder.hh"
#include "unimock/ResultSetFactory.hh"


namespace
{

void setFnc( void(*functionPtr)(int, std::string) )
{
   (*functionPtr)( 42, "fortytwo" );
}

class SomeClassI
{
public:
   virtual ~SomeClassI() {}
   virtual std::string getStr() = 0;
   virtual std::string getStr() const = 0;
};

class SomeClassMock : public unimock::Mock<SomeClassI>
{
public:
   using unimock::Mock<SomeClassI>::Mock;
   virtual std::string getStr() { return call( &SomeClassI::getStr ); }
   virtual std::string getStr() const { return call( &SomeClassI::getStr ); }
};

class SomeClassStub : public SomeClassI
{
public:
   virtual std::string getStr() { return "non-const"; }
   virtual std::string getStr() const { return "const"; }
};

} // unnamed namespace


void testResultSetFactory()
{
   using namespace unimock;

   test( "Provide a mock function" );
   {
      FunctionMock<void(int, std::string)> mock;

      setFnc( &mock.function );

      auto resultSet = makeResultSet( mock );
      require( resultSet.get<0, 0>() == 42 );
      require( resultSet.get<0, 1>() == "fortytwo" );
   }

   test( "Call a mock functor" );
   {
      FunctorMock<void(int, std::string)> mock;

      mock( 10, "ten" );

      auto resultSet = makeResultSet( mock );
      require( resultSet.get<0, 0>() == 10 );
      require( resultSet.get<0, 1>() == "ten" );
   }

   test( "Find two methods overloaded on const, with mock and method" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      auto stub = std::shared_ptr<SomeClassI>( new SomeClassStub );
      SomeClassMock mock( recorder, stub );
      const SomeClassMock constMock( recorder, stub );

      auto s1 = mock.getStr();
      s1 = mock.getStr();
      auto s2 = constMock.getStr();

      auto resultSet = makeResultSet( mock, &SomeClassI::getStr );
      require( resultSet.size() == 2 );
      require( s1 == "non-const" );
      auto resultSet2 = makeResultSet( constMock, &SomeClassI::getStr );
      require( resultSet2.size() == 1 );
      require( s2 == "const" );
   }

   test( "Find two methods overloaded on const, with recorder, mock, method" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      auto stub = std::shared_ptr<SomeClassI>( new SomeClassStub );
      SomeClassMock mock( recorder, stub );
      const SomeClassMock constMock( recorder, stub );

      auto s1 = mock.getStr();
      s1 = mock.getStr();
      auto s2 = constMock.getStr();

      auto resultSet = makeResultSet( recorder, mock, &SomeClassI::getStr );
      require( resultSet.size() == 2 );
      require( s1 == "non-const" );
      auto resultSet2 =
         makeResultSet( recorder, constMock, &SomeClassI::getStr );
      require( resultSet2.size() == 1 );
      require( s2 == "const" );
   }

   test( "Find two methods overloaded on const, with recorder and method" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      auto stub = std::shared_ptr<SomeClassI>( new SomeClassStub );
      SomeClassMock mock( recorder, stub );
      const SomeClassMock constMock( recorder, stub );

      auto s1 = mock.getStr();
      s1 = mock.getStr();
      auto s2 = constMock.getStr();

      auto resultSet = makeResultSet(
         recorder,
         static_cast<std::string(SomeClassI::*)()>( &SomeClassI::getStr ) );
      require( resultSet.size() == 2 );
      require( s1 == "non-const" );
      auto resultSet2 = makeResultSet(
         recorder,
         static_cast<std::string(SomeClassI::*)() const>(
            &SomeClassI::getStr ) );
      require( resultSet2.size() == 1 );
      require( s2 == "const" );
   }

}

