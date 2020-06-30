// ==========================================================================
//
// file : gf-st7567.hpp
//
// ==========================================================================


   enum class commands {
      // adapted from
      // https://github.com/mithunkamat/ST7567-driver/ST7567Commands.h

#ifdef nono
         DISPLAY_OFF             0xAE
         DISPLAY_ON             0xAF
         SET_START_LINE         0x40
         SET_PAGE_ADDRESS         0xB0
         SET_COLUMN_ADDRESS_MSB   0x10
         SET_COLUMN_ADDRESS_LSB   0x00
         SEG_DIRECTION         0xA0
         INVERSE_DISPLAY         0xA6
         ALL_PIXEL_ON            0xA4
         BIAS_SELECT            0xA2
         READ_MODIFY_WRITE      0xE0
         END                  0xEE
         RESET               0xE2
         COM_DIRECTION         0xC0
         POWER_CONTROL         0x28
         REGULATION_RATIO         0x20
         SET_EV               0xE1
         SET_BOOSTER            0xF8
         NOP                  0xE3
         TEST                  0xFE

         DISPOFF             0xae /* 0xae: Display OFF (sleep mode) */
         DISPON              0xaf /* 0xaf: Display ON in normal mode */

         SETSTARTLINE        0x40 /* 0x40-7f: Set display start line */
#  define STARTLINE_MASK    0x3f

         SETPAGESTART        0xb0 /* 0xb0-b7: Set page start address */
#  define PAGESTART_MASK    0x07

         SETCOLL             0x00 /* 0x00-0x0f: Set lower column address */
#  define COLL_MASK         0x0f
         SETCOLH             0x10 /* 0x10-0x1f: Set higher column address */
#  define COLH_MASK         0x0f

         SSD1305_MAPCOL0            0xa0 /* 0xa0: Column address 0 is mapped to SEG0 */
         SSD1305_MAPCOL128          0xa1 /* 0xa1: Column address 128 is mapped to SEG0 */

         DISPNORMAL          0xa6 /* 0xa6: Normal display */
         DISPINVERSE         0xa7 /* 0xa7: Inverse display */

         DISPRAM             0xa4 /* 0xa4: Resume to RAM content display */
         DISPENTIRE          0xa5 /* 0xa5: Entire display ON */

         BIAS_1_9            0xa2 /* 0xa2: Select BIAS setting 1/9 */
         BIAS_1_7            0xa3 /* 0xa3: Select BIAS setting 1/7 */

         ENTER_RMWMODE       0xe0 /* 0xe0: Enter the Read Modify Write mode */
         EXIT_RMWMODE        0xee /* 0xee: Leave the Read Modify Write mode */
         EXIT_SOFTRST        0xe2 /* 0xe2: Software RESET */

         SETCOMNORMAL        0xc0 /* 0xc0: Set COM output direction, normal mode */
         SETCOMREVERSE       0xc8 /* 0xc8: Set COM output direction, reverse mode */

         POWERCTRL_VF        0x29 /* 0x29: Control built-in power circuit */
         POWERCTRL_VR        0x2a /* 0x2a: Control built-in power circuit */
         POWERCTRL_VB        0x2c /* 0x2c: Control built-in power circuit */
         POWERCTRL           0x2f /* 0x2c: Control built-in power circuit */

         REG_RES_RR0         0x21 /* 0x21: Regulation Resistior ratio */
         REG_RES_RR1         0x22 /* 0x22: Regulation Resistior ratio */
         REG_RES_RR2         0x24 /* 0x24: Regulation Resistior ratio */

         SETEV               0x81 /* 0x81: Set contrast control */

         SETBOOSTER          0xf8 /* Set booster level */
         SETBOOSTER4X        0x00 /* Set booster level */
         SETBOOSTER5X        0x01 /* Set booster level */
#endif
};

#ifdef nono
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
      bus::write(
         address,
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
      bus::write(
         address,
         data,
         sizeof( data ) / sizeof( uint8_t )
      );
   }

   static void data( const auto & data ){
      bus::write_start();

      bus::write_byte( address << 1 );
      bus::read_ack();

      bus::write_byte( data_mode );
      (void) bus::read_ack();

      for( const auto d : data ){
         write_byte( d );
         (void) bus::read_ack();
      }

      bus::write_stop();
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
      root::location_t pos,
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
      root::location_t  pos,
      root::color_t     col
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

#endif

