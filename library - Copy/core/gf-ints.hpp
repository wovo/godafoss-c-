// ==========================================================================
//
// gf-ints.hpp
// 
// ==========================================================================
//
// uint_bits< N >::fast is the smallest 'fast' 
// unsigned integer type that stores (at least) N bits.
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

  
// if no exact match, get something bigger
template< uint64_t n > struct uint_bits {
   
   // not thing bigger than an unsigned long long int
   static_assert( 
      n <= 8 * sizeof( unsigned long long int ),
      "ports, a/d and d/a pins, etc. must fit in an unsigned integer type" 
   );
   
   // try 1 bit bigger
   typedef typename uint_bits< n + 1 >::fast fast;
}; 
   
template<> struct uint_bits< 8 * sizeof( unsigned char ) > {
   typedef unsigned char fast;
};   

#if USHRT_MAX > UCHAR_MAX
template<> struct uint_bits< 8 * sizeof( unsigned short ) > {
   typedef unsigned short fast;
};
#endif   

#if UINT_MAX > USHRT_MAX
template<> struct uint_bits< 8 * sizeof( unsigned int ) > {
   typedef unsigned int fast;
}; 
#endif  

#if ULONG_MAX > UINT_MAX
template<> struct uint_bits< 8 * sizeof( unsigned long int ) > {
   typedef unsigned long int fast;
}; 
#endif  

#if ULLONG_MAX > ULONG_MAX
template<> struct uint_bits< 8 * sizeof( unsigned long long int ) > {
   typedef unsigned long long int fast;
};  
#endif