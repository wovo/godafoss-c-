# ==============================================================================
#
# make_reference.py
#
# ==============================================================================
#
# godafoss library reference extraction & formatting
#
# Copyright
#    Wouter van Ooijen (wouter@voti.nl) 2020
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
# ==============================================================================

# donate to same title from multiple pages; order?
# add external reference for torsor
# text bij de adapters loopt nog niet helemaal
# voorbeelden bij de adapters!
# SPI dummy doesn't work, make tests, check WS2812 & MAX7219
# example pin_in, pin_out, pin_oc (one-wire)
# markdown add anchors, refs, links to code, (link under examples!)
# markdown add pages, index
# leg is_ can_ uit
# make the source files external references in the pdf?
# italic for things defined on this page?
# eeprom has multi-byte address!
# add check and example for date_and_time
# add license to html and rest
# move concepts elsewehere? At least for operaror<<?
# doc as a whole is CC BY-ND-SA (due to the images)
# continue with xy<>
# comments must be CC-BY-SA in the sources??
# images must be CC-BY-SA or 'better'
# need reandom demos for both BW and color (demo/due/oled now disabled)
# arch seems to require the reset (sometimes)
# global functions... like random and color, re-instate 'two'
# separate directroy for peripheral boards?
# all<> is now for pins, but can it be fore more types and preserve the type?
# pin::all needs no_inline?
# waar komt non-bool invert vandaan?
# buffered -> buffer, like direct?
# add exmple to cto, item for CRTP enrichment
# limit TOC to 1 level
# date and time add << as external (??) support (??) check cpprtef
# continue with string
# highlighter can't handle 1'000
# show first appearance of defined word?
# set examples path
# set html path
# create rtf?
# multiple hpp files can contribute to the same page;
#    in which order? with subheaders?
# check gf-attributes: should be betweitemen the code??
# pdf title page has header and footer
# pictures! HD44780,
# US size pdf
# @image
# markdown link to library files doesn't work
# markding link to item_type doesn't work
# make examples from library files possible (eg. NO_INLINE)

# arch:
#    sudo python -m ensurepip --upgrade
#    sudo python -m pip install restview
#    restview godafoss.rest &
#    sudo python -m pip install grip
#    grip godafoss.md -b &
#    restview godafoss.rest &
#    sudo python -m pip install rst2pdf
#    rst2pdf mydocument.txt -o mydocument.pdf
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
   for prefix in remove:
      if s.strip().startswith( prefix ):
         s = s.strip()[ len( prefix ) : ]
         if s.startswith( " " ):
            s = s[ 1 : ]
   return s

def strip_end( s, remove ):
   s = s[ : ]
   while ( "x" + s )[ -1 ] in remove:
      s = s[ : -1 ]
   return s

def after( s, prefix ):
   if s.strip().startswith( prefix ):
      return s.strip()[ len( prefix ) : ].strip()
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
   try:
      import rst2pdf
   except:
      print( "install rst2pdf:\n   # sudo python -m pip install rst2pdf " )
      return

   # https://rst2pdf.org/static/manual.html
   os.system( "rst2pdf %s -o %s " % ( in_name, out_name ) )

def make_file_name( s ):
   for c in " *,<>":
      s = s.replace( c, "-" )
   return s


# ==============================================================================
#
# text parts
#
# ==============================================================================

class text_line:

   def __init__( self, s, item, defines, replace_refs ):
      self.s = s
      self.item = item
      self.defines = defines
      self.replace_refs = replace_refs

   def insert_references( self, s, replacement ):
      if not self.replace_refs:
         return s
      for name, item in self.defines:
         if item != self.item:
            start = 0
            p = s.find( name, start )
            # print( name, start, p, s )
            while p >= 0:
               if (
                  ((s + " ")[ p + len( name ) ] in " ({<:,.'s" )
                  and ((" " + s )[ p ] in " " )
               ):
                  long_name = name[:]
                  for extra in [ "<>", "()", "s" ]:
                     if s[ p + len( long_name ) : ].startswith( extra ):
                        long_name += extra
                  s0 = s[ : p ] + replacement( item.title, name, long_name )
                  start = len( s0 )
                  s = s0 + s[ p + len( long_name ) : ]
               else:
                  start = p + 1
               p = s.find( name, start )
      return s


   def markdown( self ):
      s = self.s
      s = self.insert_references( s, lambda title, name, long_name :
         '**[%s](#%s)**' %
            ( name, name ) )
      return s

   def rest( self ):
      s = self.s
      s = self.insert_references( s, lambda title, name, long_name :
         '%s_' %
            ( name ) )
      return s

   def html( self ):
      s = self.s
      if s == "": return "<P>"
      if s.strip().startswith( "-" ):
         s = s.replace( "-", "<LI>", 1 )
      s = self.insert_references( s, lambda title, name, long_name :
         '<A HREF=%s.html#%s>%s</A>' %
            ( make_file_name( title ), name, long_name ) )
      return s

class text_italic:

   def __init__( self, s ):
      self.s = s[ : ]

   def markdown( self ):
      return "*%s*" % self.s

   def rest( self ):
      return "*%s*" % self.s

   def html( self ):
      return "<I>%s</I>" % self.s

class text_bar:

   def __init__( self ):
      pass

   def markdown( self ):
      return "\n\n---------------------------------\n"

   def rest( self ):
     return "\n\n----------------------------------\n"

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
      s = "\n"
      s += 30 * "-" + "\n"
      s += 30 * "-" + "\n"
      s += "\n## %s\n" % self.s
      return s

   def rest( self ):
      t = self.s.replace( "\n", "" )
      s = ""
      s += rest_page_break()
      h = len( t ) * "*" + "\n"
      s += h + t + "\n" +  h
      return s

   def html( self ):
      return "<H2>%s</H2>" % self.s

class section:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
      return "\n### %s\n" % self.s

   def rest( self ):
      t = self.s.replace( "\n", "" )
      h = len( t ) * "*" + "\n"
      s = "\n" + t + "\n" +  h
      return s

   def html( self ):
      return "<H3>%s</H3>" % self.s

class code:

   def __init__( self, s, url = "" ):
      self.s = s
      self.url = url

   def markdown( self ):
      return "```c++\n%s```" % self.s

   def rest( self ):
      return ":: \n\n" + indent( self.s, "  " )

   def html( self ):
      if self.url == "":
         return (
            '<pre><code class="c++">%s</code></pre></A>' ) % (
            self.s )
      else:
         return (
            '<A HREF="%s" style="text-decoration:none">\n' +
            '<pre><code class="c++">%s</code></pre></A>' ) % (
            self.url, self.s )

class define:

   def __init__( self, s ):
      self.s = s

   def markdown( self ):
       return '<a name="%s"></a>' % self.s

   def rest( self ):
      return "\n.. _%s:\n" % self.s

   def html( self ):
      return '<a name="%s"></a>' % self.s

class colofon:

   def __init__( self, item ):
      self.item = item
      self.file_path = self.item.file_name
      self.file_name = self.item.file_name.replace( "../library/", "" )

   def markdown( self ):
      return "from [%s](%s)" % \
         ( self.file_name, self.file_path )

   def rest( self ):
      return "from %s\n" % self.file_name

   def html( self ):
      return 'from <A HREF="%s">%s</A>' % \
         ( self.file_path, self.file_name )


# ==============================================================================
#
# documentation item
#
# ==============================================================================

class item:

   def __init__( self, file_name, line_number, lines, refs, global_defines ):
      self.file_name = file_name
      self.line_number = line_number
      self.lines = lines

      self.title = None
      self.names = []
      self.content = []
      self.defines = []
      self.global_defines = global_defines
      line_number = self.line_number - 1
      n_code = 0

      for line in self.lines:
         line_number += 1

         if line.strip().startswith( "@title " ):
            self.content.append( title( after( line, "@title " )))
            self.content.append( colofon( self ))
            self.title = after( line, "@title " )
            self.content.append( text_bar())

         elif line.strip().startswith( "@section " ):
            self.content.append( text_bar())
            self.content.append( section( after( line, "@section " )))

         elif line.strip().startswith( "@example " ):
            rest = after( line, "@example " ).split()
            if len( rest ) != 2:
               error( file_name, line_number, "example format error" )
            else:
               example_file_name = "examples/" + rest[ 0 ]
               t = read_file( example_file_name )
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
               self.content.append( code( s, "../" + example_file_name ))

         elif line.strip().startswith( "@define " ):
            t = after( line, "@define " ).replace( "godafoss::", "" ).replace( "item::", "" )
            self.global_defines.append( [ t, self ] )
            self.content.append( define( t ))
            if t.startswith( "is_" ):
               t = t[ 3 : ]
               self.global_defines.append( [ t, self ] )
               self.content.append( define( t ))

         elif line.strip().startswith( "@insert " ):
            t = after( line, "@insert " ).strip()
            if not t in refs:
               error( file_name, line_number, "uknown insert '%s'" % t )
            self.content.append( code(
               refs.get( t, "<missing>" ), "../" + file_name))

         elif line.strip().startswith( "@code " ):
            n_code = int( after( line, "@code " ).strip() )
            code_text = ""

         elif line.strip().startswith( "@bar" ):
            self.content.append( text_bar())

         elif line.strip().startswith( "@italic " ):
            self.content.append( text_italic( after( line, "@italic " )))

         elif line.strip().startswith( "@noref " ):
            self.content.append( text_line( after( line, "@noref " ), self, global_defines, False ))

         elif not line.strip().startswith( "@" ):

            if n_code > 0:
               n_code -= 1
               code_text += "\n" + line
               if n_code == 0:
                  self.content.append( code( code_text[ 1 : ] ))

            else:
               self.content.append( text_line( line, self, global_defines, True ))

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
      self.global_defines = []
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
               if s.endswith( "\\\n" ): s = strip_end( s[ : -2 ], " " ) + "\n"
               s = s[ : -1 ] + extra + "\n"
               refs[ name ] = refs.get( name, "" ) + s

         elif line.strip().startswith( "@quote " ):
            line_after = after( line, "@quote " )
            split = line_after.split( " " )
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
                  extra = line_after.replace( split[ 0 ] + " ", "" ).replace( split[ 1 ], " " )[ 1 : ]

            #if a.strip() in [ "...", None ]:
            #   refs[ "" ] = refs.get( "", "" )[ : - 1 ]  + " " + a.strip() + " "


      processing = 0
      lines = []
      line_number = 0
      for line in open( file_name ).readlines():
         line_number += 1
         line = strip_leading_comment( line )
         if line.strip().startswith( "@quote" ):
            pass
         elif line.strip().startswith( "@" ):
            processing = 1
         if line.strip().startswith( "=========" ):
            if processing:
               self.items.append( item( file_name, line_number, lines, refs, self.global_defines ) )
               processing = 0
               lines = []
         if processing:
            if not line.strip().startswith( "---------" ):
               lines.append( line )

      self.items.sort( key = lambda item: item.title)

   def markdown( self, file_name = "godafoss.md"  ):
      s = ""

      s += '![Godafoss waterfalls](../images/godafoss-waterfalls.jpg "the Godafoss waterfalls")\n'
      s += "# Godafoss reference\n"

      for item in self.items:
         s += item.markdown()

      write_file( file_name, s )
      return

      s = ""
      s += '<IMG SRC="../images/godafoss-waterfalls.jpg" alt="Godafoss waterfalls" width="400" >\n'

      s += "<H2>Pages</H2>\n"
      for item in self.items:
         s += "<LI><A HREF='%s'>%s</A>\n" % \
            ( make_file_name( item.title ) + ".html", item.title )

      map = {}
      for name, item in self.global_defines:
            map[ name ] = item

      s += "<H2>Index</H2>\n"
      for d in sortedkeys( map ):
         s += "<LI><A HREF='%s#%s'>%s</A>\n" % \
            ( make_file_name( map[ d ].title ) + ".html", d, d )

      write_html( dir, "index", s )

   def html( self, dir = "html" ):
      try:
         os.mkdir( "html" )
      except:
         pass

      for item in self.items:
         write_html( dir, make_file_name( item.title ), item.html() )

      s = ""
      s += '<IMG SRC="../images/godafoss-waterfalls.jpg" alt="Godafoss waterfalls" width="400" >\n'

      s += "<H2>Pages</H2>\n"
      for item in self.items:
         s += "<LI><A HREF='%s'>%s</A>\n" % \
            ( make_file_name( item.title ) + ".html", item.title )

      map = {}
      for name, item in self.global_defines:
            map[ name ] = item

      s += "<H2>Index</H2>\n"
      for d in sortedkeys( map ):
         s += "<LI><A HREF='%s#%s'>%s</A>\n" % \
            ( make_file_name( map[ d ].title ) + ".html", d, d )

      write_html( dir, "index", s )

   def rest( self, file_name = "godafoss.rest" ):
      s = ""
      s += rest_page_break( "coverPage" )

      s += "\n"
      s += ".. image:: images/godafoss-waterfalls.jpg\n";
      s += "   :width: 1800\n"
      s += "   :alt: Godafoss waterfall\n\n"

      #s += "==================\n"
      s += "Godafoss reference\n"
      #s += "==================\n"
      s += rest_page_break()

      s += ".. header:: Godafoss reference\n\n"
      s += ".. contents::\n\n"
      s += ".. footer:: page ###Page###\n\n"
      s += ".. sectnum::\n\n"

      for item in self.items:
         s += item.rest()

      write_file( file_name, s )
      run_rst2pdf( file_name, "godafoss.pdf" )


# ==============================================================================
#
# command line interface
#
# ==============================================================================

list = []
for dirpath, dirnames, filenames in os.walk( "../library" ):
   #print( dirpath, dirnames, filenames )
   for file in filenames:
      if file.endswith ( ".hpp"       ):
         list.append( dirpath + "/" + file )

d = documentation( list )
d.html()
d.markdown()
d.rest()
