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
# create pdf -> fpdf
# create rtf?
# crosslinks
# mark code in running text
# ref to source code
# a way to edit code while quoting, ... etc
# multiple hpp files can contribute to the same page;
#    in which order? with subheaders?
# order of chapters??
# check gf-attributes: should be between the code??
# pdf title page has header and footer
# pictures!

# arch:
#    sudo python -m ensurepip --upgrade
#    sudo python -m pip install fpdf
# https://www.blog.pythonlibrary.org/2018/06/05/creating-pdfs-with-pyfpdf-and-python/

# pypdf2 // no

# sudo python -m pip install restview
# restview godafoss.rest &
# sudo python -m pip install rst2pdf
# rst2pdf mydocument.txt -o mydocument.pdf
# https://docutils.sourceforge.io/docs/ref/rst/directives.html
# http://rst2pdf.ralsina.me/handbook.html

import os

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

def strip_end( s, remove ):
   s = s[ : ]
   while ( "x" + s )[ -1 ] in remove:
      s = s[ : -1 ]
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

def indent( text, prefix = "   " ):
   s = ""
   for line in text.split( "\n" ):
      s += prefix + line + "\n"
   return s

def rest_page_break( s = "oneColumn" ):
   return "\n\n.. raw:: pdf\n\n    PageBreak %s\n\n" % s

def run_rst2pdf( in_name, out_name ):
   #try:
   #   import rst2pdf
   #except:
   #   print( "install rst2pdf:\n   # sudo python -m pip install rst2pdf " )
   #   return
   #print( rstpdf.
   #rst2pdf.createpdf.createPdf( input = in_name, output = out_name )
   os.system( "rst2pdf %s -o %s " % ( in_name, out_name ) )


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

   def rest( self ):
      return self.s

   def html( self ):
      if self.s == "": return "<P>"
      return self.s

class text_bar:

   def __init__( self ):
      pass

   def markdown( self ):
      return "\n\n---------------------------------\n"

   def rest( self ):
     return "\n\n---------------------------------\n"

   def html( self ):
      return "\n\n<HR>\n"

class text_ref:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def rest( self ):
      return self.s

   def html( self ):
      if self.s == "": return "<P>"
      return self.s


class title:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s + "\n" + len( self.s ) * ""

   def rest( self ):
      t = self.s.replace( "\n", "" )
      s = ""
      s += rest_page_break()
      s += t + "\n" + len( t ) * "=" + "\n"
      return s

   def html( self ):
      return "<H2>%s</H2>" % self.s

class code:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

   def rest( self ):
      return ":: \n\n" + indent( self.s, "  " )

   def html( self ):
      return '<pre><code class="c++">%s</code></pre>' % self.s

class define:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
       return "[>%s<]" % self.s

   def rest( self ):
      return "[>%s<]" % self.s

   def html( self ):
      return '<a name="%s"></a>' % self.s

class colofon:

   def __init__( self, item ):
      self.item = item

   def markdown( self ):
      return "from: " + self.item.file_name

   def rest( self ):
      return "from: " + self.item.file_name

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

   def __init__( self, file_name, line_number, lines, refs ):
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
            rest = after( line, "@example" ).split()
            print( rest )
            if len( rest ) != 2:
               error( file_name, line_number, "example format error" )
            else:
               t = read_file( "../examples/" + rest[ 0 ] )
               s = ""
               use = 0
               for line in t.split( "\n" ):
                  if line.strip().startswith( "// @quote end" ):
                     use = 0
                  if use:
                     s += line + "\n"
                  if line.strip().startswith( "// @quote " + rest[ 1 ] ):
                     use = 1
               if( s == "" ):
                  error( file_name, line_number,
                     "example fragment '%s' not found" % rest[ 1 ] )
               self.content.append( code( s ) )

         elif line.startswith( "@define" ):
            t = after( line, "@define" ).replace( "godafoss::", "" )
            self.defines.append( t )
            self.content.append( define( t ))

         elif line.startswith( "@insert" ):
            t = after( line, "@insert" ).strip()
            if not t in refs:
               error( file_name, line_number, "uknown insert '%s'" % t )
            self.content.append( code( refs.get( t, "<>" )))

         elif line.startswith( "@ref" ):
            t = after( line, "@ref" )
            self.content.append( text_ref( t ))

         elif line.startswith( "@bar" ):
            self.content.append( text_bar())

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

   def rest( self ):
      s = ""
      for c in self.content:
         s += c.rest() + "\n"
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

      refs = {}
      line_number = 0
      count = 0
      for original_line in open( file_name ).readlines():
         line_number += 1
         line = strip_leading_comment( original_line )

         if count > 0:
            count -= 1
            quote += original_line
            if count == 0:
               s = strip_end( quote, " " )
               if s.endswith( "=\n" ): s = s[ : -1 ] + " ...\n"
               if s.endswith( "){\n" ): s = s[ : -1 ] + " ... }\n"
               if s.endswith( "\\\n" ): s = s[ : -2 ] + " ... \n"
               s = s[ : -1 ] + extra + "\n"
               refs[ name ] = refs.get( name, "" ) + s

         elif line.startswith( "@quote" ):
            split = after( line, "@quote" ).split( " " )
            if len( split ) < 2:
               error( file_name, line_number,
                  "@quote not followed by at least two elements" )
            else:
               name = split[ 0 ]
               try:
                  count = int( split[ 1 ] )
               except:
                  error( file_name, line_number,
                     "@quote invalid count" )
               quote = ""
               extra = ""
               if len( split ) > 2:
                  extra = " " + split[ 2 ]

            #if a.strip() in [ "...", None ]:
            #   refs[ "" ] = refs.get( "", "" )[ : - 1 ]  + " " + a.strip() + " "


      processing = 0
      lines = []
      line_number = 0
      for line in open( file_name ).readlines():
         line_number += 1
         line = strip_leading_comment( line )
         if line.startswith( "@quote" ):
            pass
         elif line.startswith( "@" ):
            processing = 1
         if line.startswith( "=========" ):
            if processing:
               self.items.append( item( file_name, line_number, lines, refs ) )
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

   def rest( self, file_name = "godafoss.rest" ):
      s = ""
      s += rest_page_break( "coverPage" )
      s += "==================\n"
      s += "Godafoss reference\n"
      s += "==================\n"
      s += rest_page_break()

      s += ".. header:: Godafoss reference\n\n"
      s += ".. contents::\n\n"
      s += ".. footer:: page ###Page###\n\n"
      s += ".. sectnum::\n\n"

      for item in self.items:
         s += item.rest()

      write_file( file_name, s )
      run_rst2pdf( file_name, "gf.pdf" )


# ==============================================================================
#
# command line interface
#
# ==============================================================================

list = [
   "../basics/gf-passing.hpp",
   "../basics/gf-random.hpp",
   "../basics/gf-background.hpp",
   "../basics/gf-ints.hpp",
   "../basics/gf-attributes.hpp",
   "../chips/gf-hd44780.hpp",
   "../chips/gf-hx711.hpp",
   ]
d = documentation( list )
d.html()
d.rest()
