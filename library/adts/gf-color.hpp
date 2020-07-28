// =============================================================================
//
// gf-color.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2017-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================


// =============================================================================
//
// @title colors
//
// The color abstract data types represent a color.
// You wouldn't have guessed.
// The main use of these types is to specify the color of a pixel on a display.
//
// A color can be represented
// in 1 bit (often black-or-white),
// in 3 bits (one bit each for red, green and blue),
// in one byte (3 bits each for red and green,
//    bits for blue because the eye is less sensitive to blue),
// in 16 bits (5 bits each for red and blue, 6 bits for green
//    because the eye is most sensitive to green),
// in 24 bits (one byte each for red, green and blue).
//
// All color formats have the same interface.
// The choice between the different color formats is dictated by the
// balance between color resolution and memory size.
//
//
//
// =============================================================================


// =============================================================================
//
// color root and concept
//
// =============================================================================

struct color_root {
   static const bool _color_marker = true;
};

template< typename T >
concept is_color = requires(
   T color,
   fraction< int_fast16_t, 255 > v
){
   T::_color_marker;

//   { typename T::base_type() } -> std::same_as< fraction< int_fast16_t, 255 > >;
//   { T::prefix      } -> std::same_as< char[] >;

   { color.red()    } -> std::same_as< typename T::base_type >;
   { color.green()  } -> std::same_as< typename T::base_type >;
   { color.blue()   } -> std::same_as< typename T::base_type >;

};


// =============================================================================
//
// operations for all color types
//
// =============================================================================

template< is_color T >
constexpr uint_fast32_t rgb24( const T c ){
   return (
        ( ( c.red()   * 255 ) << 16 )
      | ( ( c.green() * 255 ) <<  8 )
      | ( ( c.blue()  * 255 ) <<  0 )
   );
}

template< is_color T >
constexpr T operator - ( const T c ){
   return T(
      - c.red(),
      - c.green(),
      - c.blue()
   );
}

template< is_color T, typename V >
constexpr T operator / ( const T c, V n ){
   return T(
      c.red()   / n,
      c.green() / n,
      c.blue()  / n
   );
}

template< is_color T, typename V >
constexpr T operator * ( const T c, V n ){
   return T(
      c.red()   * n,
      c.green() * n,
      c.blue()  * n
   );
}

template< is_color T, typename V >
constexpr T operator * ( V n, const T c ){
   return c * n;
}

template< is_color T, is_color V >
constexpr bool operator == ( const T a, const V b ){
   return (
         ( a.red()    == b.red()   )
      && ( a.green()  == b.green() )
      && ( a.blue()   == b.blue()  )
   );
}

template< is_color T, is_color V >
constexpr bool operator != ( const T a, const V b ){
   return ! ( a == b );
}

template< typename T, is_color V >
T & operator<<( T & lhs, const V & rhs ){
   return lhs
      << V::prefix
      << rhs.red()    * 255 << ","
      << rhs.green()  * 255 << ","
      << rhs.blue ()  * 255 << "]";
}


// =============================================================================
//
// the common color base
//
// =============================================================================

template<
   int red_bits   = 8,
   int green_bits = 8,
   int blue_bits  = 8
>
struct color : color_root {

   static inline char prefix[]  = "c[";
   using base_type = fraction< int_fast16_t, 16 >;

   static constexpr auto red_max   = ( 1 << red_bits   ) - 1;
   static constexpr auto green_max = ( 1 << green_bits ) - 1;
   static constexpr auto blue_max  = ( 1 << blue_bits  ) - 1;

   struct {
      uint8_t red   : red_bits;
      uint8_t green : green_bits;
      uint8_t blue  : blue_bits;
   } values;

   template< int max >
   static constexpr uint8_t clip( int_fast16_t x ){
      return ( x < 0 )
         ? 0
         : ( x > max ? max : x );
   }

   constexpr base_type red() const {
      return base_type( values.red );
   };

   constexpr base_type green() const {
      return base_type( values.green );
   };

   constexpr base_type blue() const {
      return base_type( values.blue );
   };

  constexpr color(){}

/*
   constexpr color(
      base_type red,
      base_type green,
      base_type blue
   ): values {
      red   : { clip< red_max   >( red.of(   red_max   ) ) },
      green : { clip< green_max >( green.of( green_max ) ) },
      blue  : { clip< blue_max  >( blue.of(  blue_max  ) ) } }
   {}
*/

   constexpr color(
      uint_fast16_t red,
      uint_fast16_t green,
      uint_fast16_t blue
   ): values {
      red   : { clip< red_max   >( red   ) },
      green : { clip< green_max >( green ) },
      blue  : { clip< blue_max  >( blue  ) } }
   {}

   template< is_color T >
   constexpr color( T & rhs ): color(
      rhs.red()   .of( red_max   ),
      rhs.green() .of( green_max ),
      rhs.blue()  .of( blue_max  ) )
   {}

   constexpr color(
      uint_fast32_t rgb32 = 0
   ): color(
      ( rgb32 & 0xFF0000 ) >> 16,
      ( rgb32 & 0x00FF00 ) >>  8,
      ( rgb32 & 0x0000FF ) >>  0 )
   {}

   static color random(){
      return color(
         random_in_range< uint8_t >( 0, red_max   ),
         random_in_range< uint8_t >( 0, green_max ),
         random_in_range< uint8_t >( 0, blue_max  )
      );
   }

};


// =============================================================================
//
// 1 bit color
//
// =============================================================================

struct color_bw : color_root {

   static inline char prefix[]  = "c[";
   using base_type = fraction< int_fast16_t, 1 >;
   uint8_t white;

public:

   constexpr base_type red() const {
      return base_type( white );
   };

   constexpr base_type green() const {
      return base_type( white );
   };

   constexpr base_type blue() const {
      return base_type( white );
   };

  constexpr color_bw(){}

  constexpr color_bw(
      uint_fast16_t red,
      uint_fast16_t green,
      uint_fast16_t blue
   ):
      white( ( red + blue + green ) > 0  )
   {}

   template< is_color T >
   constexpr color_bw( const T & rhs ): color_bw(
      rhs.red()   .of( 1 ),
      rhs.green() .of( 1 ),
      rhs.blue()  .of( 1 ) )
   {}

   static color_bw random(){
      return color_bw(
         random_in_range< uint8_t >( 0, 1 ),
         0,
         0
      );
   }

};


// =============================================================================
//
// 3, 8, 16, 24 bit color
//
// =============================================================================

using color_3  = color< 1, 1, 1 >;
using color_8  = color< 3, 3, 2 >;
using color_16 = color< 5, 6, 5 >;
using color_24 = color< 8, 8, 8 >;


// =============================================================================
//
// color literals
//
// =============================================================================

constexpr auto black       = color<>( 0,       0,    0 );
constexpr auto white       = color<>( 0xFF, 0xFF, 0xFF );
constexpr auto red         = color<>( 0xFF,    0,    0 );
constexpr auto green       = color<>( 0,    0xFF,    0 );
constexpr auto blue        = color<>( 0,       0, 0xFF );
constexpr auto gray        = color<>( 0x80, 0x80, 0x80 );
constexpr auto yellow      = color<>( 0xFF, 0xFF,    0 );
constexpr auto cyan        = color<>(    0, 0xFF, 0xFF );
constexpr auto magenta     = color<>( 0xFF,    0, 0xFF );

constexpr auto violet      = color<>( 0xEE82EE );
constexpr auto sienna      = color<>( 0xA0522D );
constexpr auto purple      = color<>( 0x800080 );
constexpr auto pink        = color<>( 0xFFC8CB );
constexpr auto silver      = color<>( 0xC0C0C0 );
constexpr auto brown       = color<>( 0xA52A2A );
constexpr auto salmon      = color<>( 0xFA8072 );
