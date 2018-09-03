// ==========================================================================
//
// gpio.hpp
// 
// GPIO : General Purpose Input Output (Digital Input / Output) stuff
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss 
// free C++ library for close-to-the-hardware programming.
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
// PUBLIC
//
// GPIO concepts
//
// ==========================================================================

// quote ''gpio'' );
template< typename T >
concept bool gpio = requires( 
   T::is_gpio,
   item< bool >,
   simplex< bool >
);

// quote ''gpi'' );
template< typename T >
concept bool gpi = requires( 
   T::is_gpi,
   item< bool >,
   input< bool >
);

// quote ''gpo'' );
template< typename T >
concept bool gpo = requires( 
   T::is_gpo,
   item< bool >,
   output< bool >
);

// quote ''gpoc'' );
template< typename T >
concept bool gpoc = requires( 
   T::is_gpoc,
   item< bool >,
   duplex< bool >
);


// ==========================================================================
//
// PUBLIC
//
// conversion from one GPIO interface to another
//
// ==========================================================================

// quote ''can_gpio'' );
template< typename T >
concept bool can_gpio = requires( 
   gpio< T > | gpoc< T >
);

template< can_gpio T >
struct make_gpio : T {};

template< gpoc T >
struct make_gpio : T {
   
}


// ==========================================================================
//
// PUBLIC
//
// conversion to gpio
//
// ==========================================================================

// quote ''can_gpi'' );
template< typename T >
concept bool can_gpi = requires( 
   gpi< T > | gpoc< T >
);

// quote ''can_gpo'' );
template< typename T >
concept bool can_gpo = requires( 
   gpo< T > | gpoc< T >
);

// quote ''can_gpoc'' );
template< typename T >
concept bool can_gpoc = requires( 
   gpio< T > | gpoc< T >
);

