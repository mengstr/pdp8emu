# make a PDP-8 emulator
#
# Instructions:
#
#   1, configure your emulator by editing this file, as directed below.
#   2, make.
#   3, make coredump if you want symbolic dumps of saved memory images.
#   4, make coremake if you want to convert RIM binaries to memory images.
#   5, make clean to remove compilation temporaries.


##########################################################################
#
# Machine Configuration Section:
#
#   In effect, the skeleton of the emulator is an omnibus, into which you
#   plug a number of option boards for I/O devices and other machine
#   components.
#
#   RULE:  To select an option, follow the directions.
#          If, on real hardware, you'd have to jumper something on the
#          circuit board, then you must edit the source file for the
#          option and select the jumperable feature there.


#---- exactly one of the following definition pairs must be uncommented

# KK8E -- the PDP-8/E CPU, M8300, M8310, M8320
#cpu =   kk8e.o
#CPU = -DKK8E

# KK8E with debug support -- the above hardware with an added trace package
cpu =   kk8e.o debug.o
CPU = -DKK8E -DDEBUG


#---- exactly one of the following definition triplets must be uncommented

# KC8M -- PDP-8/M Simple (non-visual) TTY oriented control panel
console =   kc8m.o
CONSOLE = -DKC8M
conslib =


#---- any of the following internal options may be selected by including
#     their names in the definitions of intern and INTERN.

# KM8E -- Extended Memory and Time Share Option, M837
#	    without this option, the machine may only address 4K of memory.
#	    with this option, the machine may address 32K of memory.

# DK8E -- Real-Time Clock, M882 or M8830
#           the interrupt rate is selectable by editing dk8e.c

intern =  km8e.o dk8e.o
INTERN = -DKM8E -DDK8E


#---- any of the following external options may be selected by including
#     their names in the defintions of extern and EXTERN.

# KL8E -- Asynchronous Console Interface, M8650
#           the baud rate is selectable by editing kl8e.c

# PC8E -- Paper-Tape Reader-Punch, M840


# CR8F -- CR8F Card reader and Control

# RX8E -- RX01 diskette drive, M8357 interface

extern = kl8e.o pc8e.o cr8f.o rx8e.o
EXTERN = -DKL8E -DPC8E -DCR8F -DRX8E


#---- Memory; on a real machine, the amount of memory can be selected
#     as any multiple of 4096 up to 32768.  Here, to avoid the cost of
#     address validity checking, partial memory configurations may not
#     be specified.  The constant MAXMEM is therefore not set here; it
#     is set in bus.h and depends on the presence of the KM8E option.


##########################################################################
#
# Patch together the list of object files and the list of compiler
# options from the above

INTERP = -DPDP8NAME=\"$(INTERPRETER)\"
options = $(CPU) -std=c99 $(CONSOLE) $(INTERN) $(EXTERN)
objects = $(cpu) $(console) $(intern) $(extern)
libraries = $(conslib)


##########################################################################
#
# Override make's complex understanding of file suffixes and make sure
# that all c compiles use the options we've set up.

.SUFFIXES: .o .c

.c.o:
	cc -c -O $(options) $*.c

##########################################################################
#
# Actual makefile dependencies for the PDP-8 emulator
#
# Note that, since this makefile contains the option settings,
# it references itself!

pdp8emu: $(objects) realtime.o ttyaccess.o utility.o
	cc -o pdp8emu $(objects) realtime.o ttyaccess.o utility.o $(libraries)

$(objects) utility.o: bus.h realtime.h Makefile

##########################################################################
#
# Secondary utilities

# this makes a utility to print an octal dump of an emulator core image
coredump: coredump.o utility.o
	cc -o coredump coredump.o utility.o

# this makes a utility to load from RIM paper tape image into emulated core
coremake: coremake.o utility.o
	cc -o coremake coremake.o utility.o

# make clean to delete the object files, saving disk space
clean:
	rm -f *.o
