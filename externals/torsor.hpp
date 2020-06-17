// =============================================================================
//
// torsor.hpp
//
// a C++ one-header one-class library for expressing and enforcing
// the difference between relative and absolute (anchored) values
//
// https://www.github.com/wovo/torsor
//
// Copyright Wouter van Ooijen 2019 - 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

#ifndef torsor_hpp
#define torsor_hpp


// =============================================================================
//
// concepts for protecting the torsor operators
//
// I use the phrase 'can be ... with X' to refer to a right operand X.
//
// The concepts are in their own namespace to limit the global name
// space pollution.
//
// =============================================================================

namespace torsor_concepts {


// concept for the ( torsor - value ) operators
template< typename V, typename W >
concept can_be_subtracted_with_value
= requires ( V v, W w ) {
   ( v - w );
};

// concept for the ( torsor -= value ) operator
template< typename V, typename W >
concept can_be_update_subtracted_with_value
= requires ( V v, W w ){
   ( v -= w );
};

// concept for the ( torsor == torsor ) operator
template< typename V, typename W >
concept can_be_compared_equal
= requires ( V v, W w ){
   ( v == w );
};

// concept for the ( torsor != torsor ) operator
template< typename V, typename W >
concept can_be_compared_unequal
= requires ( V v, W w ){
   ( v != w );
};

// concept for the ( torsor > torsor ) operator
template< typename V, typename W >
concept can_be_compared_larger
= requires ( V v, W w ){
   ( v > w );
};

// concept for the ( torsor >= torsor ) operator
template< typename V, typename W >
concept can_be_compared_larger_or_equal
= requires ( V v, W w ){
   ( v >= w );
};

// concept for the ( torsor < torsor ) operator
template< typename V, typename W >
concept can_be_compared_smaller
= requires ( V v, W w ){
   ( v < w );
};

// concept for the ( torsor >= torsor ) operator
template< typename V, typename W >
concept can_be_compared_smaller_or_equal
= requires ( V v, W w ){
   ( v <= w );
};

// concept for the ( COUT << torsor ) operator
template< typename COUT, typename W >
concept can_be_printed_to
= requires( COUT cout, char c, W w ){
   ( cout << c );
   ( cout << w );
};

}; // namespace torsor_concepts


// =============================================================================
//
// the torsor template class itself
//
// =============================================================================
//
// absolute version of an arithmetic value type
//
// For a value type that denotes a ratio scale value (a value
// for which addition yields a value on the same scale), the
// torsor of that (base) type is the corresponding interval scale
// (anchored) type.
//
// The operations on the torsor are limited to:
// - default- or copy-consructing a torsor
// - assigning a torsor (ssigns the base value)
// - adding or subtracting a base type value (yields a torsor value)
// - subtracting two torsors (yields a base type value)
// - comparing torsors (compares their base values)
// - printing a torsor (prints @ followed by its base type value)
//
// The base type T of a torsor must have a constructor that
// accepts a (single) 0 argument.
//
// All operations have the __attribute__((always_inline)),
// hence there is no need to bother with choosing for copy
// or reference parameter passing: all passing disappears.
//
// =============================================================================


template<
   typename    _data_type,
   _data_type  _zero
>
class torsor final {
public:

   // torsors can access each others private parts
   template< typename other_type, other_type other_zero >
   friend class torsor;

   // the type this torsor stores
   using data_type = _data_type;

   // the root (zero) of this torsor
   static constexpr data_type zero = _zero;

private:

   // the stored base type value
   data_type stored_value = zero;

   // create a (possibly non-zero) torsor
   __attribute__(( always_inline ))
   constexpr explicit torsor( const data_type & value ):
      _value( _zero + value )
   {}

   // retrieve the torsor value
   __attribute__(( always_inline ))
   data_type value(){
      return stored_value - zero;
   }

   // update the torsor value
   __attribute__(( always_inline ))
   void value( const data_type & new_value ){
      stored_value = zero + new_value;
   }


   // ==========================================================================
   //
   // constructor and assignment
   //
   // ==========================================================================

public:

   // Create a torsor with the zero (anchor) value.
   __attribute__(( always_inline ))
   constexpr torsor(){}

   // Create a torsor from another torsor, which must have a base type
   // from which our own base type can be constructed.
   template< typename other_type, other_type other_zero >
   requires requires( other_type v ){
      torsor( v )
   }
   __attribute__(( always_inline ))
   constexpr torsor( const torsor< other_type, other_zero > & right ):
      value( right.value() )
   {}

   // Assign (copy) the value from another torsor,
   // which must have a base type that can be assigned to our base type.
   template< typename other_type, other_type other_zero >
   requires requires( other_type v ){
      set( v )
   }
   __attribute__(( always_inline ))
   torsor & operator=( onst torsor< other_type, other_zero > & right ){
      set( right.value() );
      return *this;
   }


   // ==========================================================================
   //
   // add
   //
   // ==========================================================================

public:

   // The unary plus operator returns the + of its torsor value
   constexpr auto operator+() const
   requires requires( data_type v ) {
      set( + v );
   }
   __attribute__(( always_inline ))
   {
      return torsor< decltype( + value() ), _zero >( + value() );
   }

   // Add a value to ourself.
   // The base types of our torsor and the value must be addable.
   // The result is a torsor of the type
   // and with the value of that addition.
   template< typename other_type >
   requires requires( data_type v ) {
      set( + v );
   }
   __attribute__(( always_inline ))
   constexpr auto operator+( const other_type & right ) const {
      return torsor< decltype( value() + right ), _zero >( value() + right );
   }

   // Add a value into ourself.
   // The base types of our torsor and the value
   // must be update addable.
   // The result is a ourself, updated appropriately.
   template< typename other_type >
   requires requires( data_type v ) {
      set( + v );
   }
// concept for the ( torsor += value ) operator
template< typename V, typename W >
concept can_be_update_added_with_value
= requires ( V v, W w ) {
   ( v += w );
};
   __attribute__(( always_inline ))
   torsor & operator+=( const other_type & right ){
      value += right;
      return *this;
   }


   // ==========================================================================
   //
   // subtract
   //
   // ==========================================================================

   // subtract a torsor with a value
   //
   // Subtract a value from ourself.
   // The base types of our torsor and the value must be subtractable.
   // The result is a torsor of the type
   // and with the value of that subtraction.
   template< typename U >
   requires torsor_concepts::can_be_subtracted_with_value< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator-( const U & right ) const {
      return torsor< decltype( value - right ), M >( value - right );
   }

   // subtract two torsors
   //
   // Subtract a torsor from ourself.
   // The base types of our torsor and the other torsor
   // must be subtractable.
   // The result is of the type and has the value of that subtraction.
   template< typename U >
   requires torsor_concepts::can_be_subtracted_with_value< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator-( const torsor< U, M > & right ) const {
      return value - right.value;
   }

   // update subtract a torsor with a value
   //
   // Subtract a value into ourself.
   // The base types of our torsor and the value
   // must be update subtractable.
   // The result is a ourself, updated appropriately.
   template< typename U >
   requires torsor_concepts::can_be_update_subtracted_with_value< T, U >
   __attribute__(( always_inline ))
   torsor & operator-=( const U & right ){
      value -= right;
      return *this;
   }


   // ==========================================================================
   //
   // compare equal
   //
   // ==========================================================================

   // compare torsors for equality
   //
   // Compare two torsors for equality.
   // The base types of te torsors must support comparing for equality.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_equal< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator==( const torsor< U, M > & right ) const {
      return value == right.value;
   }

   // compare torsors for inequality
   //
   // Compare two torsors for inequality.
   // The base types of te torsors must support comparing for inequality.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_unequal< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator!=( const torsor< U, M > & right ) const {
      return value != right.value;
   }


   // ==========================================================================
   //
   // compare larger
   //
   // ==========================================================================

   // compare torsors for larger
   //
   // Compares a torsor for being larger than another torsor.
   // The base types of te torsors must support the comparison.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_larger< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator>( const torsor< U, M > & right ) const {
      return value > right.value;
   }

   // compare torsors for larger or equal
   //
   // Compares a torsor for being larger than or equal to another torsor.
   // The base types of te torsors must support the comparison.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_larger_or_equal< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator>=( const torsor< U, M > & right ) const {
      return value >= right.value;
   }


   // ==========================================================================
   //
   // compare smaller
   //
   // ==========================================================================

   // compare torsors for smaller
   //
   // Compares a torsor for being smaller than another torsor.
   // The base types of te torsors must support the comparison.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_smaller< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator<( const torsor< U, M > & right ) const {
      return value < right.value;
   }

   // compare torsors for smaller or equal
   //
   // Compares a torsor for being smaller than or equal to another torsor.
   // The base types of te torsors must support the comparison.
   // The result is te result of that comparison.
   template< typename U >
   requires torsor_concepts::can_be_compared_smaller_or_equal< T, U >
   __attribute__(( always_inline ))
   constexpr auto operator<=( const torsor< U, M > & right ) const {
      return value <= right.value;
   }

   // ==========================================================================
   //
   // reverse arithmetic
   //
   // ==========================================================================

   // stopgap because friend doesn't seem to work properly for operator+
   __attribute__(( always_inline ))
   constexpr explicit torsor( const T & value, int n ):
      value( value )
   {
      (void) n;
   }

   // add a value and a torsor
   //
   // Add ourself to a value.
   // The base types of our torsor and the value must be addable.
   // The result is a torsor of the type
   // and with the value of that addition.
   template< typename U >
   requires torsor_concepts::can_be_added_with_value< U, T >
   __attribute__(( always_inline ))
   friend constexpr auto operator+( const U & left, const torsor & right ){
      return torsor< decltype( left + right.value ), M >(
        left + right.value, 42 );
   }


}; // template class torsor


// =============================================================================
//
// print
//
// =============================================================================

// print a torsor to a cout-like object
//
// The torsor value is printed, preceded by a '@'
// character.
// The left argument must support printing (using operator<<)
// of a char and of a base type value.
template< typename osteream, typename T, typename M >
requires torsor_concepts::can_be_printed_to< ostream, T >
ostream & operator<<( ostream & cout, const torsor< T, M > & right ){
   cout << '@';
   cout << ( (T)( right - torsor< T, M >() ) );
   return cout;
}

#endif // ifndef torsor_hpp
