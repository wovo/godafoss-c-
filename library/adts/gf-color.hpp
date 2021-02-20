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
// @define color<>
// @title color
//
// The instances of the color type template represent a color.
// You wouldn't have guessed.
// The main use of these types is to specify the color of a pixel 
// in an image or on a display.
//
// @insert color
//
// Displays have various color depths (number of bits used to represent
// a color), so different uses call for different color types,
// balancing memory use (and speed) against color depth. 
//
// The depths are specified in bits for red, blue and green.
// The maximum is 8 bits each, which is the default.
//
// -----------------------------------------------------------------------------
//
// @section predefined
// @insert predefined
//
// The most common color depths are predefined:
//   - 1 bit (often used for black-or-white),
//   - 3 bits (one bit each for red, green and blue),
//   - 8 bits (3 bits each for red and green,
// 2 bits for blue because the eye is less sensitive to blue),
//   - 16 bits (5 bits each for red and blue, 6 bits for green
// because the eye is most sensitive to green),
//   - 24 bits (8 bits each for red, green and blue).
//
// -----------------------------------------------------------------------------
//
// @section constructors
//
// @insert constructor-default
//
// Colors have a default constructor, but
// don't rely on the color value after default construction.
//
// @insert constructor-copy
//
// The copy constructor copies the fractions
//
// @insert constructor-values
//
// A color can be constructed from the three values
// for its read, green and blue components.
// For each component, the value is relative to the (unsigned) maximum that
// fits in the number of bits specified (by the template argument)
// for that color.
//
//
// -----------------------------------------------------------------------------
//
// @section random()
//
// @insert random
//
// The static member function random() returns a random color, for which
// each of the red, green and blue components has been selected by
// the simple random generator.
//
//
// -----------------------------------------------------------------------------
//
// @section getters
//
// @insert get
//
// The red(), geen() and blue() member functions return the
// red, green and blue components as a fraction.
//
// @code 1
// color< 8, 8, 8 >( 5, 10, 15 ).blue() == fraction< 255 >( 10 )
//
//
// =============================================================================


// =============================================================================
//
// color root and concept
//
// =============================================================================

struct color_root {
   static constexpr bool _color_marker = true;
};

template< typename T >
concept is_color = requires(
   T color,
   fraction< int_fast16_t, 255 > v
){
   T::_color_marker;

//   { typename T::base_type() } -> std::same_as< fraction< int_fast16_t, 255 > >;

//   { color.red()    } -> std::same_as< typename T::base_type >;
//   { color.green()  } -> std::same_as< typename T::base_type >;
//   { color.blue()   } -> std::same_as< typename T::base_type >;

};


// =============================================================================
//
// the general color template
//
// =============================================================================

using color_rgb_t = fraction< int_fast16_t, 255 >;

// @quote color 6 { ... }
template<
   int red_bits   = 8,
   int green_bits = 8,
   int blue_bits  = 8
>
struct color
: color_root {

   //using color_rgb_t =

   static_assert( red_bits   <= 8 );
   static_assert( green_bits <= 8 );
   static_assert( blue_bits  <= 8 );

   static constexpr auto red_max   = ( 1 << red_bits   ) - 1;
   static constexpr auto green_max = ( 1 << green_bits ) - 1;
   static constexpr auto blue_max  = ( 1 << blue_bits  ) - 1;

   struct {
      uint8_t red   : red_bits;
      uint8_t green : green_bits;
      uint8_t blue  : blue_bits;
   } values = { 0, 0, 0 };

   template< int max >
   static constexpr uint8_t clip( int_fast16_t x ){
      return ( x < 0 )
         ? 0
         : ( x > max ? max : x );
   }

   // @quote constructor-default 1 { ... }
   constexpr color()
      : values{ 0, 0, 0 }
   {}

   // @quote constructor-copy 5 { ... }
   template< is_color T >
   constexpr color( T & rhs ): color(
      rhs.red()   .of( red_max   ),
      rhs.green() .of( green_max ),
      rhs.blue()  .of( blue_max  ) )
   {}

   // @quote constructor-components 5 { ... }
   constexpr color(
      int_fast16_t red,
      int_fast16_t green,
      int_fast16_t blue
   )
   : values {
      red   : { clip< red_max   >( red   ) },
      green : { clip< green_max >( green ) },
      blue  : { clip< blue_max  >( blue  ) } }
   {}

   // @quote constructor-fractions 5 { ... }
   constexpr color(
      color_rgb_t red,
      color_rgb_t green,
      color_rgb_t blue
   )
   : values {
      red   : ( red.of( red_max ) ),
      green : ( green.of( green_max ) ),
      blue  : ( blue.of( blue_max ) ) }
   {}

   // only for 24 bits!
   // @quote constructor-rgb 5 { ... }
   constexpr color(
      uint_fast32_t rgb32
   ): color(
      ( rgb32 & 0xFF0000 ) >> 16,
      ( rgb32 & 0x00FF00 ) >>  8,
      ( rgb32 & 0x0000FF ) >>  0 )
   {}

   // @quote get 1 ... }
   constexpr color_rgb_t red() const {
      return color_rgb_t( values.red );
   };

   // @quote get 1 ... }
   constexpr color_rgb_t green() const {
      return color_rgb_t( values.green );
   };

   // @quote get 1 ... }
   constexpr color_rgb_t blue() const {
      return color_rgb_t( values.blue );
   };

   // @quote random 1 ... }
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
// special case: 1 bit color
//
// =============================================================================

// @quote predefined 1 { ... }
struct color_bw
   : color_root {

   // is_white??
   uint8_t white = 0; // otherwise global ctor for array of these, why?

public:

   constexpr color_rgb_t red() const {
      return color_rgb_t( white );
   };

   constexpr color_rgb_t green() const {
      return color_rgb_t( white );
   };

   constexpr color_rgb_t blue() const {
      return color_rgb_t( white );
   };
   
   constexpr bool is_black() const {
      return white == 0;      
   }

   constexpr bool is_white() const {
      return white != 0;      
   }

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
      << "c["
      << rhs.red()    * 255 << ","
      << rhs.green()  * 255 << ","
      << rhs.blue ()  * 255 << "]";
}


// =============================================================================
//
// 3, 8, 16, 24 bit color
//
// =============================================================================

// @quote predefined 4
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
