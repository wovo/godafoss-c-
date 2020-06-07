// ==========================================================================
//
// gf-xy.hpp
//
// ==========================================================================
//
// The xy ADT is a pair of two T signed integer values named x and y
// of at least 16 bits. It is used as distances between locations 
// on a window or terminal.
//
// The torsor of xy is used to identify a location.
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

class xy final {
public:

   int_fast16_t x, y;
   
   constexpr xy( int_fast16_t x, int_fast16_t y ): x{ x }, y{ y }{}
   
   constexpr xy():x{ 0 }, y{ 0 }{}

   constexpr xy operator+( const xy rhs ) const {
      return xy{ 
          static_cast< int_fast16_t >( x + rhs.x ),
          static_cast< int_fast16_t >( y + rhs.y )
      };      
   }      
   
   constexpr xy operator-( const xy rhs ) const {
      return xy{ 
          static_cast< int_fast16_t >( x - rhs.x ),
          static_cast< int_fast16_t >( y - rhs.y )
      };         
   }      

   constexpr xy operator/( const int_fast16_t rhs ) const {
      return xy{ 
          static_cast< int_fast16_t >( x / rhs ),
          static_cast< int_fast16_t >( y / rhs )
      };         
   }    

   constexpr xy operator*( const int_fast16_t rhs ) const {
      return xy{ 
          static_cast< int_fast16_t >( x * rhs ),
          static_cast< int_fast16_t >( y * rhs )
      };         
   }    

   constexpr bool operator==( const xy & rhs ) const {
      return ( x == rhs.x ) && ( y == rhs.y );
   }

   constexpr bool operator!=( const xy & rhs ) const {
      return ! ( *this == rhs );
   }

}; 

template< is_output_stream T >
T & operator<<( T & lhs, xy rhs ){
   return lhs << '(' << rhs.x << ',' << rhs.y << ')';
}


// ==========================================================================
//
// iterator
//
// ==========================================================================

class xy_iterator_t {
private:

   xy limits;
   xy current;

public:

   xy_iterator_t( xy limits, xy current ): 
      limits( limits ), 
      current( current )
   {}

   xy operator*() const {
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

class xy_all_t {
private:

   xy limits;

public:

   xy_all_t( xy limits ):
      limits( limits )
   {}

   xy_iterator_t begin() const {
      return xy_iterator_t( limits, xy( 0 , 0 ) );
   }

   xy_iterator_t end() const {
      return xy_iterator_t( limits, xy( 0, limits.y ) );
   }

};

