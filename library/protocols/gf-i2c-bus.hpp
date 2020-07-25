// =============================================================================
//
// gf-i2c-bus.hpp
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
// i2c_base
//
// =============================================================================

template< typename T >
concept is_i2c_base = requires(
   uint_fast8_t address,
   uint_fast8_t data
){
   { T::init()                             } -> std::same_as< void >;

   /*
   { T::start_write_transaction( address ) } -> std::same_as< void >;
   { T::write_byte( data )                 } -> std::same_as< void >;
   { T::stop_write_transaction()           } -> std::same_as< void >;

   { T::start_read_transaction( address )  } -> std::same_as< void >;
   { T::read_byte( data )                  } -> std::same_as< uint_fast8_t >;
   { T::stop_read_transaction()            } -> std::same_as< void >;
   */
};


// =============================================================================
//
// i2c_bus
//
// =============================================================================

template<
   is_i2c_base     base,
   is_i2c_profile  profile = i2c_standard
>
struct i2c_bus {

   static constexpr auto frequency = profile::frequency;

   static void init(){
      base::init();
   }

   struct _write_transaction {

      _write_transaction( uint_fast32_t address ){
         base::start_write_transaction( address );
      }

      void write_byte( const uint_fast32_t data ){
         base::write_byte( data );
      }

      void write( const uint8_t data[], int n ){
         for( int i = 0; i < n; ++i ){
            base::write_byte( data[ i ] );
         };
      }

      ~_write_transaction(){
         base::stop_write_transaction();
      }
    };

   struct _read_transaction {

      _read_transaction(  uint_fast8_t address ){
         base::start_read_transaction( address );
      }

      uint_fast8_t read_byte(){
         return base::read_byte();
      }

      void read( uint8_t data[], int n ){
         for( int i = 0; i < n; ++i ){
            data[ i ] = base::read_byte();
         };
      }

      ~_read_transaction(){
         base::stop_read_transaction();
      }
   };

   template< int address >
   struct channel {

      struct write_transaction {

         write_transaction(){
            base::start_write_transaction( address );
         }

         void write_byte( const uint_fast8_t data ){
            base::write_byte( data );
         }

         void write( const uint8_t data[], int n ){
            for( int i = 0; i < n; ++i ){
               base::write_byte( data[ i ] );
            };
         }

         ~write_transaction(){
            base::stop_write_transaction();
         }
      };

      struct read_transaction {

         read_transaction(){
            base::start_read_transaction( address );
         }

         void write( const uint8_t data[], int n ){
            for( int i = 0; i < n; ++i ){
               data[ i ] = base::read_byte();
            };
         }

         uint_fast8_t read_byte(){
            return base::read_byte();
         }

         void read( uint8_t data[], int n ){
            for( int i = 0; i < n; ++i ){
               data[ i ] = base::read_byte();
            };
         }

         ~read_transaction(){
            base::stop_read_transaction();
         }

      };

   };

};
