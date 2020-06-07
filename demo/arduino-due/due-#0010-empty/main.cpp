// ==========================================================================
//
// empty project
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "godafoss.hpp"

int main( void ){}

extern "C" {

int __wrap_atexit(void __attribute__((unused)) (*function)(void)) {
    return -1;
}

/*
void *memset(void *str, int c, unsigned int n){
   unsigned char *p = (unsigned char *) str;
   while( n ){
      *p = (unsigned char) c;
      --n;
   }    
   return str;
}

void *memcpy(void *dest, const void * src, unsigned int n){
   unsigned const char *ps = (unsigned const char *) src;
   unsigned char *pd = (unsigned char *) dest;
   while( n ){
      *pd++ = *ps++;
      --n;
   }    
    
   return dest;
}
*/

}
