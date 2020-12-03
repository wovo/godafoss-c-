This directory and its subdirectories contain the acual godafoss library.

The typical use is that the application defines the appropriate
GF_TARGET_* macro, and then includes godafoss.hpp.

This file includes the appropriate godafoss HAL files, which
in turn includes gf-all.hpp,
which include the individual godafoss header files.

Hence the include tree would look typically like this:

application.cpp
   godafoss.hpp
      gf-<target>.hpp
         gf-<mcu>.hpp
            gf-all.hpp
               torsor.hpp
               gf-attributes.hpp
               gf-shared-names.hpp
               ... and all other godafoss files too

The godafoss files are in the subdirectories.
