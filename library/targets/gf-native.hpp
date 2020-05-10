// ==========================================================================
//
// file : gf-native.hpp
//
// HAL for running native, using SFML for graphics windows and timing
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss 
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "gf-all.hpp"
#include "SFML/Graphics.hpp"

namespace godafoss {
   
template< typename _dummy = void >
struct target_native {      


// ==========================================================================
//
// window
//
// ==========================================================================

template< 
   godafoss::xy     size, 
   int              number,
   int              m = 5, 
   godafoss::color  foreground = godafoss::white, 
   godafoss::color  background = godafoss::black 
>
struct window : 
   godafoss::window_root< 
      window< size, number, m, foreground, background >, 
      godafoss::xy, 
      godafoss::color, 
      size, 
      foreground, 
      background 
   >
{     
private:

   static inline sf::RenderWindow * wp;
   static inline sf::Image image;
   
   static void make_render_window(){
      static sf::RenderWindow ww( sf::VideoMode( m * size.x, m * size.y ), "GODAFOSS-SFML window" );
      wp = &ww;
      GODAFOSS_RUN_ONCE;
      image.create( m * size.x, m * size.y );
   }   

public:   

   static void init(){
      make_render_window();
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
   
   static void write_implementation( 
      xy pos, 
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
       texture.loadFromImage(image);
	   
       sf::Sprite sprite;
       sprite.setTexture(texture, true);
	   
       wp->draw(sprite);      
       wp->display();
	   
	    poll();
   }   
};


// ==========================================================================
//
// waiting
//
// ==========================================================================

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

}; // struct target_native

template< typename _dummy = void >
using target = target_native<>; 

}; // namespace godafoss


