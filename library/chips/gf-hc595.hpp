// ==========================================================================
//
// file : pcf8574.hpp
//
// ==========================================================================   
   

// ==========================================================================
//
// pcf8574 generic
//
// ==========================================================================  

template< 
   typename     bus,
   can_pin_out  _cs
>   
struct hc595 :
   be_port_out< 8 >
{
   using cs = direct< pin_out< _cs >>;
   static inline uint_fast8_t buffer, dummy;
   
   static void GODAFOSS_INLINE init(){
      bus::init();   
      cs::init();    
   }
     
   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      buffer = d;
   }
   
   static void GODAFOSS_INLINE flush(){
      bus::write_and_read_single( buffer, dummy );
   }
};
