
all:
	make -C slowhand -f Makefile
	make -C toolbox -f Makefile
	make -C crossnull -f Makefile
	make -C crossfb -f Makefile
#	make -C crossmac -f Makefile
	make -C crossx11 -f Makefile
	make -i -C examples -f Makefile
	
clean:
	make -C slowhand -f Makefile clean
	make -C toolbox -f Makefile clean
	make -C crossnull -f Makefile clean
	make -C crossfb -f Makefile clean
	make -C crossmac -f Makefile clean
	make -C crossx11 -f Makefile clean
	make -C examples -f Makefile clean
	rm -f */*.o */*.a *.a
