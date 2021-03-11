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
   true;
//   { T::init()                             } -> std::same_as< void >;

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

template< typename T >
concept is_i2c_bus = requires(
   uint_fast8_t address,
   uint_fast8_t data
){
   true;
//   { T::init()                             } -> std::same_as< void >;

   /*
   { T::start_write_transaction( address ) } -> std::same_as< void >;
   { T::write_byte( data )                 } -> std::same_as< void >;
   { T::stop_write_transaction()           } -> std::same_as< void >;

   { T::start_read_transaction( address )  } -> std::same_as< void >;
   { T::read_byte( data )                  } -> std::same_as< uint_fast8_t >;
   { T::stop_read_transaction()            } -> std::same_as< void >;
   */
}
   && T::i2c_bus_marker;


// =============================================================================
//
// i2c_bus
//
// =============================================================================

template<
   is_i2c_base     base,
   is_i2c_profile  profile
>
struct i2c_bus {

   static constexpr bool i2c_bus_marker = true;

   static constexpr auto frequency = profile::frequency;

   using resources = use_list< base >;

   struct _write_transaction {

      _write_transaction( uint_fast8_t address ){
         base::start_write_transaction( address );
      }

      void write( const uint8_t data ){
         base::write_byte( data );
      }

      template< unsigned int n >
      void write(
         const std::array< uint8_t, n > & data,
	      int_fast16_t amount = n
      ){
         for( int i = 0; i < amount; ++i ){
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

      uint8_t read(){
         return base::read();
      }

      template< unsigned int n >
      void read(
         std::array< uint8_t, n > & data,
	      int_fast16_t amount = n
      ){
         for( int i = 0; i < amount; ++i ){
            data[ i ] = base::read();
         };
      }

      ~_read_transaction(){
         base::stop_read_transaction();
      }
   };

   template< int address >
   struct channel {

      using resources = use_list< base >;

      struct write_transaction {

         write_transaction(){
            base::start_write_transaction( address );
         }

         void write( const uint8_t data ){
            base::write( data );
         }

         template< unsigned int n >
         void write(
            const std::array< uint8_t, n > & data,
            int_fast16_t amount = n
         ){
            for( int i = 0; i < amount; ++i ){
               base::write( data[ i ] );
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

         uint8_t read(){
            return base::read();
         }

         template< unsigned int n >
         void read(
            std::array< uint8_t, n > & data,
	         int_fast16_t amount = n
         ){
            for( int i = 0; i < amount; ++i ){
               data[ i ] = base::read();
            };
         }

         ~read_transaction(){
            base::stop_read_transaction();
         }

      };

   };

};
