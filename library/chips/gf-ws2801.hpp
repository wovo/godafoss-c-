// ==========================================================================
//
// file : gf-ws2801.hpp
//
// ==========================================================================


template<
   is_spi_bus      bus,
   is_timing_wait  timing,
   unsigned int    n_chips = 1 >
struct ws2801 :
   window_root<
      ws2801< bus, timing, n_chips >,
      xy< int_fast16_t >,
      color<>,
      { n_chips, 1 }
   >
{

   using root = godafoss::window_root<
      ws2801< bus, timing, n_chips >,
      xy< int_fast16_t >,
      color_bw,
      { n_chips, 1 }
   >;

   static inline std::array< color<>, n_chips > pixels;

   using resources = use_list< bus, timing >;

   static void write_implementation(
      root::offset_t  offset,
      root::color_t   col
   ){
      pixels[ offset.x ] = col;
   }

   static void flush(){
      // why doesnt this compile??
      //typename timing::us< 500 >::wait();
      auto transfer = typename bus::transfer< pin_out_dummy >();
      for( auto const & c : pixels ){
         transfer.write( c.red()   .of( 255 ) );
         transfer.write( c.blue()  .of( 255 ) );
         transfer.write( c.green() .of( 255 ) );
      }
   }

}; // class ws2801
