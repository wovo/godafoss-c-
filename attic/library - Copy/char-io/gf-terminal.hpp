// ==========================================================================
//
// file : gf-terminal.hpp
//
// ==========================================================================
//
// This file is part of HwCpp, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2017
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// T must have:
//    putc( c )
//    position_type
//    size_x, size_y
//    cursor_x , cursor_y
//    goto_xy( x, y )
//    flush()

template< typename T >
struct terminal_add_clear : T {
	
   static void clear(){
      for( typename T::xy_t y = 0; y < T::size_y; ++y ){
         T::goto_xy( 0, y );
         for( typename T::xy_t x = 0; x < T::size_x; ++x ){
            T::write( ' ' );
         }
      }
      T::goto_xy( 0, 0 );
   }
   
}; // struct terminal_add_clear	

template< typename T >
struct terminal : T {
private:
   static inline uint_fast8_t goto_xy_state = 0;

public:

   static void write( char c ){

      switch( goto_xy_state ){

         case 0 :
		 default :
            break;

         case 1 :
            T::cursor.x = 10 * ( c - '0' );
            ++goto_xy_state;
            return;

         case 2 :
            T::cursor.x += c - '0' ;
            ++goto_xy_state;
            return;

         case 3 :
            T::cursor.y = 10 * ( c - '0' );
            ++goto_xy_state;
            return;

         case 4 :
            T::cursor.y += c - '0' ;
            goto_xy_state = 0;
            T::goto_xy( T::cursor );
            return;

      }

      if( c == '\n' ){
         T::goto_xy( xy( 0, T::cursor.y + 1 ) );

      } else if( c == '\r' ){
         T::goto_xy( xy( 0, T::cursor.y ) );

      } else if( c == '\v' ){
         T::goto_xy( xy( 0, 0 ) );

      } else if( c == '\f' ){
         T::clear();

      } else if( c == '\t' ){
         goto_xy_state = 1;

      } else if( c == '\a' ){
         while( T::cursor.x < T::size.x ){
	        T::write( ' ' );
         }			

      } else if(
         ( T::cursor.x >= 0 )
         && ( T::cursor.x < T::size.x )
         && ( T::cursor.y >= 0 )
         && ( T::cursor.y < T::size.y )
      ){
         T::write( c );
      }
   }
   
}; // struct terminal
