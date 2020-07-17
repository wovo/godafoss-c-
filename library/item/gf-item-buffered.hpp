// =============================================================================
//
// gf-item-buffered.hpp
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

// =============================================================================
//
// @define godafoss::buffered
// @title buffered
//
// The buffered<> decorator buffers read, write or direction operations,
// necessitating appropriate refresh() or flush() calls.
//
// @insert can_buffered
// @insert buffered
//
// =============================================================================


// =============================================================================
//
// read
//
// =============================================================================

template< typename T >
struct _buffered_read : T {};

template< is_input T >
struct _buffered_read< T > : T {

   static auto read(){
      return buffer;
   }

   static void refresh(){
      T::refresh();
      buffer = T::read();
   }

private:

   static typename T::value_type  buffer;

};


// =============================================================================
//
// write
//
// =============================================================================

template< typename T >
struct _buffered_write : T {};

template< is_output T >
struct _buffered_write< T > : T {

   using _vt = typename T::value_type;

   static void write( by_const< _vt > v ) {
      buffer = v;
      dirty = true;
   }

   static void flush() {
      if( dirty ){
         T::write( buffer );
         T::flush();
         dirty = false;
      }
   }

private:

   static bool   dirty;
   static _vt    buffer;

};


// =============================================================================
//
// direction
//
// =============================================================================

template< typename T >
struct _buffered_direction : T {};

template< is_simplex T >
struct _buffered_direction< T > : T {

   static void direction_set_input() {
     direction_is_input =  true;
	  dirty = true;
   }

   static void direction_set_output() {
      direction_is_input = false;
      dirty = true;
   }

   static void direction_flush() {
      if( dirty ){
         if( direction_is_input ){
            T::direction_set_input();
         } else {
            T::direction_set_output();
         }
         T::direction_flush();
         dirty = false;
      }
   }

private:

   static bool dirty;
   static bool direction_is_input;

};


// =============================================================================
//
// the buffered<> decorator
//
// =============================================================================

// @quote can_buffered 3
template< typename T >
concept can_buffered =
   is_item< T >;

// @quote buffered 2 ... ;
template< can_buffered T >
struct buffered
:
   _buffered_read<
   _buffered_write<
   _buffered_direction < T >>> {};
