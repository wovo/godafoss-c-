#include <iostream>
#include <concepts>
#include <algorithm>

#define TRACE { std::cout << __LINE__ << "\n"; }


/**
 * Literal class type that wraps a constant expression string.
 *
 * Uses implicit conversion to allow templates to *seemingly* accept constant strings.
 */
template< size_t N >
struct string_literal {
    char value[ N ];
    constexpr string_literal( const char ( &str )[ N ]) {
        std::copy_n( str, N, value );
    }
};


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

struct resource_function_root : resource_root { 
};

template< typename T >
concept resource_function = std::derived_from< T, resource_function_root >;

template< string_literal _name >
struct printable_name {
   static constexpr auto name( std::string prefix ){ 
       return prefix + _name.value; }
};

template< void f(), string_literal name = "initialization" >
struct initialization : resource_function_root, printable_name< name.value > { 
   static void run_initialization(){ f(); };
};

template< void f(), string_literal name = "background" >
struct background : resource_function_root, printable_name< name.value > { 
   static void run_background(){ };
};

template< void f(), string_literal name = "thread" >
struct thread : resource_function_root, printable_name< name.value > { 
   static void run_thread(){ };
};


// ===========================================================================
//
// A component is a resource
// ?? that has a resources list
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

// fallback, required but never used
template< resource... _tail > 
struct list : resource_root {
      // assertion failure!!
};

// and empty list of resources
template<>
struct list<> : resource_root {
   template< typename T >
   static void run_initialization() { };    
};

// a list that starts with a component
template< component _first, resource... _tail >
struct list< _first, _tail... > {
   using first  = _first;
   using next   = list< _tail... >;
   
   template< typename T >
   static constexpr auto printable_name( std::string prefix ){ 
      return first::template inner< T >::name( prefix ) + next::name( prefix + "   " ); }
   
   template< typename T >
   static void run_initialization() { 
      using res = first::template inner< T >::resources;
      res::template run_initialization< T >();
      next::template run_initialization< T >();
   };
};

// a list that starts with a resource_function
template< resource_function _first, resource... _tail >
struct list< _first, _tail... > {
   using first  = list<>;    
   using next   = list< _tail... >;
   
   template< typename T >
   static constexpr auto printable_name( std::string prefix ){ 
      return _first::printable_name( prefix ) + next::name( prefix ); }
   
   template< typename T >   
   static void run_initialization() { 
       _first::run_initialization();
      next::template run_initialization< T >();
   };
};


// ===========================================================================
//
// visualisation of a component hierarchy
//
// ===========================================================================


/*
template< typename T >
struct walk {
   static void run( std::string prefix ){
      T::first::walk 
      std::cout << prefix << T::name << "\n";
   }    
};

template< list<> T >
struct walk {
   static void run( std::string prefix ){
   }    
};

template< component T >
struct walk {
   static void run( std::string prefix ){
      std::cout << prefix << T::name << "\n";
   }    
};
*/





// ===========================================================================

template< int _n >
struct timer : component_root { template< typename application > struct inner {   
   static constexpr auto printable_name = "timer";
   static constexpr int n = 42;
   using resources = list<>;
}; };


// ===========================================================================

template< int n >
struct blink : component_root { template< typename application > struct inner {    

  static constexpr auto printable_name = "blink";
  
   using r = timer< n >;

   static void body() { 
TRACE;       
      std::cout << "n = " << r::template inner< application >::n << "\n";
TRACE;      
   };
   
   using resources = list< 
      r,
      initialization< body >
   >; 
   
}; };


// ===========================================================================

template< typename application >
struct run_class {
   using app = typename application::inner< application >;    
   static void run(){  
   TRACE;
      app::resources::template run_initialization< application >();     
   TRACE;
   }
};

template< component application >
void run(){
   TRACE;
    run_class< application >::run();
   TRACE;
}


// ===========================================================================

struct app : component_root { template< typename application > struct inner {    
    
   static constexpr auto printable_name = "app";
    
   static void body() { 
      TRACE;
   };    
    
   using resources = list< 
      initialization< body >,
      blink< 10 >,
      blink< 12 >
   >;   
  
}; };


// ===========================================================================

int main(){
   TRACE;
   run< app >();
   TRACE;   
}
