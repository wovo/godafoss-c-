// ==========================================================================
//
// file : pcf8574.hpp
//
// ==========================================================================


template<
   typename     bus,
   int          address
>
struct _pcf8574 :
   port_out_root< 8 >
{

   using chip = _pcf8574< bus, address >;
   using channel = typename bus::channel< address >;
   static inline uint8_t read_buffer;
   static inline uint_fast8_t write_buffer;
   static inline bool write_dirty;

   static void GODAFOSS_INLINE init(){

      static_assert(
         bus::frequency <= 100'000,
         "The maximum I2C bus frequency for this chip is 100 kHz" );

      write_dirty = false;
      bus::init();
   }

   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      write_buffer = d;
      write_dirty = true;
   }

   static void GODAFOSS_INLINE flush(){
      if( write_dirty ){
         typename channel::write_transaction().write( write_buffer );
         write_dirty = false;
      }
   }

   static void GODAFOSS_INLINE refresh(){
      read_buffer = typename channel::read_transaction().read();
   }

   template< int n > struct pin :
      pin_oc_root
   {

      static GODAFOSS_INLINE void init(){
         chip::init();
      }

      static GODAFOSS_INLINE void write( bool v ) {
         if( v ){
            chip::write_buffer |= ( 0x01 << n );
         } else {
			   chip::write_buffer &= ~( 0x01 << n );
         }
         chip::write_dirty = true;
      }

	  static GODAFOSS_INLINE bool read(){
         return ( read_buffer& ( 0x01 << n ) ) != 0;
      }

      static GODAFOSS_INLINE void refresh() {
         chip::refresh();
      }

      static GODAFOSS_INLINE void flush() {
         chip::flush();
      }

   };

   using p0 = pin< 0 >;
   using p1 = pin< 1 >;
   using p2 = pin< 2 >;
   using p3 = pin< 3 >;
   using p4 = pin< 4 >;
   using p5 = pin< 5 >;
   using p6 = pin< 6 >;
   using p7 = pin< 7 >;
};

template<
   typename     bus,
   int          address = 0x00
>
using pcf8574 = struct _pcf8574< bus, 0x20 | address >;

template<
   typename     bus,
   int          address = 0x00
>
using pcf8574a = struct _pcf8574< bus, 0x38 | address >;
