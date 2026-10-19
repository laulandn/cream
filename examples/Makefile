
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++

#CFLAGS=-nostdinc 
CFLAGS+=-I../CIncludes -I../slowhand -D_LATITUDE_=1

LDFLAGS=-L.. -lslowhand -ltoolbox -lcrossnull -lm


OBJS=$(subst .c,.o,$(wildcard *.c)) $(subst .cpp,.o,$(wildcard *.cpp))


.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<


all: $(OBJS) SillyBalls MWSillyBalls TubeTest LinesSilly maclines Count Sample TestPerf MPWTest


SillyBalls:	SillyBalls.o
	$(CXX) -o SillyBalls SillyBalls.o $(LDFLAGS)
#	../bin2app.sh SillyBalls
#	$(CXX) -o SillyBalls $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh SillyBalls

MWSillyBalls:	MWSillyBalls.o
	$(CXX) -o MWSillyBalls MWSillyBalls.o $(LDFLAGS)
#	../bin2app.sh MWSillyBalls
#	$(CXX) -o MWSillyBalls $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh MWSillyBalls

TubeTest:	TubeTest.o
	$(CXX) -o TubeTest TubeTest.o $(LDFLAGS)
#	../bin2app.sh TubeTest
#	$(CXX) -o TubeTest $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh TubeTest

LinesSilly:	LinesSilly.o
	$(CXX) -o LinesSilly LinesSilly.o $(LDFLAGS)
#	../bin2app.sh LinesSilly
#	$(CXX) -o LinesSilly $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh LinesSilly

maclines:	maclines.o
	$(CXX) -o maclines maclines.o $(LDFLAGS)
#	../bin2app.sh maclines
#	$(CXX) -o maclines $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh maclines

Count:	Count.o
	$(CXX) -o Count Count.o $(LDFLAGS)
#	../bin2app.sh Count
#	$(CXX) -o Count $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh Count

Sample:	Sample.o
	$(CXX) -o Sample Sample.o $(LDFLAGS)
#	../bin2app.sh Sample
#	$(CXX) -o Sample $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh Sample

TestPerf:	TestPerf.o
	$(CXX) -o TestPerf TestPerf.o $(LDFLAGS)
#	../bin2app.sh TestPerf
#	$(CXX) -o TestPerf $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh TestPerf


MPWTest:	MPWTest.o
	$(CXX) -o MPWTest MPWTest.o $(LDFLAGS)
#	../bin2app.sh MPWTest
#	$(CXX) -o MPWTest $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh MPWTest


clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb 
	rm -f SillyBalls MWSillyBalls TubeTest LinesSilly maclines Count Sample TestPerf MPWTest
