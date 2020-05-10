// ==========================================================================
//
// gf-output-stream.hpp
//
// ==========================================================================
//
// An output stream is an object that provides the usual output operations.
// Both std::ostream and a godafoss ostream satisfy this concept.
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2018-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// quote ''output_stream'' };
template< typename T >
concept is_output_stream = requires {  
   true;
};