// ==========================================================================
//
// file : gf-servo.hpp
//
// ==========================================================================

template< can_pin_out _pin, typename timing >
struct servo {

   using pin = direct< pin_out< _pin > >;

   using resources = use_list< pin, timing >;

   static void write( int n ){
      pin::write( 1 );
      timing:: template us< 1000 >::wait();
	   while( n > 0 ){
         timing:: template us< 10 >::wait();
         --n;
      }
      pin::write( 0 );
      timing:: template ms< 20 >::wait();
   }

};
