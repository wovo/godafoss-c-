// ==========================================================================
//
// File      : gf-color.hpp
// Part of   : C++ hwlib library for close-to-the-hardware OO programming
// Copyright : wouter@voti.nl 2017-2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

class color final {
private:

   static constexpr uint8_t clip( int_fast16_t x ){
      return ( x < 0 )
         ? 0
         : ( x > 0xFF ? 0xFF : x );
   }

public:

   uint8_t red;
   uint8_t green;
   uint8_t blue;

   uint_fast32_t rgb() const {
      return ( static_cast< uint_fast32_t >( red )   << 16 )
           | ( static_cast< uint_fast32_t >( green ) << 8  )
           | ( static_cast< uint_fast32_t >( blue )  << 0  );
   }

   constexpr color(
      uint_fast32_t red,
      uint_fast32_t green,
      uint_fast32_t blue
   ):
      red{   clip( red   ) },
      green{ clip( green ) },
      blue{  clip( blue  ) }
   {}

   constexpr color( uint_fast32_t rgb32 = 0 ):
      red{   clip( ( rgb32 & 0xFF0000 ) >> 16 ) },
      green{ clip( ( rgb32 & 0x00FF00 ) >>  8 ) },
      blue{  clip( ( rgb32 & 0x0000FF ) >>  0 ) }
   {}

   constexpr color operator- ( void ) const {
      return color(
         0xFF - static_cast< uint8_t >( red ),
         0xFF - static_cast< uint8_t >( green ),
         0xFF - static_cast< uint8_t >( blue ) );
   }

   constexpr bool operator== ( const color c ) const {
      return
               ( red    == c.red   )
            && ( green  == c.green )
            && ( blue   == c.blue  );
   }

   constexpr bool operator!= ( const color c ) const {
      return ! ( *this == c );
   }

   template< typename T>
   friend T & operator<<( T & lhs, const color & rhs ){
      return lhs
         << "["
<< hex
//<< setw( 2 )
//<< setfill( '0' )
         << rhs.red << ","
         << rhs.green << ","
         << rhs.blue << "]";
   }

}; // class color

constexpr color black       = color( 0,       0,    0 );
constexpr color white       = color( 0xFF, 0xFF, 0xFF );
constexpr color red         = color( 0xFF,    0,    0 );
constexpr color green       = color( 0,    0xFF,    0 );
constexpr color blue        = color( 0,       0, 0xFF );
constexpr color gray        = color( 0x80, 0x80, 0x80 );
constexpr color yellow      = color( 0xFF, 0xFF,    0 );
constexpr color cyan        = color(    0, 0xFF, 0xFF );
constexpr color magenta     = color( 0xFF,    0, 0xFF );

constexpr color violet      = color( 0xEE82EE );
constexpr color sienna      = color( 0xA0522D );
constexpr color purple      = color( 0x800080 );
constexpr color pink        = color( 0xFFC8CB );
constexpr color silver      = color( 0xC0C0C0 );
constexpr color brown       = color( 0xA52A2A );
constexpr color salmon      = color( 0xFA8072 );

template< typename color_t >
color_t random_color();

template<>
color random_color< color >(){
   return color(
      random_in_range< uint8_t >( 0, 255 ),
      random_in_range< uint8_t >( 0, 255 ),
      random_in_range< uint8_t >( 0, 255 ) );
}

class black_or_white final {
public:

   bool is_black;

   constexpr black_or_white( color c ):
      is_black( c == black )
   {}

//   static constexpr auto white = black_or_white( false );
//   static constexpr auto black = black_or_white( true );

   constexpr black_or_white operator- ( void ) const {
      return black_or_white( is_black ); // why does this invert??
   }

   template< typename T>
   friend T & operator<<( T & lhs, const black_or_white rhs ){
      return lhs <<
         ( rhs.is_black
            ? "[black]"
            : "[white]" );
   }

};


