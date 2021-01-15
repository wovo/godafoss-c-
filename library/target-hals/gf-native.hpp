// =============================================================================
//
// file : gf-sfml.hpp
//
// =============================================================================
//
// This file is part the https://www.github.com/godafoss
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// HAL for running native, using SFML for graphics windows and timing
//
// =============================================================================

#include "gf-all.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

namespace godafoss {

template< typename _dummy = void >
struct native {


// =============================================================================
//
// window
//
// =============================================================================

template<
   godafoss::xy<>   size,
   int              number = 0,
   int              m = 5,
   godafoss::color  background = godafoss::green,
   godafoss::color  foreground = godafoss::blue
>
struct window :
   godafoss::window_root<
      window< size, number, m, background, foreground >,
      godafoss::xy<>,
      godafoss::color,
      size
   >
{
private:

   using root = godafoss::window_root<
      window< size, number, m, background, foreground >,
      godafoss::xy<>,
      godafoss::color,
      size
   >;

   static inline sf::RenderWindow * wp;
   static inline sf::Image image;
   static inline char name[] = "GODAFOSS-SFML window";

   static void make_render_window(){
      GODAFOSS_RUN_ONCE;
      // how to change the name?
      static sf::RenderWindow ww(
         sf::VideoMode( m * size.x, m * size.y ),
         name );
      wp = &ww;
      image.create( m * size.x, m * size.y );
   }

   static void poll(){
      make_render_window();
      if ( wp->isOpen() ){
         sf::Event event;
         while (wp->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                wp->close();
            }
         }
      }
   }

public:

   static void init(){}

   static void write_implementation(
      root::offset_t pos,
      color col
   ){
      make_render_window();
      for( int x = 0; x < m; ++x ){
         for( int y = 0; y < m; ++y ){
            image.setPixel(
               x + m * pos.x, y + m * pos.y,
               sf::Color( col.red, col.green, col.blue, 255 ) );
         }
      }
   }

   static void flush(){
       make_render_window();
       wp->clear();

       sf::Texture texture;
       texture.loadFromImage( image) ;

       sf::Sprite sprite;
       sprite.setTexture( texture, true );

       wp->draw( sprite );
       wp->display();

        poll();
   }
};


// =============================================================================
//
// cout
//
// =============================================================================

template< uint64_t dummy_baudrate = 0 >
struct uart :
   be_uart< uart< dummy_baudrate > >
{

   static void init(){}

   static bool GODAFOSS_INLINE write_blocks(){
      return false;
   }

   static void GODAFOSS_INLINE write_unchecked( char c ){
      std::cout << c;
   }

   static bool GODAFOSS_INLINE read_blocks(){
      return false; // peek??
   }

   static char GODAFOSS_INLINE read_unchecked(){
      char c = 'x'; // << std::cin;
      return c;
   }

};


// =============================================================================
//
// waiting
//
// =============================================================================

struct waiting :
   be_timing_wait< waiting >
{

   using ticks_type = uint64_t;

   static void GODAFOSS_INLINE init(){}

   static constexpr ticks_type ticks_from_ns( uint64_t n ){
      return n / 1'000;
   }

   static ticks_type GODAFOSS_INLINE now_ticks(){
      static sf::Clock clock;
      sf::Time elapsed = clock.getElapsedTime();
      sf::Int64 usec = elapsed.asMicroseconds();
      return usec;
   }

   static void GODAFOSS_NO_INLINE wait_ticks_function( ticks_type n ){
      ticks_type t = now_ticks() + n;
      while( now_ticks() < t ){}
   }

   template< ticks_type t >
   static void GODAFOSS_INLINE wait_ticks_template(){
       wait_ticks_function( t );
   }

}; // struct waiting

using timing   = waiting;
// using timing   = clocking;

}; // struct target

}; // namespace godafoss
