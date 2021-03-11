// ==========================================================================
//
// gf-ostream.hpp
//
// ==========================================================================
//
// This file is part of godafoss,
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// concept
//
// ==========================================================================

template< typename T >
concept is_stream_out_char = requires {
      T::_ostream_marker;
      std::same_as< typename T::value_type, char >;
   }
   && is_pipe< T >
   && is_output< T >;

struct ostream_root :
   pipe_root< char >,
   output_root< char >
{
   static constexpr bool _ostream_marker = true;
};


// ==========================================================================
//
// the 'persistent' formatting information
//
// ==========================================================================

struct _formatting {

   bool align_right;
   bool show_pos;
   bool bool_alpha;
   bool show_base;

   char fill_char;
   char hex_base;

   uint_fast16_t field_width;
   uint_fast16_t numerical_radix;

   constexpr _formatting():
      align_right( true ),
      show_pos( false ),
      bool_alpha( false ),
      show_base( false ),

      fill_char( ' ' ),
      hex_base( 'A' ),

      field_width( 0 ),
      numerical_radix( 10 )
   {}

   int_fast16_t fill_count( bool left, int_fast16_t len ){
      return ( left == align_right ) ? field_width - len : 0;
   }

   const char * bool_rep( bool v ){
      return bool_alpha
         ? ( v ? "true" : "false" )
		 : ( v ? "1"    : "0"     );
   }
};


// ==========================================================================
//
// print reverse: used to print an integer
//
// ==========================================================================

template< uint64_t max_length >
struct _print_reverse {
   char body[ max_length + 1 ];
   char *content;
   int n;

   _print_reverse(){
      body[ max_length ] = '\0';
      content = & body[ max_length ];
	  n = max_length;
   }

   void add_char( char c ){
      if( n > 0 ){
         --n;
         *--content = c;
      }
   }

   void add_digit( char c, char hex_base ){
      if( c > 9 ){
         c = (char) ( c + ( hex_base - 10 ));
      } else {
         c = (char) ( c + '0' );
      }
      add_char( c );
   }

   void add_prefix( const _formatting & format ){
      if( format.show_base ){
         switch( format.numerical_radix ){
            case 2  : add_char( 'b' ); break;
            case 8  : add_char( 'o' ); break;
            case 10 : return;
            case 16 : add_char( 'x' ); break;
            default : add_char( '?' ); break;
         }
         add_char( '0' );
      }
   }
};


// ==========================================================================
//
// formatter
//
// ==========================================================================

struct _strlen {

   static uint_fast16_t __attribute__ ((pure)) strlen( const char *s ){
      uint_fast16_t n = 0;
      while( *s++ ){ ++n; }
      return n;
   }
};

template< is_stream_out_char T >
struct formatter {

   inline static _formatting format;

   //static void GODAFOSS_INLINE init(){
   //   T::init();
   //}

   using resources = use_list< T >;

   static void GODAFOSS_INLINE write( char c ){
      T::write( c );
   }

   static void GODAFOSS_INLINE fill( int_fast16_t n ){
      while( n-- > 0 ){
         T::write( format.fill_char );
      }
   }

   template< typename lambda >
   static void print_aligned( int_fast16_t field_len, lambda print_field ){
      fill( format.fill_count( true, field_len ) );
	  print_field();
      fill( format.fill_count( false, field_len ) );
      format.field_width = 0;
   }

   static void write( const char * s ){
      print_aligned(
         _strlen::strlen( s ),
         [&](){
         for( const char *p = s; *p != '\0'; p++ ){
               write( *p );
            }
         }
      );
   }

   static void write( bool v ){
      write( format.bool_rep( v ) );
   }

   template< typename Tx >
   static void _write_unsigned( Tx x ){
      _print_reverse< 70 > s;

      if( x == 0 ){
          s.add_digit( 0, format.hex_base );
      }

      while( x != 0 ){
         s.add_digit( (char) ( x % format.numerical_radix ), format.hex_base );
         x = x / format.numerical_radix;
      }

      s.add_prefix( format );

      if( format.show_pos ){
         s.add_char( '+' );
      }

      write( s.content );
   }

   template< typename Tx >
   static void _write_signed( Tx x ){
      _print_reverse< 70 > s;

      bool minus = ( x < 0 );
      if( x > 0 ){ x = -x; }

       if( x == 0 ){
          s.add_digit( 0, format.hex_base );
      }

      while( x != 0 ){
         s.add_digit( (char) ( ( - x ) % format.numerical_radix ), format.hex_base );
         x = - ( - x / format.numerical_radix );
      }

      s.add_prefix( format );

      if( minus ){
         s.add_char( '-' );
      } else if( format.show_pos ){
         s.add_char( '+' );
      }

      write( s.content );
   }

   static void write(  unsigned char      x ){ _write_unsigned( x ); }
   static void write(    signed char      x ){ _write_signed( x );   }
   static void write(  unsigned short     x ){ _write_unsigned( x ); }
   static void write(    signed short     x ){ _write_signed( x );   }
   static void write(  unsigned int       x ){ _write_unsigned( x ); }
   static void write(    signed int       x ){ _write_signed( x );   }
   static void write(  unsigned long      x ){ _write_unsigned( x ); }
   static void write(    signed long      x ){ _write_signed( x );   }
   static void write(  unsigned long long x ){ _write_unsigned( x ); }
   static void write(    signed long long x ){ _write_signed( x );   }
};

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

   //ostream(){
   //   T::init();
   //}

   using resources = use_list< T >;

   template< typename V >
      requires requires( V v ){ T::write( v ); }
   auto & GODAFOSS_INLINE write( const V & v ){
      T::write( v );
      return *this;
   }

};

// ========== worker functions ==========

//template< is_ostream ostream, typename T >
template< typename ostream, typename T >
   requires requires( ostream S, T v ){
      { S.write( v ) };
   }
auto & operator<<( ostream & stream, T v ){
   return stream.write( v );
}

