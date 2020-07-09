# ==============================================================================
#
# document.py
#
# ==============================================================================
#
# godafoss documentation tool
#
# Copyright
#    Wouter van Ooijen (wouter@voti.nl) 2020
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
# ==============================================================================

# highlighter can't handle 1'000
# show first appearance of defined word
# in header code generalization like struct bla { ... };
# example select part of the file
# example link to example file
# set examples path
# set html path
# create markdown
# crosslinks
# mark code in running text
# ref to source code

def sortedkeys( map ):
   list = []
   for key in map.keys():
      list.append( key )
   list.sort( key=str.casefold )
   return list 

def strip_leading_spaces( lines ):
   result = ""
   for line in lines.split( "\n" ):
      result += line.strip() + "\n"
   return result [ 1 : -1 ]

def strip_leading_comment( s, remove = [ "//" ] ):
   s = s.strip()
   for prefix in remove:
      if s.startswith( prefix ):
         s = s[ len( prefix ) : ].strip()
   return s

def after( s, prefix ):
   if s.startswith( prefix ):
      return s[ len( prefix ) : ].strip()
   return ""

def get_after( target, s, prefix ):
   if s.startswith( prefix ):
      target = s[ len( prefix ) : ].strip()

def error( file_name, line_number, s ):
   print( "[%s:%d] error : %s" % (file_name, line_number, s ) )

def write_file( file_name, text ):
   f = open( file_name, "w" );
   f.write( text )
   f.close()

def write_html( dir, name, text ):
   write_file( "html/" + name + ".html", text )

def read_file( file_name ):
   s = ""
   for line in open( file_name ).readlines():
      s += line
   return s


# ==============================================================================
#
# text parts
#
# ==============================================================================

class text_line:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def html( self ):
      if self.s == "": return "<P>"
      return self.s

class title:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def html( self ):
      return "<H2>%s</H2>" % self.s

class code:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def html( self ):
      return '<pre><code class="c++">%s\n</code></pre>' % self.s

class define:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def html( self ):
      return '<a name="%s"></a>' % self.s

class colofon:

   def __init__( self, item ):
      self.item = item

   def markdown( self ):
      return self.item

   def html( self ):
      return 'from <A HREF="%s">%s</A><P>\n' % (
         "../" + self.item.file_name, 
         self.item.file_name.replace( "../", "" ) )



# ==============================================================================
#
# documentation item
#
# ==============================================================================

class item:

   def __init__( self, file_name, line_number, lines ):
      self.file_name = file_name
      self.line_number = line_number
      self.lines = lines

      self.title = None
      self.names = []
      self.content = []
      self.defines = []
      line_number = self.line_number - 1

      for line in self.lines:
         line_number += 1
         
         if line.startswith( "@title" ):
            self.content.append( title( after( line, "@title" )))
            self.content.append( colofon( self ))
            self.title = after( line, "@title" )
            
         elif line.startswith( "@example" ):
            t = read_file( "../examples/" + after( line, "@example" ))
            self.content.append( code( t ) )
            
         elif line.startswith( "@define" ):
            t = after( line, "@define" ).replace( "godafoss::", "" )
            self.defines.append( t )
            self.content.append( define( t ))
            
         elif not line.startswith( "@" ):
            self.content.append( text_line( line ))
            
         else:
            error( file_name, line_number, "unknown @ '%s'" % line )

      if self.title == None:
         error( self.file_name, self.line_number, "missing @title" )
         self.title = "<missing>"

   def markdown( self ):
      s = ""
      for c in self.content:
         s += c.markdown() + "\n"
      return s

   def html( self ):
      # from https://highlightjs.org/static/demo/
      # but do check https://stackoverflow.com/questions/36187575/how-to-make-a-simple-example-with-highlight-js
      s = strip_leading_spaces( """
         <head>
         <link rel="stylesheet" href="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.2.0/styles/agate.min.css">
         <script src="http://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.2.0/highlight.min.js"></script>
         <script>hljs.initHighlightingOnLoad();</script>
         </head>

         <body>
      """ )
      for c in self.content:
         s += c.html() + "\n"
      return s + "\n</body>\n"


# ==============================================================================
#
# documentation as a whole
#
# ==============================================================================

class documentation:

   def __init__( self, file_list ):
      self.items = []
      for file in file_list:
         self.read_file( file )

   def read_file( self, file_name ):
      processing = 0
      line_number = 0
      lines = []
      for line in open( file_name ).readlines():
         line_number += 1
         line = strip_leading_comment( line )
         if line.startswith( "@" ):
            processing = 1
         if line.startswith( "=========" ):
            if processing:
               self.items.append( item( file_name, line_number, lines ) )
               processing = 0
               lines = []
         if processing:
            lines.append( line )

   def markdown( self ):
      pass

   def html( self, dir = "html" ):
      for item in self.items:
         write_html( dir, item.title, item.html() )
         
      s = "<H2>Pages</H2>\n"
      for item in self.items:
         s += "<LI><A HREF='%s'>%s</A>\n" % \
            ( item.title + ".html", item.title )
     
      map = {}
      for item in self.items:
         for define in item.defines:         
            map[ define ] = item
        
      s += "<H2>Index</H2>\n"
      for d in sortedkeys( map ):
         s += "<LI><A HREF='%s#%s'>%s</A>\n" % \
            ( map[ d ].title + ".html", d, d )
      
      write_html( dir, "index", s )


# ==============================================================================
#
# command line interface
#
# ==============================================================================

list = [
   "../basics/gf-background.hpp",
   "../basics/gf-ints.hpp",
   "../basics/gf-attributes.hpp",
   "../chips/gf-hd44780.hpp",
   ]
d = documentation( list )
d.html()
