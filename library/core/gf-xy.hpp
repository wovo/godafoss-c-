// ==========================================================================
//
// gf-xy.hpp
//
// ==========================================================================
//
// The xy< T > ADT is a pair of two T values named x and y. 
// It is used as distances between locations on a window or terminal.
//
// The xy< torsor< T > > is used 
// to identify a location on a window or terminal.
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2018-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// xy ADT
//
// ==========================================================================

template< typename T = int_fast16_t >
class xy final {
public:

   using value_t = T;

   value_t x, y;
   
   constexpr xy( value_t x, value_t y ): x{ x }, y{ y }{}
   
   constexpr xy( value_t x ): x{ x }, y{ x }{}
   
   constexpr xy():x{ 0 }, y{ 0 }{}

   template< typename V >
   constexpr auto operator+( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > { 
          static_cast< value_t >( x + rhs.x ),
          static_cast< value_t >( y + rhs.y )
      };      
   }      
   
   template< typename V >
   constexpr auto operator-( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x - rhs.x ),
          static_cast< value_t >( y - rhs.y )
      };         
   }      

   constexpr xy operator/( const value_t rhs ) const {
      return xy{ 
          static_cast< value_t >( x / rhs ),
          static_cast< value_t >( y / rhs )
      };         
   }    

   constexpr xy operator*( const value_t rhs ) const {
      return xy{ 
          static_cast< value_t >( x * rhs ),
          static_cast< value_t >( y * rhs )
      };         
   }    

   constexpr bool operator==( const xy & rhs ) const {
      return ( x == rhs.x ) && ( y == rhs.y );
   }

   constexpr bool operator!=( const xy & rhs ) const {
      return ! ( *this == rhs );
   }

}; 

template< is_output_stream T, typename V >
T & operator<<( T & lhs, xy< V > rhs ){
   return lhs << '(' << rhs.x << ',' << rhs.y << ')';
}


// ==========================================================================
//
// iterator
//
// ==========================================================================

template< typename T >
class xy_iterator_t {
private:

   xy< T > limits;
   xy< T > current;

public:

   xy_iterator_t( xy< T > limits, xy< T > current ): 
      limits( limits ), 
      current( current )
   {}

   xy< T > operator*() const {
      return current;
   }

   void operator++(){
      ++current.x;
      if( current.x == limits.x ){
         current.x = 0;
         ++current.y;
      }
   }

   bool operator==( const xy_iterator_t rhs ) const {
      return current == rhs.current;
   }

   bool operator!=( const xy_iterator_t rhs ) const {
      return current != rhs.current;
   }
      
};

template< typename T >
class xy_all_t {
private:

   xy< T > limits;

public:

   xy_all_t( xy< T > limits ):
      limits( limits )
   {}

   xy_iterator_t< T > begin() const {
      return xy_iterator_t( limits, xy< T >( 0 , 0 ) );
   }

   xy_iterator_t< T > end() const {
      return xy_iterator_t( limits, xy< T >( 0, limits.y ) );
   }

};

template< typename limits_t >
limits_t random_xy( limits_t limits ){
   return limits_t( 
      random_in_range< typename limits_t::value_t >( 0, limits.x ),
      random_in_range< typename limits_t::value_t >( 0, limits.y ) );
}  

