/*

   ChefRobot.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include "Dish.hh"

class IRefrigerator;
class IStove;


class ChefRobot
{
public:

   /// Constructor.
   ///
   /// Construct a chef robot and provide it with the necessary equipment and
   /// ingredients it needs to make an excellent dinner.
   ///
   /// \param[in] refrigerator
   ///   The refrigerator with the ingredients.
   ///
   /// \param[in] stove
   ///   The stove where to cook the food.
   ///
   /// \exception Exception neutral.
   ///
   ChefRobot( IRefrigerator& refrigerator, IStove& stove );

   /// Prepares a delicious starter.
   ///
   /// \returns
   ///   A delicious starter dish.
   ///
   /// \exception Exception neutral.
   ///
   Dish prepareStarter();

   /// Prepares an even more delicious main course.
   ///
   /// \returns
   ///   A delicious main course dish.
   ///
   /// \exception Exception neutral.
   ///
   Dish prepareMainCourse();

   /// Prepares a Grand Finale dessert.
   ///
   /// \returns
   ///   A spectacular dessert dish.
   ///
   /// \exception Exception neutral.
   ///
   Dish prepareDessert();


private:

   IRefrigerator& refrigerator_;
   IStove& stove_;


};

