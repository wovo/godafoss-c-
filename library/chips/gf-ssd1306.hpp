// ==========================================================================
//
// file : gf-ssd106.hpp
//
// ==========================================================================


namespace ssd1306 {


// ==========================================================================
//
// chip commands
//
// ==========================================================================

enum class command {
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


// ==========================================================================
//
// initialization sequence
//
// ==========================================================================

constexpr const std::array< uint8_t, 25 > initialization = {
   (uint8_t) command::display_off,
   (uint8_t) command::set_display_clock_div, 0x80,
   (uint8_t) command::set_multiplex,         0x3f,
   (uint8_t) command::set_display_offset,    0x00,
   (uint8_t) command::set_start_line       | 0x00,
   (uint8_t) command::charge_pump,           0x14,
   (uint8_t) command::memory_mode,           0x00,
   (uint8_t) command::seg_remap            | 0x01,
   (uint8_t) command::com_scan_dec,
   (uint8_t) command::set_com_pins,          0x12,
   (uint8_t) command::set_contrast,          0xcf,
   (uint8_t) command::set_precharge,         0xf1,
   (uint8_t) command::set_vcom_detect,       0x40,
   (uint8_t) command::display_all_on_resume,
   (uint8_t) command::normal_display,
   (uint8_t) command::display_on
};


// ==========================================================================
//
// raw i2c interface
//
// ==========================================================================

template< is_i2c_bus bus, uint8_t address >
struct i2c {

   static const uint8_t data_mode = 0x40;
   static const uint8_t cmd_mode  = 0x80;

   using channel  = typename bus::channel< address >;

   using resources = use_list< channel >;

   static void command( uint8_t d ){
      auto transaction = typename channel::write_transaction();
      transaction.write( cmd_mode );
      transaction.write( d );
   }

   static void data( const auto & data ){
      auto transaction = typename channel::write_transaction();
      transaction.write( data_mode );
      transaction.write( data );
   }

};


// ==========================================================================
//
// raw spi interface
//
// ==========================================================================

template< is_spi_bus bus, can_pin_out _ss, can_pin_out _dc >
struct spi_ss_dc {

   using ss  = direct< pin_out< _ss >>;
   using dc  = direct< pin_out< _dc >>;

   static void init__(){
      ss::write( 1 );
   }

   using resources = use_list< bus, dc, ss, execute< init__ > >;

   static void command( const auto & data ){
      dc::write( 0 );
      typename bus::transfer< ss >().write( data );
   }

   static void data( const auto & data ){
      dc::write( 1 );
      typename bus::transfer< ss >().write( data );
   }

};


// ==========================================================================
//
// window interface
//
// ==========================================================================

template< typename chip, xy<> _size >
struct ssd1306 :
   window_root<
      ssd1306< chip, _size >,
      xy< int_fast16_t >,
      color_bw,
      _size
   >
{

   using root = godafoss::window_root<
      ssd1306< chip, _size >,
      xy< int_fast16_t >,
      color_bw,
      _size
   >;

   static void init__(){
      for( auto b : initialization ){
         chip::command( b );
      }
   }

   using resources = use_list< chip, execute< init__ > >;

   static auto constexpr buffer_entries = 128 * 64 / 8;
   static inline std::array< uint8_t, buffer_entries > buffer;

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
      write_to_buffer( pos, a, col == black );
   }

   static void command( command cmd, uint8_t d0, uint8_t d1 ){
      chip::command( (uint8_t) cmd );
      chip::command( d0 );
      chip::command( d1 );
   }

   static void flush(){
      command( command::column_addr, 0, 127 );
      command( command::page_addr,   0,   7 );
      chip::data( buffer );
   }

};

}; // namespace ssd1306


// ==========================================================================
//
// user interfaces
//
// ==========================================================================

template< 
   is_i2c_bus bus, 
   int address = 0, 
   xy<> size = { 128, 64 } 
>
using ssd1306_i2c = ssd1306::ssd1306< 
  ssd1306::i2c< bus, address + 0x3C >, size >;

template< 
   is_spi_bus bus, 
   can_pin_out ss, 
   can_pin_out dc, 
   xy<> size = { 128, 64 } 
>
using ssd1306_spi_ss_dc = ssd1306::ssd1306< 
   ssd1306::spi_ss_dc< bus, invert< ss >, dc >, size >;

