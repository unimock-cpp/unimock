/*

   FiniteID.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <cstdint>


namespace unimock
{

/// Finite identifier to be used as a process unique identifier.
///
/// The finite identifier uses an integer as the identifier. The integer is
/// unique for the process and will work as a unique identifier as long as it's
/// not passed between processes or stored and used in consecutive runs of the
/// same process. The underlying integer isn't accessible so described abuse is
/// therefore prevented. The only way the finite identifier can be used is by
/// comparing for equality with another finite identifier.
///
/// The identifier can be copied. This way we can create dependencies between
/// objects. As long as at least one copy of the identifier is in use, the
/// identifying integer is still active. When the last copy of a particular
/// identifier goes out of scope, the underlying integer is consumed and never
/// recycled. This means that there's a finite number of identifiers available.
/// However, with a 64 bit integer there will be enough to allocate one billion
/// identifiers per second for almost three hundred years.
///
/// Since the underlying integer isn't accessible and old identifiers are never
/// recycled, we avoid the [Cuckoo Pointer][1] problem where a dangling
/// reference accidentally is reused.
///
/// #### Example ####
/// ~~~
/// FiniteID id1 = FiniteID::generate();
/// FiniteID id2 = FiniteID::generate();
/// FiniteID id3 = id1;
///
/// assert( id2 != id1 );
/// assert( id3 == id1 );
/// ~~~
///
/// [1]: http://www.nobugs.org/blog/archives/2004/11/20/cuckoo-pointers/
///
class FiniteID final
{
public:

   /// Constructor.
   ///
   /// Constructs an uninitialized finite identifier that will compare equal to
   /// other uninitialized finite identifiers.
   ///
   /// \exception No-throw.
   ///
   FiniteID() noexcept;

   /// Destructor.
   ///
   /// \exception No-throw.
   ///
   ~FiniteID() = default;

   FiniteID( const FiniteID& ) = default;
   FiniteID( FiniteID&& other ) noexcept;
   FiniteID& operator=( const FiniteID& ) = default;
   FiniteID& operator=( FiniteID&& other ) noexcept;

   /// Generates an initialized finite identifier.
   ///
   /// \returns
   ///   An initialized finite identifier.
   ///
   /// \exception No-throw.
   ///
   static FiniteID generate() noexcept;

   /// Gets the initialization state of the finite identifier.
   ///
   /// \retval true
   ///   The finite identifier is initialized with a unique identifier.
   ///
   /// \retval false
   ///   The finite identifier is uninitialized.
   ///
   /// \exception No-throw.
   ///
   operator bool() const noexcept;

   /// Compares equality between two finite identifiers.
   ///
   /// Two initialized identifiers originating from the same copy will compare
   /// equal. Two uninitialized identifiers will compare equal no matter their
   /// origin.
   ///
   /// \retval true
   ///   The finite identifier is the same as the other.
   ///
   /// \retval false
   ///   The finite identifier is not the same as the other.
   ///
   /// \exception No-throw.
   ///
   bool operator==( const FiniteID& other ) const noexcept;

   /// Compares inequality between two finite identifiers.
   ///
   /// \retval true
   ///   The finite identifier is the same as the other.
   ///
   /// \retval false
   ///   The finite identifier is not the same as the other.
   ///
   /// \exception No-throw.
   ///
   bool operator!=( const FiniteID& other ) const noexcept;


private:

   std::uint64_t integerID_;

};


} // namespace


// Implementation.
#include "FiniteID.icc"

