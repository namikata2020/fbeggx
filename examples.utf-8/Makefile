CC = gcc
INCLUDE = -I../lib -I/usr/include/freetype2/
CFLAGS = -O3
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L../lib

OBJS = 2ch nihongo rupin

all:: $(OBJS)

2ch: 2ch.c libfbeggx.a
	$(CC) -o 2ch 2ch.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

nihongo: nihongo.c libfbeggx.a
	$(CC) -o nihongo nihongo.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

rupin: rupin.c libfbeggx.a
	$(CC) -o rupin rupin.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

libfbeggx.a:
	cd ../lib;make

clean:
	rm -f $(OBJS) *.exe
