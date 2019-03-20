// ==========================================================================
//
// gf-ints.hpp
// 
// the basic stuff of the library
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

  
// if no exact match, get something bigger
template< uint64_t n > struct uint_t {
   // but we have to draw a line somewhere
   static_assert( 
      n < 130,
      "ports, a/d and d/a pins, etc. must fit in an unsigned integer type" 
   );
   typedef typename uint_t< n + 1 >::fast fast;
}; 
   
template<> struct uint_t< 8 * sizeof( unsigned char ) > {
   typedef unsigned char fast;
};   

#if USHRT_MAX > UCHAR_MAX
template<> struct uint_t< 8 * sizeof( unsigned short ) > {
   typedef unsigned short fast;
};
#endif   

#if UINT_MAX > USHRT_MAX
template<> struct uint_t< 8 * sizeof( unsigned int ) > {
   typedef unsigned int fast;
}; 
#endif  

#if ULONG_MAX > UINT_MAX
template<> struct uint_t< 8 * sizeof( unsigned long int ) > {
   typedef unsigned long int fast;
}; 
#endif  

#if ULLONG_MAX > ULONG_MAX
template<> struct uint_t< 8 * sizeof( unsigned long long int ) > {
   typedef unsigned long long int fast;
};  
#endif