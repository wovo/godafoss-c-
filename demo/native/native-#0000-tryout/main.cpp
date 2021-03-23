#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

#define TRACE { std::cout << __LINE__ << "\n"; }

/*
- name/printable-name => identification?
- superclass for the resource functions
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
// fixed-maximum-length vector
//
// ===========================================================================

template< typename T, int N >
class vector {
private:    

   std::array< T, N > data;
   int n;
   
public:   

   constexpr vector(): n( 0 ) {}

   constexpr vector( const T & d ):
      vector()
   {
      *this += d;       
   }
   
   vector & operator+=( const T & d ){
      static_assert( n < N );
      data[ n++ ] = d;
   }
   
   T * begin(){ return data;  }
   T * end(){ return data + n; }

   constexpr T * begin() const { return data + n;  }
   constexpr T * end() const { return data; }
   
   T & operator[]( int i ){
       static_assert( ( i >= 0 ) && ( i < n ) );
       return data[ i ];
   }

   constexpr T operator[]( int i ) const {
       static_assert( ( i >= 0 ) && ( i < n ) );
       return data[ i ];
   }

};


// ===========================================================================
//
// immutable list
//
// ===========================================================================

template< typename T, int N >
struct immutable_list: std::array< T, N > {

   template< int A, int B > 
   constexpr immutable_list( 
      const immutable_list< T, A > & a, 
      const immutable_list< T, B > & b  
   ){
      static_assert( N == A + B );
      for( unsigned int i = 0; i < A; ++i ){
          this->operator[]( i ) = a[ i ];
      }
      for( unsigned int i = 0; i < B; ++i ){
          this->operator[]( A + i ) = b[ i ];
      }
   }

   //constexpr T * begin() const { return this->operator[]( 0 );  }
   //constexpr T * end() const { return this->operator[]( N + 1 ); }

};

template< typename T >
struct immutable_list< T, 1 >: std::array< T, 1 > { 
    constexpr immutable_list( const T & x ){
        this->operator[]( 0 ) = x;
    }

   //constexpr T * begin() const { return this->operator[]( 0 );  }
   //constexpr T * end() const { return this->operator[]( 2 ); }
};

template< typename T >
struct immutable_list< T, 0 >: std::array< T, 0 > { 
//   constexpr const T * begin() const { return & this->operator[]( 0 );  }
//   constexpr const T * end() const { return & this->operator[]( 1 ); }
};

template< typename T, int A, int B >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, B > & b  
){
   return immutable_list< T, A + B >( a, b );
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
// encapsulate a function as a resource, in different ways:
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
struct resource_list : list_root {
      // assertion failure!!
};

// and empty list of resources
template<>
struct resource_list<> {
   template< typename T >
   static void run_initialization() { };    
   
   template< typename T >
   static auto name( std::string prefix ){ 
       return std::string( "" ); 
   }
   
   template< typename T, typename R >
   static constexpr auto info(){
       return immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{ 12 };
   }
};

// a list that starts with a component
template< component _first, resource... _tail >
struct resource_list< _first, _tail... > : list_root {
   using first  = _first;
   using next   = resource_list< _tail... >;
   
   template< typename T >
   static constexpr auto name( std::string prefix = "" ){ 
      return 
         prefix + std::string( first::template inner< T >::printable_name ) + "\n"
         + first::template inner< T >::resources::template name< T >( "   " + prefix )
         + next::template name< T >( prefix ); 
      }
   
   template< typename T >
   static void run_initialization() { 
      using resources = first::template inner< T >::resources;
      resources::template run_initialization< T >();
      next::template run_initialization< T >();
   };
   
   template< typename T, typename R >
   static constexpr auto info(){
       //return immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{ 13 };
       return
          first::template inner< T >::resources::template info< T, R >()
          + next::template info< T, R >();
   }  
 
   template< typename T, typename R >
      requires std::derived_from< first, R >
   static constexpr auto info(){
       return immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{ 15 };
       //return 
       //   immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{
       //       first::template inner< T >::info
       //   }
       //   + next::template info< T, R >();
   }   
};

// a list that starts with a resource_function
template< resource_function _first, resource... _tail >
struct resource_list< _first, _tail... > : list_root {
   using first  = resource_list<>;    
   using next   = resource_list< _tail... >;
   
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
       return immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{ 14 }
       + next::template info< T, R >();
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

template< int _marker >
struct xtimer : timer_root { 
   template< typename application > struct inner {   
       
      static constexpr int info = _marker;
      
      static constexpr int counter_number(){
          constexpr auto all = resources::template info< application, timer_root >();
          for( unsigned int n = 0; n < all.size(); ++n ){
             if( all[ n ] == info ) return (signed int) n;
          }
          return -1;
      }
      static constexpr int n = counter_number();
      
      static constexpr auto printable_name = "xtimer n= ";
      
      static void init(){
          TRACE;
          std::cout << application::template inner< application >::resources::template name< application >();
          TRACE;
          auto all = application::template inner< application >::resources::template info< application, timer_root >();
          TRACE;
          for( const auto & x : all ){
             std::cout << "x == " << x << "\n";
          }      
      }
      
      using resources = resource_list< initialization< init, "xtimer init"> >;
   }; 
};


// ===========================================================================

using namespace std::string_literals;

template< int n >
struct blink : component_root { template< typename application > struct inner {    

   static constexpr auto printable_name = "blink n= "; // + std::to_string( n );
  
   using r = xtimer< n >;

   static void body() { 
TRACE;       
      std::cout << "n = " << r::template inner< application >::n << "\n";
TRACE;      
      // r::template inner< application >::write( 1 ();
   };
   
   using resources = resource_list< 
      r,
      initialization< body >
   >; 
   
}; };


// ===========================================================================
//
// compose a component from a list of components
//
// ===========================================================================

template< component... all_components >
struct wrap : component_root { 
   template< typename components > struct inner {
       
      static constexpr auto printable_name = "component wrapper";  
     
      using resources = resource_list< all_components... >;    
      
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
   using resources = app::template inner< app >::resources;
   
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

struct app : component_root { 
   template< typename application > struct inner {    
    
      static constexpr auto printable_name = "app";
    
      static void body() {     
         TRACE;
      };    
    
      using resources = resource_list< 
         initialization< body >,
         blink< 10 >,
         blink< 12 >
      >;   
  
   }; 
};


// ===========================================================================

int main(){     
   TRACE;
   run< app >();
   TRACE;   
}
