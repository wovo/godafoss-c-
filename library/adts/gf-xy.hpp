// =============================================================================
//
// gf-xy.hpp
//
// =============================================================================
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
// =============================================================================
//
// @define godafoss::xy
// @title xy<>
//
// @insert struct
//
// The xy< xy_value_type > ADT class template is a pair of t
// wo xy_value_type values named x and y.
// It is used for distances in an xy plane, like on a window or terminal.
// For a location (carthesian coordinate) in an xy plane
// the torsor< xy< T > > is used.
//
// The xy<> ADT supports
// - constructors: default (initializes to zero),
// from x and y values, copy (from another xy<>)
// - direct acces to the x and y values
// - an origin (zero) constant
// - operators on two xy<>'s: - + == !=
// - operators on an xy<> and a scalar: * /
//
// @section attributes
//
// @insert xy
// The x and y values are freely acessible.
//
// @insert origin
//
// The origin is the (0,0) value.
//
// @section methods
//
// @insert default-constructor
//
// The default constructor intializes a and y to the zero value.
//
// @insert constructor
//
// The two-value constructor initializes the x and y from the supplied values.
//
// @insert copy-constructor
//
// An xy<> object can be constructed from an xy with the same or
// a different value type.
//
// @insert add
// @insert subtract
//
// Two xy<> values can be added to or subtracted provided that
// their xy_value_types can be added or subtracted.
// The resulting xy<> gets the xy_value_type of that addition
// or subtraction.
//
// @insert mul-div
//
// An xy<> can be multiplied or divided by a value, provided
// an xy_value_can be constructed from it.
// The result is an xy<> value of the same xy<>_value_type.
//
// @insert equal
// @insert unequal
//
// An xy<> can be compared to another xy<> for equality or inequality,
// provided that their xy_value_types can be compared.
//
// @section non-member functions
//
// @insert operator<<
//
// =============================================================================


// =============================================================================
//
// xy<> ADT
//
// =============================================================================

template< typename T >
concept xy_value_type = requires(
   T v,
   T & x
){
   // default constructor
   // copy constructor
   { v * v };
   { v / v };
   { v == v };
   { x = v };
};

// @quote struct 4 ... };
template<
   typename xy_value_type = int64_t,
   xy_value_type zero = 0 >
struct xy final {
public:

   // @quote xy 2
   using value_t = xy_value_type;
   value_t x, y;

   // @quote default-constructor 1
   constexpr xy():x{ zero }, y{ zero }{}

   // @quote constructor 1
   constexpr xy( value_t x, value_t y ): x{ x }, y{ y }{}

   // @quote copy-constructor 2
   template< typename X >
   constexpr xy( const xy< X > & rhs ): x( rhs.x ), y( rhs.y ) {}

   // @quote origin 1
   static constexpr auto origin = xy{};

   // @quote add 3 ... }
   template< typename V >
//      requires requires( V b ){ { x + b }; }   - GCC 10.0.1 ICE segfault
      requires requires( xy_value_type x, V b ){ { x + b }; }
   constexpr auto operator+( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x + rhs.x ),
          static_cast< value_t >( y + rhs.y )
      };
   }

   // @quote add 3 ... }
   template< typename V >
      requires requires( xy_value_type x, V b ){ { x += b }; }
   constexpr xy & operator+=( const xy< V > rhs ){
      x += rhs.x;
      y += rhs.y;
      return *this;
   }

   // @quote subtract 3 ... }
   template< typename V >
      requires requires( xy_value_type x, V b ){ { x - b }; }
   constexpr auto operator-( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x - rhs.x ),
          static_cast< value_t >( y - rhs.y )
      };
   }

   // @quote mul-div 1 ... }
   constexpr xy operator*( const value_t rhs ) const {
      return xy {
          static_cast< value_t >( x * rhs ),
          static_cast< value_t >( y * rhs )
      };
   }

   // @quote mul-div 1 ... }
   constexpr xy operator/( const value_t rhs ) const {
      return xy {
          static_cast< value_t >( x / rhs ),
          static_cast< value_t >( y / rhs )
      };
   }

   // @quote equal 4 ... }
   template< typename V >
      requires requires( xy_value_type a, V b ){
         { a == b } -> std::same_as< bool >; }
   constexpr bool operator==( const xy< V > & rhs ) const {
      return ( x == rhs.x ) && ( y == rhs.y );
   }

   // @quote unequal 4 ... }
   template< typename V >
      requires requires( xy_value_type lhs, V b ){
         { x == b } -> std::same_as< bool >; }
   constexpr bool operator!=( const xy & rhs ) const {
      return ! ( *this == rhs );
   }

};

// @quote operator<< 5
template< typename stream, typename value >
   requires requires( stream & s, char c, value v ){
      { s << 'c' } -> std::same_as< stream & >;
      { s << v   } -> std::same_as< stream & >;
   }
stream & operator<<( stream & lhs, xy< value > rhs ){
   return lhs << '(' << rhs.x << ',' << rhs.y << ')';
}


// =============================================================================
//
// iterator
//
// =============================================================================

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


// =============================================================================
//
// ranges
//
// =============================================================================

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


// =============================================================================
//
// random
//
// =============================================================================

template< typename limits_t >
limits_t random_xy( limits_t limits ){
   return limits_t(
      random_in_range< typename limits_t::value_t >( 0, limits.x ),
      random_in_range< typename limits_t::value_t >( 0, limits.y ) );
}
