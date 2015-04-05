/*

   IStove.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once


class IStove
{
public:

   /// Destructor.
   ///
   /// \exception No-throw.
   ///
   virtual ~IStove() {}

   /// Turns on the burner.
   ///
   /// \param[in] level
   ///   The level of the gas flow.
   ///
   /// \exception Exception neutral.
   ///
   virtual void turnOnBurner( int level ) = 0;

   /// Turns off the burner.
   ///
   /// \exception Exception neutral.
   ///
   virtual void turnOffBurner() = 0;

   /// Turns on the oven.
   ///
   /// \param[in] temperature
   ///   The temperature in the oven.
   ///
   /// \exception Exception neutral.
   ///
   virtual void turnOnOven( int temperature ) = 0;

   /// Turns off the oven.
   ///
   /// \exception Exception neutral.
   ///
   virtual void turnOffOven() = 0;

};

