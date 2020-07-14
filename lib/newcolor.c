/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * 色の変更
 * newrgbcolor() 
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fbeggx.h"

/* エラーチェック付きmalloc,realloc,strdup等 */
void *_eggx_xmalloc( size_t size )
{
    void *rt ;
    rt=malloc(size) ;
    if( rt==NULL ){
        fprintf(stderr,"malloc() failed.\n") ;
        exit(1) ;
    }
    return( rt ) ;
}

void *_eggx_xrealloc( void *ptr, size_t size )
{
    void *rt ;
    rt=realloc(ptr,size) ;
    if( rt==NULL ){
        fprintf(stderr,"realloc() failed.\n") ;
        exit(1) ;
    }
    return( rt ) ;
}

char *_eggx_xstrdup( const char *s )
{
    char *rt = strdup(s) ;
    if ( rt == NULL ) {
        fprintf(stderr,"strdup() failed.\n") ;
        exit(1) ;
    }
    return (rt) ;
}

char *_eggx_vasprintf( const char *format, va_list ap )
{
    const int ini_len = 64;
    char *rt = NULL;
    int nn;
    va_list aq;

    if ( format == NULL ) goto quit;

    va_copy(aq, ap);
    rt = (char *)_eggx_xmalloc(sizeof(char)*ini_len);
    nn = vsnprintf(rt,ini_len,format,ap);
    va_end(ap);

    if ( ini_len <= nn ) {
	    rt = (char *)_eggx_xrealloc(rt,sizeof(char)*(nn+1));
	    vsnprintf(rt,nn+1,format,aq);
    }
    va_end(aq);

 quit:
    return rt;
}

char *_eggx_asprintf( const char *format, ... )
{
    char *rt = NULL;
    va_list ap;

    if ( format == NULL ) goto quit;

    va_start(ap, format);
    rt = _eggx_vasprintf(format,ap);
    va_end(ap);

 quit:
    return rt;
}

int _eggx_atoh(char *buf) {
    char *ptr;
    int ret;
    ret = strtol(buf,&ptr,16);
    if(errno == EINVAL||errno == ERANGE) {
	fprintf(stderr,"%s: This string is not hexadecimal\n",buf);
	exit(EXIT_FAILURE);
    } 
    /*else if(errno != 0)  {
		fprintf(stderr,"%s: This string is not hexadecimal\n",buf);
		exit(EXIT_FAILURE);
	} */
    /*else if(ptr == buf) {
		fprintf(stderr,"%s: NO hex number is found.\n",buf);
		exit(EXIT_FAILURE);
	}*/
    if(*ptr != '\0')
    {
	fprintf(stderr,"Futher characters after number: %s\n",ptr);
	exit(EXIT_FAILURE);
    }
    return ret;
} 

/* 色の名前から pixel への変換 */
struct color get_color_pixel( char *color_name )
{
    struct color col;
    int i;

    if(color_name[0] != '#') {
        for(i=0;i<MAX_COLOR_NAME;i++) {
            if(strncmp(color_name,colordata[i].name,strlen(colordata[i].name))==0) {
            //if(strcmp(color_name,colordata[i].name)==0) {
                col.r = colordata[i].r;
                col.g = colordata[i].g;
                col.b = colordata[i].b;
                break;
            }
        }
        if(i>=MAX_COLOR_NAME) {
            fprintf(stderr,"%s: Color not found.\n",color_name );
            exit(EXIT_FAILURE);
        }
    } else {
        char buf[3];
        buf[0] = color_name[1];
        buf[1] = color_name[2];
        buf[2] = '\0';
        col.r = _eggx_atoh(buf);
        buf[0] = color_name[3];
        buf[1] = color_name[4];
        buf[2] = '\0';
        col.g = _eggx_atoh(buf);
        buf[0] = color_name[5];
        buf[1] = color_name[6];
        buf[2] = '\0';
        col.b = _eggx_atoh(buf);
    }
    return col;
}

/************
 * フォアグランドカラーの変更
 * 　引数　int win: ウィンドウ番号
 *        int r: 赤の輝度　0～255
 *        int g: 緑の輝度　0～255
 *        int b: 青の輝度　0～255
 * 　戻り値　なし
 */
void newrgbcolor(int win,int r,int g,int b)
{
    Pc[win].fg.r = r;
    Pc[win].fg.g = g;
    Pc[win].fg.b = b;
    Pc[win].fgc8  =  make8color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.b);
    Pc[win].fgc15 =  make15color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.b);
    Pc[win].fgc16 =  make16color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.b);
    Pc[win].fgc32 =  make32color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.b);
}

/************
 * バックグラウンドカラーの変更
 * 　引数　int win: ウィンドウ番号
 *        char *argsformat: 色の名前
 * 　戻り値　なし
 */void gsetbgcolor( int win, const char *argsformat, ... )
{
    char *color = NULL;
    va_list ap;

    if ( argsformat == NULL ) return;
	va_start(ap, argsformat);
	color = _eggx_vasprintf(argsformat,ap);
	va_end(ap);

	Pc[win].bg = get_color_pixel(color);
    Pc[win].bgc8 =  make8color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.r);
    Pc[win].bgc15 =  make15color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.r);
    Pc[win].bgc16 =  make16color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.r);
    Pc[win].bgc32 =  make32color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.r);
    if(color != NULL) free(color);
}

/************
 * フォアグラウンドカラーの変更
 * 　引数　int win: ウィンドウ番号
 *        char *argsformat: 色の名前
 * 　戻り値　なし
 */
void newcolor( int win, const char *argsformat, ... )
{
    char *color = NULL;
    va_list ap;

    if ( argsformat == NULL ) return;
	va_start(ap, argsformat);
	color = _eggx_vasprintf(argsformat,ap);
	va_end(ap);

	Pc[win].fg = get_color_pixel(color);
    Pc[win].fgc8 =  make8color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc15 =  make15color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc16 =  make16color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc32 =  make32color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    if(color != NULL) free(color);
}

/****************************
 * 描画色の変更
 * 　引数　int win: ウィンドウ番号
 *        int n: 色番号
 * 戻り値　なし
 */
void newpen( int wn, int n )
{
    /* { "Black","White","Red","Green",
         "Blue","Cyan","Magenta","Yellow",
         "DimGray","Grey","red4","green4",
         "blue4","cyan4","magenta4","yellow4" } */
    const int r[16] = { 0, 255, 255,   0,   0,   0, 255, 255, 105, 190, 139,   0,   0,   0, 139, 139 };
    const int g[16] = { 0, 255,   0, 255,   0, 255,   0, 255, 105, 190,   0, 139,   0, 139,   0, 139 };
    const int b[16] = { 0, 255,   0,   0, 255, 255, 255,   0, 105, 190,   0,   0, 139, 139, 139,   0 };
    if ( 0 <= n ) {
	    n = n % 16;
	    newrgbcolor(wn, r[n], g[n], b[n]);
    }
    return;
}

/* HSVで色を指定する */
/* 安田様@京都産業大学からいだたきました．感謝 :-) */
void newhsvcolor( int win, int h, int s, int v )
{
    int c1, c2, c3, r, g, b, t;

    if ( s == 0 ) {
	r = v;
	g = v;
	b = v;
    } else {
	t  = (h*6) % 360;
	c1 = (v*(255-s))/255;
	c2 = (v*(255-(s*t)/360))/255;
	c3 = (v*(255-(s*(360-t))/360))/255;
	switch (h/60) {
	case 0: 
	    r=v; g=c3; b=c1;
	    break;
	case 1: 
	    r=c2; g=v; b=c1;
	    break;
	case 2: 
	    r=c1; g=v; b=c3;
	    break;
	case 3: 
	    r=c1; g=c2; b=v;
	    break;
	case 4:
	    r=c3; g=c1; b=v;
	    break;
	default: /* should be 5 */
	    r=v; g=c1; b=c2;
	    break;
	}
    }
    newrgbcolor(win, r, g, b);
    return;
}
