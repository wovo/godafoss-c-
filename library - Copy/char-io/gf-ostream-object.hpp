// ==========================================================================
//
// file : hwcpp-stream-object.hpp
//
// create an ostream-like object from a stream cto
//
// ==========================================================================
//
// This file is part of HwCpp, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2017
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

/*

// ========== ostream marker and concept 
struct ostream_root {
   static constexpr bool is_ostream = true;
};

template< typename T >
concept bool is_ostream(){ 
   return T::is_ostream;
}

*/

// ========== manipulators

/// end-of-line constant
constexpr char endl = '\n';
   
/// 0-character constant
constexpr char ends = '\0';

#define _GODAFOSS_MANIPULATOR( TYPE, FIELD, NAME )          \
                                                            \
struct NAME {                                               \
   const TYPE v;                                            \
   constexpr NAME( TYPE v ): v{ v }{}                       \
};                                                          \
                                                            \
template< typename ostream >                                \
ostream & operator<<( ostream & lhs, const NAME & rhs ){    \
   ostream::base_type::format.FIELD = rhs.v;	               \
   return lhs;                                              \
}   

_GODAFOSS_MANIPULATOR( bool,           align_right,      _align_right      )
_GODAFOSS_MANIPULATOR( bool,           bool_alpha,       _boolalpha        )
_GODAFOSS_MANIPULATOR( bool,           show_pos,         _show_pos         )
_GODAFOSS_MANIPULATOR( bool,           show_base,        _show_base        )

_GODAFOSS_MANIPULATOR( char,           fill_char,        setfill           )
_GODAFOSS_MANIPULATOR( char,           hex_base,         _hex_base         )

_GODAFOSS_MANIPULATOR( uint_fast16_t,  field_width,      setw              )
_GODAFOSS_MANIPULATOR( uint_fast16_t,  numerical_radix,  _numerical_radix  )

#undef _GODAFOSS_MANIPULATOR

constexpr _align_right right( true );
constexpr _align_right left( false );

constexpr _boolalpha boolalpha( true );
constexpr _boolalpha noboolalhpa( false );

constexpr _show_pos showpos( true );
constexpr _show_pos noshowpos( false );

constexpr _show_base showbase( true );
constexpr _show_base noshowbase( false );

constexpr _numerical_radix bin(  2 );
constexpr _numerical_radix oct(  8 );
constexpr _numerical_radix dec( 10 );
constexpr _numerical_radix hex( 16 );


// ==========================================================================
//
// the formatter
//
// ==========================================================================

// template< is_stream_out_char_formatted T >
template< typename T >
struct ostream {
	
   using base_type = T;	
	
   ostream(){
      T::init();	   
   }
   
   template< typename V >
   auto & write( V v ){
      T::write( v );
      return *this;	  
   }  
   
};

// ========== worker functions ==========

//template< is_ostream ostream, typename T >
template< typename ostream, typename T >
auto & operator<<( ostream & stream, T v ){
   return stream.write( v );     	      
}	  
