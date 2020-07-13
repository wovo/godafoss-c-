// ==========================================================================
//
// file : gf-max7219.hpp
//
// ==========================================================================

template< size_t n > concept range_1_8 =
   ( ( n > 0 ) && ( n < 9 ) );

template<
   typename            bus,
   pin_out_compatible  _cs,
   int                 n_chips = 1
>
struct max7219 :
   window_root<
      max7219< bus, _cs, n_chips >,
      xy< int_fast16_t >,
      black_or_white,
      { 8 * n_chips, 8 }
   >
{

   using root = godafoss::window_root<
      max7219< bus, _cs, n_chips >,
      xy< int_fast16_t >,
      black_or_white,
      { 8 * n_chips, 8 }
   >;

   using cs = direct< invert< pin_out_from< _cs >>>;
   using transfer = typename bus:: template transfer< cs >;

   static constexpr uint8_t cmd_mode        = 0x09;
   static constexpr uint8_t cmd_intensity   = 0x0A;
   static constexpr uint8_t cmd_scan_limit  = 0x0B;
   static constexpr uint8_t cmd_shutdown    = 0x0C;
   static constexpr uint8_t cmd_test        = 0x0F;

   static void command( uint8_t high, uint8_t low ){
      auto t = typename bus:: template transfer< cs >();
      for( uint_fast8_t i = 0; i < n_chips; ++i ){
         t.write( high );
         t.write( low );
      }
   }

   static void intensity( uint_fast8_t n ){
      command( cmd_intensity, n & 0x0F );
   }

   static void init(){
      bus::init();
      cs::init();
      cs::write( 0 );

      command( cmd_mode,        0x00 );
      command( cmd_intensity,   0x0F );
      command( cmd_scan_limit,  0x07 );
      command( cmd_shutdown,    0x01 );
      command( cmd_test,        0x00 );
   }

   static inline uint8_t buffer[ 8 * n_chips ];

   static void write_implementation(
      root::offset_t  pos,
      root::color_t   col
   ){
      const uint_fast8_t index = ( pos.x / 8 ) + pos.y * n_chips;
      const uint_fast8_t offset = 7 - ( pos.x % 8 );

      if( col.is_black ){
         buffer[ index] |=  ( 0x01 << offset );
      } else {
         buffer[ index ] &= ~( 0x01 << offset );
      }
   }

   static void flush(){
      for( uint_fast8_t y = 0; y < 8; ++y ){
         auto t = transfer();
         for( uint_fast8_t x = 0; x < n_chips; ++x ){
            t.write( y + 1 );
            t.write( buffer[ n_chips * y + x ] );
         }
      }
   }


}; // class max7219

