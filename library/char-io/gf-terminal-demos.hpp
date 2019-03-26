// ==========================================================================
//
// File      : hwlib-terminal-demo.hpp
// Part of   : C++ hwlib library for close-to-the-hardware OO programming
// Copyright : wouter@voti.nl 2017-2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

/// scrolling text demo
/// 
/// This demo scrolls the text on the first line of the terminal.
/// The delays (in ms) are after showing the initial first part 
/// of the text, after scrolling a character, 
/// and after showing the last part of the texr.
template< 
   typename display, 
   typename first, 
   typename step, 
   typename last 
>
void scrolling_text( 
   const char * text
){
   using term = display;
   term::init();
   first::init();	
   auto len = _strlen( text );
   
   for(;;){
      for( uint_fast16_t i = 0; ( i + term::size.x ) <= len; ++i ){
         term::clear();
         term::goto_xy( xy( 0 , 0 ) );
         for( int_fast16_t j = 0; j < term::size.x; ++j ){
			   term::write( text[ i + j ] );
		   }
		   //term << flush;
		   if( i == 0 ){
            first::wait();
         } else {
            step::wait();
         }
      }		 
      last::wait();
   }	   

}
