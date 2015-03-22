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
#include "unimock/ResultSet.hh"
#include "unimock/MinimalConversionPolicy.hh"


namespace
{

class SomeClassI
{
public:
   virtual ~SomeClassI() {}
   virtual void setInt( int i ) = 0;
   virtual void setIntConst( int i ) const = 0;
   virtual int getInt() const = 0;
   virtual void getIntByRef( int& ir ) = 0;
   virtual void setIntPtr( int* ip ) = 0;
   virtual void setClass( SomeClassI* scp ) = 0;
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
   : public unimock::Mock<SomeClassI, ConversionPolicy>
{
public:
   using unimock::Mock<SomeClassI, ConversionPolicy>::Mock;
   using unimock::Mock<SomeClassI, ConversionPolicy>::call;

   virtual void setInt( int i ) { call( &SomeClassI::setInt, i ); }
   virtual void setIntConst( int i ) const
      { call( &SomeClassI::setIntConst, i ); }
   virtual int getInt() const { return call( &SomeClassI::getInt ); }
   virtual void getIntByRef( int& ir ) { call( &SomeClassI::getIntByRef, ir ); }
   virtual void setIntPtr( int* ip ) { call( &SomeClassI::setIntPtr, ip ); }
   virtual void setClass( SomeClassI* scp )
      { call( &SomeClassI::setClass, scp ); }
   virtual void setStrPtr( const char* ccp )
      { call( &SomeClassI::setStrPtr, ccp ); }
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip )
      { call( &SomeClassI::setConstRefUPtr, uip ); }
   virtual void setUPtr( std::unique_ptr<int> uip )
      { call( &SomeClassI::setUPtr, std::move( uip ) ); }
   virtual void setSPtr( std::shared_ptr<int> sip )
      { call( &SomeClassI::setSPtr, sip ); }
   virtual std::unique_ptr<int> getUPtr() const
      { return call( &SomeClassI::getUPtr ); }
   virtual std::string getStr()
      { return call( &SomeClassI::getStr ); }
   virtual std::string getStr() const
      { return call( &SomeClassI::getStr ); }
};

class SomeClassStub : public SomeClassI
{
public:
   virtual void setInt( int i ) {}
   virtual void setIntConst( int i ) const {}
   virtual int getInt() const { return 42; }
   virtual void getIntByRef( int& ir ) { ir = 45; }
   virtual void setIntPtr( int* ip ) {}
   virtual void setClass( SomeClassI* scp ) {}
   virtual void setStrPtr( const char* ccp ) {}
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip ) {}
   virtual void setUPtr( std::unique_ptr<int> uip ) {}
   virtual void setSPtr( std::shared_ptr<int> sip ) {}
   virtual std::unique_ptr<int> getUPtr() const
      { return std::make_unique<int>( 50 ); }
   virtual std::string getStr() { return "non-const"; }
   virtual std::string getStr() const { return "const"; }
};

struct SomeConversionPolicy
{
   template<typename T>
   static auto convert( T&& object ) { return std::forward<T>( object ); }

   template<typename T>
   static T convert( const std::unique_ptr<T>& uip ) { return *uip; }

   static int convert( SomeClassI* scp ) { return scp->getInt(); }
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

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setInt ) );
      require( resultSet.get<0, 0>() == 3 );
   }

   test( "Call a stubbed mock method" );
   {
      SomeClassMock mock( std::shared_ptr<SomeClassI>( new SomeClassStub ) );
      int intVal = 23;

      mock.setInt( 3 );
      auto value = mock.getInt();
      mock.getIntByRef( intVal );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setInt ) );
      require( resultSet.get<0, 0>() == 3 );
      require( value == 42 );
      auto resultSet2 = makeResultSet( mock.find( &SomeClassI::getIntByRef ) );
      require( resultSet2.get<0, 0>() == 23 );
      require( intVal == 45 );
   }

   test( "Call a mock method with a conversion policy" );
   {
      SomeClassMockWConversion mock;
      SomeClassStub someClass;

      mock.setClass( &someClass );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setClass ) );
      require( resultSet.get<0, 0>() == someClass.getInt() );
   }

   test( "Call a mock const method" );
   {
      SomeClassMock mock;

      mock.setIntConst( 10 );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setIntConst ) );
      require( resultSet.get<0, 0>() == 10 );
   }

   test( "Call a mock method with a converted argument" );
   {
      SomeClassMock mock;
      int* ip = new int( 15 );

      mock.setIntPtr( ip );
      delete ip;

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setIntPtr ) );
      require( resultSet.get<0, 0>() == 15 );
   }

   test( "Call a mock method with a const char*" );
   {
      SomeClassMock mock;

      mock.setStrPtr( "string literal" );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setStrPtr ) );
      require( resultSet.get<0, 0>() == "string literal" );
   }

   test( "Call a mock method with a const unique_ptr&" );
   {
      SomeClassMock mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      mock.setConstRefUPtr( uip );

      auto resultSet =
         makeResultSet( mock.find( &SomeClassI::setConstRefUPtr ) );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Call a mock method with a unique_ptr" );
   {
      SomeClassMock mock;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      mock.setUPtr( std::move( uip ) );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setUPtr ) );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Call a mock method with a shared_ptr" );
   {
      SomeClassMock mock;
      std::shared_ptr<int> sip = std::make_shared<int>( 34 );

      mock.setSPtr( sip );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setSPtr ) );
      require( *resultSet.get<0, 0>() == 34 );
   }

   test( "Call a mock method with a minimal conversion policy" );
   {
      SomeClassMockWMinimalConversion mock;
      int* ip = new int;
      std::unique_ptr<int> uip = std::make_unique<int>( 30 );
      int* uirp = uip.get();

      mock.setIntPtr( ip );
      mock.setConstRefUPtr( uip );

      auto resultSet = makeResultSet( mock.find( &SomeClassI::setIntPtr ) );
      require( resultSet.get<0, 0>() == ip );
      delete ip;
      auto resultSet2 =
         makeResultSet( mock.find( &SomeClassI::setConstRefUPtr ) );
      require( resultSet2.get<0, 0>() == uirp );
   }

   test( "Call a mock method returning a unique_ptr" );
   {
      SomeClassMock mock;

      std::unique_ptr<int> uip = mock.getUPtr();

      require( uip == nullptr );
   }

   test( "Call a stubbed mock method returning a unique_ptr" );
   {
      SomeClassMock mock( std::shared_ptr<SomeClassI>( new SomeClassStub ) );

      std::unique_ptr<int> uip = mock.getUPtr();

      require( *uip == 50 );
   }

   test( "Call two mocks connected to the same call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      SomeClassMock mock1( recorder );
      SomeClassMock mock2( recorder );

      mock1.setInt( 3 );
      mock2.setInt( 5 );

      auto resultSet = makeResultSet( recorder->find( &SomeClassI::setInt ) );
      require( resultSet.get<0, 0>() == 3 );
      require( resultSet.get<1, 0>() == 5 );
      auto resultSet2 = makeResultSet( mock1.find( &SomeClassI::setInt ) );
      require( resultSet2.get<0, 0>() == 3 );
      auto resultSet3 = makeResultSet( mock2.find( &SomeClassI::setInt ) );
      require( resultSet3.get<0, 0>() == 5 );
   }

   test( "Find a particular mock in a shared call recorder" );
   {
      auto recorder = std::make_shared<CallRecorder<>>();
      SomeClassMock mock1( recorder );
      SomeClassMock mock2( recorder );

      mock1.setInt( 3 );
      mock2.setInt( 5 );

      auto resultSet = makeResultSet( recorder->find( &SomeClassI::setInt ) );
      require( resultSet.get<0, 0>() == 3 );
      require( resultSet.get<1, 0>() == 5 );
      auto resultSet2 = makeResultSet(
         recorder->find( mock2.getID(), &SomeClassI::setInt ) );
      require( resultSet2.get<0, 0>() == 5 );
   }

   test( "Call a mock with two methods overloaded on const" );
   {
      auto stub = std::shared_ptr<SomeClassI>( new SomeClassStub );
      SomeClassMock mock( stub );
      const SomeClassMock constMock( stub );

      auto s1 = mock.getStr();
      auto s2 = constMock.getStr();

      auto resultSet = makeResultSet( mock.find( &SomeClassI::getStr ) );
      require( resultSet.size() == 1 );
      require( s1 == "non-const" );
      auto resultSet2 = makeResultSet( constMock.find( &SomeClassI::getStr ) );
      require( resultSet2.size() == 1 );
      require( s2 == "const" );
   }

   test( "Call a mock with an overrided stub" );
   {
      auto stub = std::shared_ptr<SomeClassI>( new SomeClassStub );
      SomeClassMock mock( stub );

      auto iValue1 = mock.getInt();
      mock.set( &SomeClassI::getInt, []{ return 52; } );
      auto iValue2 = mock.getInt();

      auto resultSet = makeResultSet( mock.find( &SomeClassI::getInt ) );
      require( resultSet.size() == 2 );
      require( iValue1 == 42 );
      require( iValue2 == 52 );
   }

}

