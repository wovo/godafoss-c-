// =============================================================================
//
// gf-window.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2017-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// title window
//
// define window
// insert window
// A window
//
// =============================================================================


template< typename T >
concept xy_compatible = true;

template< typename T >
concept window = requires(
   typename T::location_t  a,
   typename T::color_t     c
){
   T::_window_marker;
   // is cto?
   { T::size }           -> std::same_as< const typename T::offset_t & >;
   { T::write( a, c ) }  -> std::same_as< void >;
   { T::clear( c ) }     -> std::same_as< void >;
   { T::flush() }        -> std::same_as< void >;
};

// GODAFOSS_FROM_COMPATIBLE( window )

template<
   typename          _implementation,
   xy_compatible     _offset_t,
   is_color          _color_t,
   xy<>              _size
>
struct window_root {

   static constexpr bool _window_marker = true;

   using offset_t    = _offset_t;
   using location_t  = torsor< _offset_t, { 0, 0 } >;
   using color_t     = _color_t;

   static constexpr location_t origin = location_t();
   static constexpr offset_t   size   = _size;

   template< typename T >
   static bool within( T value, T limit ){
      return ( value >= 0 ) && ( value < limit );
   }

   static void write( location_t _location, color_t color ){
      auto location = _location - origin;
      if(
         within( location.x, size.x )
         && within( location.y, size.y )
      ){
         _implementation::write_implementation( location, color );
      }
   }

   static void clear( color_t color ){
      for( const auto offset : range( size )){
         write( origin + offset, color );
      }
   }

};
