/*

   CallRecorderTest.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include <memory>    // std::unique_ptr, std::shared_ptr

#include "Test.hh"

#include "unimock/CallRecorder.hh"
#include "unimock/ResultSet.hh"
#include "unimock/FiniteID.hh"


namespace
{

void setIntStr( int i, std::string s ) {}
void setVal( int i ) {}
void setVal( double d ) {}

class SomeClassI
{
public:
   virtual ~SomeClassI() {}
   virtual void setIntStr( int i, std::string s ) = 0;
   virtual void setIntStrConst( int i, std::string s ) const = 0;
   virtual void setInt( int i ) = 0;
   virtual void setInt( int i ) const = 0;
   virtual void setIntPtr( int* ip ) = 0;
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip ) = 0;
   virtual void setUPtr( std::unique_ptr<int> uip ) = 0;
   virtual void setSPtr( std::shared_ptr<int> sip ) = 0;
};

class SomeClass : public SomeClassI
{
public:
   virtual void setIntStr( int i, std::string s ) {}
   virtual void setIntStrConst( int i, std::string s ) const {}
   virtual void setInt( int i ) {}
   virtual void setInt( int i ) const {}
   virtual void setIntPtr( int* ip ) {};
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip ) {}
   virtual void setUPtr( std::unique_ptr<int> uip ) {}
   virtual void setSPtr( std::shared_ptr<int> sip ) {}
};

} // unnamed namespace


void testCallRecorder()
{
   using namespace unimock;

   test( "Record a function call" );
   {
      CallRecorder<> recorder;

      recorder.record( setIntStr, 3, "three" );

      auto resultSet = makeResultSet( recorder.find( setIntStr ) );
      require( resultSet.get<0, 0>() == 3 );
      require( resultSet.get<0, 1>() == "three" );
   }

   test( "Record a method call" );
   {
      CallRecorder<> recorder;

      recorder.record( FiniteID(), &SomeClassI::setIntStr, 25, "twentyfive" );

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setIntStr ) );
      require( resultSet.get<0, 0>() == 25 );
      require( resultSet.get<0, 1>() == "twentyfive" );
   }

   test( "Record a const method call" );
   {
      CallRecorder<> recorder;

      recorder.record(
         FiniteID(), &SomeClassI::setIntStrConst, 26, "twentysix" );

      auto resultSet =
         makeResultSet( recorder.find( &SomeClassI::setIntStrConst ) );
      require( resultSet.get<0, 0>() == 26 );
      require( resultSet.get<0, 1>() == "twentysix" );
   }

   test( "Record three function calls" );
   {
      CallRecorder<> recorder;

      recorder.record( setIntStr, 1, "one" );
      recorder.record( setIntStr, 2, "two" );
      recorder.record( setIntStr, 3, "three" );

      auto resultSet = makeResultSet( recorder.find( setIntStr ) );
      require( resultSet.get<0, 0>() == 1 );
      require( resultSet.get<0, 1>() == "one" );
      require( resultSet.get<1, 0>() == 2 );
      require( resultSet.get<1, 1>() == "two" );
      require( resultSet.get<2, 0>() == 3 );
      require( resultSet.get<2, 1>() == "three" );
   }

   test( "Record two overloaded function calls" );
   {
      CallRecorder<> recorder;

      recorder.record( static_cast<void(*)(int)>( setVal ), 3 );
      recorder.record( static_cast<void(*)(double)>( setVal ), 3.14 );

      auto resultSet = makeResultSet( recorder.find(
         static_cast<void(*)(int)>( setVal ) ) );
      require( resultSet.get<0, 0>() == 3 );
      auto resultSet2 = makeResultSet( recorder.find(
         static_cast<void(*)(double)>( setVal ) ) );
      require( resultSet2.get<0, 0>() > 3.1 );
   }

   test( "Record two overloaded method calls" );
   {
      CallRecorder<> recorder;

      recorder.record(
         FiniteID(),
         static_cast<void(SomeClassI::*)(int)>( &SomeClassI::setInt ),
         3 );
      recorder.record(
         FiniteID(),
         static_cast<void(SomeClassI::*)(int) const>( &SomeClassI::setInt ),
         5 );

      auto resultSet = makeResultSet( recorder.find(
         static_cast<void(SomeClassI::*)(int)>( &SomeClassI::setInt ) ) );
      require( resultSet.get<0, 0>() == 3 );
      auto resultSet2 = makeResultSet( recorder.find(
         static_cast<void(SomeClassI::*)(int) const>( &SomeClassI::setInt ) ) );
      require( resultSet2.get<0, 0>() == 5 );
   }

   test( "Record a method call with a converted argument" );
   {
      CallRecorder<> recorder;
      int* ip = new int( 15 );

      recorder.record( FiniteID(), &SomeClassI::setIntPtr, ip );
      delete ip;

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setIntPtr ) );
      require( resultSet.get<0, 0>() == 15 );
   }

   test( "Record a method call with a const unique_ptr&" );
   {
      CallRecorder<> recorder;
      const std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      recorder.record( FiniteID(), &SomeClassI::setConstRefUPtr, uip );

      auto resultSet =
         makeResultSet( recorder.find( &SomeClassI::setConstRefUPtr ) );
      require( resultSet.get<0, 0>() == 30 );
   }

   test( "Record a method call with a unique_ptr" );
   {
      CallRecorder<> recorder;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      recorder.record( FiniteID(), &SomeClassI::setUPtr, std::move( uip ) );

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setUPtr ) );
      require( resultSet.get<0, 0>() == 32 );
   }

   test( "Record a method call with a shared_ptr" );
   {
      CallRecorder<> recorder;
      std::shared_ptr<int> sip = std::make_shared<int>( 34 );

      recorder.record( FiniteID(), &SomeClassI::setSPtr, sip );

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setSPtr ) );
      require( *resultSet.get<0, 0>() == 34 );
   }

   test( "Record method calls using different objects" );
   {
      CallRecorder<> recorder;
      FiniteID id1;
      FiniteID id2 = FiniteID::generate();
      FiniteID id3 = FiniteID::generate();

      recorder.record( id1, &SomeClassI::setIntStr, 3, "three" );
      recorder.record( id2, &SomeClassI::setIntStr, 5, "five" );
      recorder.record( id3, &SomeClassI::setIntStr, 8, "eight" );

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setIntStr ) );
      require( resultSet.get<0, 0>() == 3 );
      require( resultSet.get<1, 0>() == 5 );
      require( resultSet.get<2, 0>() == 8 );
      auto resultSet2 =
         makeResultSet( recorder.find( id1, &SomeClassI::setIntStr ) );
      require( resultSet2.get<0, 0>() == 3 );
      auto resultSet3 =
         makeResultSet( recorder.find( id2, &SomeClassI::setIntStr ) );
      require( resultSet3.get<0, 0>() == 5 );
      auto resultSet4 =
         makeResultSet( recorder.find( id3, &SomeClassI::setIntStr ) );
      require( resultSet4.get<0, 0>() == 8 );
   }

   test( "Retrieve data by means of type instead of index" );
   {
      CallRecorder<> recorder;

      recorder.record( FiniteID(), &SomeClassI::setIntStr, 3, "three" );

      auto resultSet = makeResultSet( recorder.find( &SomeClassI::setIntStr ) );
      require( resultSet.get<0, int>() == 3 );
      require( resultSet.get<0, std::string>() == "three" );
   }

}

