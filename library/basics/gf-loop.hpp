// =============================================================================
//
// gf-loop.hpp
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
// @define loop<>
// @title loop
//
// @code 3
// loop< N, unroll >([&] GODAFOSS_INLINE {
//    // loop body
// });
//
// The loop type template repeats the loop body N times.
// The optional unroll parameter selects whether the repeating is
// implemented as the compiler thinks best, or by (forced) unrolling.
// The default is false (no forced unrolling).
//
// Unrolling increases code size. Unrolled code will probably run faster,
// but this effect can be neglectible.
// On a system with memory wait states and a memory read buffer a small
// non-unrolled loop that fits in the read buffer will probably be faster
// than the unrolled version. As always: measure, and check the generated
// assembly.
//
// -----------------------------------------------------------------------------
//
// @section examples
//
// @code 9
// // example, the compiler chooses
// loop< 8 >([&] GODAFOSS_INLINE {
//    mosi::write( ( d_out & 0x80 ) != 0 );
//    wait_half_period();
//    sclk::write( 1 );
//    wait_half_period();
//    d_out = d_out << 1;
//    sclk::write( 0 );
// });
//
// @code 9
// // example, always unrolled
// loop< 8, true >([&] GODAFOSS_INLINE {
//    mosi::write( ( d_out & 0x80 ) != 0 );
//    wait_half_period();
//    sclk::write( 1 );
//    wait_half_period();
//    d_out = d_out << 1;
//    sclk::write( 0 );
// });
//
// These examples show the (simplified) inner loop of
// the-bit banged spi implementation.
//
// The effect of unrolling on the bouncing square demo
// on an Arduino Due (sam3xa cortex-m3 CPU) running at the maximum
// CPU clock frequency (84 Mhz, 5 wait states for Flash access),
// with an 128x64 pixel b/w SPI SSD1306 OLED (SPI set to maximum speed) is:
//
// - no inline: 1364 bytes code, one byte takes 2.3 us, flush takes 2.9 ms
// - inline: 1536 bytes code, one byte takes 1.4 us, flush takes 1.9 ms
//
// =============================================================================

// =============================================================================
//
// A function cannot be specialized.
// Instead, the implementation uses the constructor of a template class.
//
// =============================================================================

// no unrolling: just loop
template< uint64_t N, bool Unroll = false >
struct loop {

   template< typename Body >
   GODAFOSS_INLINE loop( const Body & body ){
      for( uint32_t i = 0; i < N; ++i ){
         body();
      }
   }
};

// unrolling: call myself recursively
template< uint64_t N >
struct loop< N, true >{

   template< typename Body >
   GODAFOSS_INLINE loop( const Body & body ) {
      body();
      loop< N - 1, true > dummy_object( body );
   }
};

// recursion endpoint
template<>
struct loop< 0, true >{

   template< typename Body >
   GODAFOSS_INLINE loop( const Body & body ){
   }
};
