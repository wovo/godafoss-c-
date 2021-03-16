#include <iostream>
#include <concepts>


// =============================================================================
//
// A resource is something a component needs to implement its functionality.
//
// =============================================================================

struct resource_root {
   static constexpr auto n_threads = 0;    
   
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

template< void f() >
struct initialization : resource_root {
   static void run_initialization(){ f(); };
};

template< void f() >
struct thread : resource_root {
   static constexpr auto n_threads = 1;    
   static void run_thread(){ f(); };
};

template< void f() >
struct background : resource_root {
   static void run_background(){ f(); };
};
   

// =============================================================================
//
// A list is a resource that bundles zero or more resources
//
// =============================================================================

// fallback, required but never used
template< resource... _tail > 
struct list : resource_root {
};

// and empty list of resources
template<>
struct list<> : resource_root {
};

// a list of at least one resource
template< resource _first, resource... _tail >
struct list< _first, _tail... > {
   using next   = list< _tail... >;
   using first  = _first;
   
   static constexpr auto n_threads = first::n_threads + next::n_threads;
   
   static void run_initialization() { 
      first::run_initialization();
      next::run_initialization();
   };
};


// =============================================================================

template< int _n >
struct timer {
   static constexpr int n = 42;
};


// =============================================================================

template< int n >
struct blink { template< typename application > struct main {    

   using r = timer< n >;

   static void body() { 
      std::cout << r::n << "\n";
   };
   
   using resources = list< 
      r,
      initialization< body > 
   >; 
   
   static constexpr auto n_threads = resources::n_threads;
   
   static void run_initialization() { 
      resources::run_initialization();
   };   
   
}; };


// =============================================================================

struct app { template< typename application > struct main {    
    
   using resources = list< 
      blink< 10 >,
      blink< 12 >
   >;   
      
   static constexpr auto n_threads = resources::n_threads;
   
   static void run_initialization() { 
      resources::run_initialization();
   };         
  
}; };

template< typename application >
struct run_class {
   using app = typename application::main< application >;    
   static void run(){  
      app::resources::run_initialization();     
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
