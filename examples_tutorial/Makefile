CC = gcc
INCLUDE = -I../lib -I/usr/include/freetype2/
CFLAGS = -O3
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L../lib
OBJS = clock rotate sep_colorbars putimg24 plamoclock inkey 
#OBJS = clock inkey mousetst mousetst2 mousetst3 loupe plamoclock putimg24 rotate sep_colorbars

all:: $(OBJS)

clean::
	rm -f $(OBJS) *.exe

#CMD = sh cmd.sh ../egg -I.. -L..
#CCCMD  = gcc -O -Wall

clock: clock.c libfbeggx.a
	$(CC) -o clock clock.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

ayuclock: ayuclock.c libfbeggx.a
	$(CC) -o ayuclock ayuclock.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

inkey: inkey.c libfbeggx.a
	$(CC) -o inkey inkey.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

mousetst: mousetst.c libfbeggx.a
	$(CC) -o mousetst mousetst.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

mousetst2: mousetst2.c libfbeggx.a
	$(CC) -o mousetst2 mousetst2.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

mousetst3: mousetst3.c libfbeggx.a
	$(CC) -o mousetst3 mousetst3.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

loupe: loupe.c libfbeggx.a
	$(CC) -o loupe loupe.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

plamoclock: plamoclock.c libfbeggx.a
	$(CC) -o plamoclock plamoclock.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

putimg24: putimg24.c libfbeggx.a
	$(CC) -o putimg24 putimg24.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

rotate: rotate.c libfbeggx.a
	$(CC) -o rotate rotate.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

sep_colorbars: sep_colorbars.c libfbeggx.a
	$(CC) -o sep_colorbars sep_colorbars.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

libfbeggx.a:
	cd ../lib;make

