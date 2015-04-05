/*

   Dish.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#include "Dish.hh"


Dish::Dish( int stars, int scoville )
:
   michelinStars_( stars ),
   scovilleUnits_( scoville )
{
}

int Dish::getMichelinStars() const noexcept
{
   return michelinStars_;
}

int Dish::getScovilleUnits() const noexcept
{
   return scovilleUnits_;
}

