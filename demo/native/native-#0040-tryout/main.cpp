#include <iostream>
#include <type_traits>

/*
struct xy {
   int x, y;
   constexpr xy( short int x, float y ): x( x ), y( y ){}
};

template< xy value >
struct s {
   void f(){
      std::cout << value.x << " " << value.y << "\n";
   }
};

template< typename T >
   requires { std::is_same< T, void >; }
struct v {};
*/
int main( void ){
//   s< { 1,2 } >().f();
//   v< void > x;
//   (void) x;
}
