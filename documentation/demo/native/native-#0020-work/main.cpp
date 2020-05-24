template< int N, typename T >
struct _by_const { using type = const T; };

template< typename T >
struct _by_const< 1 + sizeof( int * ), T > { using type = const T &; };

template< typename T >
using gf_const = _by_const< sizeof( T ), T >::type;

/*
// when no exact match: pass as a 1 byte smaller data type would be passed
template< int N >
using by_const_determined_by_size = by_const_determined_by_size< N - 1 >;

// when larger than a pointer: pass by reference
template<>
using by_const_determined_by_size< 1 + sizeof( int * ) > = const & T;

// when same size as a pointer or smaller: pass by value
template<>
using by_const_determined_by_size< 0 > = const T;
*/

struct s {
   int x;
};

void f( gf_const< s > x ){
   (void)x;
}

int main( void ){
   f( s() );
}
