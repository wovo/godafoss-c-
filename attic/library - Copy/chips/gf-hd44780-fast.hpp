// ==========================================================================
//
// file : hwcpp-hd44780.hpp
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

template< 
   can_pin_out   rs,
   can_pin_out   e,
   can_port_out  port,
   uint32_t      size_x,
   uint32_t      size_y,
   typename      timing
> struct _hd44780_rs_e_d_x_y_timing {
private:	
   
   using xy_t    = uint_fast8_t;
   
public:   
   
   static constexpr auto size = xy( size_x, size_y );     
   static inline    xy cursor; 
   
private:
   
   static void write4( uint_fast8_t d ){

      port::write( d );      
      
      // minimum tDBW
      timing::template ns< 200 >::wait();
      
      e::write( 1 );

      rs::flush();
      port::flush();
      e::flush();
      
      // minimum PW-EH
      timing::template ns< 500 >::wait();
      
      direct< e >::write( 0 );
      
      // minumum TcycE = 1000 ns - PW-EH
      // snark: must be enlarged to get things working??
	   timing::template ns< 100L * 500 >::wait();
   }

   static void write8( bool is_data, uint_fast8_t d ){
      rs::write( is_data );
      write4( d >> 4 );
      write4( d );
      
	  // enough for most instructions
	  // if an instruction needs more, that is his responsibilitty
      timing::template us< 100 >::wait();
   }      
           
public:

   static void GODAFOSS_INLINE command( uint_fast8_t cmd ){
      write8( 0, cmd );
   }

   static void GODAFOSS_INLINE data( uint_fast8_t chr ){
      write8( 1, chr );
   }

   static void clear(){
      command( 0x01 );
      timing::template us< 5'000 >::wait();
      goto_xy( xy( 0, 0 ));
   }   
   
   static void goto_xy( xy target ){

      if( size.y == 1 ){
		  
         // 1-line LCDs have a split after the 8'th char
         if( target.x < 8 ){
            command( 0x80 + target.x );
         } else {
            command( 0x80 + 0x40 + ( target.x - 8 ));
         }
		 
      } else {
         if( size.y == 2 ){
            command( 
               0x80
               + (( target.y > 0 ) 
                  ? 0x40 
                  : 0x00 )
               + ( target.x )
            );
         } else {
            command( 
               0x80
               + (( target.y & 0x01 )
                  ? 0x40 
                  : 0x00 )
               + (( target.y & 0x02 )
                  ? 0x14 
                  : 0x00 )
               + ( target.x )
             );              
         }
      }
	  
	  cursor = target;
   }
   
   static bool GODAFOSS_INLINE write_blocks(){
      return false;
   }

   static void GODAFOSS_INLINE flush(){}

   static void write( char chr ){

      if( size.y == 1 ){
         if( cursor.x == 8 ){
            goto_xy( cursor );
         }
      }   
      
      data( chr );
	  ++cursor.x;
   }     
   
   static void init(){
	  
      // init the dependencies 
      rs::init();
      e:: init();
      port::init();
      timing::init(); 

      // give LCD time to wake up
      direct< e >::write( 0 );
      direct< rs >::write( 0 );
      timing::template ms< 40 >::wait();

      // interface initialization: make sure the LCD is in 4 bit mode
      // (magical sequence, taken from the HD44780 data-sheet)
      write4( 0x03 );
      timing::template ms< 5 >::wait();
      write4( 0x03 );
      timing::template us< 100 >::wait();
      write4( 0x03 );
      write4( 0x02 );          // now we are in 4 bit mode

      // functional initialization
      command( 0x28 );         // 4 bit mode, 2 lines, 5x8 font
      command( 0x0C );         // display on, no cursor, no blink
      clear();                 // clear display, 'cursor' home
      goto_xy( xy( 0, 0 ) );   // 'cursor' home    
   }    
   
}; // class _hd44780_rs_e_d_x_y_timing_foundation

template< 
   can_pin_out   rs,
   can_pin_out   e,
   can_port_out  port,
   uint32_t      size_x,
   uint32_t      size_y,
   typename      timing
> using hd44780_rs_e_d_x_y_timing = 
    terminal<
    _hd44780_rs_e_d_x_y_timing< 
       rs, e, port, size_x, size_y, timing > >;