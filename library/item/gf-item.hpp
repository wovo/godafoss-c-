// ==========================================================================
//
// gf-item.hpp
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
// @define godafoss::is_cto
// @title cto
//
// A cto is a Compile Time Object: it has the role of an object,
// but it is 'created' at compile time.
// It is implemented as a struct
// that has only static functions and static attributes.
//
// A cto always exists: it is just 'waiting' to be used.
// The features of a cto that are not used will be elimiated by the linker,
// Hence the mere presence of a cto in the source doen not increase
// the size of the executable image.
//
// A cto, being a type, is never instantiated.
// Instead, each cto provides an init() function.
// Before any of its functions or attributes are used at run-time,
// a cto must be initialized by calling its init() function.
// Failing to do so can cause unpredictable results.
//
// As a cto has only static functions and attributes
// it can be used directly, or the cto can be passed as
// a template parameter.
//
// For cto, and for each more specific cto, a concept exists
// (with the name of the cto), and a root struct from which all
// such cto's are derived (with _root appended to the name of the cto).
//
// The concept checks both for a specific marker, which serves
// no other purpose than to identify the specific cto, and for the
// features that the cto is obliged to offer.
// The concept is used to constrain templates that want to accept
// only a cto that implements a specific set of features.
//
// The root can be a plain struct, but it is often a template.
// For more complex cto's the CRTP pattern is used so the root can
// provide both base properties and enrichment based on the provided
// implementation.
//
// @insert is_cto
//
// A cto has a static init() function that can be called without arguments.
//
// @insert cto_root
//
// The struct cto_root is the root type of all cto's:
// all cto's inherit (in most cases indirectly) from this struct.
//
//
// =============================================================================

// temporary
template< typename T >
concept has_init = requires {
   { T::init() } -> std::same_as< void >;
};

// @quote is_cto 5
template< typename T >
concept is_cto = requires {
   T::_cto_marker;
//   { T::init() } -> std::same_as< void >;
} && ! has_init< T >;

// @quote cto_root 3
struct cto_root : not_constructible {
   static const bool _cto_marker = true;
};


// =============================================================================
//
// @define godafoss::is_item
// @title item
//
// An item is the basic cto from which most other cto's are derived.
//
// A summary of terms:
//    - cto: a compile-time (static) object
//    - item: holds some data elements(s))
//    - box: item that always holds one element of the data
//    - pipe: item that holds a sequence of data elements
//    - input: item that supports read()
//    - output: item that supports write()
//    - input_output: both input and output
//    - duplex: both input and output at the same time
//    - simplex: both input and output, but not at the same time
//
// @insert is_item
//
// An item is a cto that holds one (or, in case of a pipe, more)
// data elements of a specific type.
//
// @define godafoss::item::value_type
// @insert item_root
//
// All items inherit (in most cases indirectly) from the struct item_root.
//
// =============================================================================


// @quote is_item 6
template< typename T, typename VT = T::value_type >
concept is_item = requires {
      T::_item_marker;
      { typename T::value_type() } -> std::same_as< VT >;
   }
   && is_cto< T >;

// @quote item_root 5
template< typename VT >
struct item_root : cto_root {
   static const bool _item_marker = true;
   using value_type = VT;
};

// item< bool > has a different invert than other items
template<>
struct item_root< bool > : cto_root {
   static const bool _item_marker = true;
   using value_type = bool;

   GODAFOSS_INLINE static bool invert( bool v ){
      return !v;
   }
};


// =============================================================================
//
// @define godafoss::is_box
// @define godafoss::is_pipe
// @title box, pipe
//
// A box and a pipe are two kinds of items.
// The difference is their semantics: a box behaves like a variable
// that holds a single value,
// a pipe behaves like a sequence of values.
//
// -----------------------------------------------------------------------------
//
// @section box
// @insert is_box
// @insert box_root
//
// A box is an item that has or contains (at any point in time) a single value.
// A box has value semantics:
// when you read from a box twice in rapid succession,
// you will get the same value.
// Writing to an item overwrites its old value in the box.
//
// -----------------------------------------------------------------------------
//
// @section pipe
// @insert is_pipe
// @insert pipe_root
//
// A pipe is an item that holds a sequence of values.
// A write to a pipe adds a new value the sequence.
// Hence all writes to a stream matter, including
// repeated writes of the same value.
// Reading from a pipe is destructive: it consumes the value that was
// read from the sequence.
// Writing to a pipe adds a value to the sequnce.
//
// =============================================================================

// @quote is_box 5
template< typename T, typename VT = T::value_type >
concept is_box = requires {
      T::_box_marker;
   }
   && is_item< T, VT >;

// @quote box_root 6
template< typename VT >
struct box_root :
   item_root< VT >
{
   static const bool _box_marker = true;
};


// =============================================================================

// @quote is_pipe 5
template< typename T, typename VT = T::value_type >
concept is_pipe = requires {
      T::_pipe_marker;
   }
   && is_item< T, VT >;

// @quote pipe_root 6
template< typename VT >
struct pipe_root :
   item_root< VT >
{
   static const bool _pipe_marker = true;
};


// =============================================================================
//
// @define godafoss::is_input
// @define godafoss::is_output
// @define godafoss::is_input_output
// @define godafoss::is_duplex
// @define godafoss::is_simplex
// @title input, output
//
// An item can be an input (from which you can read) and/or an output
// (to which you can write).
//
// An input or output item can be buffered.
// For an output, this means that the effect of write operations
// can be postponed until the next flush call.
// For an input, this means that a read operation reflects
// the situation immediately before that last refresh call, or later.
// For immediate effect on a buffered item, a read must be preceded
// by a refresh, and a write must be followed by a flush.
//
// The direct<> decorator creates an item for which the
// read() and write() operations have
// @noref direct effect.
//
// An item can be an input, an output, or both.
// When it is an input you can read from it,
// when it is an output you can write to it.
// (In theory an item could be neither,
// but that would not be very useful.)
//
// When an item is both input and output it can be simplex
// (sometimes call half-duplex) or duplex.
// A duplex box can, at any time, be both read and written.
//
// @define godafoss::item::refresh
// @define godafoss::item::read
// @section input
// @insert is_input
// @insert input_root
//
// A input is an item that provides a read() function that returns
// a value of the value_type of the item.
//
// @define godafoss::item::write
// @define godafoss::item::flush
// @section output
// @insert is_output
// @insert output_root
//
// An output is an item that provides a write() function that accepts
// a value of the value_type of the item.
//
// @section input_output
// @insert is_input_output
// @insert input_output_root
//
// An input_output is an item that is both an input and an output.
//
// @section direction
//
// A duplex item is an input_output that can function both as
// an input and as an output at the same time.
//
// @insert is_duplex
// @insert duplex_root
//
// A simplex item is an input_output that has a current direction,
// which can be input or output.
//
// @define godafoss::item::direction_set_input
// @define godafoss::item::direction_set_output
// @define godafoss::item::direction_flush
// @insert is_simplex
// @insert simplex_root
//
// The direction of a simplex item can be changed
// with a direction_set_input or direction_set_output call.
// For a successful read, the direction of a simplex box must be input.
// For a successful write, the direction of a simplex box must be output.
// Otherwise a write can have no effect at all, or have a delayed effect,
// and a read returns an unspecified value, and for a stream it can either
// consume the value or not.
//
// The effect of calling a direction_set... function can be delayed
// up to the next direction_flush() call.
// Like for read() and write(), direct<> can be used
// to get an immediate effect.
//
// =============================================================================

// @quote is_input 7
template< typename T, typename VT = T::value_type >
concept is_input = requires {
      T::_input_marker;
      { T::refresh() }  -> std::same_as< void >;
      { T::read() }     -> std::same_as< typename T::value_type >;
   }
   && is_item< T, VT >;

// @quote input_root 6
template< typename VT >
struct input_root :
   item_root< VT >
{
   static const bool _input_marker = true;
};


// =============================================================================

// @quote is_output 7
template< typename T, typename VT = T::value_type >
concept is_output = requires (
      typename T::value_type v
   ){
      T::_output_marker;
      { T::write( v ) }  -> std::same_as< void >;
      { T::flush()    }  -> std::same_as< void >;
   }
   && is_item< T, VT >;

// @quote output_root 6
template< typename VT >
struct output_root :
   item_root< VT >
{
   static const bool _output_marker = true;
};


// =============================================================================

// @quote is_input_output 11
template< typename T, typename VT = T::value_type >
concept is_input_output = requires (
      typename T::value_type v
   ){
      T::_input_output_marker;
      { T::refresh() }   -> std::same_as< void >;
      { T::read() }      -> std::same_as< typename T::value_type >;
      { T::write( v ) }  -> std::same_as< void >;
      { T::flush()    }  -> std::same_as< void >;
   }
   && is_item< T, VT >;

// @quote input_output_root 8
template< typename VT >
struct input_output_root :
   input_root< VT >,
   output_root< VT >
{
   static const bool _input_output_marker = true;
};


// =============================================================================

// @quote is_duplex 5
template< typename T, typename VT = T::value_type >
concept is_duplex = requires {
      T::_duplex_marker;
   }
   && is_input_output< T, VT >;

// @quote duplex_root 6
template< typename VT >
struct duplex_root :
   input_output_root< VT >
{
   static const bool _duplex_marker = true;
};


// =============================================================================

// @quote is_simplex 8
template< typename T, typename VT = T::value_type >
concept is_simplex = requires {
      T::_simplex_marker;
      { T::direction_set_input() }   -> std::same_as< void >;
      { T::direction_set_output() }  -> std::same_as< void >;
      { T::direction_flush() }       -> std::same_as< void >;
   }
   && is_input_output< T, VT >;

// @quote simplex_root 6
template< typename VT >
struct simplex_root :
   input_output_root< VT >
{
   static const bool _simplex_marker = true;
};
