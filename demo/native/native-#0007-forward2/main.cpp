#include <iostream>
#include <concepts>
#include <algorithm>
#include <type_traits>

#define TRACE { std::cout << __LINE__ << "\n"; }

/*
 * name/printable-name => identification? composition? str? printable?
 * name as somthing you inherit from -> also for component
 * what is the use of the names, only for reporting, not for inclusion??
 * -> use immutable list of chars??
 * name -> application_structure
 * list + element
*/


// ===========================================================================
//
// fixed-maximum-length structural string
//
// structural means usable as template parameter
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
// for compile-time use
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
   
   constexpr immutable_list( 
      const T a[ N ]  
   ){
      for( unsigned int i = 0; i < N; ++i ){
          this->operator[]( i ) = a[ i ];
      }
   }   

};

template< typename T >
struct immutable_list< T, 1 >: std::array< T, 1 > {
    
    constexpr immutable_list( const T & x ){
        this->operator[]( 0 ) = x;
    }
    
};

template< typename T >
struct immutable_list< T, 0 >: std::array< T, 0 > { };

template< typename T, int A, int B >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, B > & b  
){
   return immutable_list< T, A + B >( a, b );
}   

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, 0 > & b  
){
   return a;
}

template< typename T, int B  >
constexpr auto operator+( 
   const immutable_list< T, 0 > & a, 
   const immutable_list< T, B > & b  
){
   return b;
}

template< typename T >
constexpr auto operator+( 
   const immutable_list< T, 0 > & a, 
   const immutable_list< T, 0 > & b  
){
   return immutable_list< T, 0 >();
}

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const T & x
){
   return a + immutable_list< T, 1 >( x );
}

template< typename T, int A  >
constexpr auto operator+( 
   const T & x,
   const immutable_list< T, A > & b
){
   return immutable_list< T, 1 >( x ) + b;
}

template< int N >
struct immutable_string : immutable_list< char, N > {
   constexpr immutable_string( 
      const char ( &s )[ N ]
   ): immutable_list< char, N >( s ){}
};

template<>
struct immutable_string< 0 > : immutable_list< char, 0 > {
   constexpr immutable_string( 
   ): immutable_list< char, 0 >(){}
};

template< int N >
constexpr auto prefix(){
   return immutable_list< char, 1 >( '@' ) + immutable_list< char, 1 >( '0' + N );
}


// ===========================================================================

template< int n >
struct the_application { }; 

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
// encapsulate a function and its name as a resource, in different ways:
// - initialization  (to be run once before the others, returns)
// - thread          (to be run as separate thread, doesn't return)
// - background      (to be run while idle, returns quickly)
//
// ===========================================================================

using resource_function_name = string_literal< 132 >;

struct resource_function_root : resource_root { };

template< typename T >
concept resource_function = 
   std::derived_from< T, resource_function_root >;

template< void f(), auto _name >
struct resource_function_with_name : 
   resource_function_root
{
    
   static constexpr auto name = _name;
   
   static void run(){ 
      f(); 
   };
   
};

template< 
   void f(), 
   immutable_string name = "initialization"
>
struct initialization : 
   resource_function_with_name< f, name > 
{};

template< 
   void f(), 
   resource_function_name name = "background" 
>
struct background : 
   resource_function_with_name< f, name > 
{};

template< 
   void f(), 
   resource_function_name name = "thread" 
>
struct thread : 
   resource_function_with_name< f, name > 
{};


// ===========================================================================
//
// A component is a resource
// ?? that has inside an inner:
//    ?? list
//    ?? a name
//
// ===========================================================================

struct component_root : resource_root { };

template< typename T >
concept component = std::derived_from< T, component_root >;

template< immutable_string _name >
struct component_name : component_root {
    static constexpr auto name = _name;
};


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

// specialization for an empty list of resources
template<>
struct resource_list<> {
    
   static void run_initialization() { };    
   
   template< int N >
   static constexpr auto name(){ 
       return immutable_string< 0 >(); 
   }
   
   template< typename R >
   static constexpr auto info(){
       return immutable_list< std::remove_cvref_t< decltype( R::info ) >, 0 >{ };
   }
   
};

// specialiation for a list that starts with a component
template< component _first, resource... _tail >
struct resource_list< _first, _tail... > : list_root {
    
   using first  = _first;
   using next   = resource_list< _tail... >;
   
   template< int N >
   static constexpr auto name(){ 
      return 
         prefix< N >() + first::name + '\n'
         + first::resources::template name< N + 1 >()
         + next::template name< N >(); 
      }
   
   static void run_initialization() { 
      using resources = first::resources;
      resources::template run_initialization();
      next::template run_initialization();
   };
   
   template< typename R >
   static constexpr auto info(){
       return 
          first::template info< R >()
          + next::template info< R >();
   }  
 
   template< typename R >
      requires std::derived_from< first, R >
   static constexpr auto info(){
       return 
          first::resources::template info< R >()
          + immutable_list< std::remove_cvref_t< decltype( R::info ) >, 1 >{
              first::info
          }
          + next::template info< R >();
   }  
 
};

// specialization for a list that starts with a resource_function
template< resource_function _first, resource... _tail >
struct resource_list< _first, _tail... > : list_root {
    
   using first  = resource_list<>;    
   using next   = resource_list< _tail... >;
   
   template< int N >
   static constexpr auto name(){ 
      return 
         prefix< N >() + _first::name + '\n'
         + next::template name< N >(); 
    }
   
   static void run_initialization() { 
      _first::run();
      next::template run_initialization();
   }   
   
   template< typename R >
   static constexpr auto info(){
       return next::template info< R >();
   }  
   
};


// ===========================================================================

struct timer_root {
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
struct xtimer : timer_root, component_name< "xtimer" >  { 
       
      static constexpr int info = _marker;
      
      static constexpr int counter_number(){
          constexpr auto all = the_application< _marker >::resources::template info< timer_root >();
          for( unsigned int n = 0; n < all.size(); ++n ){
             if( all[ n ] == info ) return (signed int) n;
          }
          return -1;
      }
      
      static constexpr int n = counter_number();
      
      static void init(){
         std::cout << "I am number " << n << "\n";
      }
      
      using resources = resource_list< 
         initialization< init > 
      >;   
};


// ===========================================================================

template< int n >
struct blink : component_name< "blink" > { 
  
   using r = xtimer< n >;

   static void body() { 
      std::cout << "n = " << r::n << "\n";
   };
   
   using resources = resource_list< 
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
struct wrap : component_name< "component wrapper" > { 
     
      using resources = resource_list< all_components... >;    
      
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
   bool prefix = false;
   for( auto c : resources::template name< app, 0 >() ){
      if( prefix ){
          for( int i = 0; i < ( c - '0' ); ++i ) std::cout << "   ";
          prefix = false;
      } else if( c == '@' ){
          prefix = true;
      } else {   
          std::cout << c;
      }   
   }
   TRACE;
   resources::template run_initialization< app >();
   TRACE;
}


// ===========================================================================
//
// test application
//
// ===========================================================================

template<>
struct the_application< 10 > : component_name< "test application" > { 
    
      static void body() {     
         TRACE;
      };    
    
      using resources = resource_list< 
         initialization< body >,
         blink< 10 >,
         blink< 12 >
      >;   

};


// ===========================================================================

int main(){     
   run< the_application< 10 > >();  
}
