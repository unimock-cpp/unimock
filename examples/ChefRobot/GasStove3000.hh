/*

   GasStove3000.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include "IStove.hh"


class GasStove3000 : public IStove
{
public:

   GasStove3000();

   virtual void turnOnBurner( int level );
   virtual void turnOffBurner();
   virtual void turnOnOven( int temperature );
   virtual void turnOffOven();

};

