// ==========================================================================
//
// gf-xy.hpp
//
// ==========================================================================
//
// The xy< T > ADT is a pair of two T values named x and y.
// It is used for distances in an xy plane, like on a window or terminal.
//
// The xy< torsor< T > > is used to identify a location on an xy plane.
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
// xy<> ADT
//
// ==========================================================================

template< typename T = int_fast64_t, T zero = 0 >
   requires requires( T & x, T & v ){
      // default constructor
      // copy constructor
      { v * v };
      { v / v };
      { v == v };
      { x = v };
   }
class xy final {
public:

   using value_t = T;

   value_t x, y;

   constexpr xy( value_t x, value_t y ): x{ x }, y{ y }{}

   // constexpr xy( value_t x ): x{ x }, y{ x }{}

   constexpr xy():x{ zero }, y{ zero }{}

   template< typename X >
   constexpr xy( const xy< X > & rhs ): x( rhs.x ), y( rhs.y ) {}

   static constexpr auto origin = xy{};

   template< typename V >
//      requires requires( V b ){ { x + b }; }   - GCC 10.0.1 ICE segfault
      requires requires( T x, V b ){ { x + b }; }
   constexpr auto operator+( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x + rhs.x ),
          static_cast< value_t >( y + rhs.y )
      };
   }

   template< typename V >
      requires requires( T x, V b ){ { x - b }; }
   constexpr auto operator-( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x - rhs.x ),
          static_cast< value_t >( y - rhs.y )
      };
   }

   constexpr auto operator/( const value_t rhs ) const {
      return xy {
          static_cast< value_t >( x / rhs ),
          static_cast< value_t >( y / rhs )
      };
   }

   constexpr xy operator*( const value_t rhs ) const {
      return xy {
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

template< typename stream, typename value >
   requires requires( stream & s, char c, value v ){
      { s << 'c' } -> std::same_as< stream & >;
      { s << v   } -> std::same_as< stream & >;
   }
stream & operator<<( stream & lhs, xy< value > rhs ){
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

   xy< T > current;
   xy< T > start;
   xy< T > limit;

public:

   xy_iterator_t( xy< T > start, xy< T > limit ):
      current( start ),
      start( start ),
      limit( limit )
   {}

   xy< T > operator*() const {
      return current;
   }

   void operator++(){
      ++current.x;
      if( current.x == limit.x ){
         current.x = start.x;
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


// ==========================================================================
//
// ranges
//
// ==========================================================================

template< typename T, T v >
   requires requires( T & x ){
      { ++x };
   }
class range {
private:

   xy< T, v > start;
   xy< T, v > limit;

public:

/*
r r -> r
a a -> a
a r -> a
(0) a -> a
(0) r -> r
*/

   range( xy< T, v > limit ):
      start( limit.origin ), limit( limit )
   {}

   xy_iterator_t< T > begin() const {
      return xy_iterator_t( start, limit );
   }

   xy_iterator_t< T > end() const {
      return xy_iterator_t( xy< T >( 0, limit.y ), limit );
   }

};


// ==========================================================================
//
// random
//
// ==========================================================================

template< typename limits_t >
limits_t random_xy( limits_t limits ){
   return limits_t(
      random_in_range< typename limits_t::value_t >( 0, limits.x ),
      random_in_range< typename limits_t::value_t >( 0, limits.y ) );
}
