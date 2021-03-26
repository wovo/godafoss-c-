#include <array>

template< int N >
constexpr auto immutable_string_decimal = 
   std::array< char, 1 >( '0' + N );

template< std::array strings >
struct component_name  { };

template< int n >
struct blink : component_name< immutable_string_decimal< 8 > > { };