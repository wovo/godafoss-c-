#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

#define TRACE { std::cout << __LINE__ << "\n"; }

/*
- name/printable-name => identification?

*/


// ===========================================================================
//
// fixed-maximum-length structural (= acceptable as template parameter) string
//
// ===========================================================================

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


// ===========================================================================
//
// totally inefficient vector addition (for compile-time only)
//
// ===========================================================================

template< typename T >
constexpr std::vector< T > operator+ ( std::vector< T > a, const std::vector< T > & b ){
   a.insert( a.end(), b.begin(), b.end() );
   return a;
}   

template< typename T >
constexpr std::vector< T > operator+ ( std::vector< T > a, T b ){
   a.push_back( b );
   return a;
}   


// ===========================================================================
//
// A resource is something a component needs to implement its functionality.
//
// ===========================================================================

struct resource_root { };

template< typename T >
concept resource = std::derived_from< T, resource_root >;


// ===========================================================================
//
// encapsulate a function  as a resource, in different ways:
// - initialization: to be run once before the others, returns
// - thread: to br run as separate thread, doesn't return
// - background: to be run while idle, returns (quickly)
//
// ===========================================================================

struct resource_function_root : resource_root { };

template< typename T >
concept resource_function = std::derived_from< T, resource_function_root >;

using resource_function_name = string_literal< 132 >;

template< resource_function_name _name >
struct printable_name {
   static constexpr auto name = _name.value;
};

template< void f(), resource_function_name name = "initialization" >
struct initialization : resource_function_root, printable_name< name > { 
   static void run_initialization(){ f(); };
};

template< void f(), resource_function_name name = "background" >
struct background : resource_function_root, printable_name< name > { 
   static void run_background(){ f(); };
};

template< void f(), resource_function_name name = "thread" >
struct thread : resource_function_root, printable_name< name > { 
   static void run_thread(){ f(); };
};


// ===========================================================================
//
// A component is a resource
// ?? that has a resources list
// ?? that has a name
//
// ===========================================================================

struct component_root : resource_root { };

template< typename T >
concept component = std::derived_from< T, component_root >;


// ===========================================================================
//
// A list is a resource that bundles zero or more resources
//
// ===========================================================================

struct list_root : resource_root { };

template< typename T >
concept is_list = std::derived_from< T, list_root >;

// fallback, required but never used
template< resource... _tail > 
struct list : list_root {
      // assertion failure!!
};

// and empty list of resources
template<>
struct list<> {
   template< typename T >
   static void run_initialization() { };    
   
   template< typename T >
   static auto name( std::string prefix ){ 
       return std::string( "" ); 
   }
   
   template< typename T, typename R >
   static constexpr auto info(){
       return std::vector< std::remove_cvref_t< decltype( R::info ) > >{};
   }
};

// a list that starts with a component
template< component _first, resource... _tail >
struct list< _first, _tail... > : list_root {
   using first  = _first;
   using next   = list< _tail... >;
   
   template< typename T >
   static constexpr auto name( std::string prefix = "" ){ 
      return 
         prefix + std::string( first::template inner< T >::printable_name ) + "\n"
         + first::resources::template name< T >( "   " + prefix )
         + next::template name< T >( prefix ); 
      }
   
   template< typename T >
   static void run_initialization() { 
      using resources = first::resources;
      resources::template run_initialization< T >();
      next::template run_initialization< T >();
   };
   
   template< typename T, typename R >
   static constexpr auto info(){
       return
          first::resources::template info< T, R >()
          + next::template info< T, R >();
   }  
 
   template< typename T, typename R >
      requires std::derived_from< first, R >
   static constexpr auto info(){
       return 
          next::template info< T, R >()
          + first::info;
   }   
};

// a list that starts with a resource_function
template< resource_function _first, resource... _tail >
struct list< _first, _tail... > : list_root {
   using first  = list<>;    
   using next   = list< _tail... >;
   
   template< typename T >
   static constexpr auto name( std::string prefix = "" ){ 
      return 
         prefix + _first::name + "\n"
         + next::template name< T >( prefix ); 
    }
   
   template< typename T >   
   static void run_initialization() { 
      _first::run_initialization();
      next::template run_initialization< T >();
   };
   
   template< typename T, typename R >
   static constexpr auto info(){
       return next::template info< T, R >();
   }  
};


// ===========================================================================

struct timer_root : component_root {
   static constexpr int info = 0;
};

template< typename T >
concept timer = std::derived_from< T, timer_root >;

void print( const std::vector< int > & data ){
   std::cout << "timer list\n";
   for( const auto & d : data ){
      std::cout << "   n=" << d << "\n";
   }   
}

template< typename application, int _marker >
struct xtimer : timer_root { 
       
   static constexpr int info = _marker;
      
   static constexpr int counter_number(){
       // auto all = list<>::template info< application, timer_root >();
       //for( unsigned int n = 0; n < all.size(); ++n ){
       //   if( all[ n ] == info ) return (signed int) n;
       // }
       return _marker; // snark
   }
   static constexpr int n = counter_number();
   
   static constexpr auto printable_name = "timer n= ";
      
   static void init(){
      //using resources = application::template inner< application >::resources;
      //TRACE;
      //print( resources::template info< application, timer_root >() );
      //TRACE;       
   }
      
   using resources = list< initialization< init, "timer init"> >;
   
};


// ===========================================================================

using namespace std::string_literals;

template< typename application, int n >
struct blink : component_root { 

   static constexpr auto printable_name = "blink n= "; // + std::to_string( n );
  
   using r = xtimer< application, n >;

   static void body() { 
TRACE;       
      std::cout << "n = " << r::n << "\n";
TRACE;      
   };
   
   using resources = list< 
      r,
      initialization< body >
   >; 
   
};


// ===========================================================================
//
// compose a component from a list of components
//
// ===========================================================================

template< component... all_components >
struct wrap : component_root { 
   template< typename components > struct inner {
       
      static constexpr auto printable_name = "component wrapper";  
     
      using resources = list< all_components... >;    
      
   }; 
};


// ===========================================================================
//
// run an application that consists of one or more components 
//
// ===========================================================================

template< component first_component, component... more_components >
void run(){
    
   // wrap the components
   using app = wrap< first_component, more_components... >;
   
   // we mainly need the resources
   using resources = app::resources;
   
   TRACE;
   std::cout << resources::template name< app >();
   TRACE;
   resources::template run_initialization< app >();
   TRACE;
}


// ===========================================================================
//
// test application
//
// ===========================================================================

template< typename application >
struct app : component_root {    
    
   static constexpr auto printable_name = "app";
    
   static void body() {     
      TRACE;
   };    
    
   using resources = list< 
      initialization< body >,
      blink< application, 10 >,
      blink< application, 12 >
   >;   
   
};


// ===========================================================================

int main(){     
   TRACE;
   run< app >();
   TRACE;   
}
