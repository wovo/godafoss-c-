// =============================================================================
//
// gf-fraction.hpp
//
// =============================================================================
//
// This file is part of godafoss,
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

// =============================================================================
//
// @define fraction<>
// @title fraction
//
// @insert fraction
//
// A fraction is a type template that stores a raw_value,
// which is to be interpreted as a fraction of its maximum value.
// Think of a fraction as a percentage,
// or a factor in the range [ 0.0 .. 1.0 ].
// The type of the stored raw_value and the maximum value
// are template parameters.
//
// A fractions can be used to avoid the use of floating point arithmetic
// in a situation where otherwise a floating point
// value (in the range [ 0.0 .. 1.0 ]) would have been used.
//
// All fraction operartions are constexpr. Fraction values
// can be used as non-type template arguments.
//
// Examples of the use of fractions in the library:
//   - an ADC (Analaog to Digital Converter) returns a fraction
//   - the amount of red, green and blue in a color is expressed as a fraction
//   - the position of a servo motor is specified as a fraction
//
// -----------------------------------------------------------------------------
//
// @section constructors
// @insert default-constructor
// @insert value-constructor
//
// The constructor that acceptes a value is explicit,
// to avoid the mistake of passing a value
// where a fraction is required.
//
// @insert copy-constructor
//
// The copy constructor copies the value from another fraction,
// which can have a different value type and/or a different maximum.
// The copying rescales relative to the maximum of the constructed fraction.
//
// @code 4
// // example
// fraction< 10 > a( 5 );
// fraction< 4 > b( a );
// // now b == fraction< 4 >( 2 )
//
// -----------------------------------------------------------------------------
//
// @section assignment
// @insert assignment
//
// A fraction can be assigned from another fraction,
// which can have a different value type and/or a different maximum.
// As with the copy constructor, assignement rescales the value.
//
// @code 4
// // example
// fraction< 10 > a;
// a = fraction< 2 >( 1 );
// // now a == fraction< 10 >( 5 );
//
// -----------------------------------------------------------------------------
//
// @section of()
// @insert of-one
// @insert of-two
//
// The of() functions return the argument, scaled according to the fraction.
// The one-argument version scales to the interval [ 0, max ],
// the two-argument version scales to the interval [ min, max ].
//
// @code 3
// // examples
// fraction< 3 >( 1 ).of( 60 ) == 20
// fraction< 8 >( 3 ).of( 10, 90 ) == 40
//
// -----------------------------------------------------------------------------
//
// @section negate
// @insert negate
//
// The - operator returns the complements of the fraction:
// when the fraction is interpreted as a value v in the range [ 0.0 .. 1.0 ],
// it returns the fraction ( 1.0 - v ).
//
// @code 3
// // examples
// fraction< 3 >( 1 ) == fraction< 3 >( 2 )
// fraction< 8 >( 3 ) == fraction< 8 >( 5 )
//
// -----------------------------------------------------------------------------
//
// @section multiply
// @insert multiply
// @insert multiply-reverse
//
// The multiplication operators multiply the fraction by the other parameter.
//
// @code 3
// // examplew
// fraction< 6 >( 2 ) * 2 == fraction< 6 >( 4 )
// 3 * fraction< 6 >( 2 ) == fraction< 6 >( 6 )
//
// -----------------------------------------------------------------------------
//
// @section divide
// @insert divide
//
// The division operator divides the fraction by the right hand side argument.
//
// @code 2
// // example
// fraction< 10 >( 6 ) / 3  == fraction< 10 >( 2 )
//
// -----------------------------------------------------------------------------
//
// @section compare
// @insert compare-equal
// @insert compare-unequal
//
// Fractions can be compared for equality and inequality.
// These comparisons take the maximum values of both sides into account.
//
// @code 3
// // examples
// fraction< 10 >( 3 ) != fraction< 5 >( 3 )
// fraction< 10 >( 6 ) == fraction< 5 >( 3 )
//
// -----------------------------------------------------------------------------
//
// @section print
// @insert print
//
// A fraction can be printed in the "f[R/M]" format, where R is the
// raw_value, and M is the maximum.
//
// =============================================================================


// @quote fraction 10 };
template< typename T, T _maximum >
struct fraction {

   using data_type = T;

   static constexpr data_type maximum = _maximum;

   data_type raw_value = _maximum;


   // @quote default-constructor 1
   constexpr fraction(){}

   // @quote value-constructor 1 ...
   constexpr explicit fraction( T x )
      : raw_value( x )
   {}

   // @quote copy-constructor 1 ...
   template< typename V, V rhs_maximum >
   constexpr explicit fraction( const fraction< V, rhs_maximum > & rhs )
      : raw_value( ( rhs.raw_value * rhs_maximum ) / maximum )
   {}

   // @quote assignment 2 ... }
   template< typename V, V rhs_maximum >
   constexpr fraction & operator=( const fraction< V, rhs_maximum > & rhs ){
      raw_value = ( rhs.raw_value * rhs_maximum ) / maximum;
   }

   // @quote of-one 2 ... }
   template< typename V >
   constexpr V of( V max ) const {
      return ( raw_value * max ) / maximum;
   }

   // @quote of-two 2 ... }
   template< typename V, typename W >
   constexpr V of( V min, W max ) const {
      return min + of( max - min );
   }

   // @quote negate 1 ... }
   constexpr fraction operator - () const {
      return fraction( maximum - raw_value );
   }

   // @quote multiply 2 ... }
   template< typename V >
   constexpr fraction operator * ( V rhs ) const {
      return fraction( raw_value * rhs );
   }

   // @quote multiply-reverse 2 ... }
   template< typename V >
   friend constexpr fraction operator * ( V lhs, fraction rhs ) {
      return rhs * lhs;
   }

   // @quote divide 2 ... }
   template< typename V >
   constexpr fraction operator / ( V rhs ) const {
      return fraction( raw_value / rhs );
   }

   // @quote compare-equal 2 ... }
   template< typename V >
   constexpr bool operator == ( V rhs ) const {
      return raw_value * V::maximum == rhs.raw_value * maximum;
   }

   // @quote compare-unequal 2 ... }
   template< typename V >
   constexpr bool operator != ( V rhs ) const {
      return ! ( *this == rhs );
   }

   // @quote print 2 ... }
   template< typename S >
   friend S & operator << ( S & sink, const fraction & v ){
      return sink << "f[" << v.raw_value << "/" << v.maximum << "]";
   }

};

