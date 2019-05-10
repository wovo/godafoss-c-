C++ micro-controller / close-to-the-hardware library

https://en.wikipedia.org/wiki/Goðafoss
https://forvo.com/word/goðafoss/

This is where I throw in all my previous work.
It is for now (and maybe forever) work in progress.

naming and architecture
- no heap, no RTTI, no exceptions, no function indirection
- constexpr rulez! (provide alternative to math?)
- bikesheds are important, especially their color
- header-only, (only) include the target
- lowercase with _ separators
- name composition order is from outer to inner, then verb, then variations
- leading _ is library-internal (can change without notice)
- macro's moeten _lowercase zijn!
- avoid verb / noun / adjective confusion
- avoid negatives (no-, not-, un- etc)
- avoid similar things or couples with the same prefix (source / sink)
- a concept is a type, so it doesn't need an is_ prefix (you don't use is_int!)
- prefer composition that changes features over extra object features (direct<> instead of read_flush in the class)
- avoid using the name of what you are in, it might change (including the library name)
- short names for small scopes, longer name for wider scopes
- short names for common operations, longer names for less common / dangerous operations (shared_ptr.get() is defintely wrong!)
- conserve the namespaces: don't use many names, prefer modifiers/adjectives. write a glossary!
- avoid abbreviations, _t, etc. (gpio?)

todo
- pullup/down should survive pin adapters
- pullup<> decorator
- pin_in from adc for my-first-devboard
- mfd: walk / select color, speed
- mfd: board test
- mfd: dimming demo
- can_pin_out etc. don't work?? in pin adapters
- 8574 + LCD
- mirror for ports, demo
- servo demo with NRF
- cout has no << flush??
- range adt
- hwlib i2c aanpasen, overnemen
-0 check https://github.com/olikraus/u8g2/wiki

template< typename T, T low, T high >;
struct interval 
   be_box
{
   T value;

   template< typename T, T low, T high >
   
};

dac::write( adc::read() );


