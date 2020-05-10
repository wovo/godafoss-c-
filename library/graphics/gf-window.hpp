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
concept window = requires {  
   T::window_marker;
   { T::init() } -> std::same_as< void >;
   { T::write( T::address_t, T::color_t ) } -> std::same_as< void >;
   { T::write( T::address_t ) } -> std::same_as< void >;
   { T::clear( T::color_t ) } -> std::same_as< void >;
   { T::clear() } -> std::same_as< void >;
   { T::flush() } -> std::same_as< void >;
};

template< 
   typename          implementation, 
   xy_compatible     _address_t,
   color_compatible  _color_t,
   _address_t        _size, 
   _color_t          _foreground, 
   _color_t          _background 
>
struct window_root {

   static constexpr bool window_marker = true;
   
   using address_t  = _address_t;
   using color_t    = _color_t;
   
   static constexpr address_t size = _size;
   static constexpr color_t foreground = _foreground;
   static constexpr color_t background = _background;
   
   static bool within( 
      address_t::value_t value, 
      address_t::value_t limit 
   ){
      return ( value >= 0 ) && ( value < limit );
   }
   
   static void write( address_t location, color_t color ){
      if( 
         within( location.x, size.x ) 
         && within( location.y, size.y )
      ){   
         implementation::write_implementation( location, color );
      }   
   } 
   
   static void write( address_t location ){
      write( location, foreground );
   }      
   
   static void clear( color_t color ){ 
      for( const auto location : xy_all_t( size )){
         implementation::write_implementation( location, color );                        
      }         
   }   
   
   static void clear(){ 
      clear( background );        
   }   
   
};

// direct
// buffered
// mirror_horizontally
// mirror_vertically
// sub_window
// combine windows
// make black-n-white
// address transformation
// color transformation
//
