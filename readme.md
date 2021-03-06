![](images/godafoss-waterfalls.png)

# Godafoss: a C++ framework for component-based micro-controller programming

-------------------------------------------------------------------------------

## Status

THIS IS WORK IN PROGRESS, NOT YET FIT FOR USE 
except by myself and the very brave.

For those brave who want to explore what is there:
   - attic: old stuff, do not look here
   - book: nothing useful here yet
   - demo: demos of library features
   - externals: external stuff used by godafoss, for now only my torsor
   - library: the library itself
   - tests: tests of library features
   - work: for temporary stuff I am working on
   
For now, I am the single contributor to this repo,
and use github as my usb-stick-in-the clouds,
so you won't learn much from the git history.
   
-------------------------------------------------------------------------------
   
## Name and background

The [Godafoss](https://en.wikipedia.org/wiki/Goðafoss) 
(god-waterfall) is a large waterfall in the north of Iceland.
Allegedly, in the year 1000 the law-speaker Porgeir decided
that Iceland officially choose Christianity over the Norse beliefs.
To signify this decision, he threw his statues of Norse gods in the waterfall.

The godafoss framework is where I throw in all my previous work on
an efficient library of re-usable micro-controller components.
It is for now (and maybe forever) work in progress.

I call godafoss a framework rather than a library,
because godafoss enforces a specific style of programming.
For me the difference between a framework and a library is that
libraries play nice with each other and can be combined
with each other and with a framework.
A framework assumes you use it and no other ones: 
frameworks tolerate libraries, but don't play nice with other frameworks.
My work in this field stared as libraries, but at a point
I realized that I needed to take overall control 
of the structure of an application to achieve both
flexibility, ease of use, and efficiency.
In my opinion a library is inherently better than a framework
because it leaves more options for the user.
I am sorry I could not keep godafoss a library.

-------------------------------------------------------------------------------
   
## Summary

Godafoss is a framework for close-to-the-hardware component-based 
software development on small embedded systems 
(which are typically micro-controllers).

In the context of godafoss an interface is a defined set of services,
defined by a C++ concept.
An example is the godafoss::pin_in_out interface, 
which comprises the services (in this case: function calls) 
typically offered by a GPIO pin:
- setting the direction (input or output) of the pin;
- writing a value to the pin;
- reading a value from the pin.

An interface template is an interface that open to some configuration.
And example is the godafoss::adc interface template, 
which takes as parameter the number of bits in the adc result.

In the context of godafoss a component is a piece of software that 
provides an interface.
An example of a component is a specific GPIO pin, 
which offers the pin_in_out interface.



Components can be nested: 
Typically the target component (the chip or board for which an application is built)
contains a GPIO component for each of its physical GPIO pins.

Components are static (determined at build time),
and present in an application only when used by that application.
A component that is present in the library or application code
does not use any resources in the application unless it is used.

A component template is (no surpise) a template for a component:
when configured by supplying the template parameters, it yields a component.
The component template parameters can be values
(for instance the size 

An example is the output component template: 
it must be configured with (for instance) a GPIO pin component.
The output component takes care of setting the direction of its GPIO minion, 
and offers only the write service.

A GPIO pin is a passive component: it doesn't do anything unless you invoke one of the functions from its interface.
A component can contain sub-components. 



A component can be passive, like a GPIO pin or an invert: 
they wont do anything unless you invoke one of their services.
A component can be active.
The blink element


Syntactically, an interface is a C++ concept.
   
