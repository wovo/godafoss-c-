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
   typename T::offset_t  a, 
   typename T::color_t   c 
){  
   T::window_marker;
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

   static constexpr bool window_marker = true;
   
   using offset_t    = _offset_t;
   using location_t  = torsor< _offset_t >;
   using color_t     = _color_t;
   
   static constexpr location_t origin = location_t( 0, 0 );
   static constexpr offset_t   size   = _size;
   
   template< typename T >
   static bool within( T value, T limit ){
      return ( value >= 0 ) && ( value < limit );
   }
   
   static void write( location_t location, color_t color ){
      auto loc = location - origin;
      if( 
         within( loc.x, size.x ) 
         && within( loc.y, size.y )
      ){   
         _implementation::write_implementation( location, color );
      }   
   } 
          
   static void clear( color_t color ){ 
      for( const auto location : range( size )){
         _implementation::write_implementation( origin + location, color );                        
      }         
   }   
   
};

template< typename minion >
struct flip_horizontally : 
   window_root< 
      flip_horizontally< minion >,
      typename minion::location_t,
      typename minion::color_t,
      minion::size 
   >      
{
   
   static void init(){
      minion::init();   
   }

   static void write_implementation( 
      minion::location_t pos, 
      minion::color_t color
   ){
      minion::write( 
         typename minion::location_t( minion::size.x - pos.x, pos.y ),
         color
      );   
   }
   
   static void flush(){
      minion::flush();   
   }   

};

template< window T >
struct support_invert< T >{
   static constexpr bool value = true;
};

template< window minion >
struct invert< minion >: 
   window_root< 
      invert< minion >,
      typename minion::address_t,
      typename minion::color_t,
      minion::size
   >      
{
   
   static void init(){
      minion::init();   
   }

   static void write_implementation( 
      minion::address_t pos, 
      minion::color_t color
   ){
      minion::write( pos, - color );   
   }
   
   static void flush(){
      minion::flush();   
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
