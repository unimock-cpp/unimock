/*

   Dish.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once


class Dish
{
public:

   /// Constructor.
   ///
   /// Constructs a dish with a taste grade according to the Michelin Guide, and
   /// a hotness grade according to the Scoville scale.
   ///
   /// http://en.wikipedia.org/wiki/Michelin_Guide
   /// http://en.wikipedia.org/wiki/Scoville_scale
   ///
   /// \param[in] stars
   ///   The number of stars according to the Michelin Guide.
   ///
   /// \param[in] scoville
   ///   The number of hotness units according to the Scoville scale.
   ///
   /// \exception No-throw.
   ///
   Dish( int stars, int scoville );

   /// Gets the taste in number of Michelin stars for this dish.
   ///
   /// \returns
   ///   The number of Michelin stars.
   ///
   /// \exception No-throw.
   ///
   int getMichelinStars() const noexcept;

   /// Gets the hotness in units of Scoville for this dish.
   ///
   /// \returns
   ///   The number of Scoville units.
   ///
   /// \exception No-throw.
   ///
   int getScovilleUnits() const noexcept;


private:

   const int michelinStars_;
   const int scovilleUnits_;

};

