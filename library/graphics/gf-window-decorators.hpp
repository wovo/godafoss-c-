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


// ==========================================================================
//
// ==========================================================================

template< typename minion >
struct flip_horizontally :
   window_root<
      flip_horizontally< minion >,
      typename minion::offset_t,
      typename minion::color_t,
      minion::size
   >
{

   static void init(){
      minion::init();
   }

   static void write_implementation(
      minion::location_t pos,
      minion::color_t color
   ){
      auto offset = pos - minion::origin;
      minion::write(
         minion::origin
            + typename minion::offset_t(
               minion::size.x - offset.x, offset.y ),
         color
      );
   }

   static void flush(){
      minion::flush();
   }

};


// ==========================================================================
//
// fold
//
// ==========================================================================

template< typename minion, unsigned int n_fold = 2 >
struct fold :
   window_root<
      fold< minion >,
      typename minion::offset_t,
      typename minion::color_t,
      xy<>( minion::size.x / n_fold, minion::size.y * n_fold )
   >
{

   using root = window_root<
      fold< minion >,
      typename minion::offset_t,
      typename minion::color_t,
      xy<>( minion::size.x / n_fold, minion::size.y * n_fold )
   >;

   static void init(){
      minion::init();
   }

   static void write_implementation(
      minion::location_t pos,
      minion::color_t color
   ){
      auto offset = pos - minion::origin;
      minion::write(
         minion::origin + typename minion::offset_t(
            offset.x + ( ( offset.y / minion::size.y ) * root::size.x ),
            offset.y % minion::size.y ),
         color
      );
   }

   static void flush(){
      minion::flush();
   }

};


// ==========================================================================
//
// invert
//
// ==========================================================================


template< window T >
struct invert_supported< T >{
   static constexpr bool supported = true;
};

template< window minion >
struct invert< minion >:
   window_root<
      invert< minion >,
      typename minion::offset_t,
      typename minion::color_t,
      minion::size
   >
{

   static void init(){
      minion::init();
   }

   static void write_implementation(
      minion::location_t pos,
      minion::color_t color
   ){
      minion::write( pos, - color );
   }

   static void flush(){
      minion::flush();
   }

};




// direct
// buffered
// mirror_horizontally
// mirror_vertically
// sub_window
// combine windows
// make black-n-white
// address transformation
// color transformation
//
