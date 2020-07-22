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
   static inline uint_fast8_t buffer;
   static inline bool dirty;

   static void GODAFOSS_INLINE init(){
      dirty = false;
      bus::init();
   }

   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      buffer = d;
      dirty = true;
   }

   static void GODAFOSS_INLINE flush(){
      if( dirty ){
         typename bus::write_transaction( address ).write( buffer );
         dirty = false;
      }
   }

   static void GODAFOSS_INLINE refresh(){
   }

   template< int n > struct pin :
      pin_oc_root
   {

      static GODAFOSS_INLINE void init(){
         chip::init();
      }

      static GODAFOSS_INLINE void write( bool v ) {
         if( v ){
            chip::buffer |= ( 1 << n );
         } else {
			chip::buffer &= ~( 1 << n );
         }
         chip::dirty = true;
      }

	  static GODAFOSS_INLINE bool read(){
         return 1;
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
