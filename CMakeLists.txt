#
#   CMakeLists.txt
#
#   Copyright (c) 2015 Daniel Markus
#
#   This file is part of the Unimock library that is distributed under the
#   University of Illinois/NCSA Open Source License (NCSA). See accompanying
#   file LICENSE.TXT for details.
#
#_______________________________________________________________________________


cmake_minimum_required( VERSION 2.8 )

project( unimock )

# The library is header-only so we only install the include files.
install( DIRECTORY include/unimock DESTINATION include )

enable_testing()

add_subdirectory( test )

