// ==========================================================================
//
// gf-drawables.hpp
// 
// ==========================================================================
//
// some things that can be written on a window
//
// ==========================================================================
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
// ==========================================================================


// ==========================================================================
//
// line
//
// ==========================================================================
      
template< typename w >      
class line {
public:
   
   using address_t  = w::address_t;
   using value_t    = w::address_t::value_t;
   using color_t    = w::color_t;
   
private:   

   static void swap( value_t & a, value_t & b ){
      auto t = a; 
      a = b;
      b = t;
   }

   static value_t abs( value_t x ){
      return x >= 0 ? x : -x;
   }
   
public:

   address_t  origin;
   address_t  size;  
   color_t    default_ink;
   bool       use_foreground;
   
   line( address_t origin, address_t size, color_t ink )
      : origin{ origin }, size{ size }, 
        default_ink{ ink }, use_foreground{ false }
   {}   
   
   line( address_t origin, address_t size )
      : origin{ origin }, size{ size }, 
        use_foreground{ true }
   {}   
   
   void write(){ 
   
      auto ink = use_foreground ? w::foreground : default_ink;
         
      value_t x0 = origin.x;
      value_t y0 = origin.y;
      value_t x1 = origin.x + size.x; 
      value_t y1 = origin.y + size.y;
                   
      // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
      // http://homepages.enterprise.net/murphy/thickline/index.html
     
      value_t Dx = x1 - x0; 
      value_t Dy = y1 - y0;
   
      value_t steep = (abs(Dy) >= abs(Dx));
   
      if( steep ){
         swap( x0, y0 );
         swap( x1, y1 );
      
         // recompute Dx, Dy after swap
         Dx = x1 - x0;
         Dy = y1 - y0;
      }
   
      value_t xstep = 1;
      if( Dx < 0 ){
         xstep = -1;
         Dx = -Dx;
      }
   
      value_t ystep = 1;
      if( Dy < 0 ){
         ystep = -1;    
         Dy = -Dy; 
      }
      value_t TwoDy = 2*Dy; 
      value_t TwoDyTwoDx = TwoDy - 2*Dx; // 2*Dy - 2*Dx
      value_t E = TwoDy - Dx; //2*Dy - Dx
      value_t y = y0;
      value_t xDraw, yDraw, x;  
      for( x = x0; x != x1; x += xstep ){    
         if (steep) {     
            xDraw = y;
            yDraw = x;
         } else {     
            xDraw = x;
            yDraw = y;
         }

         w::write( address_t( xDraw, yDraw ), ink );

         if( E > 0 ){
            E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
            y = y + ystep;
         } else {
            E += TwoDy; //E += 2*Dy;
         }
      }
   }
   
}; // class line   


// ==========================================================================
//
// rectangle
//
// ==========================================================================
      
template< typename w >      
class rectangle {
public:
   
   using address_t  = w::address_t;
   using value_t    = w::address_t::value_t;
   using color_t    = w::color_t;
   using line       = line< w >;
   
   address_t  origin;
   address_t  size;  
   color_t    border_ink;
   bool       use_foreground;
   color_t    fill_ink;
   bool       fill;
   
   rectangle( address_t origin, address_t size )
      : origin{ origin }, size{ size }, 
        use_foreground{ true }, 
        fill{ false }
   {}    
   
   rectangle( address_t origin, address_t size, color_t border )
      : origin{ origin }, size{ size }, 
        border_ink{ border }, use_foreground{ false }, 
        fill{ false }
   {}   
   
   rectangle( address_t origin, address_t size, color_t border, color_t fill )
      : origin{ origin }, size{ size }, 
        border_ink{ border }, use_foreground{ false }, 
        fill_ink{ fill }, fill{ true }
   {}   
   
   void write(){ 
      auto ink = use_foreground ? w::foreground : border_ink;
      auto end = origin + size;
      line( origin,  address_t( size.x + 1, 0 ), ink ).write();
      line( origin,  address_t( 0, size.y + 1 ), ink ).write();
      line( end,     address_t( - size.x, 0   ), ink ).write();
      line( end,     address_t( 0, - size.y   ), ink ).write();
      if( fill ){
         for( auto const a : xy_all_t( size - address_t( 1, 1 ))){
            w::write( origin + address_t( 1, 1 ) + a, fill_ink );   
         }   
      }
   }

}; // class rectangle


// ==========================================================================
//
// circle
//
// ==========================================================================

template< typename w >      
class circle {
public:
   
   using address_t  = w::address_t;
   using value_t    = w::address_t::value_t;
   using color_t    = w::color_t;
   
   address_t  origin;
   address_t  size;  
   color_t    default_ink;
   bool       use_foreground;
   
   circle( address_t origin, address_t size, color_t ink )
      : origin{ origin }, size{ size }, 
        default_ink{ ink }, use_foreground{ false }
   {}   
   
   circle( address_t origin, address_t size )
      : origin{ origin }, size{ size }, 
        use_foreground{ true }
   {}   
   
};   

/*

/// a circle object                   
class circle : public drawable {
private:   
   uint_fast16_t  radius;
   color          ink;
   
public:
   /// create a circle object 
   circle( 
      xy start, 
      uint_fast16_t radius, 
      color ink = unspecified
   )
      : drawable{ start }, radius{ radius }, ink{ ink }
   {}     
   
   void draw( window & w ) override { 

      // don't draw anything when the size would be 0 
      if( radius < 1 ){
         return;       
      }
   
      // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
   
      int_fast16_t fx = 1 - radius;
      int_fast16_t ddFx = 1;
      int_fast16_t ddFy = -2 * radius;
      int_fast16_t x = 0;
      int_fast16_t y = radius;
    
      // top and bottom
      w.write( start + xy( 0, + radius ), ink );
      w.write( start + xy( 0, - radius ), ink );

      // left and right 
      w.write( start + xy( + radius, 0 ), ink );
      w.write( start + xy( - radius, 0 ), ink );
         
      // filled circle
      if(0){
   
         // top and bottom
         w.write( start + xy( 0, + radius ), ink );
         w.write( start + xy( 0, - radius ), ink );

         // left and right
         line(  
              start - xy( radius, 0 ), 
              start + xy( radius, 0 ), 
              ink 
          ).draw( w );
      } 
    
      while( x < y ){
      
         // calculate next outer circle point
         if( fx >= 0 ){
           y--;
           ddFy += 2;
           fx += ddFy;
         }
         x++;
         ddFx += 2;
         fx += ddFx;   
                    
         w.write( start + xy( + x, + y ), ink );
         w.write( start + xy( - x, + y ), ink );
         w.write( start + xy( + x, - y ), ink );
         w.write( start + xy( - x, - y ), ink );
         w.write( start + xy( + y, + x ), ink );
         w.write( start + xy( - y, + x ), ink );
         w.write( start + xy( + y, - x ), ink );
         w.write( start + xy( - y, - x ), ink );
            
         // filled circle
         if(0) if( ! ink.is_transparent()  ){
            line( 
               start + xy( -x,  y ), 
               start + xy(  x,  y ), 
               ink ).draw( w );
            line( 
               start + xy( -x, -y ), 
               start + xy(  x, -y ), 
               ink ).draw( w );
            line( 
               start + xy( -y,  x ), 
               start + xy(  y,  x ), 
               ink ).draw( w );
            line( 
               start + xy( -y, -x ), 
               start + xy(  y, -x ), 
               ink ).draw( w );
         }
      }
   }   
    
}; // class circle

*/