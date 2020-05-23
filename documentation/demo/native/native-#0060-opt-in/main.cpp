#include <iostream>

template< typename T, typename... P >
struct from_supported { };   

template< typename T >
struct from_supported< T > { };   

template< typename... P >
concept from_supported_concept = requires {  
   from_supported< P... >::value;
};
              
template< typename... P >   
   requires from_supported_concept< P... >                                                                                          
struct from {                                                                                               
   static_assert(                                                     
      sizeof...( P ) < 0,                                
         "library internal error\n"
         "   from<> claims support for this type(s) of parameter(s),\n"     
         "   but no actual implementation was provided.\n"
         "   (check the 'required from here' line)" );                
};   

// ===========================================================================                                                      

template< typename P >
concept pin = requires {  
   P::pin_marker;
};

struct gpio {
   static constexpr bool pin_marker = true;   
   static void init(){}
};

template< pin P >
struct from_supported< P >{
   static constexpr bool value = true;      
};

template< pin P >
struct from< P > : P {};        

// ===========================================================================                                                      

template< typename A, typename B >
   requires std::same_as< A, int >
struct from_supported< A, B > {
   static constexpr bool value = true;         
};
                                                           
template< typename A, typename B >
   requires std::same_as< A, int >
struct from< A, B > : gpio {      
};
                                                           
// ===========================================================================                                                      

using ta = from< gpio >;
using tb = from< int, float >;

int main( void ){
   ta::init();
   tb::init();
}
