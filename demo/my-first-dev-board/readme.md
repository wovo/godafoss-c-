These directories contain demos for the 'my first devboard' board.

The makefile.link is included by the makefiles in the demo subdirectories;
it calls the makefile.link one step up in the directory chain.

The update_codelite_workspace.bat calls bmptk to create (or update) a CodeLite
workspace file that contains a project for each subdirectory.
