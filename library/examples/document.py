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

def strip_leading_comment( s, remove = [ "//" ] ):
   s = s.strip()
   for prefix in remove:
      if s.startswith( prefix ):
         s = s[ length( prefix ) : ].strip()
   return s

def get_after( target, s, prefix ):
   if s.stratswith( prefix ):
      target = s[ length( prefix ) : ].strip()

def error( file_name, line_number, s ):
   print( "[%s:%d] error : %s" % (file_name, line-number, s ) )


# ==============================================================================
#
# text parts
#
# ==============================================================================

class text_line:

   def init( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

class title:

   def init( self, s ):
      self.s = s

   def markdown( self ):
      return self.s

class code:

   def init( self, s ):
      self.s = s

   def markdown( self ):
      return self.s


# ==============================================================================
#
# documentation item
#
# ==============================================================================

class item:

   def init( self, file_name, line_number, lines ):
      self.file_name = file_name
      self.line_number = line_number
      self.lines = lines

      self.title = none
      self.names = []
      for line in self.lines:
         self.title = get_after( self.title, line, "@title" )
      if self.title == None:
         error( self.file_name, self.line_number, "missing @title" )

      self.content = []
      line_number = self.line_number - 1
      for line in self.lines:
         line_number += 1
         if self.startswith( "@title" ):
            self.content.append( title( after( "@title" )))
         elif not self.startswith( "@" ):
            self.content.append( text_line( line ))
         else:
            error( file_name, line_number, "unknown @ ''" % line )


# ==============================================================================
#
# documentation as a whole
#
# ==============================================================================

class documentation:

   def __init__( self, file_list ):
      self.lines = []
      for file in file_list:
         read_file( file )

   def read_file( self, file_name ):
      processing = 0
      for line in open( file_name ).readlines():
         line = line.strip_leading_comment( line )
         if line.startswith( "@" ):
            processing = 1
         if line.startswith( "=========" ):
            self.items.append( item( file_name, line_number, lines )
            processing = 0
         if processing:
            self.lines.append( line )


# ==============================================================================
#
# command line interface
#
# ==============================================================================

