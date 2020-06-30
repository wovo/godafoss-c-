// ==========================================================================
//
// gf-date-time.hpp
//
// ==========================================================================
//
// Datatype for
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


// =============================================================================
//
// date and time datatype
//
// data_and_time fields:
//    - seconds (0-59), minutes (0-59), hours (0-23)
//    - day (1..28/29/30/31), month (1-12), year (0-99)
//    - weekday (1..7)
//
// =============================================================================

struct date_and_time {
   uint8_t seconds, minutes, hours;
   uint8_t day, month, year;
   uint8_t weekday;
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


// =============================================================================
//
// pin_oc
//
// =============================================================================

template< typename stream >
   requires requires( stream & s, char c, uint8_t v ){
      { s << 'c' } -> std::same_as< stream & >;
      { s << v   } -> std::same_as< stream & >;
      // formatters
   }
stream & operator<<( stream & lhs, date_and_time dt ){
   return lhs
      << setfill( '0' ) << setw( 2 ) << right
      << dt.year  << '-'
      << dt.month << '-'
      << dt.day   << ' '
      << dt.hours   << ':'
      << dt.minutes << '.'
      << dt.seconds << ' ' << 'd'
      << dt.weekday;
}
