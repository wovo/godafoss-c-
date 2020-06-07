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
              
   static constexpr uint8_t clip( int_fast16_t x, bool transparent = false ){
      return ( transparent )
         ? 0
         : ( x < 0 ) 
            ? 0 
            : ( x > 0xFF ? 0xFF : x ); 
   }   
      
public:

   bool    is_transparent; 
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
      uint_fast32_t blue, 
      bool transparent = false 
   ): 
      is_transparent{ transparent },
      red{   clip( red,   transparent ) }, 
      green{ clip( green, transparent ) }, 
      blue{  clip( blue,  transparent ) }
   {}   
      
   constexpr color( uint_fast32_t rgb32 = 0 ):
      is_transparent( false ),  
      red{   clip( ( rgb32 & 0xFF0000 ) >> 16 ) }, 
      green{ clip( ( rgb32 & 0x00FF00 ) >>  8 ) }, 
      blue{  clip( ( rgb32 & 0x0000FF ) >>  0 ) }
   {}   
    
   constexpr color operator- ( void ) const {
      return color( 
         0xFF - static_cast< uint8_t >( red ), 
         0xFF - static_cast< uint8_t >( green ), 
         0xFF - static_cast< uint8_t >( blue ), 
         is_transparent ); 
   }   
   
   constexpr bool operator== ( const color c ) const {
      return is_transparent 
         ? c.is_transparent 
         : ( 
            ( ! c.is_transparent ) 
            && ( red    == c.red ) 
            && ( green  == c.green ) 
            && ( blue   == c.blue )
         ); 
   }

   constexpr bool operator!= ( const color c ) const {
      return ! ( *this == c );  
   }     
   
   template< typename T>
   friend T & operator<<( T & lhs, const color & rhs ){
      if( rhs.is_transparent ){
         return lhs << "[transparent]"; 
      } else {
         return lhs 
            << "[" << hex << setw(4) << setfill( '0' )
            << rhs.red << "," 
            << rhs.green << ","
            << rhs.blue << "]";
      }
}  

};

constexpr color black       = color( 0,       0,    0 );
constexpr color white       = color( 0xFF, 0xFF, 0xFF );
constexpr color red         = color( 0xFF,    0,    0 ); 
constexpr color green       = color( 0,    0xFF,    0 );
constexpr color blue        = color( 0,       0, 0xFF );
constexpr color gray        = color( 0x80, 0x80, 0x80 );
constexpr color yellow      = color( 0xFF, 0xFF,    0 );
constexpr color cyan        = color(    0, 0xFF, 0xFF );
constexpr color magenta     = color( 0xFF,    0, 0xFF );
constexpr color transparent = color( 0,       0,    0,   1 );      
constexpr color violet      = color( 0xEE82EE );        
constexpr color sienna      = color( 0xA0522D );        
constexpr color purple      = color( 0x800080 );         
constexpr color pink        = color( 0xFFC8CB );        
constexpr color silver      = color( 0xC0C0C0 );        
constexpr color brown       = color( 0xA52A2A );        
constexpr color salmon      = color( 0xFA8072 );
