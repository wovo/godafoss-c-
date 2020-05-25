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
   
   using offset_t    = w::offset_t;
   using location_t  = w::location_t;
   using value_t     = w::offset_t::value_t;
   using color_t     = w::color_t;
   
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

   offset_t  origin;
   offset_t  size;  
   color_t   ink;
   
   line( location_t origin, offset_t size, color_t ink )
      : origin{ origin - w::origin }, size{ size }, ink{ ink }
   {}   
           
   line( location_t origin, location_t end, color_t ink )
      : origin{ origin - w::origin }, size{ origin - end }, ink{ ink }
   {}   
      
   void write(){ 
         
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

         w::write( location_t( xDraw, yDraw ), ink );

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
   
   using offset_t    = w::offset_t;
   using location_t  = w::location_t;
   using value_t     = w::offset_t::value_t;
   using color_t     = w::color_t;
   using w_line      = line< w >;
   
   offset_t   origin;
   offset_t   size;  
   color_t    border_ink;
   bool       fill;
   color_t    fill_ink;
     
   rectangle( location_t origin, location_t size, color_t border )
      : origin{ origin - w::origin }, size{ size - origin }, 
        border_ink{ border }, fill{ false }
   {}   
  
   rectangle( location_t origin, location_t size, color_t border, color_t fill  )
      : origin{ origin - w::origin }, size{ size - origin }, 
        border_ink{ border }, fill{ true }, fill_ink{ fill }
   {}  
    
   void write(){ 
      auto end = origin + size;
      w_line( w::origin + origin,  offset_t( size.x + 1, 0 ), border_ink ).write();
      w_line( w::origin + origin,  offset_t( 0, size.y + 1 ), border_ink ).write();
      w_line( w::origin + end,     offset_t( - size.x, 0   ), border_ink ).write();
      w_line( w::origin + end,     offset_t( 0, - size.y   ), border_ink ).write();
      if( fill ){
         for( auto const a : range( size - offset_t( 1, 1 ))){
            w::write( w::origin + origin + offset_t( 1, 1 ) + a, fill_ink );   
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
   
   address_t  midpoint;
   value_t    radius;
   address_t  origin;
   address_t  size;  
   color_t    border_ink;
   bool       use_foreground;
   color_t    fill_ink;
   bool       fill;  

   void update_from_midpoint_radius(){ 
      origin = midpoint - address_t{ radius };
      size = address_t( 2 * radius );
   }      
   
   circle( address_t midpoint, value_t radius )
      : midpoint{ midpoint }, radius{ radius }, 
        use_foreground{ true }, 
        fill{ false }
   {
      update_from_midpoint_radius();   
   }   
   
   circle( address_t midpoint, value_t radius, color_t ink )
      : midpoint{ midpoint }, radius{ radius },
        border_ink{ ink }, use_foreground{ false },
        fill{ false }
   {
      update_from_midpoint_radius();   
   }   
   
   circle( address_t midpoint, value_t radius, color_t border, color_t fill )
      : midpoint{ midpoint }, radius{ radius },
        border_ink{ border }, use_foreground{ false }, 
        fill_ink{ fill }, fill{ true }
   {
      update_from_midpoint_radius();   
   }   
      
   void write(){ 

      // don't draw anything when the size would be 0 
      if( radius < 1 ){
         return;       
      }
   
      auto ink = use_foreground ? w::foreground : border_ink;
      auto start = midpoint;

      // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
   
      value_t fx = 1 - radius;
      value_t ddFx = 1;
      value_t ddFy = -2 * radius;
      value_t x = 0;
      value_t y = radius;
    
      // top and bottom
      w::write( start + xy( 0, + radius ), ink );
      w::write( start + xy( 0, - radius ), ink );

      // left and right 
      w::write( start + xy( + radius, 0 ), ink );
      w::write( start + xy( - radius, 0 ), ink );
         
      // filled circle
      if( 0 ) if( fill ){
   
         // top and bottom
         w::write( start + xy( 0, + radius ), fill_ink );
         w::write( start + xy( 0, - radius ), fill_ink );

         // left and right
         line< w >(  
              start - xy( radius, 0 ), 
                      xy( radius, 0 ), 
              fill_ink 
          ).write();
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
                    
         w::write( start + xy( + x, + y ), ink );
         w::write( start + xy( - x, + y ), ink );
         w::write( start + xy( + x, - y ), ink );
         w::write( start + xy( - x, - y ), ink );
         w::write( start + xy( + y, + x ), ink );
         w::write( start + xy( - y, + x ), ink );
         w::write( start + xy( + y, - x ), ink );
         w::write( start + xy( - y, - x ), ink );
            
         // filled circle
         if( fill ){
            line< w >( 
               start + xy( -x + 1,  y ), 
                       xy(  2 * x - 2, 0 ), 
               fill_ink ).write();
            if(0) line< w >( 
               start + xy( -x, -y ), 
                       xy(  2 * x, 0 ), 
               fill_ink ).write();
            if(0) line< w >( 
               start + xy( -y,  x ), 
                       xy(  2 * y,  0 ), 
               fill_ink ).write();
            if(0)line< w >( 
               start + xy( -y, -x ), 
                       xy(  2 * y, 0 ), 
               fill_ink ).write();
         }

      }
   }   
    
}; // class circle
