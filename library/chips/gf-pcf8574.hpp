// ==========================================================================
//
// file : pcf8574.hpp
//
// ==========================================================================   
   

template< 
   typename     bus,
   int          address = 0x00,
   int          base    = 0x20
>   
struct pcf8574 :
   be_port_out< 8 >
{

   static inline uint_fast8_t buffer;
   
   static void GODAFOSS_INLINE init(){
      bus::init();   
   }
     
   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      buffer = d;
   }
   
   static void GODAFOSS_INLINE flush(){
      typename bus::write_transaction( address + base ).write( buffer );
   }
};

template< 
   typename     bus,
   int          address = 0x00,
   int          base    = 0x38
>   
using pcf8574a = struct pcf8574< bus, address, base >;
