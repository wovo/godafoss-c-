// =============================================================================
//
// gf-i2c-profile.hpp
//
// =============================================================================
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
// =============================================================================


// =============================================================================
//
// @define godafoss::is_i2c_profile
// @title i2c profiles
//
// @insert is_i2c_profile
//
// An i2c profile defines the timing of signals on an i2c bus.
// It is implemented as a template class that takes a timing
// as the template parameter, and provides the bus frequency and
// timings that must be met by the communication on the i2c bus.
//
// @insert standard
// @insert fast
//
// Conform the "I2C-bus specification and user manual,
// 4 April 2014", UM10204.pdf, Table 10, p 48, two profiles
// are defined: i2c_standard (100 kHz) and i2c_fast (400 kHz).
//
// =============================================================================


struct i2c_profile_root {
   static constexpr bool _i2c_profile_marker = true;
};

// @quote is_i2c_profile 11
template< typename T >
concept is_i2c_profile = requires {
   T::_i2c_profile_marker;

/*
   { T::t_hd_sta  ::wait() } -> std::same_as< void >;
   { T::t_low     ::wait() } -> std::same_as< void >;
   { T::t_high    ::wait() } -> std::same_as< void >;
   { T::t_su_dat  ::wait() } -> std::same_as< void >;
   { T::t_su_sto  ::wait() } -> std::same_as< void >;
   { T::t_buf     ::wait() } -> std::same_as< void >;
*/
};


// =============================================================================

// @quote standard 12
struct i2c_standard : i2c_profile_root {

   static constexpr int64_t frequency = 100'000;

   template< is_timing_wait timing >
   struct intervals {
      using t_hd_sta  = typename timing::ns< 4'000 >;
      using t_low     = typename timing::ns< 4'700 >;
      using t_high    = typename timing::ns< 4'000 >;
      using t_su_dat  = typename timing::ns<   250 >;
      using t_su_sto  = typename timing::ns< 4'000 >;
      using t_buf     = typename timing::ns< 4'700 >;
   };
};


// =============================================================================

// @quote fast 12
struct i2c_fast : i2c_profile_root {

   static constexpr int64_t frequency = 400'000;

   template< is_timing_wait timing >
   struct intervals {
      using t_hd_sta  = typename timing::ns<   600 >;
      using t_low     = typename timing::ns< 1'300 >;
      using t_high    = typename timing::ns<   600 >;
      using t_su_dat  = typename timing::ns<   100 >;
      using t_su_sto  = typename timing::ns<   600 >;
      using t_buf     = typename timing::ns< 1'300 >;
   };
};
