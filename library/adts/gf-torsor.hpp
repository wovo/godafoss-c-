// =============================================================================
//
// gf-torsor.hpp
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

// =============================================================================
//
// @title torsor<>
// @define godafoss::torsor
// @insert torsor
//
// The torsor<> template expresses and enforces the difference between
// relative and absolute (anchored) values.
// Much like a compile-time unit system like boost::units,
// torsor<> uses the type system
// to eliminate erroneous operations at compile-time.
// It also helps to make interfaces simpler and more elegant
// by making the difference between relative and absolute
// values explicit in the type system.
//
// For a value type that denotes a ratio scale value (a value
// for which addition yields a value on the same scale), the
// torsor of that (base) type is the corresponding interval scale
// (anchored) type.
//
// The canonical example of a base and its torsor is
// @italic distance
// (in the vector sense), and its torsor,
// @italic location.
//
// Distances can be added or subtracted, which yields a distance.
// Locations can't be meaningfully added, but adding a location
// and a distance is meaningfull and yields a distance.
// Two locations can be subtracted, yielding a distance.
//
// Whether a scale is a torsor or not has nothing to do with its
// unit: in a unit system (like SI) a basic (ration)
// type and its torsor have the same unit.
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


// @quote torsor 13 ... \\n };
template<
   typename    _data_type,
   _data_type  _zero
>
class torsor final {
public:

   // the type this torsor stores
   using data_type = _data_type;

   // the base (zero) value of this torsor
   static constexpr data_type zero = _zero;

private:

   // torsors can access each others private parts
   template< typename other_type, other_type other_zero >
   friend class torsor;

   // the stored base type value
   data_type stored_value = zero;

   // create a (possibly non-zero) torsor
   __attribute__(( always_inline ))
   constexpr explicit torsor( const data_type & value ):
      stored_value( zero + value )
   {}

   // retrieve the torsor value
   __attribute__(( always_inline ))
   constexpr data_type value() const {
      return stored_value - zero;
   }

   // update the torsor value
   __attribute__(( always_inline ))
   void set( const data_type & new_value ){
      stored_value = zero + new_value;
   }


   // ==========================================================================
   //
   // constructor and assignment
   //
   // ==========================================================================

public:

   // Create a torsor with the zero (anchor) value.
   GODAFOSS_INLINE
   constexpr torsor(){}

   // Create a torsor from another torsor,
   // which must have a compatible base type.
   template< typename other_type, other_type other_zero >
   requires requires( other_type v ){
      torsor( v );
   }
   GODAFOSS_INLINE
   constexpr torsor( const torsor< other_type, other_zero > & right ):
      stored_value( right.value() )
   {}

   // Assign (copy) the value from another torsor,
   // which must have a compatible base type.
   template< typename other_type, other_type other_zero >
   requires requires( torsor t, other_type v ){
      t.set( v );
   }
   GODAFOSS_INLINE
   torsor & operator=( const torsor< other_type, other_zero > & right ){
      this->set( right.value() );
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
      this->set( + v );
   }
   GODAFOSS_INLINE
   {
      return
         torsor< decltype( + this->value() ), _zero >(
            + this->value() );
   }

   // Add a value to ourself.
   // The base types of our torsor and the value must be addable.
   // The result is a torsor of the type
   // and with the value of that addition.
   template< typename other_type >
   requires requires( torsor t, data_type v ) {
      t.set( t.value() + v );
   }
   GODAFOSS_INLINE
   constexpr auto operator+( const other_type & right ) const {
      return
         torsor< decltype( this->value() + right ), this->zero >(
            this->value() + right );
   }

   // Add a value into ourself.
   // The base types of our torsor and the value
   // must be update addable.
   // The result is a ourself, updated appropriately.
   template< typename other_type >
   requires requires( data_type v ) {
      set( value() + v );
   }
   GODAFOSS_INLINE
   torsor & operator+=( const other_type & right ){
      this->set( this->value() + right );
      return *this;
   }


   // ==========================================================================
   //
   // subtract
   //
   // ==========================================================================

public:

   // The unary minus operator returns the - of its torsor value
    constexpr auto operator+() const
   requires requires( data_type v ) {
      this->set( - v );
   }
   GODAFOSS_INLINE
   {
      return
         torsor< decltype( - this->value() ), _zero >(
            + this->value() );
   }

   // Subtract a value from ourself.
   // The base types of our torsor and the value must be subtractable.
   // The result is a torsor of the type
   // and with the value of that subtraction.
   template< typename other_type >
   requires requires( torsor< _data_type, _zero> t, other_type v ) {
      t.set( t.value() - v );
   }
   GODAFOSS_INLINE
   constexpr auto operator-( const other_type & right ) const {
      return
         torsor< decltype( this->value() - right ), this->zero >(
            this->value() - right );
   }

   // Subtract a torsor from ourself.
   // The base types of our torsor and the other torsor
   // must be subtractable.
   // The result is of the type and has the value of that subtraction.
   template< typename other_type, other_type other_zero >
   requires requires(
      const torsor left,
      const torsor< other_type, other_zero > right
   ){
      left.value() - right.value();
   }
   constexpr auto operator-(
      const torsor< other_type, other_zero > & right
   ) const {
      return this->value() - right.value();
   }

   // Subtract a value into ourself.
   // The base types of our torsor and the value
   // must be update subtractable.
   // The result is a ourself, updated appropriately.
   template< typename other_type >
   requires requires( data_type v ) {
      set( value() - v );
   }
   GODAFOSS_INLINE
   torsor & operator-=( const other_type & right ){
      this->set( this->value() - right );
      return *this;
   }


   // ==========================================================================
   //
   // compare equal
   //
   // ==========================================================================

   // Compare two torsors for equality.
   // The base types of the torsors must support comparing for equality.
   // The result is te result of that comparison.
   template< typename other_type, other_type other_zero >
   requires requires(
      const torsor left,
      const torsor< other_type, other_zero > right
   ){
      left.value == right.value();
   }
   GODAFOSS_INLINE
   constexpr auto operator==(
      const torsor< other_type, other_zero > & right
   ) const {
      return this->value == right.value;
   }

   // compare torsors for inequality
   //
   // Compare two torsors for inequality.
   // The base types of te torsors must support comparing for inequality.
   // The result is te result of that comparison.
   template< typename other_type, other_type other_zero >
   requires requires(
      const torsor left,
      const torsor< other_type, other_zero > right
   ){
      left.value != right.value();
   }
   GODAFOSS_INLINE
   constexpr auto operator==(
      const torsor< other_type, other_zero > & right
   ) const {
      return this->value != right.value;
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
   template< typename other_type, other_type other_zero >
   requires requires(
      const torsor left,
      const torsor< other_type, other_zero > right
   ){
      left.value == right.value();
   }
   GODAFOSS_INLINE
   constexpr auto operator>(
      const torsor< other_type, other_zero > & right
   ) const {
      return this->value > right.value;
   }

   // compare torsors for larger or equal
   //
   // Compares a torsor for being larger than or equal to another torsor.
   // The base types of te torsors must support the comparison.
   // The result is te result of that comparison.
   template< typename other_type, other_type other_zero >
   requires requires(
      const torsor left,
      const torsor< other_type, other_zero > right
   ){
      left.value == right.value();
   }
   GODAFOSS_INLINE
   constexpr auto operator>=(
      const torsor< other_type, other_zero > & right
   ) const {
      return this->value >= right.value;
   }

/*
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
   GODAFOSS_INLINE
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
   GODAFOSS_INLINE
   constexpr auto operator<=( const torsor< U, M > & right ) const {
      return value <= right.value;
   }

   // ==========================================================================
   //
   // reverse arithmetic
   //
   // ==========================================================================

   // stopgap because friend doesn't seem to work properly for operator+
   GODAFOSS_INLINE
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
   GODAFOSS_INLINE
   friend constexpr auto operator+( const U & left, const torsor & right ){
      return torsor< decltype( left + right.value ), M >(
        left + right.value, 42 );
   }
*/

}; // template class torsor


// =============================================================================
//
// print a torsor to a cout-like object
//
// The torsor value is printed, preceded by a '@'
// character.
// The left argument must support printing (using operator<<)
// of a char and of a base type value.
//
// =============================================================================

/*
template< typename stream, typename T, typename M >
requires torsor_concepts::can_be_printed_to< ostream, T >
stream & operator<<( stream & cout, const torsor< T, M > & right ){
   cout << '@';
   cout << ( (T)( right - torsor< T, M >() ) );
   return cout;
}
*/
