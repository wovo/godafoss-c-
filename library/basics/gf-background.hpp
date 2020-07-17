// =============================================================================
//
// gf-background.hpp
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
//
// @define godafoss::background
// @title background processing
//
// The background class provides a hook for run-to-completion style
// background processing.
//
// @insert text
//
// A class that needs background processing must inherit from background
// and implement the work function. This work function will be called
// when plain wait functions (the ones that allow background processing)
// are called.
//
// When an application contains background work,
// all plain wait functions can take longer than the specified time,
// up to the run time of the longest runtime of the work() functions.
//
// No background work will be done from wait calls made while
// a work() function is running.
//
// For all background jobs: be careful to preserve the object,
// or your servicing will end.
// This is not UB: the background destructor
// removes itself from the list of background jobs.
//
// When the application would terminate (exit from its main()),
// background::run() can be called instead, which will serve
// the background processing (it will never return).
//
// -----------------------------------------------------------------------------
// @section example
// @example native/background/main.cpp text
//
// =============================================================================

// @quote text 1
struct background : public not_copyable {
private:

   static inline background * first = nullptr;
   static inline background * current = nullptr;;
   static inline bool running = false;
   background * next;

public:

   background():
      next( first )
   {
      first = this;
   }

   ~background(){

      // find the pointer that points to us
      for( background **p = &first; *p != nullptr; p = &(*p)->next ){
         if( (*p) == this ){

            //remove myself from the list:
            // make it point to the next background itenm
            (*p) = next;
            return;
         }
      }
   }

   // @quote text 2
   // This function will be called to do background work for its object.
   virtual void work() = 0;

   // This function is called by the wait functions to do background work.
   static void do_background_work(){

      // do not run any background work when called from background
      // work: this would seriously impact stack use and delay granularity
      if( running ){
         return;
      }

      // if no current work object, start at the start of the list
      if( current == nullptr ){
         current = first;
      }

      // if there is a current work item
      if( current != nullptr ){

         // run it
         running = true;
         current->work();
         running = false;

         // next time, it is the turn of the next work item in the list
         current = current->next;
      }
   }

   // Call this function instead of terminating the application
   // to continue performing the background work.
   static void GODAFOSS_NO_RETURN run(){
      for(;;){
         do_background_work();
      }
   }

// @quote text 1
};
