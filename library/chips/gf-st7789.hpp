// ==========================================================================
//
// file : gf-st7789.hpp
//
// ==========================================================================

namespace st7789

// color modes
#define COLOR_MODE_65K      0x50
#define COLOR_MODE_262K     0x60
#define COLOR_MODE_12BIT    0x03
#define COLOR_MODE_16BIT    0x05
#define COLOR_MODE_18BIT    0x06
#define COLOR_MODE_16M      0x07

// ==========================================================================
//
// chip commands
//
// adapted from github.com/devbis/st7789_mpy
//
// ==========================================================================

enum class command {

   NOP      = 0x00,
   SWRESET  = 0x01,
   RDDID    = 0x04,
   RDDST    = 0x09,

   SLPIN    = 0x10,
   SLPOUT   = 0x11,
   PTLON    = 0x12,
   NORON    = 0x13,

   INVOFF   = 0x20,
   INVON    = 0x21,
   DISPOFF  = 0x28,
   DISPON   = 0x29,
   CASET    = 0x2A,
   RASET    = 0x2B,
   RAMWR    = 0x2C,
   RAMRD    = 0x2E,

   PTLAR    = 0x30,
   COLMOD   = 0x3A,
   MADCTL   = 0x36
};

/*
   MADCTL_MY  0x80  // Page Address Order
   MADCTL_MX  0x40  // Column Address Order
   MADCTL_MV  0x20  // Page/Column Order
   MADCTL_ML  0x10  // Line Address Order
   MADCTL_MH  0x04  // Display Data Latch Order
   MADCTL_RGB 0x00
   MADCTL_BGR 0x08

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD
      */


// ==========================================================================
//
// raw spi interface
//
// ==========================================================================

template< is_spi_bus bus, can_pin_out _ss, can_pin_out _dc >
struct spi_ss_dc {

   using ss  = direct< pin_out< _ss >>;
   using dc  = direct< pin_out< _dc >>;

   static void init(){
      bus::init();
      dc::init();
      ss::init();
      ss::write( 1 );
   }

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

template< typename chip >
struct ssd1306 :
   window_root<
      ssd1306< chip >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >
{

   using root = godafoss::window_root<
      ssd1306< chip >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >;

   static void init(){
      chip::init();
    b"\x01\x80\x96"  # _SWRESET and Delay 150ms
    b"\x11\x80\xFF"  # _SLPOUT and Delay 500ms
    b"\x3A\x81\x55\x0A"  # _COLMOD and Delay 10ms
    b"\x36\x01\x08"  # _MADCTL
    b"\x21\x80\x0A"  # _INVON Hack and Delay 10ms
    b"\x13\x80\x0A"  # _NORON and Delay 10ms
    b"\x36\x01\xC0"  # _MADCTL
    b"\x29\x80\xFF"  # _DISPON and Delay 500ms
   }


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
      write_to_buffer( pos, a, col.is_black );
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

template< is_i2c_bus bus, int address = 0 >
using oled = ssd1306::ssd1306< ssd1306::i2c< bus, address + 0x3C > >;

template< is_spi_bus bus, can_pin_out ss, can_pin_out dc >
using oled_spi_ss_dc = ssd1306::ssd1306< ssd1306::spi_ss_dc< bus, invert< ss >, dc > >;



