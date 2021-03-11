// ==========================================================================
//
// gf-timing-static-duration.hpp
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
// @define godafoss::is_static_duration
// @title static_duration
//
// @insert is_static_duration
//
// A static_duration is a cto that represents a duration (amount of time).
// It provides two functions: wait() and wait_busy().
//
// A wait() call will return after at least the amount of time
// static_duration represents, but may well take longer due to background
// work being done.
//
// A wait_busy() call will return after the amount of time
// static_duration represents, without further delay.
// For very small delays, a wait_busy() call might be
// implemented as a few in-lined machine instructions.
//
// @define static_duration_root
// @insert static_duration_root
//
// All static durations inherit from static_duration_root.
//
// @define can_static_duration<>
// @insert can_static_duration
//
// The can_static_duration<> concept matches cto's that are acceptable to
// static_duration<>.
//
// @define static_duration
// @insert static_duration
//
// The static_duration<> adapter accepts a cto that matches
// the can_static_duraction<> concept, and yields a cto that is
// only a static_duration.
//
// =============================================================================


// @quote is_static_duration 6
template< typename T >
concept is_static_duration = requires {
      T::_static_duration_marker;
      { T::wait() } -> std::same_as< void >;
      { T::wait_busy() } -> std::same_as< void >;
   } && is_cto< T  >;

// @quote static_duration_root 3
struct static_duration_root : cto_root {
   static const bool _static_duration_marker = true;
};

// @quote can_static_duration 3
template< typename T >
concept can_static_duration =
   is_static_duration< T >;

// @quote static_duration 2 ... };
template< can_static_duration T >
struct static_duration : static_duration_root {

   //static void GODAFOSS_INLINE __init(){
   //   T::init();
   //}

   using resources = use_list< T >;

   static void GODAFOSS_INLINE wait(){
      T::wait();
   }

   static void GODAFOSS_INLINE wait_busy(){
      T::wait_busy();
   }

};
