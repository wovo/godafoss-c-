// ==========================================================================
//
// gf-string.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// @title string
// @define godafoss::string
// @insert string
//
// This is a fixed-maximum-size string.
// It offers an alternative to std::string and raw 0-terminated char arrays.
// It doesn't use the heap, and doesn't cause Undefined Behaviour with
// buffer overflows or out-of-bounds indexes.
//
// The functions that extend the string by appending characters do so
// up to the maximum length of the string. Appending characters beyond
// this maximum length has no effect: excess characters are ignored.
//
// The functions that access a character at an index (a position within
// the stored string) do so only when the index is valid. When the index
// is invalid, an undefined character (or for an lvalue,
// a reference to an undefined character) is returned.
//
// @section attributes
//
// @insert size
// The maxmimum_length is the maxiumum number of character that can be
// stored by the string.
//
// @bar
// @insert length
// The member function length() returns number of characters
// that are currently stored.
//
// @bar
// @insert valid_index
// The member function valid_index( n ) returns whether n is a valid index
// into the curretly stored string of characters.
//
// @bar
// @insert append
// The append function, the operator+= and the operator<< all append a
// single character to the string. If the string is already at its maximum
// length the character is ignored.
//
// =============================================================================


// =============================================================================
//
// iterate over something that can be used to
// construct a godafoss::string
//
// =============================================================================

class _char_iterate {

private:

   const char c;
   const char * const start;
   const char * const beyond;

   // end iterator from an asciz string
   constexpr const char * asciz_beyond( const char * p ){
      while( *p != '\0' ){
         ++p;
      }
      return p;
   }

public:

   // a single char
   constexpr _char_iterate( char ch ) :
      c( ch ), start( &c ), beyond( &this->c + 1)
   {}

   // an asciz string
   constexpr _char_iterate( const char *p ) :
      c( 0 ), start( p ), beyond( asciz_beyond( p )) {
   }

   // anything that has begin() and end()
   template< typename T >
   requires requires( const T s ){
      // use an std conecpt when one is available
      { * s.begin() } -> std::same_as< char >;
      { * s.end() }   -> std::same_as< char >;
   }
   constexpr _char_iterate( const T & s ) :
      c( 0 ), start( s.begin() ), beyond( s.end() )
   {}

   constexpr const char * begin() const {
      return start;
   }

   constexpr const char * end() const {
      return beyond;
   }

};


// =============================================================================
//
// string<> class
//
// =============================================================================

// @quote string 2 ... }
template< std::size_t _maximum_length >
struct string {

public:

   // @quote size 2
   using size_t = std::size_t;
   static constexpr size_t maximum_length = _maximum_length;

private:

   // the store for the characters
   char content[ maximum_length ];

   // current length (number of valid characters) of the string
   size_t current_length;

   // dummy char returned on out-of-bounds accesses
   static char dummy;

public:

   //=========================================================================
   //
   // miscellaneous
   //
   //=========================================================================

   // special value for beyond-end or not-found
   static const size_t nsize = 0xFFFF;

   // @quote length 1 ... }
   constexpr size_t length() const {
      return current_length;
   }

   // @quote valid_index 1 ... }
   constexpr bool valid_index( const size_t n ) const {
      return( n < maximum_length );
   }

   // output to any target that supports operator<< for a char
   template< typename sink >
   requires requires( sink s, char c ){
      { s << c };
   }
   friend sink & operator<< ( sink & lhs, const string & rhs ){
      for( char c : rhs ){
         lhs << c;
      }
      return lhs;
   }


   //=========================================================================
   //
   // append something (as far as possible, ignore the excess)
   //
   //=========================================================================

   // @quote append 1 ... }
   string & append( char c ){
      if( current_length < maximum_length ){
         content[ current_length++ ] = c;
      }
      return *this;
   }

   // @quote append 1 ... }
   string & operator+=( char c ){
      return append( c );
   }

   // @quote append 1 ... }
   string & operator<<( char c ){
      return append( c );
   }

   template< typename T >
   // must be a readonly char forward iterator
   string & operator+=( const T & rhs ){
      for( char c : iterate( rhs ) ){
         append( c );
      }
      return *this;
   }

   template< typename T >
   // idem??
   string & operator<<( const T & rhs ){
      return operator+=( rhs );
   }


   //=========================================================================
   //
   // assign
   //
   //=========================================================================

   // set to ""
   string & clear(){
      current_length = 0;
      return *this;
   }

   // assign something
   template< typename T >
   string & operator=( const T & rhs ){
      return clear() += rhs;
   }


   //=========================================================================
   //
   // iterators
   //
   //=========================================================================

   // start
   char * begin() {
      return content;
   }

   // end
   char * end() {
      return content + current_length;
   }

   // start const
   const char * begin() const {
      return content;
   }

   // end const
   const char * end() const {
      return content + current_length;
   }


   //=========================================================================
   //
   // range
   //
   //=========================================================================

   /// non-owning string (sub)range from two iterators
   ///
   /// This function returns a const string object that appears to contains
   /// the characters range pointed to by the start parameter up to
   /// (but excluding) the character pointed to by the end parameter.
   ///
   /// The object is a non-owning range: it doesn't make a copy.
   /// Hence any change to the characters will be reflected in the range.
   static constexpr const string range(
      const char * start,
      const char * beyond
   ){
      // Casting the const away is OK because the returned range object is
      // returned as const, so the stored char pointers are const again.
      // Blame C++ for not having a const constructor :(
      return string(
         const_cast< char * >( start ),
         const_cast< char * >( beyond )
      );
   }

/*
   /// non-owning string (sub)range from an asciz string
   ///
   /// This function returns a const string object that appears to contains
   /// the characters in the asciz string pointed to by the start parameter.
   ///
   /// The object is a non-owning range: it doesn't make a copy.
   /// Hence any change to the characters will be reflected in the range.
   static constexpr const string range(
      const char * start
   ){
      // see above
      return string(
         const_cast< char * >( start ),
         const_cast< char * >( asciz_beyond( start ) )
      );
   }
*/


   //=========================================================================
   //
   // erase & replace:
   // remove a substring
   //
   // replace_inplace
   // inplace_replace
   // inplace_erase
   //
   //=========================================================================

   // TBW


   //=========================================================================
   //
   // find
   //
   //=========================================================================

   /// check whether a substring occurs in a string at a given position
   ///
   /// Searches the string for the first occurrence of the
   /// sequence specified by its arguments.
   ///
   /// When pos is specified, the search only includes characters
   /// at or after position pos, ignoring any possible occurrences
   /// that include characters before pos.
   template< typename T >
   bool find_at( const T & s, size_t pos = 0 ) const {
      for( char c : iterate( s ) ){
         if( ( pos >= current_length ) || ( content[ pos++ ] != c ) ){
            return false;
         }
      }
      return true;
   }

   /// find a substring in a string
   ///
   /// Searches the string for the first occurrence of the
   /// sequence specified by its arguments.
   ///
   /// When pos is specified, the search only includes characters
   /// at or after position pos, ignoring any possible occurrences
   /// that include characters before pos.
   ///
   /// When no match is found, nsize is returned.
   template< typename T >
   size_t find( const T & s, size_t pos = 0 ) const {
      while( pos < current_length ){
         if( find_at( s, pos ) ){
            return pos;
         }
         pos++;
      }
      return nsize;
   }

   /// find a substring in a string
   ///
   /// Searches the string for the last occurrence of the
   /// sequence specified by its arguments.
   ///
   /// When pos is specified, the search only includes characters
   /// at or after position pos, ignoring any possible occurrences
   /// that include characters before pos.
   ///
   /// When no match is found, nsize is returned.
   template< typename T >
   size_t rfind( const T & s, size_t pos = 0 ) const {
      size_t i = current_length;
      while( pos < i ){
         if( find_at( s, i ) ){
            return i;
         }
         i--;
      }
      return nsize;
   }


   //=========================================================================
   //
   // operator[]
   //
   //=========================================================================

   /// return n'th character reference
   ///
   /// Return a reference to the n'th character if the index is valid,
   /// otherwise return the dummy character.
   char & operator[]( int n ){
      return valid_index( n ) ? content[ n ] : dummy;
   }

   /// return n'th character value
   ///
   /// Return the value of the n'th character if the index is valid,
   /// otherwise return the dummy character value.
   char operator[]( int n ) const {
      return valid_index( n ) ? content[ n ] : '?';
   }


   //=========================================================================
   //
   // ranges
   //
   //=========================================================================

private:

   // return a pointer into the context, clipped to the valid range
   const char * sanitize( size_t n ) const {
      return ( n <= current_length )
         ? content + n
         : content + current_length;
   }

   char * sanitize( size_t n ){
      return ( n <= current_length )
         ? content + n
         : content + current_length;
   }

public:

   /// subrange based on start index and length
   string range_start_length( size_t a, size_t b ){
      return string_base( sanitize( a ), sanitize( a + b ) );
   }

   /// subrange based on start index and end index
   string range_start_end( size_t a, size_t b ){
      return string_base( sanitize( a ), sanitize( b ) );
   }

   /// subrange based on substring start and length
   template< typename T >
   string range_find_length( const T & a, size_t b ){
      return string_base( sanitize( a ), sanitize( a + b ) );
   }

   /// subrange based on first substring start and second substring start
   template< typename T, typename Q >
   string range_find_find( const T & a, const Q & b ){
      return string_base( sanitize( a ), sanitize( a + b ) );
   }


   //=========================================================================
   //
   // compare
   //
   //=========================================================================

   /// compare for equality
   template< typename T >
   bool operator==( const T & rhs ) const {
      const char * p = content;
      for( const char c : iterate( rhs ) ){
          if( p == end() ){
             return false;
          }
          if( c != *p++ ){
             return false;
          }
      }
      return p == end();
   }

   /// compare for inequality
   template< typename T >
   bool operator!=( const T & rhs ) const {
      return ! ( *this == rhs );
   }

   /// compare for larger than
   template< typename T >
   bool operator>( const T & rhs ) const {
      const char * p = content;
      for( const char c : iterate( rhs ) ){
          if( p == end() ){
             return false;
          }
          if( *p++ <= c ){
             return false;
          }
      }
      return true;
   }

   /// compare for larger than or equal
   template< typename T >
   bool operator>=( const T & rhs ) const {
      const char * p = content;
      for( const char c : iterate( rhs ) ){
          if( p == end() ){
             return false;
          }
          if( *p++ < c ){
             return false;
          }
      }
      return true;
   }

   /// compare for smaller than
   template< typename T >
   bool operator<( const T & rhs ) const {
      const char * p = content;
      for( const char c : iterate( rhs ) ){
          if( p == end() ){
             return true;
          }
          if( *p++ >= c ){
             return false;
          }
      }
      return false;
   }

   /// compare for smaller than or equal
   template< typename T >
   bool operator<=( const T & rhs ) const {
      const char * p = content;
      for( const char c : iterate( rhs ) ){
          if( p == end() ){
             return true;
          }
          if( *p++ > c ){
             return false;
          }
      }
      return false;
   }

};


//============================================================================
//
// reverse compare
//
//============================================================================

/*

/// compare for equality
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator==( const T & lhs, const string_base & rhs ){
   return rhs.operator==( lhs );
}

/// compare for inequality
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator!=( const T & lhs, const string_base & rhs ){
   return rhs.operator!=( lhs );
}

/// compare for large than
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator>( const T & lhs, const string_base & rhs ){
   return ! rhs.operator<=( lhs );
}

/// compare for large than or equal
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator>=( const T & lhs, const string_base & rhs ){
   return ! rhs.operator<( lhs );
}

/// compare for smaller than
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator<( const T & lhs, const string_base & rhs ){
   return ! rhs.operator>=( lhs );
}

/// compare for smaller than or equal
template< typename T >
typename std::enable_if <
   ! std::is_base_of< string_base, T >::value, bool >::type
operator<=( const T & lhs, const string_base & rhs ){
   return ! rhs.operator>( lhs );
}

*/
