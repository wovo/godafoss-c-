cto

A cto is a Compile Time Object, implemented as a struct or class that has only static functions and static attributes.
A cto is never instantiated.
Instead, cto's are passed as template parameters to create new cto's.

init

Before it is used at run-time, a cto must be initialized by calling its init function.
When a cto 


box

A box is a cto that you can read a value from and/or write a value to.
A box is either an item or a stream.

```C++
template< typename T >
concept bool box = requires( 
   typename T::is_box,
   typename T::value_type v,
   typename T::is_item | typename T::is_stream
){  
   { T::init() } -> void;
};
```

item

An item is a box that has or contains (at any point in time) a single value.
When you read it twice in rapid succession, you will read the same value.
Writing to an item overwrites its old value.

```C++
template< typename T >
concept bool box = requires( 
   box< T >,
   typename T::is_item
);
```

stream

A stream is a box that holds a sequence of values.
All writes to a stream matter, including writes of the same value.
Reading from a stream consumes the value that was read.

```C++
template< typename T >
concept bool stream = requires( 
   box< T >,
   typename T::is_item
);
```

direction

A box can be an input, an output, or both.
(A box could also be neither, but that would serve no purpose.)
When it is an input you can read from it, when it is an output you can write to it.

```C++
template< typename T >
concept bool box = requires( 
   box< T >,
   typename T::is_item
);
```

When a box is both input and output it can be simplex (sometimes call half-duplex) or duplex.
A duplex box can be both read and written.

A simplex box has a current direction, which can be input or output.
The direction of a simplex box can be changed with a set_direction_input or set_direction_output call.
For a successful read, the direction of a simplex box must be input.
For a successful write, the direction of a simplex box must be output.
Otherwise a write can have no effect at all, or have a delayed effect,
and a read returns an unspecified value, and for a stream it can either consume the value or not.

buffering

A box can be buffered. 
For an output box, this means that the effect of write operations can be postponed until the next flush call.
For an input box, this means that a read operation can reflect the situation immediately before that last refresh call. 
For immediate effect on a buffered box, a read must be preceded by a refresh, and a write must be followed by a flush.

immediate

An immediate box is not buffered: its refresh and/or flush operation has no effect.
The immediate decorator creates an immediate box from a (possibly buffered) box.

todo: blocking / polling

