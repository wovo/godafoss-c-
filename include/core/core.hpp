// ==========================================================================
//
// basics.hpp
// 
// the basic stuff of the library
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
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
// interface elements
//
// ==========================================================================

// quote ''box'' };
template< typename T >
concept bool box = requires( 
   T::is_box,
   typename T::value_type v,
   typename T::is_item | typename T::is_stream
){  
   { T::init() } -> void;
};

// quote ''item'' );
template< typename T >
concept bool item = requires( 
   T::is_item,
   box< T >
);

// quote ''stream'' );
template< typename T >
concept bool stream = requires( 
   T::is_stream,
   box< T >
);

// quote ''input'' };
template< typename T >
concept bool input = requires( 
   T::is_input,
   box< T >,
   typename T::value_type
){
   refresh() -> void,
   read() -> value_type
};

// quote ''output'' };
template< typename T >
concept bool output = requires( 
   T::is_output,
   box< T >,
   typename T::value_type
){
   write( value_type ) -> void,
   flush() -> void
};

// quote ''input_output'' );
template< typename T >
concept bool input_output = requires( 
   input< T >,
   output< T >
);

// quote ''duplex'' );
template< typename T >
concept bool duplex = requires( 
   T::is_duplex,
   input_output< T >
);

// quote ''simplex'' };
template< typename T >
concept bool simplex = requires( 
   T::is_simplex,
   input_output< T >
){
   direction_set_input() -> void,
   direction_set_output() -> void,
   direction_flush() -> void
};


// ==========================================================================
//
// PUBLIC
//
// digital interfaces
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

// quote ''can_gpio_etc'' end_can_gpio_etc
template< typename T >
concept bool can_gpio = requires( 
   gpio< T > | gpoc< T >
);

template< typename T >
concept bool can_gpi = requires( 
   gpi< T > | gpoc< T >
);

template< typename T >
concept bool can_gpo = requires( 
   gpo< T > | gpoc< T >
);

template< typename T >
concept bool can_gpoc = requires( 
   gpio< T > | gpoc< T >
);
// end_can_gpio_etc

// ==========================================================================
//
// PUBLIC
//
// analog interfaces
//
// ==========================================================================

// quote ''adc'' };
template< typename T >
concept bool adc = requires( 
   T::is_adc,
   item< T >,
   input< T >,
   constexpr T::value_type adc_min,
   constexpr T::value_type adc_max
);

// quote ''dac'' };
template< typename T >
concept bool dac = requires( 
   T::is_dac,
   item< T >,
   output< T >,
   constexpr T::value_type dac_min,
   constexpr T::value_type dac_max
);


// ==========================================================================
//
// PUBLIC
//
// invert
//
// ==========================================================================

template< typename T >
concept bool can_invert = requires( 
   box< T >,
   T::invert( T::value_type ) -> T::value_type
);

template< can_invert T >
struct invert : _invert_read< _invert_write< T >>;

template< typename T >
struct _invert_read : T {};

template< input T >
struct _invert_read : T {
   static auto read() -> T::value_type {
      return T::invert( T::read() );
   }
};
   
template< typename T >
struct _invert_write : T {};

template< input T >
struct _invert_write : T {
   static void write( T::value_type v ) {
      T::write( T::invert( v ));
   }
};
   
   
// ==========================================================================
//
// PUBLIC
//
// direct
//
// ==========================================================================

template< typename T >
concept bool can_direct = requires( 
   box< T >
);

template< can_direct T >
struct direct : _direct_read< _direct_write< _direct_direction < T >>>;

template< typename T >
struct _direct_read : T {};

template< input T >
struct _diret_read : T {
   static auto read() -> T::value_type {
      T::refresh();
      return T::invert( T::read() );
   }
};
   
template< typename T >
struct _direct_write : T {};

template< input T >
struct _direct_write : T {
   static void write( T::value_type v ) {
      T::write( T::invert( v ));
      T::flush();
   }
};
   
template< typename T >
struct _direct_direction : T {};

template< simplex T >
struct _direct_direction : T {
   static void direction_set_input() {
      T::direction_set_input();
      T::direction_flush();
   }
   static void direction_set_output() {
      T::direction_set_output();
      T::direction_flush();
   }
};
   
