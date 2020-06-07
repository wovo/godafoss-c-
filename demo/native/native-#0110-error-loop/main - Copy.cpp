template< typename T = int_fast64_t, T zero = 0 >
   requires requires( T & x, T & v ){
      // default constructor
      // copy constructor
      { v * v };
      { v / v };
      { v == v };
      { x = v };
   }
class xy final {
public:

   using value_t = T;

   value_t x, y;
   
   constexpr xy( value_t x, value_t y ): x{ x }, y{ y }{}
   
   constexpr xy( value_t x ): x{ x }, y{ x }{}
   
   constexpr xy():x{ zero }, y{ zero }{}
   
   static constexpr auto origin = xy{};

   template< typename V >
      requires requires( V b ){ { x + b }; }   
   constexpr auto operator+( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > { 
          static_cast< value_t >( x + rhs.x ),
          static_cast< value_t >( y + rhs.y )
      };      
   }      
   
   template< typename V >
      requires requires( V b ){ { x - b }; }      
   constexpr auto operator-( const xy< V > rhs ) const {
      return xy< decltype( x + rhs.x ) > {
          static_cast< value_t >( x - rhs.x ),
          static_cast< value_t >( y - rhs.y )
      };         
   }      

   constexpr auto operator/( const value_t rhs ) const {
      return xy { 
          static_cast< value_t >( x / rhs ),
          static_cast< value_t >( y / rhs )
      };         
   }    

   constexpr xy operator*( const value_t rhs ) const {
      return xy { 
          static_cast< value_t >( x * rhs ),
          static_cast< value_t >( y * rhs )
      };         
   }    

   constexpr bool operator==( const xy & rhs ) const {
      return ( x == rhs.x ) && ( y == rhs.y );
   }

   constexpr bool operator!=( const xy & rhs ) const {
      return ! ( *this == rhs );
   }

}; 


// ==========================================================================
//
// iterator
//
// ==========================================================================

template< typename T >
class xy_iterator_t {
private:

   xy< T > current;
   xy< T > start;
   xy< T > limit;

public:

   xy_iterator_t( xy< T > start, xy< T > limit ): 
      current( start ),
      start( start ), 
      limit( limit ) 
   {}

   xy< T > operator*() const {
      return current;
   }

   void operator++(){
      ++current.x;
      if( current.x == limit.x ){
         current.x = start.x;
         ++current.y;
      }
   }

   bool operator==( const xy_iterator_t rhs ) const {
      return current == rhs.current;
   }

   bool operator!=( const xy_iterator_t rhs ) const {
      return current != rhs.current;
   }
      
};


// ==========================================================================
//
// ranges
//
// ==========================================================================

template< typename T, T v >
   requires requires( T & x ){
      { ++x };      
   }      
class range {
private:

   xy< T, v > start;
   xy< T, v > limit;

public:

   range( xy< T, v > limit ):
      start( limit::zero, limit( limit )
   {}

   xy_iterator_t< T > begin() const {
      return xy_iterator_t( start, limit );
   }

   xy_iterator_t< T > end() const {
      return xy_iterator_t( xy< T >( 0, limits.y ), limit );
   }

};

// ==========================================================================
//
// gf-box.hpp
// 
// ==========================================================================
//
// the basic interface properties:
//    - box: holds some data elements(s))
//    - item: box that always holds one element of the data
//    - stream: box that holds a sequence of data elements
//    - input: box that supports read()
//    - output: box that supports write()
//    - input_output: both input and output
//    - duplex: both input and output at the same time
//    - simplex: both input and output, but only one at a time
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2019-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// box
//
// holds some data elements(s))
//
// ==========================================================================

// quote ''box'' };
template< typename T >
concept box = requires {  
   T::_box_marker;
   { T::init() } -> std::same_as< void >;
};

template< typename T >
struct _box_root {
   static const bool _box_marker = true;	
   using value_type = T;
};

template< typename T >
struct box_root : _box_root< T > {};

// box< bool > has a different invert than other boxes
template<>
struct box_root< bool > :
   _box_root< bool >
{
   GODAFOSS_INLINE static bool invert( bool v ){
      return !v;	   
   }   
};


// ==========================================================================
//
// item 
//
// box that always holds one element of the data
//
// ==========================================================================

// quote ''item'' );
template< typename T >
concept item = requires {
   box< T >;
   T::_item_marker;
};

template< typename T >
struct item_root : 
   box_root< T > 
{
   static const bool _item_marker = true;	
};


// ==========================================================================
//
// stream
//
// box that holds a sequence of data elements
//
// ==========================================================================

// quote ''stream'' );
template< typename T >
concept stream = requires {
   box< T >;
   T::_stream_marker;
};

template< typename T >
struct stream_root : 
   box_root< T > 
{
   static const bool _stream_marker = true;	
};


// ==========================================================================
//
// input
//
// box that supports read()
//
// ==========================================================================

// quote ''input'' };
template< typename T >
concept input = requires {
   box< T >;
   T::_input_marker;
   { T::refresh() }  -> std::same_as< void >;
   { T::read() }     -> std::same_as< typename T::value_type >;
};

template< typename T >
struct input_root : 
   box_root< T > 
{
   static const bool _input_marker = true;	
};


// ==========================================================================
//
// output
//
// box that supports write()
//
// ==========================================================================

// quote ''output'' };
template< typename T >
concept output = requires ( 
   typename T::value_type v
){
   box< T >;
   T::_output_marker;
   { T::write( v ) }  -> std::same_as< void >;
   { T::flush()    }  -> std::same_as< void >;
};

template< typename T >
struct output_root : 
   box_root< T > 
{
   static const bool _output_marker = true;	
};


// ==========================================================================
//
// input_output
//
// both input and output
//
// ==========================================================================

// quote ''input_output'' );
template< typename T >
concept input_output = requires {
   input< T >;
   output< T >;
};

template< typename T >
struct input_output_root : 
   input_root< T >, 
   output_root< T > 
{};


// ==========================================================================
//
// duplex
//
// both input and output, at the same time
//
// ==========================================================================

// quote ''duplex'' );
template< typename T >
concept duplex = requires {
   input_output< T >;
   T::_duplex_marker;
};

template< typename T >
struct duplex_root : 
   input_output_root< T > 
{
   static const bool _duplex_marker = true;	
};


// ==========================================================================
//
// simplex
//
// both input and output, but only one at a time
//
// ==========================================================================

// quote ''simplex'' };
template< typename T >
concept simplex = requires ( 
){
   input_output< T >;
   T::_simplex_marker;
   { T::direction_set_input() }   -> std::same_as< void >;
   { T::direction_set_output() }  -> std::same_as< void >;
   { T::direction_flush() }       -> std::same_as< void >;
};

template< typename T >
struct simplex_root : input_output_root< T > {
   static const bool _simplex_marker = true;	
};

// ==========================================================================
//
// gf-box-inherit.hpp
//
// ==========================================================================
//
// Templates for inheriting only the init, 
// read, write, or direction functions of a box.
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2019-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// init
//
// ==========================================================================

template< typename T >
struct box_inherit_init {};

template< box T >
struct box_inherit_init< T > { 
    
   static void GODAFOSS_INLINE init(){ 
      T::init(); 
   }     
}; 


// ==========================================================================
//
// write
//
// ==========================================================================

template< typename T >
struct box_inherit_write {};
   
template< output T >
struct box_inherit_write< T > { 

   using _value_type = typename T::value_type;
    
   static void GODAFOSS_INLINE write( _value_type v ){ 
      T::write( v );  
   }
   
   static void GODAFOSS_INLINE flush(){ 
      T::flush(); 
   }
};    


// ==========================================================================
//
// read
//
// ==========================================================================

template< typename T >
struct box_inherit_read {};

template< input T >
struct box_inherit_read< T > { 

   using _value_type = typename T::value_type;
    
   static _value_type GODAFOSS_INLINE read(){ 
      return T::read(); 
   }
   
   static void GODAFOSS_INLINE refresh(){ 
      T::refresh(); 
   }
}; 


// ==========================================================================
//
// direction
//
// ==========================================================================

template< typename T >
struct box_inherit_direction {};
   
template< simplex T >
struct box_inherit_direction< T > { 
   
   static void GODAFOSS_INLINE direction_set_input(){ 
      T::direction_set_input(); 
   }
   
   static void GODAFOSS_INLINE direction_set_output(){ 
      T::direction_set_output; 
   }
   
   static void GODAFOSS_INLINE direction_flush(){ 
      T::direction_flush(); 
   }
}; 

// ==========================================================================
//
// gf-box-no-inline.hpp
//
// ==========================================================================
//
// no_inline< T > provides functions that call the corresponding functions
// in T without a forced inlining.
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2019-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// init
//
// ==========================================================================

template< typename T >
struct _no_inline_box_init : T {};

template< box T >
struct _no_inline_box_init< T > : T {
	
   static void init(){ 
      T::init(); 
   }	

};


// ==========================================================================
//
// write
//
// ==========================================================================

template< typename T >
struct _no_inline_box_write : T {};

template< output T >
struct _no_inline_box_write< T > : T {
	
   using _vt = typename T::value_type;
    
   static void write( _vt v ){ 
      T::write( v ); 
   }
  
   static void flush(){ 
      T::flush(); 
   }	
   
};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T >
struct _no_inline_box_read : T {};

template< input T >
struct _no_inline_box_read< T > : T { 

   using _vt = typename T::value_type;
    
   static _vt read(){ 
      return T::read(); 
   }
   
   static void refresh(){ 
      T::refresh(); 
   }
   
}; 


// ==========================================================================
//
// direction
//
// ==========================================================================

template< typename T >
struct _no_inline_box_direction : T {};

template< simplex T >
struct _no_inline_box_direction< T > : T { 
    
   static void direction_set_output(){ 
      T::direction_set_output(); 
   }

   static void direction_set_input(){ 
      T::direction_set_input();; 
   }
   
   static void direction_flush(){ 
      T::direction_flush(); 
   }
   
}; 


// ==========================================================================
//
// no_inline
//
// ==========================================================================

template< typename T >
using no_inline = 
   _no_inline_box_init<
   _no_inline_box_write<
   _no_inline_box_read<
   _no_inline_box_direction< T > > > >;




