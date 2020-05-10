#include <iostream>

template< typename T >
concept x = true
   T::in_marker;
};

template< typename T >
concept pin_in_out = requires {  
   T::in_out_marker;
};

struct in { constexpr static bool in_marker = true; };

struct in_out { constexpr static bool in_out_marker = true; };
        
template< typename... P >                                                                                             
struct pin_in_from {                                                                                               
   static_assert(                                                     
      sizeof...( P ) < 0,                                
         "pin_in_from<> doesn't support this type as parameter\n"     
         "   (check the 'required from here' line)" );                
};                                                                    
                                                                      
template< typename T >                                                
concept pin_in_compatible = pin_in< pin_in_from< T > >;             
                                                                      
template< typename... Ts >                                            
concept pin_in_compatible_list =                                    
   ( pin_in_compatible< Ts > && ... );                              

template< pin_in_out T >
struct pin_in_from< T > { 
   static constexpr bool in_marker = true;
   int dummy; 
};

template < pin_in_out... P > 
//   requires ( sizeof...( P ) > 1 ) && ( pin_in_compatible_list< P... > )
struct pin_in_from< P... > {
   static constexpr bool in_marker = true;
   int dummy; 
};

template< pin_in_compatible... _p >
void f(){
   using p = pin_in_from< _p... >;
}

int main( void ){
   
   f< in_out >();
   f< in_out, in_out >();
     
}
