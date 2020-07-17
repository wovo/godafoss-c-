// =============================================================================
//
// gf-date-and-time.hpp
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
// @define godafoss::date_and_time
//
// @insert struct
//
// This is a datatype for representing a date-and-time,
// intended for use with timekeeping chips or peripherals.
//
// @section attributes
//
// @insert attributes
//
// The atributes of data_and_time are:
//    - seconds (0-59), minutes (0-59), hours (0-23)
//    - day (1..28/29/30/31), month (1-12), year (0-99)
//    - weekday (1..7)
//
// Fields that are unknow (some chips don't have a weekday ) are set to 0.
//
// @section non-member functions
//
// @insert operator<<
//
// The operator<< prints a data_and_time in the format YY-MM-DD HH:MM.SS dW.
//
// =============================================================================

// @quote struct 1 ... };
struct date_and_time {

   // @quote attributes 3
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
      { s << c          } -> std::same_as< stream & >;
      { s << v          } -> std::same_as< stream & >;
      { s << setw( 2 )  } -> std::same_as< stream & >;
      { s << right      } -> std::same_as< stream & >;
      { s << dec        } -> std::same_as< stream & >;
   }
// @quote operator<< 1 ... }
stream & operator<<( stream & lhs, by_const< date_and_time > dt ){
   return lhs
      << setfill( '0' ) << setw( 2 ) << right << dec
      << dt.year     << '-'
      << dt.month    << '-'
      << dt.day      << ' '
      << dt.hours    << ':'
      << dt.minutes  << '.'
      << dt.seconds  << ' '
      << setw( 1 ) << 'd' << dt.weekday;
}
