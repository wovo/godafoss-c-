// =============================================================================
//
// gf-output-stream.hpp
//
// =============================================================================
//
// An output stream is an object that provides the usual output operations.
// Both std::ostream and a godafoss ostream satisfy this concept.
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

// quote ''output_stream'' };
template< typename ostream >
concept output_stream = requires (
   ostream s,
   char c,
   const char * p,
   short int si,
   unsigned short int usi,
   int i,
   unsigned int ui,
   long int li,
   unsigned long int uli,
   long long int lli,
   unsigned long long int ulli
){
   { operator<<( s, c ) }     -> std::same_as< ostream & >;
   { operator<<( s, p) }      -> std::same_as< ostream & >;
   { operator<<( s, si ) }    -> std::same_as< ostream & >;
   { operator<<( s, usi ) }   -> std::same_as< ostream & >;
   { operator<<( s, i ) }     -> std::same_as< ostream & >;
   { operator<<( s, ui ) }    -> std::same_as< ostream & >;
   { operator<<( s, li ) }    -> std::same_as< ostream & >;
   { operator<<( s, uli ) }   -> std::same_as< ostream & >;
   { operator<<( s, lli ) }   -> std::same_as< ostream & >;
   { operator<<( s, ulli ) }  -> std::same_as< ostream & >;
};
