#include <string>
#include <algorithm>
#include <iostream>


template< size_t N >
struct string_literal {
    char value[ N ];
    constexpr string_literal( const char * s ) {
        for( auto & v : value ){
           v = ( *s == '\0' ) ? *s : *s++;
        }
        value[ N - 1 ] = '\0';
    }
};

template< string_literal< 100 > name = "" >
struct initialization { 
    static void run(){
       std::cout << name.value;
    }
};

int main(){
    initialization< "hello\n" >::run();
}