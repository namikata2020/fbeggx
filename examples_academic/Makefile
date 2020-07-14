CC = gcc
INCLUDE = -I../lib -I/usr/include/freetype2/
CFLAGS = -O3 -g
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L../lib

#OBJS = galaxies mandel raynolds_color
OBJS = galaxies mandel roche raynolds_color

all:: $(OBJS)
#	( cd simulations ; make )

clean::
	rm -f $(OBJS) *.exe
#	( cd simulations ; make clean )

CMD = sh cmd.sh ../egg -I.. -L..
CCCMD  = gcc -O -Wall

galaxies: galaxies.c libfbeggx.a
	$(CC) -o galaxies galaxies.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

mandel: mandel.c libfbeggx.a
	$(CC) -o mandel mandel.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

roche: roche.c libfbeggx.a
	$(CC) -o roche roche.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

raynolds_color: raynolds_color.c libfbeggx.a
	$(CC) -o raynolds_color raynolds_color.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

libfbeggx.a:
	cd ../lib;make
