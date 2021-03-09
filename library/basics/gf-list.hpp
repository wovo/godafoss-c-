// ==========================================================================
//
// gf-list.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2021
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

// https://codereview.stackexchange.com/questions/127925/typelist-with-extractor

template< typename ...Args >
struct list
{
    template< std::size_t N >
    using type = typename std::tuple_element< N, std::tuple< Args... >>::type;
    
};

// https://stackoverflow.com/questions/34314193/iterating-over-different-types

