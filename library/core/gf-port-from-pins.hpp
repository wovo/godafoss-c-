// ==========================================================================
//
// gf-port-from-pins.hpp
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss 
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// recursive components
//
// ==========================================================================

// ========== init

template< typename pin, typename tail >
struct _port_recurse_init : tail {
	
  static void GODAFOSS_INLINE init() { 
      pin::init();
      tail::init(); 
  }	
};

// ========= output

template< typename pin, typename tail >
struct _port_recurse_set : tail {
	
   using _vt = typename tail::value_type;
	
   static void GODAFOSS_INLINE write( _vt v ) {
      pin::write( ( v & 0x01 ) != 0 );
      tail::write( (_vt) ( v >> 1 ) );
   }
      
   static void GODAFOSS_INLINE flush() {
      pin::flush();
      tail::flush();
   }
};


// ========= input

template< typename pin, typename tail >
struct _port_recurse_get : tail {
	
   using _vt = typename tail::value_type;
	
   static _vt GODAFOSS_INLINE read() {   
      return (_vt)
	     pin::get_buffered()
         | ( tail::get_buffered() << 1 );
   }
            
   static void GODAFOSS_INLINE refresh() {
      pin::refresh();
      tail::refresh();
   }
};

// ========= direction 

template< typename pin, typename tail >
struct _port_recurse_direction : tail {
	
   static void GODAFOSS_INLINE direction_set_input(){
      pin::direction_set_input( d );
      tail::direction_set_input( d ); 
   }    
   
   static void GODAFOSS_INLINE direction_set_output(){
      pin::direction_set_output( d );
      tail::direction_set_output( d );         
   }    
  
   static void GODAFOSS_INLINE direction_flush(){
      pin::direction_flush();
      tail::direction_flush();   
   } 
};


// ==========================================================================
//
// PUBLIC
//
// create a port_out from pins
//
// ==========================================================================

// fallback, required but never used
template< int n, typename... arg_tail >
struct _port_out_from_pins {};

// recursion endpoint
template< int n > 
struct _port_out_from_pins< n > : 
   port_out_dummy< n >
{};
    
// add one pin and recurse
template< int n, typename pin, typename... tail >
struct _port_out_from_pins< n, pin, tail... > :
   _port_recurse_set< pin_out< pin >, 
   _port_recurse_init< pin_out< pin >, 
      _port_out_from_pins< n, tail... > > >
{};

// determine the number of arguments, break the forced inlining, 
// and defer to the recursive template
template< _can_pin_out_list... Ts > 
struct port_out< Ts... > :
   _no_inline_wrapper<
      _port_out_from_pins< sizeof...( Ts ), Ts... > >
{};


// ==========================================================================
//
// PUBLIC
//
// create a port_in from pins
//
// ==========================================================================

// fallback, required but never used
template< int n, typename... arg_tail >
struct _port_in_from_pins {};

// recursion endpoint
template< int n > 
struct _port_in_from_pins< n > : 
   port_in_dummy< n >
{};
    
// add one pin and recurse
template< int n, typename pin, typename... tail >
struct _port_in_from_pins< n, pin, tail... > :
   _port_recurse_get< pin_in< pin >, 
   _port_recurse_init< pin_in< pin >, 
      _port_in_from_pins< n, tail... > > >
{};

// determine the number of arguments, break the forced inlining, 
// and defer to the recursive template
template< _can_pin_in_list... Ts > 
struct port_in< Ts... > :
   _no_inline_wrapper<
      _port_in_from_pins< sizeof...( Ts ), Ts... > >
{};

      
// ==========================================================================
//
// PUBLIC
//
// create a port_in_out from pins
//
// ==========================================================================

// fallback, required but never used
template< int n, typename... arg_tail >
struct _port_in_out_from_pins {};

// recursion endpoint
template< int n > 
struct _port_in_out_from_pins< n > : 
   port_in_out_dummy< n >
{};
    
// add one pin and recurse
template< int n, typename pin, typename... tail >
struct _port_in_out_from_pins< n, pin, tail... > :
   _port_recurse_set< pin_in_out< pin >, 
   _port_recurse_get< pin_in_out< pin >, 
   _port_recurse_direction< pin_in_out< pin >, 
   _port_recurse_init< pin_in_out< pin >, 
      _port_in_out_from_pins< n, tail... > > > > >
{};

// determine the number of arguments, break the forced inlining, 
// and defer to the recursive template
template< _can_pin_in_out_list... Ts > 
struct port_in_out< Ts... > :
   _no_inline_wrapper<
      _port_in_out_from_pins< sizeof...( Ts ), Ts... > >
{};
   
   
// ==========================================================================
//
// PUBLIC
//
// create a port_oc from pins
//
// ==========================================================================

// fallback, required but never used
template< int n, typename... arg_tail >
struct _port_oc_from_pins {};

// recursion endpoint
template< int n > 
struct _port_oc_from_pins< n > : 
   port_oc_dummy< n >
{};
    
// add one pin and recurse
template< int n, typename pin, typename... tail >
struct _port_oc_from_pins< n, pin, tail... > :
   _port_recurse_set< pin_oc< pin >, 
   _port_recurse_get< pin_oc< pin >, 
   _port_recurse_init< pin_oc< pin >, 
      _port_oc_from_pins< n, tail... > > > >
{};

// determine the number of arguments, break the forced inlining, 
// and defer to the recursive template
template< _can_pin_oc_list... Ts > 
struct port_oc< Ts... > :
   _no_inline_wrapper<
      _port_oc_from_pins< sizeof...( Ts ), Ts... > >
{};