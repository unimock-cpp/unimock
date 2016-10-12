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

class ISomeClass
{
public:
   virtual ~ISomeClass() {}
   virtual void setIntStr( int i, std::string s ) = 0;
   virtual void setIntStrConst( int i, std::string s ) const = 0;
   virtual void setInt( int i ) = 0;
   virtual void setInt( int i ) const = 0;
   virtual void setDouble( double d ) = 0;
   virtual void setAnotherDouble( double d ) = 0;
   virtual void setIntPtr( int* ip ) = 0;
   virtual void setConstRefUPtr( const std::unique_ptr<int>& uip ) = 0;
   virtual void setUPtr( std::unique_ptr<int> uip ) = 0;
   virtual void setSPtr( std::shared_ptr<int> sip ) = 0;
};

class SomeClass : public ISomeClass
{
public:
   void setIntStr( int i, std::string s ) override {}
   void setIntStrConst( int i, std::string s ) const override {}
   void setInt( int i ) override {}
   void setInt( int i ) const override {}
   void setDouble( double d ) override {}
   void setAnotherDouble( double d ) override {}
   void setIntPtr( int* ip ) override {};
   void setConstRefUPtr( const std::unique_ptr<int>& uip ) override {}
   void setUPtr( std::unique_ptr<int> uip ) override {}
   void setSPtr( std::shared_ptr<int> sip ) override {}
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
      ensure( resultSet.get<0, 0>() == 3 );
      ensure( resultSet.get<0, 1>() == "three" );
   }

   test( "Record a method call" );
   {
      CallRecorder<> recorder;

      recorder.record( FiniteID(), &ISomeClass::setIntStr, 25, "twentyfive" );

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setIntStr ) );
      ensure( resultSet.get<0, 0>() == 25 );
      ensure( resultSet.get<0, 1>() == "twentyfive" );
   }

   test( "Record a const method call" );
   {
      CallRecorder<> recorder;

      recorder.record(
         FiniteID(), &ISomeClass::setIntStrConst, 26, "twentysix" );

      auto resultSet =
         makeResultSet( recorder.find( &ISomeClass::setIntStrConst ) );
      ensure( resultSet.get<0, 0>() == 26 );
      ensure( resultSet.get<0, 1>() == "twentysix" );
   }

   test( "Record three function calls" );
   {
      CallRecorder<> recorder;

      recorder.record( setIntStr, 1, "one" );
      recorder.record( setIntStr, 2, "two" );
      recorder.record( setIntStr, 3, "three" );

      auto resultSet = makeResultSet( recorder.find( setIntStr ) );
      ensure( resultSet.get<0, 0>() == 1 );
      ensure( resultSet.get<0, 1>() == "one" );
      ensure( resultSet.get<1, 0>() == 2 );
      ensure( resultSet.get<1, 1>() == "two" );
      ensure( resultSet.get<2, 0>() == 3 );
      ensure( resultSet.get<2, 1>() == "three" );
   }

   test( "Record two overloaded function calls" );
   {
      CallRecorder<> recorder;

      recorder.record( static_cast<void(*)(int)>( setVal ), 3 );
      recorder.record( static_cast<void(*)(double)>( setVal ), 3.14 );

      auto resultSet = makeResultSet( recorder.find(
         static_cast<void(*)(int)>( setVal ) ) );
      ensure( resultSet.get<0, 0>() == 3 );
      auto resultSet2 = makeResultSet( recorder.find(
         static_cast<void(*)(double)>( setVal ) ) );
      ensure( resultSet2.get<0, 0>() > 3.1 );
   }

   test( "Record two overloaded method calls" );
   {
      CallRecorder<> recorder;

      recorder.record(
         FiniteID(),
         static_cast<void(ISomeClass::*)(int)>( &ISomeClass::setInt ),
         3 );
      recorder.record(
         FiniteID(),
         static_cast<void(ISomeClass::*)(int) const>( &ISomeClass::setInt ),
         5 );

      auto resultSet = makeResultSet( recorder.find(
         static_cast<void(ISomeClass::*)(int)>( &ISomeClass::setInt ) ) );
      ensure( resultSet.get<0, 0>() == 3 );
      auto resultSet2 = makeResultSet( recorder.find(
         static_cast<void(ISomeClass::*)(int) const>( &ISomeClass::setInt ) ) );
      ensure( resultSet2.get<0, 0>() == 5 );
   }

   test( "Record two method calls with the same signature" );
   {
      CallRecorder<> recorder;

      recorder.record( FiniteID(), &ISomeClass::setDouble, 10.0 );
      recorder.record( FiniteID(), &ISomeClass::setAnotherDouble, 20.0 );

      auto resultSet =
         makeResultSet( recorder.find( &ISomeClass::setDouble ) );
      ensure( resultSet.size() == 1 );
      auto resultSet2 =
         makeResultSet( recorder.find( &ISomeClass::setAnotherDouble ) );
      ensure( resultSet2.size() == 1 );
   }

   test( "Record a method call with a converted argument" );
   {
      CallRecorder<> recorder;
      int* ip = new int( 15 );

      recorder.record( FiniteID(), &ISomeClass::setIntPtr, ip );
      delete ip;

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setIntPtr ) );
      ensure( resultSet.get<0, 0>() == 15 );
   }

   test( "Record a method call with a const unique_ptr&" );
   {
      CallRecorder<> recorder;
      const std::unique_ptr<int> uip = std::make_unique<int>( 30 );

      recorder.record( FiniteID(), &ISomeClass::setConstRefUPtr, uip );

      auto resultSet =
         makeResultSet( recorder.find( &ISomeClass::setConstRefUPtr ) );
      ensure( resultSet.get<0, 0>() == 30 );
   }

   test( "Record a method call with a unique_ptr" );
   {
      CallRecorder<> recorder;
      std::unique_ptr<int> uip = std::make_unique<int>( 32 );

      recorder.record( FiniteID(), &ISomeClass::setUPtr, std::move( uip ) );

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setUPtr ) );
      ensure( resultSet.get<0, 0>() == 32 );
   }

   test( "Record a method call with a shared_ptr" );
   {
      CallRecorder<> recorder;
      std::shared_ptr<int> sip = std::make_shared<int>( 34 );

      recorder.record( FiniteID(), &ISomeClass::setSPtr, sip );

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setSPtr ) );
      ensure( *resultSet.get<0, 0>() == 34 );
   }

   test( "Record method calls using different objects" );
   {
      CallRecorder<> recorder;
      FiniteID id1;
      FiniteID id2 = FiniteID::generate();
      FiniteID id3 = FiniteID::generate();

      recorder.record( id1, &ISomeClass::setIntStr, 3, "three" );
      recorder.record( id2, &ISomeClass::setIntStr, 5, "five" );
      recorder.record( id3, &ISomeClass::setIntStr, 8, "eight" );

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setIntStr ) );
      ensure( resultSet.get<0, 0>() == 3 );
      ensure( resultSet.get<1, 0>() == 5 );
      ensure( resultSet.get<2, 0>() == 8 );
      auto resultSet2 =
         makeResultSet( recorder.find( id1, &ISomeClass::setIntStr ) );
      ensure( resultSet2.get<0, 0>() == 3 );
      auto resultSet3 =
         makeResultSet( recorder.find( id2, &ISomeClass::setIntStr ) );
      ensure( resultSet3.get<0, 0>() == 5 );
      auto resultSet4 =
         makeResultSet( recorder.find( id3, &ISomeClass::setIntStr ) );
      ensure( resultSet4.get<0, 0>() == 8 );
   }

   test( "Retrieve data by means of type instead of index" );
   {
      CallRecorder<> recorder;

      recorder.record( FiniteID(), &ISomeClass::setIntStr, 3, "three" );

      auto resultSet = makeResultSet( recorder.find( &ISomeClass::setIntStr ) );
      ensure( resultSet.get<0, int>() == 3 );
      ensure( resultSet.get<0, std::string>() == "three" );
   }

}

