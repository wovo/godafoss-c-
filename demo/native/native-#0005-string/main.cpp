#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

#define TRACE { std::cout << __LINE__ << "\n"; }


// ===========================================================================
//
// immutable list 
//
// for compile-time use
//
// ===========================================================================

template< typename T, int N >
struct immutable_list: std::array< T, N > {

   template< int A, int B > 
   constexpr immutable_list( 
      const immutable_list< T, A > & a, 
      const immutable_list< T, B > & b  
   ){
      static_assert( N == A + B );
      for( unsigned int i = 0; i < A; ++i ){
          this->operator[]( i ) = a[ i ];
      }
      for( unsigned int i = 0; i < B; ++i ){
          this->operator[]( A + i ) = b[ i ];
      }
   }
   
   constexpr immutable_list( 
      const T a[ N ]  
   ){
      for( unsigned int i = 0; i < N; ++i ){
          this->operator[]( i ) = a[ i ];
      }
   }   

};

template< typename T >
struct immutable_list< T, 1 >: std::array< T, 1 > {
    
    constexpr immutable_list( const T & x ){
        this->operator[]( 0 ) = x;
    }
    
};

template< typename T >
struct immutable_list< T, 0 >: std::array< T, 0 > { };

template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    
    char value[N];
};

template< typename T, int A, int B >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, B > & b  
){
   return immutable_list< T, A + B >( a, b );
}   

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, 0 > & b  
){
   return a;
}

template< typename T, int B  >
constexpr auto operator+( 
   const immutable_list< T, 0 > & a, 
   const immutable_list< T, B > & b  
){
   return b;
}

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const T & x
){
   return a + immutable_list< T, 1 >( x );
}

constexpr StringLiteral(const char (&str)[N]) {


// ===========================================================================
//
// A resource is something a component needs to implement its functionality.
//
// ===========================================================================

template< auto s >
struct named {
   static void print(){
      for( auto c : s ) {
         std::cout << c;
      }   
      std::cout << "\n";
   }
};


// ===========================================================================

int xmain(){     
   //std::cout << resources::template name< app >();    
   named< immutable_list< char, 0 >() + 'y' + 'z' > :: print();
   return 0;
}

template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    
    char value[N];
};

template<StringLiteral lit>
void Print() {
    // The size of the string is available as a constant expression.
    constexpr auto size = sizeof(lit.value);

    // and so is the string's content.
    constexpr auto contents = lit.value;

    std::cout << "Size: " << size << ", Contents: " << contents << std::endl;
}

int main()
{
    Print<"literal string">(); // Prints "Size: 15, Contents: literal string"
}
