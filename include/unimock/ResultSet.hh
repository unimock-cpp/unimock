/*

   ResultSet.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <vector>
#include <tuple>


namespace unimock
{

/// ResultSet to facilitate the retrieval of call history data.
///
/// The data returned from a mock or call recorder returns as an iterable
/// container of tuples. That type of return value is generic enough to allow
/// different kinds of tools to browse through the returned information. This
/// ResultSet is one way to access the data, and the idea with this helper class
/// is that it should provide an interface similar to a database result set.
///
template<typename... Parameters>
class ResultSet
{
public:

   /// Constructor.
   ///
   /// This constructor makes a result set out of the container provided.
   ///
   /// \param[in] callHistory
   ///   The call history container to initialize the result set with.
   ///
   /// \exception Exception neutral.
   ///
   ResultSet( std::vector<std::tuple<Parameters...>>&& callHistory );

   /// Gets the number of rows in the result set.
   ///
   /// This method returns the number of rows of the result set and can be
   /// used to make sure the correct number of calls has been made.
   ///
   /// \returns
   ///   The number of rows in the result set.
   ///
   /// \exception No-throw.
   ///
   auto size() const noexcept;

   /// Gets the element at position [row, column].
   ///
   /// This method returns the element at the row and column provided. The row
   /// and column shall be provided as integers.
   ///
   /// \returns
   ///   The element at the position [row, column].
   ///
   /// \exception Exception neutral.
   ///
   template<std::size_t row, std::size_t column>
   auto get() const;

   /// Gets the element at position [row, column].
   ///
   /// This method returns the element at the row and column provided. The row
   /// shall be provided as an integer but the column shall be provided as the
   /// type it was stored in the result set. If another column is of the same
   /// type then an index must be used instead.
   ///
   /// \returns
   ///   The element at the position [row, column] where row is an integer and
   ///   column is the type of the column sought.
   ///
   /// \exception Exception neutral.
   ///
   template<std::size_t row, typename ColumnT>
   auto get() const;


private:

   std::vector<std::tuple<Parameters...>> resultSet_;


};


/// Creates a ResultSet from a container of call history.
///
/// Since the constructor of ResultSet can't be a template method, this
/// non-member template function is provided as a convenience function to remove
/// the need to provide the tuple parameter types.
///
/// \param[in] callHistory
///   The call history container to initialize the result set with.
///
/// \exception Exception neutral.
///
template<typename... Parameters>
auto makeResultSet( std::vector<std::tuple<Parameters...>>&& callHistory );


} // namespace


// Implementation.
#include "ResultSet.tcc"

