
all: clean pdp8emu coremakerim coremakebin coredump

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
console = frontpanel-kc8m.o
intern =  mem-km8e.o rtc-dk8e.o
extern = tty-kl8e.o hstape-pc8e.o card-cr8f.o floppy-rx8e.o

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

pdp8emu: $(objects) realtime.o ttyaccess.o utility.o disasm.o
	cc --version
	cc -o pdp8emu $(objects) realtime.o ttyaccess.o utility.o disasm.o $(libraries)

$(objects) utility.o: bus.h realtime.h disasm.h Makefile

##########################################################################
#
# Secondary utilities

# this makes a utility to print an octal dump of an emulator core image
coredump: coredump.o utility.o
	cc -o coredump coredump.o utility.o

# this makes a utility to load from RIM paper tape image into emulated core
coremakerim: coremakerim.o utility.o 
	cc -o coremakerim coremakerim.o utility.o

# this makes a utility to load from BIN paper tape image into emulated core
coremakebin: coremakebin.o utility.o 
	cc -o coremakebin coremakebin.o utility.o

# Compile the code inside a docker container using this Makefile again
docker:	
	docker run --rm -v "$(PWD)":/usr/src/myapp -w /usr/src/myapp gcc:latest make

# Run some checks to see if code works correctly
check: pdp8emu coremakebin coremakerim
	./coremakebin CORE1 < tests/D0AB-InstTest-1.pt
	./pdp8emu CORE1 2>CORE1.tmp &
	@sleep 1
	@printf "5314/" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "7402\n" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "S" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "7777\n" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "0200G" > /dev/udp/127.0.0.1/2288
	@sleep 2
	@printf "C" > /dev/udp/127.0.0.1/2288
	@sleep 20
	@printf "Q" > /dev/udp/127.0.0.1/2288
	./coremakebin CORE2 < tests/D0BB-InstTest-2.pt
	./pdp8emu CORE2 2>CORE2.tmp &
	@sleep 1
	@printf "3745/" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "7402\n" > /dev/udp/127.0.0.1/2288
	@sleep 1
	@printf "0200G" > /dev/udp/127.0.0.1/2288
	@sleep 30
	@printf "Q" > /dev/udp/127.0.0.1/2288

# make clean to delete the object files, saving disk space
clean:
	rm -f pdp8emu coremakerim coremakebin coredump *.o *.bak *.tmp CORE* *~
