/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 * 
 * 時間関連操作
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include "fbeggx.h"

void msleep( unsigned long msec )
{
#ifndef NO_USLEEP
    unsigned long t ;
    t=(unsigned long)(msec*1000) ;
    usleep( t ) ;
#else
    int rt ;
    struct timeval delay ;
    delay.tv_sec = (msec*1000) / 1000000L ;
    delay.tv_usec = (msec*1000) % 1000000L ;
    rt = select(0,
		(fd_set *) NULL,
		(fd_set *) NULL,
		(fd_set *) NULL,
		&delay) ;
    if( rt == -1 ){
        /* perror("select") ; */
        /* return(rt) ; */
	return ;
    }
    /* return(rt) ; */
    return ;
#endif	/* NO_USLEEP */
}
