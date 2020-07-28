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

template< typename... tail >
struct windows_max_size { };

template<>
struct windows_max_size<> {
   static constexpr auto value = xy{ 0, 0 };
};

template< typename w, typename... tail >
struct windows_max_size< w, tail... > {

   static constexpr auto max( auto a, auto b ) -> decltype( a + b ){
      return a > b ? a : b;
   }

   template< typename T, typename V >
   static constexpr xy<> max( xy< T > a, xy< V > b ){
      return xy( max( a.x, b.x ), max( a.y, b.y ) );
   }

   static constexpr auto value = max(
     w::size,
     windows_max_size< tail... >::value );

};


// ==========================================================================

template< typename w, typename... tail >
struct _windows_all {

   static void init(){
      w::init();
      _windows_all< tail... >::init();
   }

   static void write( xy<> offset, is_color auto col ){
      w::write( w::origin + offset, col );
      _windows_all< tail... >::write( offset, col );
   }

   static void flush(){
      w::flush();
      _windows_all< tail... >::flush();
   }
};


template< typename w >
struct _windows_all< w > {

   static void init(){
      w::init();
   }

   static void write( xy<> offset, is_color auto col ){
      w::write( w::origin + offset, col );
   }

   static void flush(){
      w::flush();
   }

};


//
// ==========================================================================

template< typename T >
concept window_compatible = requires {
   T::_window_marker;
   { T::init() } -> std::same_as< void >;
};

template< typename... Ts >
concept window_compatible_list =
   ( window_compatible< Ts > && ... );

template< typename first, typename... tail >
   requires window_compatible< first >
         && window_compatible_list< tail... >
struct all_supported< first, tail... > {
   static constexpr bool supported = true;
};

template< typename first, typename... tail >
   requires window_compatible< first >
         && window_compatible_list< tail... >
struct all< first, tail... > :
   window_root<
      all< first, tail... >,
      xy<>,
      color<>,
      windows_max_size< first, tail... >::value
   >
{
   static void init(){
      _windows_all< first, tail... >::init();
   }

   static void write_implementation( xy<> offset, is_color auto col ){
      _windows_all< first, tail... >::write( offset, col );
   }

   static void flush(){
      _windows_all< first, tail... >::flush();
   }
};
