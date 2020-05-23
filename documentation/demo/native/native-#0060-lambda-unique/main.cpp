#include <iostream>
#include "h.hpp"

using a = s<>;
using b = s<>;

int main(){
   std::cout << (long long int) & a::x << "\n";
   std::cout << (long long int) & b::x << "\n";
   x();
}

