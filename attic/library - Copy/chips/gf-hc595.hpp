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
   using chip = hc595< bus, _cs >;	 
   using cs = direct< pin_out< _cs >>;
   static inline uint_fast8_t buffer;
   static inline bool dirty;
   
   static void GODAFOSS_INLINE init(){
      dirty = false;
      bus::init();   
      cs::init();    
   }
     
   static void GODAFOSS_INLINE write( uint_fast8_t d ){
      buffer = d;
	  dirty = true;
   }
   
   static void GODAFOSS_INLINE flush(){
      if( dirty ){	   
         typename bus:: template transfer< cs >().write( buffer );
		 dirty = false;
      }		 
   }
   
   template< int n > struct pin :
      be_pin_out
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
