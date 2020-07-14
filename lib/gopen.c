/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * グラフィックスの初期化と終了+α
 * gopen() 
 * gclose()
 * gcloseall()
 * gresize()
 * gclr()
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define __GLOBAL_REALIZE_FBEGGX__ 
#include "fbeggx.h"

/* 8bit per pixelのときのカラーマップ　保存用と設定用 */
uint16_t red[256], green[256], blue[256];
struct fb_cmap map332 = {0, 256, red, green, blue, NULL};
uint16_t red_b[256], green_b[256], blue_b[256];
struct fb_cmap map_back = {0, 256, red_b, green_b, blue_b, NULL};

/************
 * カラーマップを設定する
 * 　引数　int fh: フレームバッファのファイルハンドラ
 * 　　　　struct fb_cmap *map:　カラーマップ
 * 　戻り値　なし
 */
static void set8bppmap(int fh, struct fb_cmap *map)
{
	if (ioctl(fh, FBIOPUTCMAP, map) < 0)
	{
		fprintf(stderr, "Error putting colormap");
		exit(EXIT_FAILURE);
	}
}

/************
 * カラーマップを取得する
 * 　引数　int fh: フレームバッファのファイルハンドラ
 * 　　　　struct fb_cmap *map:　カラーマップ
 * 　戻り値　なし
 */
static void get8bppmap(int fh, struct fb_cmap *map)
{
	if (ioctl(fh, FBIOGETCMAP, map) < 0)
	{
		fprintf(stderr, "Error getting colormap");
		exit(EXIT_FAILURE);
	}
}

/************
 * カラーマップの設定を行う
 * 8bit par pixelのカラーのとき　R:3,G:3,B:2 のbit配置でカラーを
 * 表すようにカラーマップを設定する
 * 　引数　int fh: フレームバッファのファイルハンドラ
 * 　戻り値　なし
 */
static void set332map(int fh)
{
	int rs, gs, bs, i;
	int r = 8, g = 8, b = 4;

	map332.red = red;
	map332.green = green;
	map332.blue = blue;

	rs = 256 / (r - 1);
	gs = 256 / (g - 1);
	bs = 256 / (b - 1);

	for (i = 0; i < 256; i++)
	{
		map332.red[i]   = (rs * ((i / (g * b)) % r)) * 255;
		map332.green[i] = (gs * ((i / b) % g)) * 255;
		map332.blue[i]  = (bs * ((i) % b)) * 255;
	}

	set8bppmap(fh, &map332);
}

/*************************
 * フレームバッファに点を描く 8bit
 * 色はfbglobal.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
/*static void _pset8(int win,int layer,int x,int y)
{
	if((x<0)||(x>Pc[win].wszx)) return;
	if((y<0)||(y>Pc[win].wszy)) return;

	(Pc[win].fb8[layer])[y*fbglobal.width+x] = Pc[win].fgc8;
}*/

/*************************
 * フレームバッファに点を描く 15bit
 * 色はfbglobal.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
/*static void _pset15(int win,int layer,int x,int y)
{
	if((x<0)||(x>Pc[win].wszx)) return;
	if((y<0)||(y>Pc[win].wszy)) return;

	(Pc[win].fb16[layer])[y*fbglobal.width+x] = Pc[win].fgc15;
}*/

/*************************
 * フレームバッファに点を描く 16bit
 * 色はfbglobal.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
/*static void _pset16(int win,int layer,int x,int y)
{
	if((x<0)||(x>Pc[win].wszx)) return;
	if((y<0)||(y>Pc[win].wszy)) return;

	(Pc[win].fb16[layer])[y*fbglobal.width+x] = Pc[win].fgc16;
}*/

/*************************
 * フレームバッファに点を描く 32bit
 * 色はfbglobal.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
/*static void _pset32(int win,int layer,int x,int y)
{
	if((x<0)||(x>Pc[win].wszx)) return;
	if((y<0)||(y>Pc[win].wszy)) return;

	(Pc[win].fb32[layer])[y*fbglobal.width+x] = Pc[win].fgc32;
}*/

/************
 * フレームバッファをオープンする
 * 　引数　char *name: フレームバッファ デバイスファイル名 
 * 　戻り値　デバイスファイルのファイルハンドラ
 */
int openFB(const char *name)
{
	int fh;
	char *dev;

	if(name == NULL)
	{
		dev = getenv("FRAMEBUFFER");
		if(dev) name = dev;
		else name = DEFAULT_FRAMEBUFFER;
	}

	if((fh = open(name, O_RDWR)) == -1)
	{
		fprintf(stderr, "can't open %s\n", name);
		exit(EXIT_FAILURE);
	}
	return fh;
}

/************
 * フレームバッファをのサイズを取得する
 * 　引数　int fh: フレームバッファのファイルハンドラ
 * 　　　　int *width: フレームバッファの幅
 * 　　　　int *height:　フレームバッファの高さ
 * 　　　　int *colorWidth:　フレームバッファの色数
 * 　戻り値　なし
 */
void getFBSize(int fh,int *width, int *height, int *colorWidth)
{
    struct fb_var_screeninfo var;
    //struct fb_fix_screeninfo fix;
    int ret;
    //デバイス非依存のフレームバッファデバイスについての変更不能な、情報と固有のビデオモード
    //ret = ioctl(fh, FBIOGET_FSCREENINFO, &fix);
    //デバイス非依存のフレームバッファデバイスについての変更可能な情報と、固有のビデオモード
    ret = ioctl(fh, FBIOGET_VSCREENINFO, &var);
    *colorWidth = var.bits_per_pixel;
    *width      = var.xres_virtual;
    *height     = var.yres_virtual;
    if(ret) {
		fprintf(stderr,"can't get FB data\n");
		exit(EXIT_FAILURE);
	}
}

/************
 * グラフィックを使用可能にする
 * 　引数　w:ウィンドウの幅　h:ウィンドウの高さ
 * 　戻り値　ウィンドウ番号
 */
int gopen(int w,int h)
{
	int win = 0;
	fbglobal.fh = openFB(NULL);
	getFBSize(fbglobal.fh,&fbglobal.width,&fbglobal.height,&fbglobal.colorperpixel);
	
	for(int i=0;i<MAX_NLAYER;i++) {
		Pc[win].fb8[i] = NULL;
		Pc[win].fb16[i] = NULL;
		Pc[win].fb32[i] = NULL;
	}
	switch(fbglobal.colorperpixel)
	{
	case 8:
		get8bppmap(fbglobal.fh,&map_back); 	//現在のカラーマップを保存
		set332map(fbglobal.fh);				//新しいカラーマップをセット
		fbglobal.cpp = 1;
		Pc[win].fb8[0] = (uint8_t *) mmap(NULL, fbglobal.width * fbglobal.height * fbglobal.cpp, PROT_WRITE | PROT_READ, MAP_SHARED, fbglobal.fh, 0);
		if(Pc[win].fb8[0] == MAP_FAILED) {
            fprintf(stderr,"mmap error\n");
	        exit(EXIT_FAILURE);
        }
		//fbglobal.lowpset = _pset8;
		break;
	case 15:
		fbglobal.cpp = 2;
		Pc[win].fb16[0]  = (uint16_t *) mmap(NULL, fbglobal.width * fbglobal.height * fbglobal.cpp, PROT_WRITE | PROT_READ, MAP_SHARED, fbglobal.fh, 0);
		if(Pc[win].fb16[0] == MAP_FAILED) {
            fprintf(stderr,"mmap error\n");
	        exit(EXIT_FAILURE);
        }
		//fbglobal.lowpset = _pset15;
		break;
	case 16:
		fbglobal.cpp = 2;
		Pc[win].fb16[0]  = (uint16_t *) mmap(NULL, fbglobal.width * fbglobal.height * fbglobal.cpp, PROT_WRITE | PROT_READ, MAP_SHARED, fbglobal.fh, 0);
		if(Pc[win].fb16[0] == MAP_FAILED) {
            fprintf(stderr,"mmap error\n");
	        exit(EXIT_FAILURE);
        }
		//fbglobal.lowpset = _pset16;
		break;
	case 24:
	case 32:
		fbglobal.cpp = 4;
		Pc[win].fb32[0]  = (uint32_t *) mmap(NULL, fbglobal.width * fbglobal.height * fbglobal.cpp, PROT_WRITE | PROT_READ, MAP_SHARED, fbglobal.fh, 0);
		if(Pc[win].fb32[0] == MAP_FAILED) {
            fprintf(stderr,"mmap error\n");
	        exit(EXIT_FAILURE);
        }
		//fbglobal.lowpset = _pset32;
		break;
	default:
		fprintf(stderr, "Unsupported video mode! You've got: %dbpp\n", fbglobal.colorperpixel);
		exit(EXIT_FAILURE);
	}
	Pc[win].fg.r = Pc[win].fg.g = Pc[win].fg.b = 255;	//白
    Pc[win].fgc8 =  make8color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc15 =  make15color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc16 =  make16color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
    Pc[win].fgc32 =  make32color(Pc[win].fg.r,Pc[win].fg.g,Pc[win].fg.r);
	Pc[win].bg = fbglobal.Pc_bg;
    Pc[win].bgc8 =  make8color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.b);
    Pc[win].bgc15 =  make15color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.b);
    Pc[win].bgc16 =  make16color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.b);
    Pc[win].bgc32 =  make32color(Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.b);
	Pc[win].linewidth = 1;//太さ
	if((fbglobal.width<w)||(fbglobal.height<h)) {
		fprintf(stderr,"window size too learge.\n");
		exit(EXIT_FAILURE);
	}
	Pc[win].wszx = w;	//ウィンドウサイズ
	Pc[win].wszy = h;
	Pc[win].fontload = 0;//フォントを読み込んでいない
	Pc[win].sly = Pc[win].wly = 0;//書き込むレイヤーと表示するレイヤー
	Pc[win].attributes = fbglobal.Pc_attributes;
    Pc[win].acx0 = 0.0 ;		/* ウィンドゥ */
    Pc[win].acy0 = 0.0 ;
    Pc[win].scalex = 1 ;
    Pc[win].scaley = 1 ;
	fbglobal.offsetx = fbglobal.width - Pc[win].wszx;
	fbglobal.offsety = 0;//fbglobal.height - Pc[win].wszy;
	gclr(win);
	newfontset(win,DEFAULT_FONT);//デフォルトフォントの設定
	return win;
}

/************
 * グラフィックの使用を終了する
 * 　引数　ウィンドウ番号
 * 　戻り値　なし
 */
void gclose(int win)
{
	switch(fbglobal.colorperpixel)
	{
	case 8:
		set8bppmap(fbglobal.fh,&map_back); 	//カラーマップをもとに戻す
		if(munmap(Pc[win].fb8[Pc[win].sly] , fbglobal.width * fbglobal.height * fbglobal.cpp)<0) {
			fprintf(stderr,"munmap error\n");
			exit(EXIT_FAILURE);
		}
		break;
	case 15:
	case 16:
		if(munmap(Pc[win].fb16[Pc[win].sly] , fbglobal.width * fbglobal.height * fbglobal.cpp)<0) {
			fprintf(stderr,"munmap error\n");
			exit(EXIT_FAILURE);
		}
		break;
	case 24:
	case 32:
		if(munmap(Pc[win].fb32[Pc[win].sly] , fbglobal.width * fbglobal.height * fbglobal.cpp)<0){
			fprintf(stderr,"munmap error\n");
			exit(EXIT_FAILURE);
		}
		break;
	}
	for(int i = 0;i<MAX_NLAYER;i++) { //レイヤー用メモリの解放
		if(i != Pc[win].sly) {
			if(Pc[win].fb8[i]!=NULL) free(Pc[win].fb8[i]);
			if(Pc[win].fb16[i]!=NULL) free(Pc[win].fb16[i]);
			if(Pc[win].fb32[i]!=NULL) free(Pc[win].fb32[i]);
		}
	}

	if(Pc[win].fontload != 0) { //フォント関連のメモリ解放
		int error = FT_Done_Face( Pc[win].face );
		if(error) {
			fprintf(stderr,"error FT_Done_Face\n");
			exit(EXIT_FAILURE);
		}
		error = FT_Done_FreeType( Pc[win].library );
		if(error) {
			fprintf(stderr,"error FT_Done_FreeType\n");
			exit(EXIT_FAILURE);
		}
		Pc[win].fontload = 0;
	}
	close(fbglobal.fh);
}

/************
 * すべてのウィンドウを閉じる
 * 　引数　無し
 * 　戻り値　なし
 */
void gcloseall(void)
{
	fprintf(stderr,"gcloseall is not implemented.\n");
	gclose(0);
}

/************
 * グラフィック領域のサイズ変更を行う
 * 　引数　int win: ウィンドウ番号
 *        int xsize: x方向サイズ
 *        int ysize: y方向サイズ
 * 　戻り値　なし
 */
void gresize(int win,int xsize,int ysize)
{
	if((fbglobal.width<xsize)||(fbglobal.height<ysize)) {
		fprintf(stderr,"window size too learge.\n");
		exit(EXIT_FAILURE);
	}
	Pc[win].wszx = xsize;	//ウィンドウサイズ
	Pc[win].wszy = ysize;
	gclr(win);
}

/************
 * 画面を背景色で塗りつぶす
 * 　引数　int win: ウィンドウ番号
 * 　戻り値　なし
 */
void gclr(int win)
{
	struct color tmp = Pc[win].fg;

    newrgbcolor(win,Pc[win].bg.r,Pc[win].bg.g,Pc[win].bg.b);
    for(int y=0;y<Pc[win].wszy;y++) {
        for(int x=0;x<Pc[win].wszx;x++) {
            lowpset(win,x,y);//(*fbglobal.lowpset)(win,Pc[win].wly,x,y);
        }
    }
    newrgbcolor(win,tmp.r,tmp.g,tmp.b);
}
