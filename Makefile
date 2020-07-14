# fbeggx make file
#libfbeggx.a
#EGGX/ProCALL をビルドする時の C コンパイラ
CC    = gcc

CFLAGS = -O3 -Wall

DESTDIR =

#ユーザが利用する C コンパイラ，フラグ
USERCC   = gcc
USERCCFLAGS = -O2 -Wall
#USERCC   = icc

#ユーザが利用する FORTRAN コンパイラ，フラグ
USERFC   = gfortran
#USERFC   = g77
#USERFC   = f77
USERFCFLAGS = -O2 -Wall
#USERFC   = ifc

#freetype のあるディレクトリ
IINC  = -I/usr/include/freetype2


#libfreetype.a libfreetype.so のあるディレクトリ
LLIB  =

PREFIX = /usr/local

INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
#LIBDIR = $(PREFIX)/lib64
BINDIR = $(PREFIX)/bin

#root になれない場合は以下の 2 行は # でコメントにする
LOCALINC = -I$(INCDIR)
LOCALLIB = -L$(LIBDIR)

#root になれない場合の設定
#(eggx*.h, libeggx.a, egg を同一ディレクトリに入れる場合は
# LOCALINC LOCALLIB は空でもよい)
#LOCALINC =
#LOCALLIB =
#LOCALINC = -I${HOME}/EGGX
#LOCALLIB = -L${HOME}/EGGX
#LOCALINC = -I${HOME}/include
#LOCALLIB = -L${HOME}/lib

LLINKS = -lfbeggx -lfreetype -lm
#LLINKS = -leggx -lX11 -lXinerama -lm

DEFS = $(VDEFS) $(TDEFS) $(UDEFS) $(PDEFS) $(XDEFS)

INCLUDE = -I./lib -I/usr/include/freetype2/
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L./lib
TARGET = sample
OBJS = $(SRCS:.c=.o)

all: $(TARGET) lib fbegg

sample: sample.c lib
	$(CC) -o sample sample.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) 


lib::
	cd lib; make

clean:
	rm -f *.o *~  fbegg fbegg.t $(TARGET)
	cd lib; make clean

install:: lib fbegg
	if [ ! -d $(DESTDIR)$(LIBDIR) ];then sh install-sh -d $(DESTDIR)$(LIBDIR); fi
	if [ ! -d $(DESTDIR)$(INCDIR) ];then sh install-sh -d $(DESTDIR)$(INCDIR); fi
	if [ ! -d $(DESTDIR)$(BINDIR) ];then sh install-sh -d $(DESTDIR)$(BINDIR); fi
	sh install-sh -m 644 ./lib/libfbeggx.a $(DESTDIR)$(LIBDIR)
	sh install-sh -m 644 ./lib/fbeggx*.h $(DESTDIR)$(INCDIR)
	sh install-sh -m 755 fbegg $(DESTDIR)$(BINDIR)
	ranlib $(DESTDIR)$(LIBDIR)/libfbeggx.a

fbegg:: fbegg.sh
	cat fbegg.sh > fbegg
	rm -f fbegg.t
	for i in USERCC "x@@$(USERCC)" USERFC "x@@$(USERFC)" \
	  USERCCFLAGS "x@@$(USERCCFLAGS)" USERFCFLAGS "x@@$(USERFCFLAGS)" \
	  IINC "x@@$(IINC)" LLIB "x@@$(LLIB)" LOCALINC "x@@$(LOCALINC)" \
	  LOCALLIB "x@@$(LOCALLIB)" LLINKS "x@@$(LLINKS)" ; do { \
	  if [ -f fbegg.t ] ; then \
	    if [ "$$i" = "x@@" ] ; then \
	      cat fbegg.t | sed -e "s|@@@TARGET@@@||" > fbegg ; \
	    else \
	      cat fbegg.t | sed -e "s|@@@TARGET@@@|$$i|" | sed -e 's|x@@||' > fbegg ; \
	    fi ; \
	    rm -f fbegg.t ; \
	  else \
	    cat fbegg | sed -e "s|@$$i@|@@@TARGET@@@|" > fbegg.t ; \
	  fi ; \
	} done
	chmod 755 fbegg
