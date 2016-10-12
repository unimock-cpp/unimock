/*

   MockTest.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include <memory>       // std::unique_ptr, std::shared_ptr
#include <iostream>

#include "Test.hh"

#include "unimock/Mock.hh"
#include "unimock/ResultSetFactory.hh"
#include "unimock/MinimalConversionPolicy.hh"


namespace
{

class ISomeClass
{
public:
   virtual ~ISomeClass() {}
   virtual void setInt( int i ) = 0;
   virtual void setAnotherInt( int i ) = 0;
   virtual void setIntConst( int i ) const = 0;
   virtual int getInt() const = 0;
   virtual void getIntByRef( int& ir ) = 0;
   virtual void setIntPtr( int* ip ) = 0;
   virtual void setClass( ISomeClass* scp ) = 0;
   virtual void setStrPtr( const char* ccp ) = 0;
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip ) = 0;
   virtual void setUPtr( std::unique_ptr<int> uip ) = 0;
   virtual void setSPtr( std::shared_ptr<int> sip ) = 0;
   virtual std::unique_ptr<int> getUPtr() const = 0;
   virtual std::string getStr() = 0;
   virtual std::string getStr() const = 0;
};

template<class ConversionPolicy>
class SomeClassBaseMock
   : public unimock::Mock<ISomeClass, ConversionPolicy>
{
public:
   using unimock::Mock<ISomeClass, ConversionPolicy>::Mock;
   using unimock::Mock<ISomeClass, ConversionPolicy>::call;

   void setInt( int i ) override { call( &ISomeClass::setInt, i ); }
   void setAnotherInt( int i ) override
      { call( &ISomeClass::setAnotherInt, i ); }
   void setIntConst( int i ) const override
      { call( &ISomeClass::setIntConst, i ); }
   int getInt() const override { return call( &ISomeClass::getInt ); }
   void getIntByRef( int& ir ) override
      { call( &ISomeClass::getIntByRef, ir ); }
   void setIntPtr( int* ip ) override { call( &ISomeClass::setIntPtr, ip ); }
   void setClass( ISomeClass* scp ) override
      { call( &ISomeClass::setClass, scp ); }
   void setStrPtr( const char* ccp ) override
      { call( &ISomeClass::setStrPtr, ccp ); }
   void setConstRefUPtr( const std::unique_ptr<int>& uip ) override
      { call( &ISomeClass::setConstRefUPtr, uip ); }
   void setUPtr( std::unique_ptr<int> uip ) override
      { call( &ISomeClass::setUPtr, std::move( uip ) ); }
   void setSPtr( std::shared_ptr<int> sip ) override
      { call( &ISomeClass::setSPtr, sip ); }
   std::unique_ptr<int> getUPtr() const override
      { return call( &ISomeClass::getUPtr ); }
   std::string getStr() override
      { return call( &ISomeClass::getStr ); }
   std::string getStr() const override
      { return call( &ISomeClass::getStr ); }
};

class SomeClassStub : public ISomeClass
{
public:
   void setInt( int i ) override {}
   void setAnotherInt( int i ) override {}
   void setIntConst( int i ) const override {}
   int getInt() const override { return 42; }
   void getIntByRef( int& ir ) override { ir = 45; }
   void setIntPtr( int* ip ) override {}
   void setClass( ISomeClass* scp ) override {}
   void setStrPtr( const char* ccp ) override {}
   void setConstRefUPtr( const std::unique_ptr<int>& uip ) override {}
   void setUPtr( std::unique_ptr<int> uip ) override {}
   void setSPtr( std::shared_ptr<int> sip ) override {}
   std::unique_ptr<int> getUPtr() const override
      { return std::make_unique<int>( 50 ); }
   std::string getStr() override { return "non-const"; }
   std::string getStr() const override { return "const"; }
};

struct SomeConversionPolicy
{
   template<typename T>
   static auto convert( T&& object ) { return std::forward<T>( object ); }

   template<typename T>
   static T convert( const std::unique_ptr<T>& uip ) { return *uip; }

   static int convert( ISomeClass* scp ) { return scp->getInt(); }
};

using SomeClassMock =
   SomeClassBaseMock<unimock::DefaultConversionPolicy>;

using SomeClassMockWConversion =
   SomeClassBaseMock<SomeConversionPolicy>;

using SomeClassMockWMinimalConversion =
   SomeClassBaseMock<unimock::MinimalConversionPolicy>;


} // unnamed namespace


void testMock()
{
   using namespace unimock;

   test( "Call a mock method" );
   {
      SomeClassMock mock;

      mock.setInt( 3 );

      auto resultSet = makeResultSet( mock, &ISomeClass::setInt );
      ensure( resultSet.get<0, 0>() == 3 );
   }

   test( "Call a stubbed mock method" );
   {
      SomeClassMock mock( std::shared_ptr<ISomeClass>( new SomeClassStub ) );
      int intVal = 23;

      mock.setInt( 3 );
      auto value = mock.getInt();
      mock.getIntByRef( intVal );

      auto resultSet = makeResultSet( mock, &ISomeClass::setInt );
      ensure( resultSet.get<0, 0>() == 3 );
      ensure( value == 42 );
      auto resultSet2 = makeResultSet( mock, &ISomeClass::getIntByRef );
      ensure( resultSet2.get<0, 0>() == 23 );
      ensure( intVal == 45 );
   }

   test( "Call a mock method with a conversion policy" );
   {
      SomeClassMockWConversion mock;
      SomeClassStub someClass;

      mock.setClass( &someClass );

      auto resultSet = makeResultSet( mock, &ISomeClass::setClass );
      ensure( resultSet.get<0, 0>() == someClass.getInt() );
   }

   test( "Call two mock methods with the same signature" );
   {
      SomeClassMock mock;

      mock.setInt( 3 );
      mock.setAnotherInt( 5 );

      auto resultSet = makeResultSet( mock, &ISomeClass::setInt );
      ensure( resultSet.size() == 1 );
      auto resultSet2 = makeResultSet( mock, &ISomeClass::setAnotherInt );
      ensure( resultSet2.size() == 1 );
   }

   test( "Call a mock const method" );
   {
      SomeClassMock mock;

      mock.setIntConst( 10 );

      auto resultSet = makeResultSet( mock, &ISomeClass::setIntConst );
      ensure( resultSet.get<0, 0>() == 10 );
   }

   test( "Call a mock method with a converted argument" );
   {
      SomeClassMock mock;
      int* ip = new int( 15 );

      mock.setIntPtr( ip );
      delete ip;

      auto resultSet = makeResultSet( mock, &ISomeClass::setIntPtr );
      ensure( resultSet.get<0, 0>() == 15 );
   }

   test( "Call a mock method with a const char*" );
   {
      SomeClassMock mock;

      mock.setStrPtr( "string literal" );

      auto resultSet = makeResultSet( mock, &ISomeClass::setStrPtr );
      ensure( resultSet.get<0, 0>() == "string literal" );
   }

   test( "Call a mock method with a const unique_ptr&" );
   {
      SomeClassMock mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock.setConstRefUPtr( uip );

      auto resultSet = makeResultSet( mock, &ISomeClass::setConstRefUPtr );
      ensure( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a mock method with a unique_ptr" );
   {
      SomeClassMock mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock.setUPtr( std::move( uip ) );

      auto resultSet = makeResultSet( mock, &ISomeClass::setUPtr );
      ensure( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a mock method with a shared_ptr" );
   {
      SomeClassMock mock;
      std::shared_ptr<int> sip = std::make_shared<int>( 34 );

      mock.setSPtr( sip );

      auto resultSet = makeResultSet( mock, &ISomeClass::setSPtr );
      ensure( *resultSet.get<0, 0>() == 34 );
   }

   test( "Call a mock method with a minimal conversion policy" );
   {
      SomeClassMockWMinimalConversion mock;
      int* ip = new int;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );
      int* uirp = uip.get();

      mock.setIntPtr( ip );
      mock.setConstRefUPtr( uip );

      auto resultSet = makeResultSet( mock, &ISomeClass::setIntPtr );
      ensure( resultSet.get<0, 0>() == ip );
      delete ip;
      auto resultSet2 = makeResultSet( mock, &ISomeClass::setConstRefUPtr );
      ensure( resultSet2.get<0, 0>() == uirp );
   }

   test( "Call a mock method returning a unique_ptr" );
   {
      SomeClassMock mock;

      std::unique_ptr<int> uip = mock.getUPtr();

      ensure( uip == nullptr );
   }

   test( "Call a stubbed mock method returning a unique_ptr" );
   {
      SomeClassMock mock( std::shared_ptr<ISomeClass>( new SomeClassStub ) );

      std::unique_ptr<int> uip = mock.getUPtr();

      ensure( *uip == 50 );
   }

   test( "Call two mocks connected to the same call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      SomeClassMock mock1( recorder );
      SomeClassMock mock2( recorder );

      mock1.setInt( 3 );
      mock2.setInt( 5 );

      auto resultSet = makeResultSet( recorder, &ISomeClass::setInt );
      ensure( resultSet.get<0, 0>() == 3 );
      ensure( resultSet.get<1, 0>() == 5 );
      auto resultSet2 = makeResultSet( mock1, &ISomeClass::setInt );
      ensure( resultSet2.get<0, 0>() == 3 );
      auto resultSet3 = makeResultSet( mock2, &ISomeClass::setInt );
      ensure( resultSet3.get<0, 0>() == 5 );
   }

   test( "Find a particular mock in a shared call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      SomeClassMock mock1( recorder );
      SomeClassMock mock2( recorder );

      mock1.setInt( 3 );
      mock2.setInt( 5 );

      auto resultSet = makeResultSet( recorder, &ISomeClass::setInt );
      ensure( resultSet.get<0, 0>() == 3 );
      ensure( resultSet.get<1, 0>() == 5 );
      auto resultSet2 = makeResultSet( recorder, mock2, &ISomeClass::setInt );
      ensure( resultSet2.get<0, 0>() == 5 );
   }

   test( "Call a mock with two methods overloaded on const" );
   {
      auto stub = std::shared_ptr<ISomeClass>( new SomeClassStub );
      SomeClassMock mock( stub );
      const SomeClassMock constMock( stub );

      auto s1 = mock.getStr();
      auto s2 = constMock.getStr();

      auto resultSet = makeResultSet( mock, &ISomeClass::getStr );
      ensure( resultSet.size() == 1 );
      ensure( s1 == "non-const" );
      auto resultSet2 = makeResultSet( constMock, &ISomeClass::getStr );
      ensure( resultSet2.size() == 1 );
      ensure( s2 == "const" );
   }

   test( "Call a mock with an overrided stub" );
   {
      auto stub = std::shared_ptr<ISomeClass>( new SomeClassStub );
      SomeClassMock mock( stub );

      auto iValue1 = mock.getInt();
      mock.override( &ISomeClass::getInt, []{ return 52; } );
      auto iValue2 = mock.getInt();

      auto resultSet = makeResultSet( mock, &ISomeClass::getInt );
      ensure( resultSet.size() == 2 );
      ensure( iValue1 == 42 );
      ensure( iValue2 == 52 );
   }

}

