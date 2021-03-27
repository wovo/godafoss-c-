#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

#define TRACE { std::cout << __LINE__ << "\n"; }

template< int n >
struct app {}; 

template< int N >
struct component {
   static void run(){
      std::cout << app< N >::name;
   }       
};

template<>
struct app< 42 > {
   using c = component< 42 >;   
   static constexpr auto name = "Hello\n";
   static void run(){
      c::run();
   }  
};

int main(){     
   app< 42 >::run();
}
