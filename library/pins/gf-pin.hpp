// ==========================================================================
//
// gf-pin.hpp
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss 
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// in_out
//
// ==========================================================================

// quote ''pin_in_out'' );
template< typename T >
concept bool is_pin_in_out = requires {
   is_item< bool >;
   is_simplex< bool >;
   T::is_pin_in_out;
};

struct be_pin_in_out : 
   be_item< bool >, 
   be_simplex< bool > 
{
   static constexpr bool is_pin_in_out  = true;	
};


// ==========================================================================
//
// out
//
// ==========================================================================

// quote ''pin_out'' );
template< typename T >
concept bool is_pin_out = requires {
   is_item< bool >;
   is_output< bool >;
   T::is_pin_out;
};

struct be_pin_out : 
   be_item< bool >, 
   be_output< bool >
{
   static constexpr bool is_pin_out     = true;	
};


// ==========================================================================
//
// in
//
// ==========================================================================

// quote ''pin_in'' );
template< typename T >
concept bool is_pin_in = requires {
   is_item< bool >;
   is_input< bool >;
   T::is_pin_in;
};

struct be_pin_in : 
   be_item< bool >, 
   be_input< bool > 
{
   static constexpr bool is_pin_in      = true;	
};


// ==========================================================================
//
// oc
//
// ==========================================================================

// quote ''pin_oc'' );
template< typename T >
concept bool is_pin_oc = requires {
   is_item< bool >;
   is_duplex< bool >;
   T::is_pin_oc;
};

struct be_pin_oc : 
   be_item< bool >, 
   be_duplex< bool > 
{
   static constexpr bool is_pin_oc      = true;	
};
