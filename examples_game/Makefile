CC = gcc
INCLUDE = -I../lib -I/usr/include/freetype2/
CFLAGS = -O3
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L../lib

OBJS = landing
#OBJS = landing 9puzzle balance

all:: $(OBJS)

clean::
	rm -f $(OBJS) *.exe

CMD = sh cmd.sh ../egg -I.. -L..
CCCMD  = gcc -Wall

landing: landing.c libfbeggx.a
	$(CC) -o landing landing.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

9puzzle: 9puzzle_ctrl.c libfbeggx.a
	$(CC) -o 9puzzle 9puzzle_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

balance: balance.c libfbeggx.a
	$(CC) -o balance balance.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 

libfbeggx.a:
	cd ../lib;make
