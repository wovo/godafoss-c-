// ==========================================================================
//
// file : pcf8591.hpp
//
// ==========================================================================


// ==========================================================================
//
// pcf8591 chip
//
// ==========================================================================

template< typename bus, uint_fast8_t address = 0 >
struct pcf8591 {

   static constexpr uint8_t base = 0x48;
   using channel = typename bus::channel< base + address >;
   static inline uint8_t configuration = 0x40;

   static void GODAFOSS_INLINE init(){

      static_assert(
         bus::frequency <= 100'000,
         "The maximum I2C bus frequency for this chip is 100 kHz" );

      bus::init();
   }

   static uint_fast8_t _read( uint_fast8_t channel ){

      // select the correct channel
      {
         uint8_t control[ 1 ] = { (uint8_t) (( configuration & ( ~ 0x03 )) | (uint8_t)channel ) };
         typename channel::write_transaction().write( control, 1 );
      }

      // Read results, note that the first byte is the
      // *previous* ADC result, the second byte is what we want.
      //
      // At least, that is what the documentation suggests.
      // Actually, it seems we need the 3d byte!
      // Something fishy is going on.
      {
         uint8_t results[ 3 ];
         typename channel::read_transaction().read( results, 3 );
         return results[ 2 ];
      }
   }

   template< uint_fast8_t channel >
   struct _adc {

      using value_type = fraction< int_fast16_t, 256 >;

      static void GODAFOSS_INLINE init(){
         pcf8591< bus, address >::init();
      }

      static void GODAFOSS_INLINE refresh(){
      }

      static value_type GODAFOSS_INLINE read(){
         return value_type( pcf8591< bus, address >::_read( channel ) );
      }

   };

   using adc0 = _adc< 0 >;
   using adc1 = _adc< 1 >;
   using adc2 = _adc< 2 >;
   using adc3 = _adc< 3 >;

};
