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
// @define godafoss::cto
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
// features that the cto isw obliged to offer.
// The concept is used to constrain templates that want to accept
// only a cto that implements a specific set of features.
//
// The root can be a plain struct, but it is often a template.
// For more complex cto's the CRTP pattern is used so the root can
// provide both base properties and enrichment based on the provided
// implementation.
//
// @insert cto_concept
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

// @quote cto_concept 5
template< typename T >
concept cto = requires {
   T::_cto_marker;
   { T::init() } -> std::same_as< void >;
};

// @quote cto_root 3
struct cto_root {
   static const bool _cto_marker = true;
};


// =============================================================================
//
// @define godafoss::item
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
// @insert item_concept
//
// An item is a cto that holds one or more data elements of a specific type.
//
// @insert item_root
//
// All items inherit (in most cases indirectly) from the struct item_root.
//
// =============================================================================


// @quote item_concept 5
template< typename T >
concept item = requires {
   cto< T >;
   T::_item_marker;
};

// @quote item_root 5
template< typename T >
struct item_root : cto_root {
   static const bool _item_marker = true;
   using value_type = T;
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
// @define godafoss::box
// @define godafoss::pipe
// @title box, pipe
//
// A box and a pipe are two kinds of items.
// The difference is their semantics: a box behaves like a variable
// that holds a single value,
// a pipe behaves like a sequence of values.
//
// @section box
//
// A box is an item that has or contains (at any point in time) a single value.
// A box has value semantics:
// when you read from a box twice in rapid succession,
// you will get the same value.
// Writing to an item overwrites its old value in the box.
//
// @insert box_concept
// @insert box_root
//
// @section pipe
//
// A pipe is an item that holds a sequence of values.
// A write to a pipe adds a new value the sequence.
// Hence all writes to a stream matter, including
// repeated writes of the same value.
// Reading from a pipe is destructive: it consumes the value that was
// read from the sequence.
// Writing to a pipe adds a value to the sequnce.
//
// @insert pipe_concept
// @insert pipe_root
//
// =============================================================================

// @quote box_concept 5
template< typename T >
concept box = requires {
   item< T >;
   T::_box_marker;
};

// @quote box_root 6
template< typename T >
struct box_root :
   item_root< T >
{
   static const bool _box_marker = true;
};


// =============================================================================

// @quote pipe_concept 5
template< typename T >
concept pipe = requires {
   item< T >;
   T::_pipe_marker;
};

// @quote pipe_root 6
template< typename T >
struct pipe_root :
   item_root< T >
{
   static const bool _pipe_marker = true;
};


// =============================================================================
//
// @define godafoss::input
// @define godafoss::output
// @define godafoss::input_output
// @define godafoss::duplex
// @define godafoss::simplex
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
// read() and write() operations have direct effect.
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
// @section input
// @insert input_concept
// @insert input_root
//
// A input is an item that provides a read function that returns
// a value of the value_type of the item.
//
// @section output
// @insert output_concept
// @insert output_root
//
// An output is an item that provides a write function that accepts
// a value of the value_type of the item.
//
// @section input_output
// @insert input_output_concept
// @insert input_output_root
//
// An input_output is an item that is both an input and an output.
//
// @section direction
//
// A duplex item is an input_output that can function both as
// an input and as an output at the same time.
//
// @insert duplex_concept
// @insert duplex_root
//
// A simplex item is an input_output that has a current direction,
// which can be input or output.
//
// @insert simplex_concept
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

// @quote input_concept 7
template< typename T >
concept input = requires {
   item< T >;
   T::_input_marker;
   { T::refresh() }  -> std::same_as< void >;
   { T::read() }     -> std::same_as< typename T::value_type >;
};

// @quote input_root 6
template< typename T >
struct input_root :
   item_root< T >
{
   static const bool _input_marker = true;
};


// =============================================================================

// @quote output_concept 7
template< typename T >
concept output = requires (
   typename T::value_type v
){
   item< T >;
   T::_output_marker;
   { T::write( v ) }  -> std::same_as< void >;
   { T::flush()    }  -> std::same_as< void >;
};

// @quote output_root 6
template< typename T >
struct output_root :
   item_root< T >
{
   static const bool _output_marker = true;
};


// =============================================================================

// @quote input_output_concept 5
template< typename T >
concept input_output = requires {
   input< T >;
   output< T >;
};

// @quote input_output_root 5
template< typename T >
struct input_output_root :
   input_root< T >,
   output_root< T >
{};


// =============================================================================

// @quote duplex_concept 5
template< typename T >
concept duplex = requires {
   input_output< T >;
   T::_duplex_marker;
};

// @quote duplex_root 6
template< typename T >
struct duplex_root :
   input_output_root< T >
{
   static const bool _duplex_marker = true;
};


// =============================================================================

// @quote simplex_concept 8
template< typename T >
concept simplex = requires {
   input_output< T >;
   T::_simplex_marker;
   { T::direction_set_input() }   -> std::same_as< void >;
   { T::direction_set_output() }  -> std::same_as< void >;
   { T::direction_flush() }       -> std::same_as< void >;
};

// @quote simplex_root 6
template< typename T >
struct simplex_root :
   input_output_root< T >
{
   static const bool _simplex_marker = true;
};
