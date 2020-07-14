# FBEGGX  version 0.93
Easy and Gratifying Graphics library for frame buffer

## はじめに

　FBEGGXは[EGGX / ProCALL](http://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/)互換のライブラリでLinuxのフレームバッファ上で動作するように
作成しました．X11が動作しない環境でもグラフィックスプログラムを作成することができます．

大きな違いは次の3点ですが，EGGX / ProCALLの一部の機能をサポートしていません．
- ウィンドウシステムが存在しないため，開けるウィンドウは1つだけです．
- フォントはTrueTypeフォントを使用します．そのためint newfontset( int wn, const char *fontfile) 関数は引数にTrueTypeフォントそのものを指定します。
- マウスのサポートはありません．

## インストール方法

### TrueTypeフォントのインストールおよびlib/fbeggx.hファイルの編集

文字の表示はtruetypeフォントを使用します．1つのTruetypeフォントとfreetypeライブラリをインストールします．
debian ubuntu系では，次のようにインストールします．
```
$ sudo apt install fonts-ipafont
$ sudo apt install libfreetype6-dev
```

### ./lib/fbeggx.hファイル中のDEFAULT_FONTの書き換え
```
/* デフォルトのフォントファイル　*/
#define DEFAULT_FONT "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"
```
  フォントを指定しない場合のデフォルトのフォントとなります．

### フレームバッファデバイスを指定するための./lib/fbeggx.hファイルの編集

  　フレームバッファ・デバイスファイルは環境変数"FRAMEBUFFER"で指定することができます．
   デフォルトで使用するフレームバッファ・デバイスファイルを変更したい場合はfbeggx.hを書き換えます．
```
/* デフォルトのレームバッファ デバイスファイル名 */
#define DEFAULT_FRAMEBUFFER "/dev/fb0"
```
  の部分を書き換えてください．

### Makefile を選んで編集

Makefileの IINC, LLIB を環境にあわせて変更してください．

Makefile の CC, USERCC, USERFC 等の部分を確認し，必要に応じて修正してください．
FBEGGX をビルドする時のコンパイラと，ユーザが使うコンパイラを別々に指定できます．

FBEGGXのライブラリファイル，ヘッダファイルはデフォルトでは /usr/local/lib/,
/usr/local/include/ にインストールする設定になっています．これは Makefile 
の PREFIX，INCDIR，LIBDIR，BINDIR で指定します(ほとんどの場合は，PREFIX の
変更だけで十分だと思います)．


### make します．
```
$ make
```
### fbegg , fbeggx*.h と libfbeggx.a をコピーします．

root になれる場合，su で root になり，make install します．
```
$ su
# make install
 あるいは
$ sudo make install
```


### 動作テスト

ターミナルから，次のようにしてください．
```
$ ./sample
```
動作確認を兼ねたグラフィックスが表示されます．
  
また，次のように実行すると
```
$ cd examples_tutorial
$ make clock
$ ./clock &
```

簡単なデジタル時計が現れれば，EGGX のビルドは成功です．

さらに，基本的な日本語フォントが表示できるかを，次のようにテストします．
```
$ cd examples.utf-8
$ make nihongo
$ ./nihongo
```
14，16，24ドットで文字列が表示されればOKです．
うまく表示されない場合は，truetypeフォントがうまくインストールされていないか，
./lib/fbeggx.hファイル中のDEFAULT_FONTの設定が正しくないので訂正します．



## 実装してある関数一覧
```
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
```


EGGX/ProCALL   [Webページ](http://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/) 　　[github](https://github.com/cyamauch/eggx)

[wineggx](https://github.com/MasutaniLab/wineggx)



