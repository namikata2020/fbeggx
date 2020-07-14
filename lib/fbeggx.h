/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 * 
 * eggx.h
 * ヘッダファイル
 */
 
#ifndef __FBEGGX_H__
#define __FBEGGX_H__

#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fbeggx_color.h>

/* デフォルトのレームバッファ デバイスファイル名 */
#define DEFAULT_FRAMEBUFFER "/dev/fb0"

/* デフォルトのフォントファイル　*/
#define DEFAULT_FONT "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"

#define MAX_NLAYER 8	// 最大レイヤー数
#define MAX_WINDOW 1	//最大ウィンドウ数

#define MAX_COLOR_NAME 753 //色名データの最大数

/* サポートするウィンドゥ属性 (1<<0は使わない) */
#define SCROLLBAR_INTERFACE (1<<1)
#define DOCK_APPLICATION (1<<3)
#define OVERRIDE_REDIRECT (1<<4)
#define BOTTOM_LEFT_ORIGIN (1<<7)
#define OFFSET_RIGHT (1<<6)             //右寄せにする

/* フォントサイズの指定 */
#define FONTSET 14

/* 許可・禁止 */
#define ENABLE -1
#define DISABLE 0

/* line描画 */
#define PENDOWN 2
#define PENUP   3
#define PSET    1

/* 線の設定 */
#define LineSolid 1
#define LineOnOffDash 2

/* GCファンクション　ダミーデータ */
#define GXclear         1
#define GXand           2
#define GXandReverse    3
#define GXcopy          4
#define GXandInveryed   5
#define GXnoop          6
#define GXxor           7
#define GXor            8
#define GXnor           9
#define GXequiv         10
#define GXinvert        11
#define GXorReverse     12
#define GXcopyInverted  13
#define GXorInverted    14
#define GXnand          15
#define GXset           16

/* 色情報を保存する構造体 */
struct color {
    unsigned int r,g,b;
};

/* 座標を保持する構造体 */
struct point {
    int x;
    int y;
};

/* ウィンドゥ管理用の構造体 */
struct pctg {
    int flg ;			/* 有効かどうかのフラグ */
    int attributes ;		/* EGGX での属性フラグ */
    unsigned int sbarkeymask;	/* スクロールバーのキー操作のためのkeyマスク */
    int wszx ;			/* グラフィックスエリアのサイズ */
    int wszy ;
    //Window pwin ;		/* 親 Window ID */
    //Window clipwin ;		/* 描画Window を載せるための子窓 */
    //Window hsbarwin ;		/* 水平スクロールバーのための小窓 */
    //Window vsbarwin ;		/* 垂直スクロールバーのための小窓 */
    //Window win ;		/* 描画Window (scrollbar無し時はpwinと同じ) */
    //Window iconwin ;		/* Icon Window ID */
    int sly ;			/* 表示するレイヤ */
    int wly ;			/* 書き込むレイヤ */
    //Pixmap pix[MAX_NLAYER] ;	/* 各レイヤの Pixmap ID */
    //GC pxgc ;			/* レイヤのコピー用GC */
    unsigned long bgcolor ;	/* 背景色 */
    //GC bggc ;
    int linewidth;		/* ラインの幅 */
    int linestyle;		/* ラインスタイル */
    //int gcfunc;			/* GC function */
    //int fsz ;			/* フォントサイズ */
    //XFontStruct *fontstruct ;	/* フォント */
    //XFontSet fontset ;		/* フォントセット */
    unsigned short fontheight ;
    //GC gc ;			/* 描画に使うGC */
    //Pixmap tmppix;		/* 一時 Pixmap ID */
    double acx0 ;		/* window座標(0,0) での application座標 */
    double acy0 ;
    double scalex ;		/* スケーリングファクター (xyconvで使用) */
    double scaley ;
    int prevx ;			/* moveto(), lineto() で記憶している座標 */
    int prevy ;
	FT_Library library;		//フォントライブラリ
    FT_Face    face;		//フォントフェイス
    FT_GlyphSlot slot;		//フォントスロット
    int fontload;			//フォントのロードフラグ
    uint8_t *fb8[MAX_NLAYER];			// cpp=8 のときのフレームバッファアドレス
    uint16_t *fb16[MAX_NLAYER];			// cpp=16 のときのフレームバッファアドレス
    uint32_t *fb32[MAX_NLAYER];			// cpp=32 のときのフレームバッファアドレス
    struct color fg;		//フォアグランドカラー
    uint8_t  fgc8;           //フォアグランドカラー 8bit
    uint16_t fgc15;         //フォアグランドカラー 15bit
    uint16_t fgc16;         //フォアグランドカラー 16bit
    uint32_t fgc32;         //フォアグランドカラー 32bit
    struct color bg;		//バックグランドカラー
    uint8_t  bgc8;           //バックグランドカラー 8bit
    uint16_t bgc15;         //バックグランドカラー 15bit
    uint16_t bgc16;         //バックグランドカラー 16bit
    uint32_t bgc32;         //バックグランドカラー 32bit
} ;

/*フレームバッファのデータを保存する構造体*/
struct framebuffdata {
	int fh;					//ファイルハンドラ
	int width,height;		//フレームバッファのサイズ
	int cpp,colorperpixel;	//カラー　パー　ピクセル　色数
//	void (*lowpset)(int,int,int,int);		//点を表示する関数

    int Pc_nonblock;        //キー入力でブロックするかしないか
    int Pc_nonflush;        //X11をフラッシュするか
    int Pc_attributes;      //設定の保存用
    struct color Pc_bg;        //背景色
    int offsetx,offsety;    // 表示時のオフセット
};

/* 色の名前とrgb値を保存する構造体 */
struct colorname {
    uint16_t r; //赤の輝度
    uint16_t g; //緑の輝度
    uint16_t b; //青の輝度
    char *name; //色の名前
};

#ifdef __GLOBAL_REALIZE_FBEGGX__
struct framebuffdata fbglobal = // {-1,0,0,0,0,DISABLE,DISABLE,BOTTOM_LEFT_ORIGIN,{0,0,0}};
{  .fh = -1,
    .width = 0,
    .height = 0,
    .cpp = 0,
    .colorperpixel = 0,
    .Pc_nonblock = DISABLE,
    .Pc_nonflush = DISABLE,
    .Pc_attributes = BOTTOM_LEFT_ORIGIN ,
    .Pc_bg = {0,0,0},
    .offsetx = 0,
    .offsety = 0,
};
struct pctg Pc[MAX_WINDOW];
#else /* __GLOBAL_REALIZE_FBEGGX__ */
extern struct framebuffdata fbglobal;
extern struct pctg Pc[MAX_WINDOW];
#endif /* __GLOBAL_REALIZE_FBEGGX__ */

#ifndef __GLOBAL_COLORDATA_FBEGGX__ 
extern const struct colorname colordata[MAX_COLOR_NAME];  //色情報テーブル
#endif /* __GLOBAL_COLORDATA_FBEGGX__ */

/*************************
 * 色のデータを8bitにまとめる
 * 　引数　uint8_t r:　赤色の輝度
 * 　　　　uint8_t g:　緑色の輝度
 *　　　　 uint8_t char b:　青色の輝度
 * 　戻り値　8bitにパックされた色
 */
inline static uint8_t make8color(uint8_t r, uint8_t g, uint8_t b)
{
	return (
	(((r >> 5) & 7) << 5) |
	(((g >> 5) & 7) << 2) |
	 ((b >> 6) & 3)	   );
}

/*************************
 * 色のデータを15bitにまとめる
 * 　引数　uint8_t r:　赤色の輝度
 * 　　　　uint8_t g:　緑色の輝度
 *　　　　 uint8_t b:　青色の輝度
 * 　戻り値　15bitにパックされた色
 */
inline static uint16_t make15color(uint8_t r, uint8_t g, uint8_t b)
{
	return (
	(((r >> 3) & 31) << 10) |
	(((g >> 3) & 31) << 5)  |
	 ((b >> 3) & 31));
}

/*************************
 * 色のデータを16bitにまとめる
 * 　引数　uint8_t r:　赤色の輝度
 * 　　　　uint8_t g:　緑色の輝度
 *　　　　 uint8_t b:　青色の輝度
 * 　戻り値　16bitにパックされた色
 */
inline static uint16_t make16color(uint8_t r, uint8_t g, uint8_t b)
{
	return (
	(((r >> 3) & 31) << 11) |
	(((g >> 2) & 63) << 5)  |
	 ((b >> 3) & 31));
}

/*************************
 * 色のデータを32bitにまとめる
 * 　引数　uint8_t r:　赤色の輝度
 * 　　　　uint8_t g:　緑色の輝度
 *　　　　 uint8_t b:　青色の輝度
 * 　戻り値　32bitにパックされた色
 */
inline static uint32_t make32color(uint8_t r, uint8_t g, uint8_t b)
{
	return (
	((r << 16) & 0xFF0000) |
	((g << 8) & 0xFF00) |
	(b & 0xFF));
}

/* 関数のプロトタイプ宣言 */
int gopen(int w,int h);
void gclose(int win);
void gcloseall(void);
void gresize(int win,int xsize,int ysize);
int winname( int wn, const char *argsformat, ... );
void window( int wn, double xs, double ys, double xe, double ye );
void coordinate( int wn, int xref, int yref,
		      double xs, double ys, double xscale, double yscale );
void gclr(int win);
void gresize(int win,int xsize,int ysize);

void copylayer( int wn, int src, int dst );
void layer( int wn, int sl, int wl );

/* 色の指定 */
void newrgbcolor(int win,int r,int g,int b);
void gsetbgcolor( int win, const char *argsformat, ... );
void newpen( int wn, int n );
void newcolor( int win, const char *argsformat, ... );
void newhsvcolor( int win, int h, int s, int v );
int makecolor( int cmode, double d_min, double d_max, double data,
		    int *rt_r, int *rt_g, int *rt_b );
int generatecolor( color_prms *p, double dmin, double dmax,
			double in_data, int *rt_r, int *rt_g, int *rt_b );
            
/* 点 */
void pset(int win,double x,double y);
void drawpts( int wn, const double x[], const double y[], int n );

/* 線 */
void newlinewidth( int wn, int width );
void drawline( int wn, double xg0, double yg0, double xg1, double yg1 );
void line( int wn, double xg, double yg, int mode );
void moveto( int wn, double xg, double yg );
void lineto( int wn, double xg, double yg );
void drawlines( int wn, const double x[], const double y[], int n );

/* 多角形 */
void drawpoly( int wn, const double x[], const double y[], int n );
void fillpoly(int wn, const double x[], const double y[], int n, int shape);
void drawrect(int wn,double x,double y,double w,double h);
void fillrect(int wn,double x,double y,double w,double h);

/* 円 */
void drawcirc( int wn, double xcen, double ycen, double xrad, double yrad );
void circle( int wn, double xcen, double ycen, double xrad, double yrad );
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad );
void drawarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir );
void fillarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir );

/* シンボル */
void drawsym( int wn, double xg, double yg, int size, int sym );
void drawarrow( int wn, double xs, double ys, double xt, double yt, 
		     double s, double w, int shape );

/* 文字 */
int newfontset( int wn, const char *fontfile);
int gsetfontset( int wn, const char *fontfile);
int drawstr( int wn, double x, double y, int size, double theta,
		  const char *argsformat, ... );

/* 端末関連 */
void tclr( void );
void ggotoxy(int x,int y);
int ggetch(void);
void gsetnonblock( int flag );

/* 時間管理 */
void msleep( unsigned long msec );

/* 画像関連 */
int gputimage( int wn, double x, double y,
		    unsigned char *buf, int width, int height, int msk );
unsigned char *ggetimage( int wn, int ly, 
			       double xs, double ys, double xe, double ye,
			       int *r_width, int *r_height );
unsigned char *readimage( const char *filter, const char *filename,
			       int *r_width, int *r_height, int *r_msk );
int writeimage( const unsigned char *buf, int width, int height, int msk,
		   const char *filter, int depth, const char *argsformat, ... );
void gscroll( int wn, int x, int y, int clr );
int gsaveimage( int wn, int ly, double xs,double ys, double xe,double ye,
		   const char *filter, int depth, const char *argsformat, ... );

/* Advanced関数 */
int ggetdisplayinfo( int *rt_depth, int *root_width, int *root_height );
void gsetnonflush( int flag );
int ggetnonflush( void );
void gflush( void );
void gsetinitialattributes( int values, int att_msk );
int ggetinitialattributes( void );
void gsetinitialbgcolor( const char *argsformat, ... );
void gsetborder( int wn, int width, const char *argsformat, ... );
void gsetinitialborder( int width, const char *argsformat, ... );
void gsetinitialgeometry( const char *argsformat, ... );
void gsetinitialwinname( const char *storename, const char *iconname,
			      const char *resname, const char *classname );
void gsetscrollbarkeymask( int wn, unsigned int key_mask );

/* 低レベル下請け関数 */
int openFB(const char *name);
void getFBSize(int fh,int *width, int *height, int *colorWidth);
void lowpset(int win,int x,int y);
void mpset(int win,int x,int y,int bx,int by);
void mlowline(int wn,int x1,int y1,int x2,int y2);
void lowline(int wn,int x1,int y1,int x2,int y2);
void lowcircle(int win,int x,int y,int rx,int ry);
void lowpolygonf(int win,int vnum,int *data, int shape);
struct color lowpget(int win,int ly,int x,int y);
void xyconv( int wn, double x, double y, int *rx, int *ry );
struct color get_color_pixel( char *color_name );
int _eggx_iround( double v );
void *_eggx_xmalloc( size_t size );
void *_eggx_xrealloc( void *ptr, size_t size );
char *_eggx_xstrdup( const char *s );
char *_eggx_vasprintf( const char *format, va_list ap );
char *_eggx_asprintf( const char *format, ... );

/* 未実装 */
void newlinestyle( int wn, int style );
void newgcfunction( int wn, int fnc );
void gputarea( int wn, double x, double y,
	   int src_wn, int src_ly, double xs, double ys, double xe, double ye );
int ggetevent( int *type_r, int *code_r, double *x_r, double *y_r );
int ggetxpress( int *type_r, int *code_r, double *x_r, double *y_r );

        
#endif /* __FBEGGX_H__ */
