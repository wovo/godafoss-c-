/*
 * #include <algorithm>

template< size_t N >
struct string_literal {
    char value[ N ];
    constexpr string_literal( const char ( &str )[ N ]) {
        std::copy_n( str, N, value );
    }
};

template< string_literal name >
struct printable_name { };

template< void f(), string_literal name = "" >
struct initialization : printable_name< name.value > { };

int main(){}
*/

#include <algorithm>
#include <iostream>

template< size_t N >
struct string_literal {
    char value[ N ];
    constexpr string_literal( const char ( &str )[ N ]) {
        std::copy_n( str, N, value );
    }
};


template< string_literal name = "" >
struct initialization { 
    static void run(){
       std::cout << name.value;
    }
};

int main(){
    initialization< "hello\n" >::run();
}