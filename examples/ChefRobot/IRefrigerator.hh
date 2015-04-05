/*

   IRefrigerator.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once


class IRefrigerator
{
public:

   /// Destructor.
   ///
   /// \exception No-throw.
   ///
   virtual ~IRefrigerator() {}

   /// Gets the tomatoes from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getTomatoes() = 0;

   /// Gets the peanuts from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getPeanuts() = 0;

   /// Gets the eggs from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getEggs() = 0;

   /// Gets the fish from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getFish() = 0;

   /// Gets the black pepper from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getBlackPepper() = 0;

   /// Gets the tabasco from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getTabasco() = 0;

   /// Gets the red savina habanero from the refrigerator.
   ///
   /// \exception Exception neutral.
   ///
   virtual void getRedSavinaHabanero() = 0;

};

