#include <iostream>
#include <concepts>


// =============================================================================
//
// A resource is something a component needs to implement its functionality.
//
// =============================================================================

struct resource_root {
   static constexpr n_threads = 0;    
   static void run_initialization() { };
   static void run_thread()         { };
   static void run_background()     { };    
};

template< typename T >
concept resource = requires {
   std::derived_from< T, resource_root >;
};


// =============================================================================
//
// encapsulate a function in a resource in different ways:
// - initialization: to be run once before the others, returns
// - thread: to br run as separate thread, doesn't return
// - background: to be run while idle, returns
//
// =============================================================================

struct initialization< void f() >
struct initialization : resource_root {
   static void run_initialization(){ f(); };
};

template< void f() >
struct thread : resource_root {
   static constexpr n_threads = 0;    
   static void run_thread(){ f(); };
};

template< void f() >
struct background : resource_root {
   static void run_background(){ f(); };
};
   

// =============================================================================
//
// A list bundles resources, and is a resource.
//
// =============================================================================


// fallback, required but never used
template< typename... _tail > 
struct list : resource_root {
};

// and empty list of resources
template<>
struct list<> : resource_root {
};

template< typename _first, typename... _tail >
struct list< _first, _tail... > {
    
   using first  = _first;
   using next   = list< _tail... >;
};


// =============================================================================

template< int _n >
struct resource {
   static constexpr int n = 42;
};


// =============================================================================

template< int n >
struct blink { template< typename application > struct main {    

   using r = resource< n >;

   static void body() { 
      std::cout << r::n << "\n";
   };
   
   using resources = list< 
      r,
      initialization< body > 
   >; 
   
}; };


// =============================================================================

struct app { template< typename application > struct main {    
    
   using resources = list< 
      blink< 10 >,
      blink< 12 >
  >; 
  
}; };

template< typename application >
struct run_class {
   using app = typename application::main< application >;    
   static void run(){  
      // init          
   }
};

template< typename application >
void run(){
    run_class< application >::run();
}


// =============================================================================

int main(){
   run< app >();
}
