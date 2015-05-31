/*

   Main.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include "TestFramework.hh"
#include "ChefRobot.hh"
#include "CoolFood.hh"
#include "GasStove3000.hh"
#include "Dish.hh"

////////////////////////////////////////////////////////////////////////////////
// Since your new chef robot caused some unexpected situations last time you
// invited your friends for dinner, you decided that you want to keep an eye on
// what it's doing next time. With Unimock that's easy.
////////////////////////////////////////////////////////////////////////////////


// These are the only header files you need to include to get started with your
// own tests.
#include "unimock/Mock.hh"
#include "unimock/ResultSetFactory.hh"

// First create mock objects for the equipment that the robot will be using.
// With these replacement mock objects we will trick the robot to believe its
// working with the real thing. That way we can use the mock objects to make
// sure the robot isn't setting the kitchen on fire, or the throats of the
// guests for that matter.

// Using the same interface as the real equipment, the robot won't notice it's
// monitored.
class RefrigeratorMock : public unimock::Mock<IRefrigerator>
{
public:

   // We must say that we want to use Mock's constructor in some tests where
   // we provide stubs or real objects.
   using unimock::Mock<IRefrigerator>::Mock;

   // We must use the mock's call() method to pass on the method call and its
   // arguments. Otherwise we can't connect the real equipment and the robot
   // would become suspicious and/or fail to do its work.
   virtual void getTomatoes() { call( &IRefrigerator::getTomatoes ); }
   virtual void getPeanuts() { call( &IRefrigerator::getPeanuts ); }
   virtual void getEggs() { call( &IRefrigerator::getEggs ); }
   virtual void getFish() { call( &IRefrigerator::getFish ); }
   virtual void getBlackPepper() { call( &IRefrigerator::getBlackPepper ); }
   virtual void getTabasco() { call( &IRefrigerator::getTabasco ); }
   virtual void getRedSavinaHabanero()
   {
      call( &IRefrigerator::getRedSavinaHabanero );
   }
};

class StoveMock : public unimock::Mock<IStove>
{
public:

   using unimock::Mock<IStove>::Mock;

   virtual void turnOnBurner( int level )
   {
      call( &IStove::turnOnBurner, level );
   }

   virtual void turnOffBurner() { call( &IStove::turnOffBurner ); }

   virtual void turnOnOven( int temperature )
   {
      call( &IStove::turnOnOven, temperature );
   }

   virtual void turnOffOven() { call( &IStove::turnOffOven ); }
};

// That's it! The mocks above are the only things we need to create before we
// can start monitoring our chef robot.


int main()
{
   // We use the popular Given/When/Then notation for our tests.
   // See http://en.wikipedia.org/wiki/Behavior-driven_development

   // First make sure the robot doesn't burn down your house.
   test( "Stove level" );
   {
      // Given: The equipment and the robot using it.
      RefrigeratorMock refrigeratorMock;
      StoveMock stoveMock;
      ChefRobot swedishChef( refrigeratorMock, stoveMock );

      // When: The robot starts out with the starter dish.
      swedishChef.prepareStarter();

      // Then: Make sure the robot doesn't set the stove burner level higher
      //       than 8000.
      auto resultSet = makeResultSet( stoveMock, &IStove::turnOnBurner );
      require( resultSet.get<0, 0>() < 8000 );
   }

   // Second most important thing is to make sure the robot doesn't use peanuts
   // since people could be very allergic to that.
   test( "Peanuts not used" );
   {
      // Given: The equipment and the robot using it.
      RefrigeratorMock refrigeratorMock;
      StoveMock stoveMock;
      ChefRobot swedishChef( refrigeratorMock, stoveMock );

      // When: The robot prepares the dishes.
      swedishChef.prepareStarter();
      swedishChef.prepareMainCourse();
      swedishChef.prepareDessert();

      // Then: Make sure the robot doesn't use peanuts in any of the dishes.
      auto resultSet =
         makeResultSet( refrigeratorMock, &IRefrigerator::getPeanuts );
      require( resultSet.size() == 0 );
   }

   // Another thing all of us could be interested in is the level of spiciness.
   // To make it bit interesting we actually check it from the dish itself
   // instead of monitoring the used ingredients.
   test( "Starter spiciness" );
   {
      // Given: The equipment and the robot using it. This time we must connect
      //        the real equipment, otherwise the robot may not be able to
      //        complete the the dish.
      std::shared_ptr<IRefrigerator> realRefrigerator( new CoolFood );
      RefrigeratorMock refrigeratorMock( realRefrigerator );
      std::shared_ptr<IStove> realStove( new GasStove3000 );
      StoveMock stoveMock( realStove );
      ChefRobot swedishChef( refrigeratorMock, stoveMock );

      // When: The robot prepares a dish.
      Dish starter = swedishChef.prepareStarter();

      // Then: Try the dish and see if it's too hot.
      require( starter.getScovilleUnits() <= 100000 );
   }

   // Finally it's always good to turn off the stove.
   test( "Stove turned off after use" );
   {
      // Given: The equipment and the robot using it.
      std::shared_ptr<IRefrigerator> realRefrigerator( new CoolFood );
      RefrigeratorMock refrigeratorMock( realRefrigerator );
      std::shared_ptr<IStove> realStove( new GasStove3000 );
      StoveMock stoveMock( realStove );
      ChefRobot swedishChef( refrigeratorMock, stoveMock );

      // When: The robot prepares the dishes.
      swedishChef.prepareStarter();
      swedishChef.prepareMainCourse();
      swedishChef.prepareDessert();

      // Then: Make sure the robot turned off the stove.
      auto resultSet = makeResultSet( stoveMock, &IStove::turnOffBurner );
      require( resultSet.size() >= 1 );
   }

   return 0;
}
