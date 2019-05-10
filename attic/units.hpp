// requirements: 
// unit_type has + - * int / int == != monadic -?, also derfauklts to zero
// value_type has + - * /

// need a way to select value-or-naked based on unit_type::zero

// must + absolute remember the anchor value?? => no, it is in the value!

// milli, micro etc. are independent concepts, can be applied to naked values too (scaling)

template< 
   uint64_t number_of_bases
>
struct uint64_array {
   std:array< uint64_t,  number_of_classes > values;

   constexpr uint64_array(): values{ { 0 } }{}
   constexpr auto operator<=>( const uint64_array & ) const = default;
   constexpr auto operator+( const uint64_array & right ) const {
      
}
   


// ==========================================================================
//
// scaling
//
// ==========================================================================

template<
   typename        value_type, 
   std::ratio      factor
>
struct scaled_value {
   value_type value;

   scaled_value(){}
   scaled_value( value_type value ): value( value ){}
   auto operator<=>( const value_type & ) const = default;
};




// ==========================================================================
//
// the value type
//
// ==========================================================================

template< 
   typename        value_type, 
   dimension_type  dimension,
   std::intmax_t   anchor_count
>
struct united_value {

   value_type value;

   value(){}
   value( value_type value ): value( value ){}
   auto operator<=>( const value & ) const = default;

   static constexpr value zero = ...;
   static constexpr value one = ...;
   
};


// ==========================================================================
//
// v * v
//
// multiplication
//
// v<n,0,a> * v<m,0,b> -> v<n+m,0,a*b> | a*b
//
// The dimensions are added, the values are multiplied.
// The anchors must both be 0.
// When the dimension is zero, the result is a naked value.
//
// ==========================================================================

template<
   typename        left_value_type, 
   typename        right_value_type, 
   dimension_type  left_dimension,
   dimension_type  right_dimension,
>
auto operator*( 
   const value< left_value_type,  left_dimension,   0 > & left,
   const value< right_value_type, rigtht_dimension, 0 > & right
){
   return select_type<
      ( left_dimension + right_dimension ) == dimension_type(),
      decltype( left.value + right.value ), 
      value< 
         decltype( left.value * right.value ), 
         left_dimension + right_dimension,
         0
      >
    >( left.value * right.value );
}


// ==========================================================================
//
// v * a
// a * v
//
// multiplication with a naked value
//
// v<n,0,a> * b -> v<n,0,a*b>
// a * v<n,0,b> -> v<n,0,a*b>
//
// The dimension is preserved, the anchor must be zero, the value
// is multiplied.
//
// ==========================================================================

template<
   typename        left_value_type, 
   typename        right_value_type,
   dimension_type  dimension,
   int64_t         anchors
>
auto operator*( 
   const value< value_type, dimension, 0 > & left,
   const right_value_type & right
){
   return value< 
      decltype( left.value * right >, 
      dimension,
      0
    >( left.value * right );
}

template<
   typename        left_value_type, 
   typename        right_value_type,
   dimension_type  dimension,
   int64_t         anchors
>
auto operator*( 
   const right_value_type & left
   const value< value_type, dimension, 0 > & right,
){
   return value< 
      decltype( left * right.value >, 
      dimension,
      0
    >( left * right.value );
}


// ==========================================================================
//
// v + v
//
// addition
//
// v<n,x,a> + v<n,y,b> -> v<n,x+y,a+b>
//
// The dimensions must be the same, the anchors and values are added.
//
// ==========================================================================

template<
   typename        left_value_type, 
   typename        right_value_type, 
   dimension_type  dimension,
   int64_t         left_anchors,
   int64_t         right_anchors
>
auto operator+( 
   const value< left_value_type,  dimension, left_anchors > & left,
   const value< right_value_type, dimension, righ_anchors > & right
){
   return value<
      decltype( left.value + right.value ), 
      dimension, 
      left_anchors + right_anchors
   >( left.value + right.value );
}


// ==========================================================================
//
// - v
//
// negation
//
// -v<n,x> -> v<-n,-x>
//
// The dimension is preserved, the anchors and values are negated.
//
// ==========================================================================

template<
   typename        value_type, 
   dimension_type  dimension,
   int64_t         anchors
>
auto operator-( 
   const value< value_type,  dimension, anchors > & arg,
){
   return value< value_type, dimension, - anchors >( - arg.value );
}


// ==========================================================================
//
// + v 
//
// identity
//
// + a == a 
//
// ==========================================================================

template<
   typename        value_type, 
   dimension_type  dimension,
   int64_t         anchors
>
auto operator+( 
   const value< value_type,  dimension, anchors > & arg,
){
   return arg;
}


// ==========================================================================
//
// dyadic subtraction
//
// a - b == a + ( - b )
//
// ==========================================================================

template<
   typename        left_value_type, 
   typename        right_value_type, 
   dimension_type  dimension,
   int64_t         left_anchors,
   int64_t         right_anchors
>
auto operator-( 
   const value< left_value_type,  dimension, left_anchors > & left,
   const value< right_value_type, dimension, righ_anchors > & right
){
   return left + ( - right );
}


// +=, -=

