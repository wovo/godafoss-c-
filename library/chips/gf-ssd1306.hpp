// ==========================================================================
//
// file : gf-ssd106.hpp
//
// ==========================================================================


enum class ssd1306_commands {
   set_contrast                           = 0x81,
   display_all_on_resume                  = 0xa4,
   display_all_on                         = 0xa5,
   normal_display                         = 0xa6,
   invert_display                         = 0xa7,
   display_off                            = 0xae,
   display_on                             = 0xaf,
   set_display_offset                     = 0xd3,
   set_com_pins                           = 0xda,
   set_vcom_detect                        = 0xdb,
   set_display_clock_div                  = 0xd5,
   set_precharge                          = 0xd9,
   set_multiplex                          = 0xa8,
   set_low_column                         = 0x00,
   set_high_column                        = 0x10,
   set_start_line                         = 0x40,
   memory_mode                            = 0x20,
   column_addr                            = 0x21,
   page_addr                              = 0x22,
   com_scan_inc                           = 0xc0,
   com_scan_dec                           = 0xc8,
   seg_remap                              = 0xa0,
   charge_pump                            = 0x8d,
   external_vcc                           = 0x01,
   switch_cap_vcc                         = 0x02,
   activate_scroll                        = 0x2f,
   deactivate_scroll                      = 0x2e,
   set_vertical_scroll_area               = 0xa3,
   right_horizontal_scroll                = 0x26,
   left_horizontal_scroll                 = 0x27,
   vertical_and_right_horizontal_scroll   = 0x29,
   vertical_and_left_horizontal_scroll    = 0x2a
};

constexpr const uint8_t ssd1306_initialization[] = {
   (uint8_t) ssd1306_commands::display_off,
   (uint8_t) ssd1306_commands::set_display_clock_div, 0x80,
   (uint8_t) ssd1306_commands::set_multiplex,         0x3f,
   (uint8_t) ssd1306_commands::set_display_offset,    0x00,
   (uint8_t) ssd1306_commands::set_start_line       | 0x00,
   (uint8_t) ssd1306_commands::charge_pump,           0x14,
   (uint8_t) ssd1306_commands::memory_mode,           0x00,
   (uint8_t) ssd1306_commands::seg_remap            | 0x01,
   (uint8_t) ssd1306_commands::com_scan_dec,
   (uint8_t) ssd1306_commands::set_com_pins,          0x12,
   (uint8_t) ssd1306_commands::set_contrast,          0xcf,
   (uint8_t) ssd1306_commands::set_precharge,         0xf1,
   (uint8_t) ssd1306_commands::set_vcom_detect,       0x40,
   (uint8_t) ssd1306_commands::display_all_on_resume,
   (uint8_t) ssd1306_commands::normal_display,
   (uint8_t) ssd1306_commands::display_on
};


// ==========================================================================

template< typename bus, uint8_t address >
struct ssd1306_i2c {

   static const uint8_t data_mode = 0x40;
   static const uint8_t cmd_mode  = 0x80;

   static void init(){
      bus::init();
   }

   static void command( uint8_t d ){
      auto transaction = typename bus::write_transaction( address );
      transaction.write( cmd_mode );
      transaction.write( d );
   }

   static void data( const auto & data ){
      auto transaction = typename bus::write_transaction( address );
      transaction.write( data_mode );
      transaction.write(
         data,
         sizeof( data ) / sizeof( uint8_t )
      );
   }

};


// ==========================================================================

template< typename chip >
struct glcd_ssd1306 :
   window_root<
      glcd_ssd1306< chip >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >
{

   using root = godafoss::window_root<
      glcd_ssd1306< chip >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >;

   static void init(){
      chip::init();
      for( auto b : ssd1306_initialization ){
         chip::command( (uint8_t) b );
      }
   }


   static auto constexpr buffer_entries = 128 * 64 / 8;
   static inline uint8_t buffer[ buffer_entries ];

   static void write_to_buffer(
      root::offset_t pos,
      uint_fast8_t a,
      bool v
   ){
      if( v ){
         buffer[ a ] |=  ( 0x01 << (pos.y % 8 ));
      } else {
         buffer[ a ] &= ~( 0x01 << ( pos.y % 8 ));
      }
   }

   static void write_implementation(
      root::offset_t  pos,
      root::color_t    col
   ){
      const uint_fast8_t a = pos.x + ( pos.y / 8 ) * root::size.x;
      write_to_buffer( pos, a, col.is_black );
   }

   static void command( ssd1306_commands cmd, uint8_t d0, uint8_t d1 ){
      chip::command( (uint8_t) cmd );
      chip::command( d0 );
      chip::command( d1 );
   }

   static void flush(){
      command( ssd1306_commands::column_addr, 0, 127 );
      command( ssd1306_commands::page_addr,   0,   7 );
      chip::data( buffer );
   }


}; // class glcd_oled


// ==========================================================================

template< typename bus, int address = 0x3C >
using oled = glcd_ssd1306< ssd1306_i2c< bus, address > >;

