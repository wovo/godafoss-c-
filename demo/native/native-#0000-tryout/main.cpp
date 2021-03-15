/*
 * template< typename T = int_fast64_t, T zero = 0 >
class xy final { };

template< typename T, T v >
class range {
   xy< T, v > start;
   xy< T, v > limit;
public:
   range( xy< T, v > limit ):
      start( limit::zero, limit( limit )  // wovo missing ) after zero
   {}
};

template<>
struct box_root< bool > : _box_root< bool > {};

template< typename T >
struct _no_inline_box_init : T {};

template< typename T >
struct _no_inline_box_write : T {};

template< typename T >
struct _no_inline_box_read : T {};

template< typename T >
struct _no_inline_box_direction : T {};

template< typename T >
using no_inline =
   _no_inline_box_init<
   _no_inline_box_write<
   _no_inline_box_read<
   _no_inline_box_direction< T > > > >;
*/

int main(){}
