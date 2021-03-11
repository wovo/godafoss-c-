// ==========================================================================
//
// file : gf-ws2801.hpp
//
// ==========================================================================


template<
   can_pin_out  _data,
   typename            timing,
   unsigned int        n_chips = 1 >
struct ws2812 :
   window_root<
      ws2812< _data, timing, n_chips >,
      xy< int_fast16_t >,
      color_bw,
      { n_chips, 1 }
   >
{

   using root = godafoss::window_root<
      ws2812< _data, timing, n_chips >,
      xy< int_fast16_t >,
      color_bw,
      { n_chips, 1 }
   >;

   using data = direct< pin_out< _data >>;

   static inline std::array< color_bw, n_chips > pixels;

   static void init__(){
      data::write( 0 );
   }

   using resources = use_list< data, timing, execute< init__ > >;

   static void write_implementation(
      root::offset_t  offset,
      root::color_t   col
   ){
      pixels[ offset.x ] = col;
   }

   static void send( uint8_t b ){
      for( int i = 0; i < 8; ++i ){
         if( ( b & 0x80 ) == 0  ){
            data::write( 1 );
            timing::template ns< 350 >::wait();
            data::write( 0 );
            timing::template ns< 800 >::wait();
         } else {
            data::write( 1 );
            timing::template ns< 800 >::wait();
            data::write( 0 );
            timing::template ns< 600 >::wait();
         }
         b = b << 1;
      }
   }

   static void flush(){
      for( auto const & c : pixels ){
         send( c.green() .of( 255 ) );
         send( c.red()   .of( 255 ) );
         send( c.blue()  .of( 255 ) );
      }
      timing::template us< 50 >::wait();
   }


}; // class ws2812
