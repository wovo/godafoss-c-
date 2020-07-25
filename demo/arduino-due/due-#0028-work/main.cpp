/*
#include <concepts>

template< typename T >
concept can_pin_in =
      is_pin_in< T >
   || is_pin_oc< T >;

template< can_pin_in T >
struct pin_in {};



// =========================

struct a { };

struct b { };

template< typename T >
concept is_pin_in = requires {
   { T() } -> std::same_as< a >;
};

template< typename T >
concept is_pin_oc = requires {
   { T() } -> std::same_as< b >;
};

template< is_pin_in T >
struct pin_in_a {};

// ====================================

template< is_a T  >
using pin_in< T > = pin_in_a< T >;

// ====================================


struct c { };

int main( void ){

   pin_in< a >();
   pin_in< b >();
   pin_in< c >();

}
&*/
int main(){}
