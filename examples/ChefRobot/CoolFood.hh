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

   void getTomatoes() override;
   void getPeanuts() override;
   void getEggs() override;
   void getFish() override;
   void getBlackPepper() override;
   void getTabasco() override;
   void getRedSavinaHabanero() override;

};

