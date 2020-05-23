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

template< typename minion_1, typename minion_2 >
struct both : window_root< 
   both< minion_1, minion_2 >,
   typename minion_1::address_t,
   typename minion_1::color_t,
   minion_1::size >
{
   
   static void init(){
      minion_1::init();   
      minion_2::init();
   }

   static void write_implementation( 
      minion_1::address_t pos, 
      minion_1::color_t color
   ){
      minion_1::write( pos, color );   
      minion_2::write( pos, color );
   }

   static void flush(){
      minion_1::flush();   
      minion_2::flush();
   }

};