/*

   CoolFood.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include "IRefrigerator.hh"


class CoolFood : public IRefrigerator
{
public:

   CoolFood();

   virtual void getTomatoes();
   virtual void getPeanuts();
   virtual void getEggs();
   virtual void getFish();
   virtual void getBlackPepper();
   virtual void getTabasco();
   virtual void getRedSavinaHabanero();

};

