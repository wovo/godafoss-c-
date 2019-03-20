#include "hwcpp.hpp"

namespace gp = godafoss;
using target = gf::target<>;
using timing = target::timing;

int main(){ 
   target::led_type led;
   gp::
}

template< typename pin_type, typename delay_type >
struct blink {
   
   // still needs the this pointer (or not?)  => undefined behaviour...
   pin_type    & pin;
   delay_type  & delay;    
   
   blinker( pin_type & pin_arg, delay_type & delay_arg ):
      pin{ pin_arg }, delay{ delay_arg }{}   
	  
   void INLINE operator(){
      for(;;){
         pin.set( 1 );
         ( delay / literal< 2 > ).wait();
		 pin.set( 0 );
		 ( delay / literal< 2 > ).wait();
	  }		  
   }	   
};


// pass a real object
auto led = target::pin_out( 1, 5 );
blink( led );

// pass a dummy (tag-type) object
auto led = target::pin_out< 1, 5 >;
blink( led );

// pass a type
using led = target::pin_out< 1, 5 >;
blink< led >();

template< typename A, typename B >
auto INLINE operator/( const A & a, const B & b ) 
   -> A::base_type< a.raw_value / b.raw_value >
{ return A::base_type< a.raw_value / b.raw_value >(); }   

template< int n >
struct interval {
   template< int n > base = interval
   int raw_value = n;
   
};

options:

pass plain template parameters < p >
   + no need to create objects
   + can be used anywhere
   + they clearly stand out
   - unfamiliar syntax
   - values must be passed separately like f< p >( 5 )
   - init() must be called
   
pass template paramters implicitly ( p )
   +    

pass either template parameters or plain objects   
   + same syntax for both cases
   
===================================================================

template< typename pin_type, typename delay_type >
struct blink {
   
   save< pin_type >    pin;
   save< delay_type >  delay;    
   
   blinker( pin_type && pin_arg, delay_type && delay_arg ):
      pin{ pin_arg }, delay{ delay_arg }{}   
	  
   void INLINE operator(){
      for(;;){
         pin.set( 1 );
         ( delay / literal< 2 > ).wait();
		 pin.set( 0 );
		 ( delay / literal< 2 > ).wait();
	  }		  
   }	   
};




