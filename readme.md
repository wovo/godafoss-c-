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
   
For now, I am the single contributor to this repo.
Github is my usb stick in the clouds,
so do don't do any commit messages or versioning.
   
-------------------------------------------------------------------------------
   
## Name and background

The [Godafoss](https://en.wikipedia.org/wiki/Goðafoss) 
(god-waterfall) is a large waterfall in the north of Iceland.
Allegedly, in the year 999 or 1000 the law-speaker 
[Thorgeir](https://en.wikipedia.org/wiki/Thorgeir_Ljosvetningagodi) 
decided that Iceland officially choose Christianity over the Norse beliefs.
To signify this decision, he threw his statues of Norse gods in the waterfall.

The godafoss framework is where I threw in my previous work on
an efficient library of re-usable micro-controller components.

I call godafoss a framework rather than a library,
because godafoss enforces a specific style of programming
and a specific application architecture.
For me the difference between a framework and a library is that
libraries play nice with each other and can be combined
with each other and with a framework.
A framework assumes you use it and no others: 
frameworks tolerate libraries, but don't play nice with other frameworks.
My efforts in this field started as libraries, 
but at a point I realized that I needed to take control 
of the structure of an application to achieve both
flexibility, ease of use, and efficiency.
In my opinion a library is inherently better than a framework
because it leaves more options for the user,
so I am sorry I could not restrict godafoss be a library.

-------------------------------------------------------------------------------
   
## Summary

Godafoss is a framework for close-to-the-hardware component-based 
software development on small embedded systems, 
typically micro-controllers.

In the context of godafoss an ***interface*** is a specific set of services,
defined by a C++ concept.
An example is the godafoss::pin_in_out interface, 
which comprises the services (which are in this case function calls) 
typically offered by a GPIO pin:
- setting the direction (input or output) of the pin;
- writing a value to the pin;
- reading a value from the pin.

An ***interface template*** is an interface that is open to configuration.
It is defined by a C++ concept with additional parameters.
And example is the godafoss::adc interface template, 
which takes as parameter the number of bits in the adc result.

In the context of godafoss a ***component*** is a building block that 
provides an interface.
An example of a component is a specific GPIO pin 
of the target (the chip or board for which an application is built), 
which offers the pin_in_out interface.

Components can be nested, in the sense that a component definition
can contain the definitions of other 'sub' components.
In the context of godafoss, the ***target*** is the chip or
PCB for which the application is built.
When the target has GPIO pins, the target component defines 
a GPIO component for each of its physical GPIO pins.

Components are ***static*** (determined at build time),
and present in an application only when used by that application.
In Object Oriented terms a component can be seen as
an object that has whole-application lifetime.
A component that is present in the library or application code
does not use any resources in an application unless it is used.

A ***component template*** is a template for a component:
when configured by supplying the template parameters, 
it yields a component.
And example is the godafoss::hd77480 character LCD component template.
As template parameters it requires the number of columns and rows of the
LCD ( a value), and the GPIO pins that connect to the LCD
(which are components).

Most components are ***passive***: they won't do anything except when 
one of their service functions is called.
Some components are ***active***.
An example is godafoss:blink, which periodically switches 
its output between high and low (for instance to blink an attached LED).

A component will often require the services of other components.
Those components will often be components 
supplied to it as template parameters
A godafoss application is a single 'main' component that is executed by
the godafoss::run function. 
The main component and all components that are transitively 
used by the main component are part of the application, 
and hence claim resources in the form of ROM, RAM and CPU time.
Other components and component templates are not present and
do not use any run-time resources.

-------------------------------------------------------------------------------

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
   
