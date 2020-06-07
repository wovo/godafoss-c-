// =============================================================================
//
// gf-pin.hpp
//
// =============================================================================
//
// Items that represent micro-controller (or preipheral chip)
// GPIO (general-purpose input-output interface,
// the more limited input-only and output-only versions,
// and the open-collector variant.
//
// =============================================================================
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
// =============================================================================


// =============================================================================
//
// pin_in_out
//
// =============================================================================

// quote ''pin_in_out'' );
template< typename T >
concept pin_in_out = requires {
   item< bool >;
   simplex< bool >;
   T::_pin_in_out_marker;
};

struct pin_in_out_root :
   item_root< bool >,
   simplex_root< bool >
{
   static constexpr bool _pin_in_out_marker = true;
};


// =============================================================================
//
// pin_out
//
// =============================================================================

// quote ''pin_out'' );
template< typename T >
concept pin_out = requires {
   item< bool >;
   output< bool >;
   T::_pin_out_marker;
};

struct pin_out_root :
   item_root< bool >,
   output_root< bool >
{
   static constexpr bool _pin_out_marker = true;
};


// =============================================================================
//
// pin_in
//
// =============================================================================

// quote ''pin_in'' );
template< typename T >
concept pin_in = requires {
   item< bool >;
   input< bool >;
   T::_pin_in_marker;
};

struct pin_in_root :
   item_root< bool >,
   input_root< bool >
{
   static constexpr bool _pin_in_marker = true;
};


// =============================================================================
//
// pin_oc
//
// =============================================================================

// quote ''pin_oc'' );
template< typename T >
concept pin_oc = requires {
   item< bool >;
   duplex< bool >;
   T::_pin_oc_marker;
};

struct pin_oc_root :
   item_root< bool >,
   duplex_root< bool >
{
   static constexpr bool _pin_oc_marker = true;
};
