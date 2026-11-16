LIBNAME=crossmac

CC=m68k-apple-macos-gcc
CXX=m68k-apple-macos-g++
AR=m68k-apple-macos-ar
RANLIB=m68k-apple-macos-ranlib
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++
#AR=powerpc-apple-macos-ar
#RANLIB=powerpc-apple-macos-ranlib

#CFLAGS=-nostdinc 
CFLAGS+=-I. -I../slowhand -DNO_DEFAULT_DRAW=1 -DDO_NOT_OVERRIDE=1

OBJS=$(subst .c,.o,$(wildcard *.c)) $(subst .cpp,.o,$(wildcard *.cpp))


.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<


all: $(OBJS)
	$(AR) rcv lib$(LIBNAME).a *.o
	$(RANLIB) lib$(LIBNAME).a
	cp *.a ..

clean:
	rm -f *.o
