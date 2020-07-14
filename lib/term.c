
/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 * 
 * 端末関連操作
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

/*************
 *  端末のクリア 
 */
void tclr( void )
{
    printf("\033[H\033[2J") ;
    fflush(stdout) ;
}

/**************
 *  テキストカーソルの移動
 * 引数 int x: x座標
 *      int y: y座標
 * 戻り値　なし
 */
void ggotoxy(int x,int y)
{
    printf("\033[%d;%df",y,x);
}

/**************
 *  キー入力でブロックするかしないかの指定
 * 引数 int flag: ブロックしないか
 * 戻り値　なし
 */
void gsetnonblock( int flag )
{
    if ( flag == DISABLE ) {
	    fbglobal.Pc_nonblock = DISABLE ;
    } else {
        fbglobal.Pc_nonblock = ENABLE ;
    }

    return;
}

static int ggetchBL(void);
static int ggetchNB(void);
/**************************
 * 押されたキーの値を返す
 * 引数　なし
 * 戻り値　押されたキーのコード
 */
int ggetch(void)
{
    int ret;

    if(fbglobal.Pc_nonblock == DISABLE) {
        ret = ggetchBL();
    } else {
        ret = ggetchNB();
    }
    if(ret == 0x1b5b41) ret = 0x1e;//[up]
    if(ret == 0x1b5b44) ret = 0x1d;//[left]
    if(ret == 0x1b5b43) ret = 0x1c;//[right]
    if(ret == 0x1b5b42) ret = 0x1f;//[down]
    if(ret == 0x1b5b337e) ret = 0x7f;//[delete]
    return ret;
}

/**************************
 * 押されたキーの値を返す
 * キーが押されるまで戻らない。
 * 引数　なし
 * 戻り値　押されたキーのコード
 */
static int ggetchBL(void)
{
    struct termios oldt, newt;
    int ch0,ch1,ch2,ch3,ch4;
    int ret;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_iflag = ~( BRKINT | ISTRIP | IXON  );
    newt.c_lflag = ~( ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHONL );
	newt.c_cc[VTIME]    = 0;     /* キャラクタ間タイマを使わない */
	newt.c_cc[VMIN]     = 1;     /* 1文字来るまで，読み込みをブロックする */
	if(tcsetattr(STDIN_FILENO, TCSANOW, &newt)==-1) {
		fprintf(stderr,"error tcsetattr\n");
		exit(EXIT_FAILURE);
	}
	
    ch0 = getchar();
    if(ch0==0x1B) {  //矢印キー　Fnキーなどの取得　取得できないキーもある
		ch1 = getchar();
		ch2 = getchar();
		if(ch2==0x32) {
			ch3 = getchar();
			if(ch3==0x7e) {
				ret = (ch0<<24) | (ch1<<16) | (ch2<<8) | ch3;
			} else {
				ch4 = getchar();
				ret = (ch1<<24) | (ch2<<16) | (ch3<<8) | ch4;
			}
		} else if(ch2==0x31) {
			ch3 = getchar();
			ch4 = getchar();
			ret = (ch1<<24) | (ch2<<16) | (ch3<<8) | ch4;
		} else if((ch2==0x33)||(ch2==0x35)||(ch2==0x36)) {
			ch3 = getchar();
			ret = (ch0<<24) | (ch1<<16) | (ch2<<8) | ch3;
		} else {
			ret = (ch0<<16) | (ch1<<8) | ch2;
		}
	} else if(ch0 != EOF) {
		ret = ch0;
	} else {
		ret = 0;
	}

    if(tcsetattr(STDIN_FILENO, TCSANOW, &oldt)==-1) {
		fprintf(stderr,"error tcsetattr\n");
		exit(EXIT_FAILURE);
	}
	if(ret == 0x03) { //[Ctrl]+[C] で割り込み発生
		pid_t pid = getpid();
		kill(pid, SIGINT);
	}

    return ret;
}

/*************************
 * 押されたキーの値を返す
 * ブロックしない。キーが押されていない場合は戻り値は0となる。
 * 引数　なし
 * 戻り値　キーコード
 */
static int ggetchNB(void)
{
    struct termios oldt, newt;
    int ch0,ch1,ch2,ch3,ch4;
    int ret,oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_iflag = ~( BRKINT | ISTRIP | IXON  );
    newt.c_lflag = ~( ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHONL );
	newt.c_cc[VTIME]    = 0;     /* キャラクタ間タイマを使わない */
	newt.c_cc[VMIN]     = 1;     /* 1文字来るまで，読み込みをブロックする */
	//newt.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	//newt.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	//newt.c_cc[VERASE]   = 0;     /* del */
	//newt.c_cc[VKILL]    = 0;     /* @ */
	//newt.c_cc[VEOF]     = 4;     /* Ctrl-d */
	//newt.c_cc[VSWTC]    = 0;     /* '\0' */
	//newt.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	//newt.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	//newt.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	//newt.c_cc[VEOL]     = 0;     /* '\0' */
	//newt.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	//newt.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	//newt.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	//newt.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	//newt.c_cc[VEOL2]    = 0;     /* '\0' */    
	if(tcsetattr(STDIN_FILENO, TCSANOW, &newt)==-1) {
		fprintf(stderr,"error tcsetattr\n");
		exit(EXIT_FAILURE);
	}
	/* ノンブロックモードに設定 */
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	if(oldf<0) {
		fprintf(stderr,"error fcntl\n");
		exit(EXIT_FAILURE);
	}	
	if(fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK)<0) {
		fprintf(stderr,"error fcntl\n");
		exit(EXIT_FAILURE);
	}	
	
    ch0 = getchar();
    if(ch0==0x1B) {  //矢印キー　Fnキーなどの取得　取得できないキーもある
		ch1 = getchar();
		ch2 = getchar();
		if(ch2==0x32) {
			ch3 = getchar();
			if(ch3==0x7e) {
				ret = (ch0<<24) | (ch1<<16) | (ch2<<8) | ch3;
			} else {
				ch4 = getchar();
				ret = (ch1<<24) | (ch2<<16) | (ch3<<8) | ch4;
			}
		} else if(ch2==0x31) {
			ch3 = getchar();
			ch4 = getchar();
			ret = (ch1<<24) | (ch2<<16) | (ch3<<8) | ch4;
		} else if((ch2==0x33)||(ch2==0x35)||(ch2==0x36)) {
			ch3 = getchar();
			ret = (ch0<<24) | (ch1<<16) | (ch2<<8) | ch3;
		} else {
			ret = (ch0<<16) | (ch1<<8) | ch2;
		}
	} else if(ch0 != EOF) {
		ret = ch0;
	} else {
		ret = -1;
	}

    if(tcsetattr(STDIN_FILENO, TCSANOW, &oldt)==-1) {
		fprintf(stderr,"error tcsetattr\n");
		exit(EXIT_FAILURE);
	}
	if(fcntl(STDIN_FILENO, F_SETFL, oldf)<0) {
		fprintf(stderr,"error fcntl\n");
		exit(EXIT_FAILURE);
	}			
	if(ret == 0x03) { //[Ctrl]+[C] で割り込み発生
		pid_t pid = getpid();
		kill(pid, SIGINT);
	}
	/*if(ret == 0x1a) { //[Ctrl]+[Z] で割り込み発生
		pid_t pid = getpid();
		kill(pid, SIGTSTP);
	}*/
    return ret;
}
