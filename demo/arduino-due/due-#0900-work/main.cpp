#include "godafoss.hpp"


// @quote fraction 8 };
template< typename T, T _full_scale >
struct fraction {

   using data_type = T;
   static constexpr data_type full_scale = _full_scale;

   data_type raw_value;

   // @quote constructor 1 ...
   constexpr explicit fraction(){}

   // @quote constructor 1 ...
   constexpr explicit fraction( T x )
      : raw_value( x )
   {}

   // @quote assignment 2 ... }
   template< typename V, V rhs_full_scale >
   constexpr fraction & operator=( const fraction< V, rhs_full_scale > & rhs ){
      raw_value = ( rhs.raw_value * rhs_full_scale ) / full_scale;
   }

   // @quote of 2 ... }
   template< typename V >
   constexpr V of( V max ) const {
      return ( raw_value * max ) / _full_scale;
   }

   // @quote of 2 ... }
   template< typename V, typename W >
   constexpr V of( V x, W y ) const {
      return x + of( y - x );
   }

   // @quote negate 1 ... }
   constexpr fraction operator-() const {
      return fraction( full_scale - raw_value );
   }

};

struct color_root {
   static constexpr bool _color_marker = true;
};

template< typename T >
concept is_color = requires(
   T color,
   fraction< int_fast16_t, 255 > v
){
   T::_color_marker;

};

template<
   int red_bits   = 8,
   int green_bits = 8,
   int blue_bits  = 8
>
struct color : color_root {

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

   constexpr color()
 //     : values { red : ( 0 ), green : (0), blue : (0) }
   {}

   constexpr color(
      uint_fast16_t red,
      uint_fast16_t green,
      uint_fast16_t blue
   ): values {
      red   : { clip< red_max   >( red   ) },
      green : { clip< green_max >( green ) },
      blue  : { clip< blue_max  >( blue  ) } }
   {}

};

constexpr auto black = color<>( 0,       0,    0 );

struct ws2801 {
   static inline color< 8, 8, 8 > pixels;

   static void init(){
      pixels = black;
   }
};

int main( void ){
   ws2801::init();
}
