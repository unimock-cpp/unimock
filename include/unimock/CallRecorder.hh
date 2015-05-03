/*

   CallRecorder.hh

   Copyright (c) 2015 Daniel Markus

   This file is part of the Unimock library that is distributed under the
   University of Illinois/NCSA Open Source License (NCSA). See accompanying
   file LICENSE.TXT for details.

________________________________________________________________________________
*/

#pragma once

#include <vector>
#include <tuple>
#include <typeindex>
#include <memory>       // std::unique_ptr

#include "unimock/DefaultConversionPolicy.hh"
#include "unimock/FiniteID.hh"


namespace unimock
{

class ArgumentTupleI;


/// CallRecorder to record call activity.
///
/// The call recorder can record a set of arguments for a particular function
/// or method. The arguments are stored with the function or method pointer as
/// key, forming a history of the function and method call activity. The history
/// can then later be retrieved and analyzed.
///
/// All arguments are stored by copy. This may not be appropriate for all types
/// of parameters, like for instance pointers where the data may have been
/// deleted by the time it's analyzed. For that reason a conversion policy can
/// be provided that converts chosen types of arguments before they're stored. A
/// pointer can for instance be dereferenced before stored in the call history.
///
/// #### See also ####
/// [Policy-based Design](http://en.wikipedia.org/wiki/Policy-based_design)
///
template<class ConversionPolicy = DefaultConversionPolicy>
class CallRecorder : public ConversionPolicy
{
public:

   /// Default constructor.
   ///
   /// \exception Exception neutral.
   ///
   CallRecorder();

   /// Records a set of arguments for a function.
   ///
   /// This record method records a set of arguments with a non-member or static
   /// member function pointer as key. Before the arguments are stored, they
   /// will be converted according to the conversion policy. The default
   /// conversion policy will just pass along the arguments unchanged, except
   /// for pointers that are dereferenced.
   ///
   /// \param[in] functionPtr
   ///   The function pointer as key.
   ///
   /// \param[in] arguments
   ///   The arguments to be stored.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... FncParameters, typename... Parameters>
   void record(
      R(*functionPtr)(FncParameters...),
      Parameters&&... arguments );

   /// Records a set of arguments for a class method.
   ///
   /// This record method records a set of arguments with an object identifier
   /// and a class method pointer as key. Before the arguments are stored, they
   /// will be converted according to the conversion policy. The default
   /// conversion policy will just pass along the arguments unchanged, except
   /// for pointers that are dereferenced.
   ///
   /// \param[in] objectID
   ///   The object identifier as a compound key together with the method
   ///   pointer.
   ///
   /// \param[in] methodPtr
   ///   The method pointer as a compound key together with the object
   ///   identifier.
   ///
   /// \param[in] arguments
   ///   The arguments to be stored.
   ///
   /// \exception Exception neutral.
   ///
   template<
      typename R, class T, typename... FncParameters, typename... Parameters>
   void record(
      const FiniteID& objectID,
      R(T::*methodPtr)(FncParameters...),
      Parameters&&... arguments );

   /// Records a set of arguments for a class method.
   ///
   /// This record method works the same as the other record method for class
   /// methods. The difference is that it takes a const method pointer.
   ///
   /// \param[in] objectID
   ///   The object identifier as a compound key together with the method
   ///   pointer.
   ///
   /// \param[in] methodPtr
   ///   The method pointer as a compound key together with the object
   ///   identifier.
   ///
   /// \param[in] arguments
   ///   The arguments to be stored.
   ///
   /// \exception Exception neutral.
   ///
   template<
      typename R, class T, typename... FncParameters, typename... Parameters>
   void record(
      const FiniteID& objectID,
      R(T::*methodPtr)(FncParameters...) const,
      Parameters&&... arguments );

   /// Finds all recorded calls for the function provided.
   ///
   /// This find method looks up all recorded occasions of calls to the
   /// non-member or static member function provided and returns the result as
   /// an iterable container of tuples.
   ///
   /// \param[in] functionPtr
   ///   The function who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided function, returned as an iterable
   ///   container of tuples where each tuple contains the converted arguments
   ///   provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, typename... Parameters>
   auto find( R(*functionPtr)(Parameters...) ) const;

   /// Finds all recorded calls for the method provided.
   ///
   /// This find method looks up all recorded occasions of calls to the method
   /// provided and returns the result as an iterable container of tuples. The
   /// lookup will find all entries of the method provided, not taking the
   /// stored object identifier into account. Note that this may not be what you
   /// usually want. Take a look at the find method taking also the object
   /// identifier as a search criteria before you decide which functionality
   /// suits you best.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided method, returned as an iterable
   ///   container of tuples where each tuple contains the converted arguments
   ///   provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, class T, typename... Parameters>
   auto find( R(T::*methodPtr)(Parameters...) ) const;

   /// Finds all recorded calls for the method provided.
   ///
   /// This find method works the same as the other find method for class
   /// methods without object identifier as a search criteria. The difference is
   /// that it takes a const method pointer.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided method, returned as an iterable
   ///   container of tuples where each tuple contains the converted arguments
   ///   provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, class T, typename... Parameters>
   auto find( R(T::*methodPtr)(Parameters...) const ) const;

   /// Finds all recorded calls for the object identifier and method provided.
   ///
   /// This find method looks up all recorded occasions of calls to the object
   /// with the identifier and method provided, and returns the result as an
   /// iterable container of tuples. If the identifier provided is
   /// uninitialized, the lookup will work the same as in the find method where
   /// the identifier isn't provided at all.
   ///
   /// \param[in] objectID
   ///   The object identifier who's recorded calls shall be looked up.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided object identifier and method,
   ///   returned as an iterable container of tuples where each tuple contains
   ///   the converted arguments provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, class T, typename... Parameters>
   auto find( const FiniteID& objectID, R(T::*methodPtr)(Parameters...) ) const;

   /// Finds all recorded calls for the object identifier and method provided.
   ///
   /// This find method works the same as the other find method for object
   /// identifier and class methods.
   ///
   /// \param[in] objectID
   ///   The object identifier who's recorded calls shall be looked up.
   ///
   /// \param[in] methodPtr
   ///   The method who's recorded calls shall be looked up.
   ///
   /// \returns
   ///   The recorded calls for the provided object identifier and method,
   ///   returned as an iterable container of tuples where each tuple contains
   ///   the converted arguments provided in that particular call.
   ///
   /// \exception Exception neutral.
   ///
   template<typename R, class T, typename... Parameters>
   auto find(
      const FiniteID& objectID, R(T::*methodPtr)(Parameters...) const ) const;


private:

   class VoidType_;

   // We want to be able to see the interleave between function calls and method
   // calls, so therefore we register these calls in the same container. Some
   // call this way of doing it a "Massively Unified Code-Key". In essence we
   // use either the function pointer or a combination of the object ID / method
   // pointer to store and retrieve the data.
   std::vector<std::tuple<
      std::type_index,
      void(*)(),
      FiniteID,
      void(VoidType_::*)(),
      std::unique_ptr<ArgumentTupleI>>> callHistory_;

   template<
      typename... TupleParameters,
      class TypeIndex,
      class FunctionPtr,
      class MethodPtr,
      typename... Parameters>
   void record_(
      TypeIndex&& typeIndex,
      FunctionPtr functionPtr,
      const FiniteID& objectID,
      MethodPtr methodPtr,
      Parameters&&... arguments );

   template<typename... Parameters, class FunctionPtr, class MethodPtr>
   auto find_(
      const std::type_index& typeIndex,
      FunctionPtr functionPtr,
      const FiniteID& objectID,
      MethodPtr methodPtr ) const;

};


} // namespace


// Implementation.
#include "CallRecorder.tcc"

