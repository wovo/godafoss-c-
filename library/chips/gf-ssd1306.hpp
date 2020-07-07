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

constexpr uint8_t ssd1306_cmd_prefix      = 0x80;
constexpr uint8_t ssd1306_data_prefix     = 0x40;

constexpr const uint8_t ssd1306_initialization[] = {
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::display_off,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_display_clock_div, 0x80,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_multiplex,         0x3f,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_display_offset,    0x00,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_start_line       | 0x00,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::charge_pump,           0x14,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::memory_mode,           0x00,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::seg_remap            | 0x01,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::com_scan_dec,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_com_pins,          0x12,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_contrast,          0xcf,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_precharge,         0xf1,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::set_vcom_detect,       0x40,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::display_all_on_resume,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::normal_display,
   ssd1306_cmd_prefix, (uint8_t) ssd1306_commands::display_on
};

template< typename _bus, uint_fast8_t address >
struct ssd1306_i2c {

   using bus = _bus;
   using cmd = ssd1306_commands;

   static const uint_fast8_t data_mode = 0x40;
   static const uint_fast8_t cmd_mode  = 0x80;

   static void command( cmd d ){
      uint8_t data[] = {
          static_cast< uint8_t >( cmd_mode ),
          static_cast< uint8_t >( d )
      };
      bus::write(
         address,
         data,
         sizeof( data ) / sizeof( uint8_t )
      );
   }

   static void command( cmd d0, uint_fast8_t d1 ){
      uint8_t data[] = {
         static_cast< uint8_t >( cmd_mode ),
         static_cast< uint8_t >( d0 ),
         static_cast< uint8_t >( cmd_mode ),
         static_cast< uint8_t >( d1 )
      };
      typename bus::write_transaction( address ).write(
         data,
         sizeof( data ) / sizeof( uint8_t )
      );
   }

   static void command( cmd d0, uint_fast8_t d1, uint_fast8_t d2 ){
      uint8_t data[] = {
         static_cast< uint8_t >( cmd_mode ),
         static_cast< uint8_t >( d0 ),
         static_cast< uint8_t >( cmd_mode ),
         static_cast< uint8_t >( d1 ),
         static_cast< uint8_t >( cmd_mode ),
         static_cast< uint8_t >( d2 )
      };
      typename bus::write_transaction( address ).write(
         data,
         sizeof( data ) / sizeof( uint8_t )
      );
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
   //   chip::bus::write( ssd1306_initialization );
   }

   // current cursor setting in the controller;
   // used to avoid explicit cursor updates when such are not needed
   static inline uint_fast8_t cursor_x, cursor_y;

   static void pixels_to_chip(
      uint_fast8_t x,
      uint_fast8_t y,
      uint_fast8_t d
   ){
      if(( x != cursor_x ) || ( y != cursor_y )){
         chip::command( ssd1306_commands::column_addr,  x,  127 );
         chip::command( ssd1306_commands::page_addr,    y,    7 );
         cursor_x = x;
         cursor_y = y;
      }

      const uint8_t data[] = { d };
      chip::data( data );
      cursor_x++;
   }

   static auto constexpr buffer_entries = 128 * 64 / 8;
   static inline uint8_t buffer[ buffer_entries ];
   static inline bool dirty[ buffer_entries ];

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

   /*
   static void set_direct( location pos, color col ){
      const uint_fast8_t a = pos.x + ( pos.y / 8 ) * size.x;
      write_to_buffer( pos, a, col == foreground );
      pixels_to_chip( pos.x, pos.y / 8, buffer[ a ] );
   }
    * */

   static void write_implementation(
      root::offset_t  pos,
      root::color_t    col
   ){
      const uint_fast8_t a = pos.x + ( pos.y / 8 ) * root::size.x;
      write_to_buffer( pos, a, col.is_black );
      dirty[ a ] = true;
   }

   static void flush(){
      chip::command( ssd1306_commands::column_addr,  0,  127 );
      chip::command( ssd1306_commands::page_addr,    0,    7 );
      chip::data( buffer );
   }


}; // class glcd_oled

template< typename bus, int address = 0x30 >
using oled = glcd_ssd1306< ssd1306_i2c< bus, address > >;

