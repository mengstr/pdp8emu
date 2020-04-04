# Console-based PDP-8 emulator ![C/C++ CI](https://github.com/SmallRoomLabs/pdp8emu/workflows/C/C++%20CI/badge.svg)

This PDP8 Emulator was originally written by Douglas W. Jones at the University of Iowa back in 1995.

It has now been converted from K&R style real UNIX targeted code into a more modern C99 compatible POSIX codebase with a few bugs and "syntax errors" fixed


## Files
```
original/         - Original files from 1995
Makefile          - Used by make, contains the emulator configuration
README.md         - This file

main.c		      - CPU
mem-km8e.c        - Memory manager
tty-kl8e.c        - Console interface
frontpanel-kc8m.c - Front panel (minimal function, dumb terminal)
rtc-dk8e.c		  - Real time clock option
hstape-pc8e.c     - Paper-tape reader/punch
card-cr8f.c       - Card reader
floppy-rx8e.c     - RX01 floppy disk interface
bus.h             - Contains definitions all "boards" share
debug.c           - A makefile-selectable debugging aid
realtime.c        - Internal mechanisms to keep track of time
ttyaccess.c       - Internal mechanisms for terminal interface
utility.c         - Internal mechanisms for command-line interface

coredump.c        - A utility - prints an octal dump of a saved core image.
coremake.c        - A utility - loads a RIM file into a core image.
```
