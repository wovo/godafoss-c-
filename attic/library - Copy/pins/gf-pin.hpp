// ==========================================================================
//
// gf-pin.hpp
//
// ==========================================================================
//
// Items that represent micro-controller (or preipheral chip) 
// GPIO (general-purpose input-output interface,
// the more limited input-only and output-only versions,
// and the open-collector variant.
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
// pin_in_out
//
// ==========================================================================

// quote ''pin_in_out'' );
template< typename T >
concept is_pin_in_out = requires {
   is_item< bool >;
   is_simplex< bool >;
   T::is_pin_in_out;
};

struct be_pin_in_out : 
   be_item< bool >, 
   be_simplex< bool > 
{
   static constexpr bool is_pin_in_out = true;	
};


// ==========================================================================
//
// pin_out
//
// ==========================================================================

// quote ''pin_out'' );
template< typename T >
concept is_pin_out = requires {
   is_item< bool >;
   is_output< bool >;
   T::is_pin_out;
};

struct be_pin_out : 
   be_item< bool >, 
   be_output< bool >
{
   static constexpr bool is_pin_out = true;	
};


// ==========================================================================
//
// pin_in
//
// ==========================================================================

// quote ''pin_in'' );
template< typename T >
concept is_pin_in = requires {
   is_item< bool >;
   is_input< bool >;
   T::is_pin_in;
};

struct be_pin_in : 
   be_item< bool >, 
   be_input< bool > 
{
   static constexpr bool is_pin_in = true;	
};


// ==========================================================================
//
// pin_oc
//
// ==========================================================================

// quote ''pin_oc'' );
template< typename T >
concept is_pin_oc = requires {
   is_item< bool >;
   is_duplex< bool >;
   T::is_pin_oc;
};

struct be_pin_oc : 
   be_item< bool >, 
   be_duplex< bool > 
{
   static constexpr bool is_pin_oc = true;	
};
