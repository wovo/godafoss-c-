#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

template< typename... all >
struct list { };

template<>
struct list<> {
   static constexpr int data(){ return 0; }       
};

template< typename _first, typename... _next >
struct list< _first, _next... > {
   using first = _first;
   using next = list< _next... >;
   
   static constexpr int data(){ 
      return first::template inner<>::x + next::data();          
   }
};

template< int _x >
struct comp {
   template< typename t = list<> >
   struct inner {
      static constexpr int x = _x; 
      static constexpr int get_n(){
         return t::data();          
      }
      static constexpr int n = get_n();
      static void print(){
         std::cout << x << " " << n << "\n";
      }
   };
};   



int main(){
   using app = list< comp< 55 >, comp< 99 > >;
   app::first::inner< app >::print();
}