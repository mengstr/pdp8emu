#
# KK8E with debug support -- the above hardware with an added trace package
#
# KC8M -- PDP-8/M Simple (non-visual) TTY oriented control panel
#
# KM8E -- Extended Memory and Time Share Option, M837
#	    without this option, the machine may only address 4K of memory.
#	    with this option, the machine may address 32K of memory.
#
# DK8E -- Real-Time Clock, M882 or M8830
#           the interrupt rate is selectable by editing dk8e.c
#
# KL8E -- Asynchronous Console Interface, M8650
#           the baud rate is selectable by editing kl8e.c
#
# PC8E -- Paper-Tape Reader-Punch, M840
#
# CR8F -- CR8F Card reader and Control
#
# RX8E -- RX01 diskette drive, M8357 interface
#

cpu = main.o debug.o
CPU = -DKK8E -DDEBUG

console = frontpanel-kc8m.o
CONSOLE = -DKC8M

intern =  mem-km8e.o rtc-dk8e.o
INTERN = -DKM8E -DDK8E

extern = tty-kl8e.o hstape-pc8e.o card-cr8f.o floppy-rx8e.o
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
options = $(CPU) -Wall -Wimplicit-fallthrough -Wunused-result -std=c99 $(CONSOLE) $(INTERN) $(EXTERN)
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
	cc --version
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

# Compile the code inside a docker container using this Makefile again
docker:	
	docker run --rm -v "$(PWD)":/usr/src/myapp -w /usr/src/myapp gcc:latest make check

# Run some checks to see if code works correctly
check: pdp8emu
	@echo "OK"

# make clean to delete the object files, saving disk space
clean:
	rm -f pdp8emu *.o *.bak *~
