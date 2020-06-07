#include <experimental/source_location>
#include <iostream>

template<
   typename t,
   int line = std::experimental::source_location::current().line() >
struct s {
   static inline int x;
   static void print(){
      std::cout << "@ " << line << " &x= " << (long long int) & x << "\n";
   }
};

void f( int line = std::experimental::source_location::current().line() ){
   std::cout << "f @ " << line << "\n";
};

int main( void ){
   s< int, std::experimental::source_location::current().line() >::print();
   s< int, std::experimental::source_location::current().line() >::print();
   f();
   f();
}
