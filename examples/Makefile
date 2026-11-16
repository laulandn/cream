
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++

#CFLAGS=-nostdinc 
CFLAGS+=-I../CIncludes -I../slowhand -D_LATITUDE_=1
#CFLAGS=-I../real

LDFLAGS=-L.. -lslowhand -ltoolbox -lcrossnull -lm


OBJS=$(subst .c,.o,$(wildcard *.c)) $(subst .cpp,.o,$(wildcard *.cpp))


.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<


all: $(OBJS) SillyBalls MWSillyBalls TubeTest LinesSilly maclines Count Sample TestPerf MPWTest DoubleBuffer SndForEver TestGestaltQD TestGestaltVM Rotate PaletteAnimation GiMeDaPaletteAndSound LaunchWithDoc VoxII CopyMask


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

DoubleBuffer:	DoubleBuffer.o
	$(CXX) -o DoubleBuffer DoubleBuffer.o $(LDFLAGS)
	../bin2app.sh DoubleBuffer
#	$(CXX) -o MPWTest $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh MPWTest

SndForEver:	SndForEver.o
	$(CXX) -o SndForEver SndForEver.o $(LDFLAGS)
	../bin2app.sh SndForEver
#	$(CXX) -o SndForEver $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh SndForEver

TestGestaltQD:	TestGestaltQD.o
	$(CXX) -o TestGestaltQD TestGestaltQD.o $(LDFLAGS)
	../bin2app.sh TestGestaltQD
#	$(CXX) -o TestGestaltQD $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh TestGestaltQD

TestGestaltVM:	TestGestaltVM.o
	$(CXX) -o TestGestaltVM TestGestaltVM.o $(LDFLAGS)
	../bin2app.sh TestGestaltVM
#	$(CXX) -o TestGestaltVM $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh TestGestaltVM

Rotate:	Rotate.o
	$(CXX) -o Rotate Rotate.o $(LDFLAGS)
	../bin2app.sh Rotate
#	$(CXX) -o Rotate $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh Rotate

PaletteAnimation:	PaletteAnimation.o
	$(CXX) -o PaletteAnimation PaletteAnimation.o $(LDFLAGS)
	../bin2app.sh PaletteAnimation
#	$(CXX) -o PaletteAnimation $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh PaletteAnimation

GiMeDaPaletteAndSound:	GiMeDaPaletteAndSound.o
	$(CXX) -o GiMeDaPaletteAndSound GiMeDaPaletteAndSound.o $(LDFLAGS)
	../bin2app.sh GiMeDaPaletteAndSound
#	$(CXX) -o GiMeDaPaletteAndSound $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh GiMeDaPaletteAndSound

LaunchWithDoc:	LaunchWithDoc.o
	$(CXX) -o LaunchWithDoc LaunchWithDoc.o $(LDFLAGS)
	../bin2app.sh LaunchWithDoc
#	$(CXX) -o LaunchWithDoc $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh LaunchWithDoc

VoxII:	VoxII.o
	$(CXX) -o VoxII VoxII.o $(LDFLAGS)
	../bin2app.sh VoxII
#	$(CXX) -o VoxII $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh VoxII

CopyMask:	CopyMask.o
	$(CXX) -o CopyMask CopyMask.o $(LDFLAGS)
	../bin2app.sh CopyMask
#	$(CXX) -o CopyMask $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh CopyMask


clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb 
	rm -f SillyBalls MWSillyBalls TubeTest LinesSilly maclines Count Sample TestPerf MPWTest DoubleBuffer SndForEver TestGestaltQD TestGestaltVM Rotate PaletteAnimation GiMeDaPaletteAndSound LaunchWithDoc VoxII CopyMask
