
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


# Run some MAINDEC diags to see if it works correctly
check: pdp8emu coremakebin coremakerim
	./coremakebin CORE-CHEKMO < tests/CHEKMO.BN
	./pdp8emu CORE-CHEKMO 2>CORE-CHEKMO.2.tmp | tee CORE-CHEKMO.1.tmp &
		@sleep 1
		@printf "c\n" 				| nc -w 1 -u 127.0.0.1 2288
		@printf "g200\n" 			| nc -w 3 -u 127.0.0.1 2288
		@printf "MV\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "MV\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "MV\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "MV\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "MV\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "BD\r" 				| nc -w 3 -u 127.0.0.1 2288
		@printf "\03\03\03\03\03" 	| nc -w 1 -u 127.0.0.1 2288
		@printf "q\n" 				| nc -w 1 -u 127.0.0.1 2288
		@if [ "$$(grep -ao '\-\-' CORE-CHEKMO.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0AB < tests/D0AB-InstTest-1.pt
	# ./pdp8emu CORE-D0AB 2>CORE-D0AB.2.tmp | tee CORE-D0AB.1.tmp &
	# 	@sleep 1
	# 	@printf "0120/" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0261\n" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "S" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "7777\n" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "C" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao '1' CORE-D0AB.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0BB < tests/D0BB-InstTest-2.pt
	# ./pdp8emu CORE-D0BB 2>CORE-D0BB.2.tmp | tee CORE-D0BB.1.tmp & 
	# 	@sleep 1
	# 	@printf "3751/" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0262\n" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao '2' CORE-D0BB.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0CC < tests/D0CC-AddTest.pt
	# ./pdp8emu CORE-D0CC 2>CORE-D0CC.2.tmp | tee CORE-D0CC.1.tmp &
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 320
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'RANDOM' CORE-D0CC.1.tmp | wc -l)" -lt "1" ]; then false; fi

	# ./coremakebin CORE-D0DB < tests/D0DB-RandomAND.pt
	# ./pdp8emu CORE-D0DB 2>CORE-D0DB.2.tmp | tee CORE-D0DB.1.tmp &
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'A' CORE-D0DB.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0EB < tests/D0EB-Random-TAD.pt
	# ./pdp8emu CORE-D0EB 2>CORE-D0EB.2.tmp | tee CORE-D0EB.1.tmp &
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'T' CORE-D0EB.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0FC < tests/D0FC-Random-ISZ.pt
	# ./pdp8emu CORE-D0FC 2>CORE-D0FC.2.tmp | tee CORE-D0FC.1.tmp&
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'FC' CORE-D0FC.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0GC < tests/D0GC-Random-DCA.pt
	# ./pdp8emu CORE-D0GC 2>CORE-D0GC.2.tmp | tee CORE-D0GC.1.tmp &
	# 	@sleep 1
	# 	@printf "0013/" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0307\n" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'G' CORE-D0GC.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0HC < tests/D0HC-Random-JMP.pt
	# ./pdp8emu CORE-D0HC 2>CORE-D0HC.2.tmp | tee CORE-D0HC.1.tmp&
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'HC' CORE-D0HC.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0IB < tests/D0IB-JMPJMS.pt
	# ./pdp8emu CORE-D0IB 2>CORE-D0IB.2.tmp | tee CORE-D0IB.1.tmp &
	# 	@sleep 1
	# 	@printf "3567/" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0311\n" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'I' CORE-D0IB.1.tmp | wc -l)" -lt "5" ]; then false; fi

	# ./coremakebin CORE-D0JB < tests/D0JB-JMPJMS-RANDOM.pt
	# ./pdp8emu CORE-D0JB 2>CORE-D0JB.2.tmp | tee CORE-D0JB.1.tmp &
	# 	@sleep 1
	# 	@printf "0200G" 	| nc -w 1 -u 127.0.0.1 2288
	# 	@sleep 20
	# 	@printf "\03\03\03\03\03" | nc -w 1 -u 127.0.0.1 2288
	# 	@printf "Q" 		| nc -w 1 -u 127.0.0.1 2288
	# 	@if [ "$$(grep -ao 'JB' CORE-D0JB.1.tmp | wc -l)" -lt "5" ]; then false; fi



# make clean to delete the object files
clean:
	rm -f pdp8emu coremakerim coremakebin coredump *.o *.bak *.tmp CORE* *~
