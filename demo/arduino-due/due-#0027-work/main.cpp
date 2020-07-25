/*
#include <concepts>

// =========================

template< typename T >
struct pin_in_supported {};

template< typename T >
concept can_pin_in = requires {
   { pin_in_supported< T >::supported };
};

template< can_pin_in T >
struct pin_in;

// ====================================

struct a { };

template<>
struct pin_in_supported< a >{ static inline bool supported = true; };

template<>
struct pin_in< a > {};

//template<>
//struct pin_in< float > {};

struct b { };

int main( void ){

   pin_in< a >();
   pin_in< b >();

}
*/

int main(){}
