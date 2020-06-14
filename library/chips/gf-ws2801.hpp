// ==========================================================================
//
// file : gf-ws2801.hpp
//
// ==========================================================================


template<
   spi           bus,
   typename      timing,
   unsigned int  n_chips = 1 >
struct ws2801 :
   window_root<
      ws2801< bus, timing, n_chips >,
      xy< int_fast16_t >,
      color,
      { n_chips, 1 }
   >
{

   using root = godafoss::window_root<
      ws2801< bus, timing, n_chips >,
      xy< int_fast16_t >,
      color,
      { n_chips, 1 }
   >;

   static inline std::array< color, n_chips > pixels;

   static void init(){
      timing::init();
      bus::init();
   }

   static void write_implementation(
      root::offset_t  offset,
      root::color_t   col
   ){
      pixels[ offset.x ] = col;
   }

   static void flush(){
      //timing::us< 500 >::wait();
      auto transfer = typename bus::transfer< pin_out_dummy >();
      for( auto const & c : pixels ){
         transfer.write( c.red );
         transfer.write( c.blue );
         transfer.write( c.green );
      }
   }


}; // class ws2801
