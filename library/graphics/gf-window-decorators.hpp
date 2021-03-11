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

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      minion::color_t color
   ){
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
// ==========================================================================

template< typename minion >
struct flip_vertically :
   window_root<
      flip_vertically< minion >,
      typename minion::offset_t,
      typename minion::color_t,
      minion::size
   >
{

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      minion::color_t color
   ){
      minion::write(
         minion::origin
            + typename minion::offset_t(
               offset.x, minion::size.y - offset.y ),
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

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      minion::color_t color
   ){
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

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      minion::color_t color
   ){
      minion::write( minion::origin + offset, - color );
   }

   static void flush(){
      minion::flush();
   }

};


// ==========================================================================
//
// recolor
//
// ==========================================================================


template<
   window minion,
   typename input_color_t,
   typename minion::color_t (* remap )( const input_color_t & )
>
struct recolor :
   public window_root<
      recolor< minion, input_color_t, remap >,
      typename minion::offset_t,
      input_color_t,
      minion::size
   >
{

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      input_color_t color
   ){
      minion::write( minion::origin + offset, remap( color ) );
   }

   static void flush(){
      minion::flush();
   }

};


// ==========================================================================
//
// monochrome
//
// ==========================================================================

/*
 *
 * warning: whose type uses the anonymous namespace
 *
template<
   window minion,
   minion::color_t for_white,
   minion::color_t for_black
>
struct monochrome: public recolor<
   minion,
   black_or_white,
   []( const black_or_white & c ){
      return ( c.is_black ) ? for_black : for_white;
   }
> {};
*/

template<
   window minion,
   minion::color_t for_white,
   minion::color_t for_black
>
struct monochrome :
   public window_root<
      monochrome< minion, for_black, for_white >,
      typename minion::offset_t,
      color_bw,
      minion::size
   >
{

   using resources = use_list< minion >;

   static void write_implementation(
      minion::offset_t offset,
      const color_bw & c
   ){
      minion::write(
         minion::origin + offset,
         ( c.is_black() ) ? for_black : for_white );
   }

   static void flush(){
      minion::flush();
   }

};

// ==========================================================================
//
// port from window
//
// ==========================================================================


template<
   window minion
>
struct port_from_windowx:
   public port_out_root< minion::size.x >
{
   using root = port_out_root< minion::size.x >;

   using resources = use_list< minion >;

   static void write( typename root::value_type v ){
      (void) v;
      for( int i = 0; i < root::n_pins; ++i ){
         const typename root::value_type one = 0x01;
         (void) one;


         minion::write(
            minion::origin + typename minion::offset_t( i, 0 ), black );
        //    ( ( v & ( one  << i ) ) != 0 ) ? black : white );

      }
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
