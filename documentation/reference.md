godafoss reference

*****************************************************************************

<a name="toc-anchor-0"></a>

# 1 Content

<!-- update table_of_contents( input, 2 ) -->

  - [1 Content](#toc-anchor-0)

  - [2 vocabulary](#toc-anchor-1)

    - [2.1 cto](#toc-anchor-2)
      [2.2 interface](#toc-anchor-3)
      [2.3 init(): initialization](#toc-anchor-4)
      [2.4 box](#toc-anchor-5)
      [2.5 item](#toc-anchor-6)
      [2.6 stream](#toc-anchor-7)
      [2.7 buffering](#toc-anchor-8)
      [2.8 input, output](#toc-anchor-9)

      - [2.8.1 input](#toc-anchor-10)

      - [2.8.2 output](#toc-anchor-11)

      - [2.8.3 direction](#toc-anchor-12)
      [2.9 digital IO: gpio, gpi, gpo, gpoc](#toc-anchor-13)

      - [2.9.1 gpio](#toc-anchor-14)

      - [2.9.2 gpi](#toc-anchor-15)

      - [2.9.3 gpo](#toc-anchor-16)

      - [2.9.4 gpoc](#toc-anchor-17)
      [2.10 Analog IO: adc, dac](#toc-anchor-18)

<!-- update end -->

*****************************************************************************

<a name="toc-anchor-1"></a>

# 2 Vocabulary

This section defines the terms used in the library,
in an order that tries to minimize forward references.

todo: 
- blocking / polling
- examples
- value split to item and stream, or use value directly? container? (no, that overloads)
- range (= can always invert) - eigenlijk een eigenschap van de waarde???
- 

<!-- update example_path( "../include/core.hpp" ) -->

<a name="toc-anchor-2"></a>

## 2.1 cto

A cto is a Compile Time Object, implemented as a struct or class 
that has only static functions and static attributes.
A cto doesn't need to be instantiated:
its static functions can be used directly, or it can be passed as 
a template parameter to create other cto's.

When a cto is instantiated, the created object is a tag object.
When it is passed around (as auto parameter) it carries only its type.

A cto implements one or more interfaces. 
It advertises this by inheriting from each interface root class, xyz_rooot 
for the interface xyz.

for each interface it implements. 
This inserts a tag element into the class, and probably some more 
items that are mandatory for that interface. 

<a name="toc-anchor-3"></a>

## 2.2 tag object

A tag object is an instance of a cto,
hence it has only static functions and static attributes.

It is passed as an auto function parameter to pass its type.

<a name="toc-anchor-3"></a>

## 2.2 interface

An interface is a set of services 
(functions, data elements, cto's, templates) provided by a cto.

For each interface xyz a root class xyz_root exists.
A cto that implements an interface inherits from its root class.
This root class for the interface xyz contains at least a static 
constexpr bool is_xyz with the value true.

For each interface a concept exists with the same name that tests, 
as far as possible, whether a cto provides the required interface.
This concept is used to constrain templates that want to accept 
only a cto that implement a specific the interface.

<a name="toc-anchor-4"></a>

## 2.3 initialization

Each cto provides an init() function.
Before any of its functions or (static) attributes are used at run-time, 
a cto must be initialized by calling its init().

When a cto (or the main) directly uses a cto, 
it is responsible for initializing that cto.
When a cto (or the main) receives a cto and (only) passes
it to another cto, the first cto should not init() the second cto.

<a name="toc-anchor-5"></a>

## 2.4 value

A value is a cto that you can read a value from and/or write a value to.
The type of the value is provided as value_type.

A value cto is either an item or a stream.

<!-- update quote( input, "", "''box''" ) -->
~~~C++
template< typename T >
concept bool box = requires( 
   T::is_box,
   typename T::value_type v,
   typename T::is_item | typename T::is_stream
){  
   { T::init() } -> void;
};
~~~

<a name="toc-anchor-6"></a>

## 2.5 item

An item is a box that has or contains (at any point in time) a single value.
When you read it twice in rapid succession, you will read the same value.
Writing to an item overwrites its old value.

<!-- update quote( input, "", "''item''" ) -->
~~~C++
template< typename T >
concept bool item = requires( 
   T::is_item,
   box< T >
);
~~~

<a name="toc-anchor-7"></a>

## 2.6 stream

A stream is a box that holds a sequence of values.
All writes to a stream matter, including writes of the same value.
Reading from a stream consumes the value that was read.

<!-- update quote( input, "", "''stream''" ) -->
~~~C++
template< typename T >
concept bool stream = requires( 
   T::is_stream,
   box< T >
);
~~~

<a name="toc-anchor-8"></a>

## 2.7 buffering

A box can be buffered. 
For an output box, this means that the effect of write operations 
can be postponed until the next flush call.
For an input box, this means that a read operation can reflect 
the situation immediately before that last refresh call. 
For immediate effect on a buffered box, a read must be preceded 
by a refresh, and a write must be followed by a flush.

An immediate box is not buffered: its refresh and/or 
flush operation has no effect.
The immediate decorator creates an immediate box from a 
(possibly buffered) box
by sticking an refresh() before each read() and a flush() 
after each write().

~~~C++
// be lazy, use immediate<>!
immediate< p >::write( 0 );

// this has the same effect as
p::write( 0 );
p::flush();
~~~

<a name="toc-anchor-9"></a>

## 2.8 input, output

<a name="toc-anchor-10"></a>

### 2.8.1 input

A input box provides a read function that returns 
a value of the value_type of the box.

<!-- update quote( input, "", "''input''" ) -->
~~~C++
template< typename T >
concept bool input = requires( 
   T::is_input,
   box< T >,
   typename T::value_type
){
   refresh() -> void,
   read() -> value_type
};
~~~

<a name="toc-anchor-11"></a>

### 2.8.2 output

An output box provides a write function that accepts 
a value of the value_type of the box.

<!-- update quote( input, "", "''output''" ) -->
~~~C++
template< typename T >
concept bool output = requires( 
   T::is_output,
   box< T >,
   typename T::value_type
){
   write( value_type ) -> void,
   flush() -> void
};
~~~

<a name="toc-anchor-12"></a>

### 2.8.3 direction

A box can be an input, an output, or both.
When it is an input you can read from it, 
when it is an output you can write to it.
(In theory a box could also be neither, 
but that would not be very useful.)

When a box is both input and output it can be simplex 
(sometimes call half-duplex) or duplex.
A duplex box can, at any time, be both read and written.

<!-- update quote( input, "", "''input_output''" ) -->
~~~C++
template< typename T >
concept bool input_output = requires( 
   input< T >,
   output< T >
);
~~~

<!-- update quote( input, "", "''duplex''" ) -->
~~~C++
template< typename T >
concept bool duplex = requires( 
   T::is_duplex,
   input_output< T >
);
~~~

A simplex box has a current direction, which can be input or output.
The direction of a simplex box can be changed 
with a direction_set_input or direction_set_output call.
For a successful read, the direction of a simplex box must be input.
For a successful write, the direction of a simplex box must be output.
Otherwise a write can have no effect at all, or have a delayed effect,
and a read returns an unspecified value, and for a stream it can either 
consume the value or not.

The effect of calling a direction_set... function can be delayed 
up to the next direction_flush() call.
Like for read() and write(), direct<> can be used 
to get an immediate effect.

<!-- update quote( input, "", "''simplex''" ) -->
~~~C++
template< typename T >
concept bool simplex = requires( 
   T::is_simplex,
   input_output< T >
){
   direction_set_input() -> void,
   direction_set_output() -> void,
   direction_flush() -> void
};
~~~

<a name="toc-anchor-13"></a>

## 2.9 digital IO: gpio, gpi, gpo, gpoc

<a name="toc-anchor-14"></a>

### 2.9.1 gpio

A gpio is a (digital) General Purpose Input Output pin.
Most pins of micro-controllers can be used as gpio 
(but can often also have other, more specialized, uses).
An I/O extender chip can also provide gpio pins.
A gpio must be explicitly instructed to be an input or output, 
hence it is simplex.

<!-- update quote( input, "", "''gpio''" ) -->
~~~C++
template< typename T >
concept bool gpio = requires( 
   T::is_gpio,
   item< bool >,
   simplex< bool >
);
~~~

<a name="toc-anchor-15"></a>

### 2.9.2 gpi

A gpi is like a gpio, but can only be used for input.

<!-- update quote( input, "", "''gpi''" ) -->
~~~C++
template< typename T >
concept bool gpi = requires( 
   T::is_gpi,
   item< bool >,
   input< bool >
);
~~~

<a name="toc-anchor-16"></a>

### 2.9.3 gpo

A gpo is like a gpio, but can only be used for output.

<!-- update quote( input, "", "''gpo''" ) -->
~~~C++
template< typename T >
concept bool gpo = requires( 
   T::is_gpo,
   item< bool >,
   output< bool >
);
~~~

<a name="toc-anchor-17"></a>

### 2.9.4 gpoc

A gpoc is like a gpio, but electrically it can only 'pull' 
its pin to ground.
When it is high, it doesn't 'pull' its pin high 
(the pin is floating, as if it were an input).
Hence it is duplex (it doesn't provide set_direction... calls), 
but when its output is low the input will 
in normal circumstances always read low.

<!-- update quote( input, "", "''gpoc''" ) -->
~~~C++
template< typename T >
concept bool gpoc = requires( 
   T::is_gpoc,
   item< bool >,
   duplex< bool >
);
~~~

The templates make_gpio<>, make_gpi<>, make_gpo<> and 
make_gpoc<> create a pin with the requested interface 
from their argument (if possible).
The concepts can_gpio<>, can_gpi<>, can_gpo<> and can_gpoc<> 
indicate whether these conversions are possible. 

<!-- update quote( input, "", "''can_gpio_etc''" ) -->
~~~C++
template< typename T >
concept bool can_gpio = requires( 
   gpio< T > | gpoc< T >
);

template< typename T >
concept bool can_gpi = requires( 
   gpi< T > | gpoc< T >
);

template< typename T >
concept bool can_gpo = requires( 
   gpo< T > | gpoc< T >
);

template< typename T >
concept bool can_gpoc = requires( 
   gpio< T > | gpoc< T >
);
// end_can_gpio_etc
~~~

A common idiom is that a template specifies its parameter 
as for instance can_gpo,and to use make_gpo internally 
to do the necessary conversion.
This much like a run-time parameter accepting a derived type.

~~~C++
template< can_gpo _pin >
void blink(){
   alias pin = gpo< _pin >;
   pin::init();
   for(;;){
      pin::write( 0 );
      ...
   }
}
~~~

<a name="toc-anchor-18"></a>

## 2.10 Analog IO: adc, dac

An adc (Analog to Digital Conversion) pin is an item that has a 
read() function that returns the analog value on the pin, 
converted to a binary value in the range adc_min..adc_max.

<!-- update quote( input, "", "''adc''" ) -->
~~~C++
template< typename T >
concept bool adc = requires( 
   T::is_adc,
   item< T >,
   input< T >,
   constexpr T::value_type adc_min,
   constexpr T::value_type adc_max
);
~~~

A dac (Digital to Analog Conversion) pin is an item that has a 
write() function that causes the pin to output the analog 
equivalent of the value that was written.
The value must be in the range dac_min..dac_max.

<!-- update quote( input, "", "''dac''" ) -->
~~~C++
template< typename T >
concept bool dac = requires( 
   T::is_dac,
   item< T >,
   output< T >,
   constexpr T::value_type dac_min,
   constexpr T::value_type dac_max
);
~~~

## 1 duration

A duration is an amount of time. 

template< typename T >
class duration {
};

constexpr

## timing services

### waiting

template< typename T >
concept bool waiting = requires(
   T::duration_type D;
){
   wait( D ) -> void,
   wait_busy( D ) -> void
};

### clocking

template< typename T >
concept bool waiting = requires(
   waiting< T >,
   T::duration_type D;   
){
   now() -> D
};

### tasking

template< typename T >
concept bool waiting = requires(
   waiting< T >,
   T::duration_type D;   
){
   now() -> D
};

## 1 xy

An xy is a ADT for 2D coordinate (absolute) or vector (relative) values.

template< typename T >
struct xy {
   T x, y;
   constexpr xy( const T & x, const T & y ): x{ x }, y{ y }{}
   constexpr operator+
   constexpr operator-
   template< typename X > constexpr operator* 
};   

## color
## pixel window
## character stream
## monospaced character window

## string

template< size_t N >
class string { ... }