# Console-based PDP-8 emulator ![C/C++ CI](https://github.com/SmallRoomLabs/pdp8emu/workflows/C/C++%20CI/badge.svg)

This PDP8 Emulator was originally written, and released as Freeware, by Douglas W. Jones at the University of Iowa back in 1995.

It has now been updated and converted from K&R style real UNIX targeted code into a more modern C99 compatible POSIX codebase with a few bugs and "syntax errors" fixed. It compiles cleanly and runs under MacOS / Linux using clang / gcc.

## Changes

### Bugfixes
- Fix missing ) preventing compilation
- Fix unintended commenting out a line of code
- Fix buggy bitmask comparison
- Fix missing stub functions in coremake/dump
- Fix buggy handling of blocking/vs unblocking stdio reads
- Make IRQ on RTC card disabled at reset as according to specs 

### Cleanups
- Removal of all X11 -relates files and functionallity
- Change K&R style function syntax into ANSI style
- Add .h files for all .c source files
- Remove speed-increase FUDGE factor
- Change BSD termio handling into POSIX
- Always include all modules/cards and remove their #ifdefs
- Remove stubs for modules/cards never implemented
- Only have externs in bus.h and then the actual globals in main.c

### New features
- Tracing of runs with dissasembly and registers
- Control of "frontpanel" and TTY input via UDP port 2288 
- New utility for creading core files from bin files

## Licence
When I asked about the freeware status the original author told me:

>I wrote both the code and the readme, and I stand by it.  It's free.  Use it any way you want, make any modifications, and feel free to pass it on to anyone for any purpose.

So with that statement I assume it is fair to re-release his code with my updates and changes under the MIT license. Anyone that wants the full freedom of the Freeware can still use the original code.

## Files
```
original/         - Original files from 1995
tests/            - PDP-8 tape images to run for testing
Makefile          - Used by make, contains the emulator configuration
README.md         - This file

main.c		      - CPU
mem-km8e.c        - Memory manager
tty-kl8e.c        - Console interface
frontpanel-kc8m.c - Front panel (minimal function, dumb terminal)
rtc-dk8e.c        - Real time clock option
hstape-pc8e.c     - Paper-tape reader/punch
card-cr8f.c       - Card reader
floppy-rx8e.c     - RX01 floppy disk interface
bus.h             - Contains definitions all "boards" share
debug.c           - A makefile-selectable debugging aid
realtime.c        - Internal mechanisms to keep track of time
ttyaccess.c       - Internal mechanisms for terminal interface
utility.c         - Internal mechanisms for command-line interface
disasm.c          - Looks up octal op-code into disassembly
coredump.c        - A utility - prints an octal dump of a saved core image.
coremakerim.c     - A utility - loads a RIM file into a core image.
coremakebin.c     - A utility - loads a BIN file into a core image.
```

## References

- Data Formats for Peripheral Device Emulation - http://homepage.divms.uiowa.edu/~jones/pdp8/formats/

- Page with a link to the original code - http://homepage.divms.uiowa.edu/~jones/pdp8/