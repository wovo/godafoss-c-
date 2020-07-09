// =============================================================================
//
// gf-date-time.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================


// =============================================================================
//
// @title date and time
// @define date_and_time
//
// This is a datatype for representing a date-and-time,
// intended for use with timekeeping chips or peripherals.
//
// data_and_time fields:
//    - seconds (0-59), minutes (0-59), hours (0-23)
//    - day (1..28/29/30/31), month (1-12), year (0-99)
//    - weekday (1..7)
//
// When the weekday is not known (some chips don't have it), it is set to 0.
//
// =============================================================================

@quote 5 date_and_time
struct date_and_time {
   uint8_t seconds, minutes, hours;
   uint8_t day, month, year;
   uint8_t weekday;
};


// =============================================================================
//
// operator<<
//
// =============================================================================

template< typename stream >
   requires requires( stream & s, char c, uint8_t v ){
      { s << x          } -> std::same_as< stream & >;
      { s << v          } -> std::same_as< stream & >;
      { s << setw( 2 )  } -> std::same_as< stream & >;
      { s << right      } -> std::same_as< stream & >;
      { s << dec        } -> std::same_as< stream & >;
   }
stream & operator<<( stream & lhs, date_and_time dt ){
   return lhs
      << setfill( '0' ) << setw( 2 ) << right << dec
      << dt.year     << '-'
      << dt.month    << '-'
      << dt.day      << ' '
      << dt.hours    << ':'
      << dt.minutes  << '.'
      << dt.seconds  << ' '
      << 'd' << dt.weekday;
}
