// ==========================================================================
//
// File      : gf-window-all.hpp
// Part of   : C++ hwlib library for close-to-the-hardware OO programming
// Copyright : wouter@voti.nl 2017-2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

template< window... tail >
struct windows_max_size { };

template<>
struct windows_max_size<> { 
   static constexpr auto value = xy{ 0, 0 }; 
};

template< window w, typename... tail >
struct windows_max_size< w, tail... > { 

   static constexpr xy max( xy a, xy b ){
      return xy( std::max( a.x, b.x ), std::max( a.y, b.y ) );   
   }

   static constexpr auto value = max( 
     w::size, 
     windows_max_size< tail... >::value );

};


// ==========================================================================

template< window... tail >
struct _windows_all { };

template< window w >
struct _windows_all< w > {
   
   static void init(){}
   
   static void write( xy address, color col ){ 
      (void)address; 
      (void)col; 
   }
   
   static void flush(){}   
   
};

template< window w, typename... tail >
struct _windows_all< w, tail... > { 
   
   static void init(){
      w::init();
      _windows_all< tail... >::init();
   }
   
   static void write( xy address, color col ){
      w::write( address, col );
      _windows_all< tail... >::write( address, col );  
   }
   
   static void flush(){
      w::flush();
      _windows_all< tail... >::flush();
   }      
};

//
// ==========================================================================

template< typename T >
concept window_compatible = true;

template< window_compatible... tail >
//   requires window_compatible_list< tail... >
struct all< tail... > :
   window_root<
      all< tail... >,
      xy,
      color,
      windows_max_size< tail... >::value
   >
{
   static void init(){
      _windows_all< tail... >::init();
   }
   
   static void write_implementation( xy address, color col ){
      _windows_all< tail... >::write( address, col );  
   }
   
   static void flush(){
      _windows_all< tail... >::flush();
   }      
};      


