// =============================================================================
//
// gf-application.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// 
// not useful, cto can have a run too!
// concept for cto - has resources
// concept for application -> has also run
//
// =============================================================================

template< typename T >
concept resource = requires {
    true;
//   typename T::resources;
};

// fallback, required but never used
template< resource... _tail >
struct use {};

// recursion endpoint
template<>
struct use<> {
   using first = void;

   static void GODAFOSS_INLINE initialize(){}
};

// initialize() will call that same function in all arguments
template< resource _first, resource... _tail >
struct use< _first, _tail... > {
   using first      = _first;
   using resources  = use< _tail... >;

   static void initialize(){
      first::resources::initialize();
      resources::initialize();
   }
};


// =============================================================================

// background< f > can be used as an element of a use<> list,
// or instead of a use<> list
template< void f() >
struct execute {
   //using first      = execute< f >;
   //using resources  = use<>;

   // called when used as
   //    using resources = use< ..., excecute< f > >
   static void initialize(){
      f();
   };

   // called when used as
   //    using resources = excecute< f >
   struct resources {
      static void initialize(){
         f();
      };
   };
};


// =============================================================================

// background f > can be used as an element of a use<> list,
// or instead of a use<> list
template< void f() >
struct _background {
};


// =============================================================================

// concept for cto - has resources
// concept for application -> has also run

template< typename application >
class run {
   
   // determine the 
   
   // run the inits
   //application< timing >::
   
   
   
   // run the main    
   run(){
      application::resources::initialize();
      application::run();
   }
};

