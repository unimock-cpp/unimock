/*

   ResultSet.tcc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <utility>      // std::move
#include <cassert>


namespace unimock
{

template<typename... Parameters>
ResultSet<Parameters...>::ResultSet(
   std::vector<std::tuple<Parameters...>>&& callHistory )
:
   resultSet_( std::move( callHistory ) )
{
}

template<typename... Parameters>
auto ResultSet<Parameters...>::size() const noexcept
{
   return resultSet_.size();
}

template<typename... Parameters>
template<std::size_t row, std::size_t column>
auto ResultSet<Parameters...>::get() const
{
   assert( row < resultSet_.size() );

   return std::get<column>( resultSet_[ row ] );
}

template<typename... Parameters>
template<std::size_t row, typename ColumnT>
auto ResultSet<Parameters...>::get() const
{
   assert( row < resultSet_.size() );

   return std::get<ColumnT>( resultSet_[ row ] );
}


template<typename... Parameters>
auto makeResultSet( std::vector<std::tuple<Parameters...>>&& callHistory )
{
   return ResultSet<Parameters...>(
      std::forward<std::vector<std::tuple<Parameters...>>>( callHistory ) );
}


} // namespace

