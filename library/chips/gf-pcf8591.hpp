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

template< is_i2c_bus bus, uint_fast8_t address = 0 >
struct pcf8591 :
   _port_oc_buffer_root< 8 >
{
   static constexpr uint8_t base = 0x48;	
   static inline uint8_t configuration;
	
   static void HWCPP_INLINE init(){
      static_assert( 
         bus::profile::f <= 100'000,
         "The maximum I2C bus frequency for this chip is 100 kHz" );
      bus::init();       
   }
   
   static uint_fast8_t _read( uint_fast8_t channel ){
   
      // select the correct channel
      uint8_t control[ 1 ] = { ( configuration & ( ~ 0x03 )) | (uint8_t)channel }; 
      bus::write( base + address, control, 1 ); 
      
      // read results, note that the first byte is the 
      // *previous* ADC result, the second byte is what we want
	  //
	  // At least, that is what the documentation suggests.
      // Actually, it seems we need the 3d byte!
      // Something fishy is going on!
      uint8_t results[ 3 ];
      bus::read( base + address, results, 3 );
      return results[ 2 ];  
   }	  
   
   template< uint_fast8_t channel >
   struct _adc_foundation :
      _adc_root< 8 >         
   {
	   
      static void HWCPP_INLINE init(){
         pcf8591< bus, address >::init();
      }

      static value_type HWCPP_INLINE get_direct(){
         return pcf8591< bus, address >::_read( channel );
      }	   
      	   
   };	   
     
   using adc0 = _adc_builder< _adc_foundation< 0 > >;
   using adc1 = _adc_builder< _adc_foundation< 1 > >;
   using adc2 = _adc_builder< _adc_foundation< 2 > >;
   using adc3 = _adc_builder< _adc_foundation< 3 > >;

};	
