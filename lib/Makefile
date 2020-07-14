# fbeggx make file
CC = gcc
INCLUDE = -I. -I/usr/include/freetype2/
CFLAGS = -Wall -O3
LDFLAGS = 
LIBS = -lm -lfreetype
TARGET = libfbeggx.a
SRCINC = fbeggx.h
SRCS = circle.c coordinate.c gopen.c line.c polygon.c term.c colorname.c eggx_color.c\
		 layer.c newcolor.c pset.c unsupport.c syms.c drawstr.c timer.c advanced.c image.c

OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $*.c

$(TARGET): $(OBJS)
	ar crsv $(TARGET) $?
	ranlib $(TARGET)

$(OBJS): $(SRCINC)

clean:
	rm -f *.o *~ $(TARGET)