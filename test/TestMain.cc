/*

   TestMain.cc

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

void testCallRecorder();
void testFunctionMock();
void testFunctorMock();
void testMock();


int main()
{
   testCallRecorder();
   testFunctionMock();
   testFunctorMock();
   testMock();

   return 0;
}
