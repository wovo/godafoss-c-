This directory and its subdirectories contain the acual godafoss library.

The typical use is that the application defines the appropriate
GF_TARGET_* macro, and then includes godafoss.hpp.

This file includes the appropriate godafoss HAL files, which
in turn includes gf-all.hpp,
which include the individual godafoss header files.

Hence the include tree would look like this:

application.cpp
   godafoss.hpp
      gf-<target-hal>.hpp
         gf-all.hpp
            torsor.hpp
            gf-attributes.hpp
            gf-shared-names.hpp
            ... all further godafoss files

The godafoss files are in the subdirectories.
