/*

   ChefRobot.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include "ChefRobot.hh"

#include "IRefrigerator.hh"
#include "IStove.hh"
#include "Dish.hh"


ChefRobot::ChefRobot( IRefrigerator& refrigerator, IStove& stove )
:
   refrigerator_( refrigerator ),
   stove_( stove )
{
}

Dish ChefRobot::prepareStarter()
{
   refrigerator_.getTomatoes();
   refrigerator_.getEggs();
   refrigerator_.getRedSavinaHabanero();
   stove_.turnOnBurner( 2000 );
   stove_.turnOffBurner();
   return Dish( 0, 100000 );
}

Dish ChefRobot::prepareMainCourse()
{
   refrigerator_.getFish();
   refrigerator_.getBlackPepper();
   return Dish( 0, 0 );
}

Dish ChefRobot::prepareDessert()
{
   refrigerator_.getEggs();
   refrigerator_.getTabasco();
   stove_.turnOnBurner( 100 );
   stove_.turnOffBurner();
   return Dish( 3, 0 );
}

