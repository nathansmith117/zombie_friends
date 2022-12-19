# Zombie Friends
Zombie Friends is based in part on the work of the FLTK project (http://www.fltk.org).

# What is Zombie Friends?
It is a crazy top down rpg zombie game where you play the
zombie.

# Compiling Unix/Linux
Needs fltk to compile.  Tested with fltk 1.3.8.  Other versions many not work correctly.

Set these flags before compiling fltk.

export CFLAGS="-fPIC"

export CXXFLAGS="-fPIC"

Make sure to change FLTK_DIR in CMakeLists.txt in game main directory.

# Compiling windows
Same as unix but needs msys2 and mingw to compile.
