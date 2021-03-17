#include <iostream>
#include <concepts>

#define TRACE { std::cout << __LINE__ << "\n"; }


// ===========================================================================
//
// A resource is something a component needs to implement its functionality.
//
// ===========================================================================

struct resource_root {
   static constexpr auto n_threads = 0;    
   
   static void run_initialization() { };
   static void run_thread()         { };
   static void run_background()     { };    
};

template< typename T >
concept resource = std::derived_from< T, resource_root >;


// ===========================================================================
//
// encapsulate a function in a resource in different ways:
// - initialization: to be run once before the others, returns
// - thread: to br run as separate thread, doesn't return
// - background: to be run while idle, returns
//
// ===========================================================================

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
   

// ===========================================================================
//
// A list is a resource that bundles zero or more composition
//
// ===========================================================================

// fallback, required but never used
template< typename... _tail > 
struct list : resource_root {
};

// and empty list of composition
template<>
struct list<> : resource_root {
};

// a list that starts with a resource
template< resource _first, typename... _tail >
struct list< _first, _tail... > {
   using first  = _first;
   using next   = list< _tail... >;
   
   static constexpr auto n_threads = first::n_threads + next::n_threads;
   
   static void run_initialization() { 
   TRACE;
      first::run_initialization();
   TRACE;
      next::run_initialization();
   TRACE;
   };
};

// a list that starts with a component
template< typename _first, typename... _tail >
struct list< _first, _tail... > {
   using first  = _first::composition;
   using next   = list< _tail... >;
   
   static constexpr auto n_threads = first::n_threads + next::n_threads;
   
   static void run_initialization() { 
   TRACE;
      first::run_initialization();
   TRACE;
      next::run_initialization();
   TRACE;
   };
};


// ===========================================================================

template< int _n >
struct timer { template< typename application > struct inner {    
   using composition = list<>;
   static constexpr int n = 42;
}; };


// ===========================================================================

template< int n >
struct blink { template< typename application > struct inner {    

   using r = timer< n >;

   static void body() { 
TRACE;       
      std::cout << "n = " << r::n << "\n";
TRACE;      
   };
   
   using composition = list< 
      initialization< body >,
      r
   >; 
   
}; };


// ===========================================================================

template< typename application >
struct run_class {
   using app = typename application::inner< application >;    
   static void run(){  
   TRACE;
      app::composition::run_initialization();     
   TRACE;
   }
};

template< typename application >
void run(){
   TRACE;
    run_class< application >::run();
   TRACE;
}


// ===========================================================================

struct app { template< typename application > struct inner {    
    
   static void body() { 
      TRACE;
   };    
    
   using composition = list< 
      initialization< body >,
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
