// ==========================================================================
//
// file : hc595.hpp
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
   static inline uint_fast8_t buffer;
   
   static void GODAFOSS_INLINE init(){
      bus::init();   
      cs::init();    
   }
     
   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      buffer = d;
   }
   
   static void GODAFOSS_INLINE flush(){
      typename bus:: template transfer< cs >().write( buffer );
   }
};
