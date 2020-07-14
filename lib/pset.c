/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * 点の描画
 * pset() 
 * mpset()
 */

#include "fbeggx.h"

/*************************
 * フレームバッファに点を描く
 * 色はframe.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
void lowpset(int win,int x,int y)
{
	if(Pc[win].attributes&OFFSET_RIGHT) {
		x = x + fbglobal.offsetx;
		y = y + fbglobal.offsety;
	}
	if((x<0)||(x>=fbglobal.width)) return;
	if((y<0)||(y>=fbglobal.height)) return;
	//if((x<0)||(x>=Pc[win].wszx)) return;
	//if((y<0)||(y>=Pc[win].wszy)) return;

	switch(fbglobal.colorperpixel)
	{
	case 8:
		(Pc[win].fb8[Pc[win].wly])[y*fbglobal.width+x] = Pc[win].fgc8;
		break;
	case 15:
		(Pc[win].fb16[Pc[win].wly])[y*fbglobal.width+x] = Pc[win].fgc15;
		break;
	case 16:
		(Pc[win].fb16[Pc[win].wly])[y*fbglobal.width+x] = Pc[win].fgc16;
		break;
	case 24:
	case 32:
		(Pc[win].fb32[Pc[win].wly])[y*fbglobal.width+x] = Pc[win].fgc32;
		break;
	}
}   

/************
 * 点を描画する
 * 　引数　int win: ウィンドウ番号
 *        int x: x座標
 *        int y: y座標
 *        int bx: x方向の太さ
 *        int by: y方向の太さ
 * 　戻り値　なし
 */
void mpset(int wn,int x,int y,int bx,int by)
{
	int x0=x,y0=y;
	if(bx>1) {
		x=x-bx/2; bx=bx/2;
		y=y-by/2; by=by/2;
	}
	while(x<x0+bx) {
		while(y<y0+by) {
			lowpset(wn,x,y);//(*fbglobal.lowpset)(win,Pc[win].wly,x,y);
			y++;
			if(y>=Pc[wn].wszx) break;
		}
		x++;
		y=y0;
		if(x>=Pc[wn].wszy) break;
	}
}
/*void mpset(int wn,int x,int y,int bx,int by)
{
	int x0=x,y0=y;

	while(x<x0+bx) {
		while(y<y0+by) {
			lowpset(wn,x,y);//(*fbglobal.lowpset)(win,Pc[win].wly,x,y);
			y++;
			if(y>=Pc[wn].wszx) break;
		}
		x++;
		y=y0;
		if(x>=Pc[wn].wszy) break;
	}
}*/

/************
 * 点を描画する
 * 　引数　int win: ウィンドウ番号
 *        double x: x座標
 *        double y: y座標
 * 　戻り値　なし
 */
void pset( int wn, double xg, double yg )
{
    int xxg,yyg;
    xyconv(wn,xg,yg,&xxg,&yyg) ;
    
    lowpset(wn, xxg, yyg ) ;
}

/************
 *  複数の点を描く
 * 　引数　int win: ウィンドウ番号
 *        double x[]: x座標の配列
 *        double y[]: y座標の配列
 *        int n: 描画する点の数
 * 　戻り値　なし
 */
void drawpts( int wn, const double x[], const double y[], int n )
{
    int i;

    if ( n < 1 ) return;

    for ( i=0 ; i < n ; i++ ) {
		pset( wn, x[i], y[i]);
    }

}

/*************************
 * フレームバッファの色を取得する
 * 色はframe.fgに設定されているものを使う
 * 　引数　int x: x座標
 * 　　　　int y: y座標
 * 　戻り値　なし
 */
struct color lowpget(int win,int ly,int x,int y)
{
	struct color c={0,0,0};
	uint8_t cl8;
	uint16_t cl15,cl16;
	uint32_t cl32;

	if(Pc[win].attributes&OFFSET_RIGHT) {
		x = x + fbglobal.offsetx;
		y = y + fbglobal.offsety;
	}
	if((x<0)||(x>=fbglobal.width)) return c;
	if((y<0)||(y>=fbglobal.height)) return c;
	//if((x<0)||(x>=Pc[win].wszx)) return c;
	//if((y<0)||(y>=Pc[win].wszy)) return c;

	switch(fbglobal.colorperpixel)
	{
	case 8:
		cl8 = (Pc[win].fb8[ly])[y*fbglobal.width+x];
		c.r = ((cl8 >> 5) & 0x07) << 5;
		c.g = ((cl8 >> 2) & 0x07) << 5;
		c.b = ((cl8     ) & 0x03) << 6;
		break;
	case 15:
		cl15 = (Pc[win].fb16[ly])[y*fbglobal.width+x];
		c.r = ((cl15 >> 10) & 0x1F) << 3;
		c.g = ((cl15 >>  5) & 0x1F) << 3;
		c.b = ((cl15      ) & 0x1F) << 3;
		break;
	case 16:
		cl16 = (Pc[win].fb16[ly])[y*fbglobal.width+x];
		c.r = ((cl16 >> 11) & 0x1F) << 3;
		c.g = ((cl16 >>  5) & 0x3F) << 2;
		c.b = ((cl16      ) & 0x1F) << 3;
		break;
	case 24:
	case 32:
		cl32 = (Pc[win].fb32[ly])[y*fbglobal.width+x];
		c.r = (cl32 >> 16) & 0xFF;
		c.g = (cl32 >>  8) & 0xFF;
		c.b = (cl32      ) & 0xFF;
		break;
	}
	return c;
}   
