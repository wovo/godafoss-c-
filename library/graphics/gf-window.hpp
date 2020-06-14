// ==========================================================================
//
// File      : gf-window
// Part of   : C++ hwlib library for close-to-the-hardware OO programming
// Copyright : wouter@voti.nl 2017-2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

template< typename T >
concept xy_compatible = true;

template< typename T >
concept color_compatible = true;

template< typename T >
concept window = requires(
   typename T::location_t  a,
   typename T::color_t   c
){
   T::_window_marker;
   { T::size }           -> std::same_as< const typename T::offset_t & >;
   { T::init() }         -> std::same_as< void >;
   { T::write( a, c ) }  -> std::same_as< void >;
   { T::clear( c ) }     -> std::same_as< void >;
   { T::flush() }        -> std::same_as< void >;
};

// GODAFOSS_FROM_COMPATIBLE( window )

template<
   typename          _implementation,
   xy_compatible     _offset_t,
   color_compatible  _color_t,
   xy<>              _size
>
struct window_root {

   static constexpr bool _window_marker = true;

   using offset_t    = _offset_t;
   using location_t  = torsor< _offset_t >;
   using color_t     = _color_t;

   static constexpr location_t origin = location_t();
   static constexpr offset_t   size   = _size;

   template< typename T >
   static bool within( T value, T limit ){

//   ostream< formatter< target::uart<> > > cout;
//   cout << value << " " << limit << "\n";

      return ( value >= 0 ) && ( value < limit );
   }

   static void write( location_t location, color_t color ){
      auto loc = location - origin;
      if(
         within( loc.x, size.x )
         && within( loc.y, size.y )
      ){
         _implementation::write_implementation( loc, color );
      }
   }

   static void clear( color_t color ){
      for( const auto offset : range( size )){
         write( origin + offset, color );
      }
   }

};
