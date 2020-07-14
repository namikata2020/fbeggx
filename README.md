=============================================================================
               Easy and Gratifying Graphics library for frame buffer
                       FBEGGX  version 0.93
=============================================================================

●はじめに

　FBEGGXはEGGX / ProCALL互換のライブラリでLinuxのフレームバッファ上で動作するように
作成しました．X11が動作しない環境でもグラフィックスプログラムを作成することができます。

大きな違いは次の3点ですが，EGGX / ProCALLの一部の機能をサポートしていません．
○ウィンドウシステムが存在しないため，開けるウィンドウは1つだけです．
○フォントはTrueTypeフォントを使用します．そのためint newfontset( int wn, const char *fontfile)
　関数は引数にTrueTypeフォントそのものを指定します。
○マウスのサポートはありません．

実装してある関数一覧

int gopen(int w,int h);
void gclose(int win);
void gcloseall(void);
void gresize(int win,int xsize,int ysize);
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
int newfontset( int wn, const char *fontfile);    <----Truetypeフォントの指定に仕様変更
int gsetfontset( int wn, const char *fontfile);   <----Truetypeフォントの指定に仕様変更
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
void gsetinitialattributes( int values, int att_msk );
int ggetinitialattributes( void );
void gsetinitialbgcolor( const char *argsformat, ... );




以下EGGX / ProCALLの解説です．
-----------------------------------------------------------
  EGGX / ProCALL は， 究極の簡単さを目指して作成した X11 グラフィックスライブ
ラリです．懐かしの BASIC 風の関数で簡単に 2D グラフィックスを楽しむことができ
ます．

  このライブラリは，作り始めた頃(1999年頃)は「Pro-FORTRAN」と呼ばれる FORTRAN
の API と互換だったので，「PROCALL」と呼んでいました．しかし，その後の拡張に
よって C の関数は Pro-FORTRAN の原型をとどめていない状態になったので，

 - このライブラリの C の関数群 -> EGGX
 - このライブラリの FORTRAN サブルーチン群 -> ProCALL

と呼ぶことにしました．

 「EGG」とは Xlib の機能の一部を使っているという意味での「卵」と，プログラマ
の「卵」であるビギナーの「絵を描きたい」「ゲームを作りたい」といった希望をか
なえよう，という意味合いがあります．

  EGGX の API を使うと，AfterStep や WindowMaker のアプレットが簡単に作成でき
ます．examples_tutorial ディレクトリの plamoclock.c をご覧いただくとわかりま
すが，デジタル時計程度のものであれば，わずか数十行で作れてしまいます．EGGX を
使ってオリジナルのアプレットを作ってみるのも楽しいかもしれません．

  EGGX/ProCALL が多くの「卵」を育てる道具になれば幸いです．


●インストール方法・補助ツールについて

  INSTALL.ja をご覧ください．


●サンプルコード

  EXAMPLES.ja をご覧ください．


●マニュアル

  pdf のユーザーズガイド eggx_procall.ja.pdf をご覧ください．簡単なチュートリ
アルと関数の詳細な説明があります．

  EGGX/ProCALL の Web ページ，

  http://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/

  にも同じマニュアルを置いています。
-----------------------------------------------------------------


