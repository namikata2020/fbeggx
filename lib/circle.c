/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * 円・弧の描画
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fbeggx.h"

/*************************
 * 楕円を描く
 *  引数　int wn: ウィンドウ番号　
 *        int x,y: 円の中心座標
 * 　　　 int rx: x方向の半径
 *       int ry: y方向の半径
 * 戻り値　なし
 */
void lowcircle(int win,int x,int y,int rx,int ry)
{
    int wdt = Pc[win].linewidth;
	if(rx>ry) { 
		int x0=rx,s=x0; 
		int y0=0; 
		while(x0>=y0) { 
			int x1=(long int)x0*ry/rx; 
			int y1=(long int)y0*ry/rx; 
			mpset(win,x+x0,y+y1,wdt,wdt);mpset(win,x+x0,y-y1,wdt,wdt); 
			mpset(win,x-x0,y+y1,wdt,wdt);mpset(win,x-x0,y-y1,wdt,wdt); 
			mpset(win,x+y0,y+x1,wdt,wdt);mpset(win,x+y0,y-x1,wdt,wdt); 
			mpset(win,x-y0,y+x1,wdt,wdt);mpset(win,x-y0,y-x1,wdt,wdt); 
			s-=2*y0-1;
			if(s<0) s+=2*(x0-1),x0--;
			y0++; 
		} 
	} else { 
		int x0=ry,s=x0; 
		int y0=0; 
		while(x0>=y0) { 
			int x1=(long int)x0*rx/ry; 
			int y1=(long int)y0*rx/ry; 
			mpset(win,x+x1,y+y0,wdt,wdt);mpset(win,x+x1,y-y0,wdt,wdt); 
			mpset(win,x-x1,y+y0,wdt,wdt);mpset(win,x-x1,y-y0,wdt,wdt); 
			mpset(win,x+y1,y+x0,wdt,wdt);mpset(win,x+y1,y-x0,wdt,wdt); 
			mpset(win,x-y1,y+x0,wdt,wdt);mpset(win,x-y1,y-x0,wdt,wdt); 
			s-=2*y0-1;
			if(s<0) s+=2*(x0-1),x0--;
			y0++; 
		} 
	}
}

/*************************
 * 塗りつぶした楕円を描く
 *  引数　int win: ウィンドウ番号
 *      　int x,y: 円の中心座標
 * 　　　 int rx: x方向の半径
 *       int ry: y方向の半径
 * 戻り値　なし
 */
static void lowcirclef(int win,int x,int y,int rx,int ry)
{
	if(rx>ry) { 
		int x0=rx,s=x0; 
		int y0=0; 
		while(x0>=y0) { 
			int x1=(long int)x0*ry/rx; 
			int y1=(long int)y0*ry/rx; 
			lowline(win,x+x0,y+y1,x+x0,y-y1); 
			lowline(win,x-x0,y+y1,x-x0,y-y1); 
			lowline(win,x+y0,y+x1,x+y0,y-x1); 
			lowline(win,x-y0,y+x1,x-y0,y-x1); 
			s-=2*y0-1;
			if(s<0) s+=2*(x0-1),x0--;
			y0++; 
		} 
	} else { 
		int x0=ry,s=x0; 
		int y0=0; 
		while(x0>=y0) { 
			int x1=(long int)x0*rx/ry; 
			int y1=(long int)y0*rx/ry; 
			lowline(win,x+x1,y+y0,x+x1,y-y0); 
			lowline(win,x-x1,y+y0,x-x1,y-y0); 
			lowline(win,x+y1,y+x0,x+y1,y-x0); 
			lowline(win,x-y1,y+x0,x-y1,y-x0); 
			s-=2*y0-1;
			if(s<0) s+=2*(x0-1),x0--;
			y0++; 
		} 
	}
}

/*************************
 * 楕円を描く
 *  引数　int wn: ウィンドウ番号
 *      　int xcen,ycen: 円の中心座標
 * 　　　 int xrad: x方向の半径
 *       int yrad: y方向の半径
 * 戻り値　なし
 */
void drawcirc( int wn, double xcen, double ycen, double xrad, double yrad )
{
    int x0,y0,x1,y1,xsize,ysize ;

    xyconv(wn,xcen,ycen,&x0,&y0) ;
    xyconv(wn,xcen+xrad,ycen+yrad,&x1,&y1) ;
    xsize=abs(x1-x0) ;
    ysize=abs(y1-y0) ;
	lowcircle(wn,x0,y0,xsize,ysize);
}

/*************************
 * 楕円を描く
 *  引数　int wn: ウィンドウ番号
 *      　double xcen,ycen: 円の中心座標
 * 　　　 double xrad: x方向の半径
 *       double yrad: y方向の半径
 * 戻り値　なし
 */
void circle( int wn, double xcen, double ycen, double xrad, double yrad )
{
	drawcirc( wn, xcen, ycen, xrad, yrad );
}

/*************************
 * 塗りつぶした楕円を描く
 *  引数　int wn: ウィンドウ番号
 *      　double xcen,ycen: 円の中心座標
 * 　　　 double xrad: x方向の半径
 *       double yrad: y方向の半径
 * 戻り値　なし
 */
void fillcirc( int wn, double xcen, double ycen, double xrad, double yrad )
{
    int x0,y0,x1,y1,xsize,ysize ;

    xyconv(wn,xcen,ycen,&x0,&y0) ;
    xyconv(wn,xcen+xrad,ycen+yrad,&x1,&y1) ;
    xsize=abs(x1-x0) ;
    ysize=abs(y1-y0) ;
	lowcirclef(wn,x0,y0,xsize,ysize);
}

/*************************
 * 角度を0〜2πの間に変換する
 *  引数　double angle: 角度
 * 戻り値　0〜2πの間の角度
 */
static double Rangle(double angle)
{
	while(angle<0) angle += 2*M_PI;
	while(angle>=2*M_PI) angle -= 2*M_PI;
	return angle;
}

/***************************
 * 楕円と中心から角度angleで伸びる直線との交点を求める
 * 引数  int x:　描画するx座標
 *      int y:　描画するy座標
 *      int rx: x方向半径
 *      int ry: y方向半径
 *      int angle:　角度(0〜2π)
 * 　　　int px:　交点x座標
 *      int py:　交点y座標
 * 戻り値　なし
 */
static void calcArcPoint(int x,int y,int rx,int ry,double angled,int *px,int *py)
{
	double r,a,b;
	angled = Rangle(angled);

	r=1.0;
	a=1.0/((double)rx*(double)rx);
	b=1.0/((double)ry*(double)ry);
		 
	if(angled==M_PI/2) {			//90度
		*px = x;
		*py = y - ry;
	} else if(angled==M_PI*3/2) { //270度
		*px = x;
		*py = y + ry;
	} else {
		double m = tan(angled);
		*px = sqrt((r*r)/(a + b * m*m));
		*py = sqrt((r*m*r*m)/(a + b * m*m));

		if(angled<M_PI/2) {
			*px = x + *px;
			*py = y - *py;
		} else if(angled<M_PI) {
			*px = x - *px;
			*py = y - *py;
		} else if(angled<M_PI*3/2) {
			*px = x - *px;
			*py = y + *py;
		} else {
			*px = x + *px;
			*py = y + *py;
		}
	}
}
static void dcalcArcPoint(double x,double y,double rx,double ry,double angled,double *px,double *py)
{
	double r,a,b;
	angled = Rangle(angled);

	r=1.0;
	a=1.0/(rx*rx);
	b=1.0/(ry*ry);
		 
	if(angled==M_PI/2) {			//90度
		*px = x;
		*py = y - ry;
	} else if(angled==M_PI*3/2) { //270度
		*px = x;
		*py = y + ry;
	} else {
		double m = tan(angled);
		*px = sqrt((r*r)/(a + b * m*m));
		*py = sqrt((r*m*r*m)/(a + b * m*m));

		if(angled<M_PI/2) {
			*px = x + *px;
			*py = y - *py;
		} else if(angled<M_PI) {
			*px = x - *px;
			*py = y - *py;
		} else if(angled<M_PI*3/2) {
			*px = x - *px;
			*py = y + *py;
		} else {
			*px = x + *px;
			*py = y + *py;
		}
	}
}

/*************************
 * フレームバッファに楕円を描く
 *  引数　int cx,cy: 円の中心座標
 * 　　　 int rx: x方向の半径
 *       int ry: y方向の半径
 *       double Sangle: 描画スタート角度
 *       double Eangle: 描画終了角度
 * 戻り値　なし
 *
 * 参考
 * 円弧を描いてみよう
 * http://www.iot-kyoto.com/satoh/2016/01/23/algorithm-001/
*/
static void lowarc(int win,int cx, int cy, int rx,int ry, double Sangle,double Eangle)
{
	int dx, dy, s ;
	int x1, y1, x2, y2, x3, y3, x4, y4 ;
	int sx,sy,ex,ey;
	int psx, psy, pex, pey, ss, es, i, j ;
	unsigned int pass[4] ;
    int wdt = Pc[win].linewidth;

	calcArcPoint(cx,cy,rx,ry,Sangle,&sx,&sy);
	calcArcPoint(cx,cy,rx,ry,Eangle,&ex,&ey);
	psx = sx - cx ;
	pex = ex - cx ;
	psy = sy - cy ;
	pey = ey - cy ;
								// 始点の象限
	if (psx >= 0) {
		if (psy >= 0) ss = 3 ;
		else          ss = 0 ;
	} else {
		if (psy >= 0) ss = 2 ;
		else          ss = 1 ;
	}
								// 終点の象限
	if (pex >= 0) {
		if (pey >= 0) es = 3 ;
		else          es = 0 ;
	} else {
		if (pey >= 0) es = 2 ;
		else          es = 1 ;
	}

	if (ss == es) {										// 始点終点の象限が同じ
		if (((psy <  0) && (psx >= pex)) ||
			((psy >= 0) && (psx <= pex))) {
			for (i = 0 ; i < 4 ; i++) pass[i] = 0 ;		// 小円( < 90 )
			pass[ss] = 5 ;
		} else {
			for (i = 0 ; i < 4 ; i++) pass[i] = 1 ;		// 大円( > 270 )
			pass[ss] = 4 ;
		}
	} else {
		for (i = 0 ; i < 4 ; i++) pass[i] = 0 ;
		j = ss ;
		for (i = 0 ; i < 4 ; i++) {
			pass[j] = 1 ;								// 全表示
			if (j == es) break ;
			++j ;
			if (j > 3) j = 0 ;
		}
		pass[ss] = 2 ;									// 始点のみ
		pass[es] = 3 ;									// 終点のみ
	}
	
	if(rx>ry) {
		dx = rx ;
		s = rx ;
		dy = 0 ;

		while (dx >= dy) {
			unsigned int  flag ;
			int dxx = dx*ry/rx;
			int dyy = dy*ry/rx;
			x1 = cx + dx;
			x2 = cx - dx;
			x3 = cx + dy;
			x4 = cx - dy;
			y1 = cy + dyy;
			y2 = cy - dyy; 
			y3 = cy + dxx;
			y4 = cy - dxx;
			

			flag = 0x00 ;
											// 第１象限
			switch (pass[0]) {
				default:
					break ;
				case    1:
					flag = 0x03 ;
					break ;
				case    2:
					if ((x1 <= sx) && (y2 <= sy)) flag |= 0x01 ;
					if ((x3 <= sx) && (y4 <= sy)) flag |= 0x02 ;
					break ;
				case    3:
					if ((x1 >= ex) && (y2 >= ey)) flag |= 0x01 ;
					if ((x3 >= ex) && (y4 >= ey)) flag |= 0x02 ;
					break ;
				case    4:
					if (((x1 <= sx) || (x1 >= ex)) && ((y2 <= sy) || (y2 >= ey))) flag |= 0x01 ;
					if (((x3 <= sx) || (x3 >= ex)) && ((y4 <= sy) || (y4 >= ey))) flag |= 0x02 ;
					break ;
				case    5:
					if ((x1 <= sx) && (x1 >= ex) && (y2 <= sy) && (y2 >= ey)) flag |= 0x01 ;
					if ((x3 <= sx) && (x3 >= ex) && (y4 <= sy) && (y4 >= ey)) flag |= 0x02 ;
					break ;
			}
											// 第２象限
			switch (pass[1]) {
				default:
					break ;
				case    1:
					flag |= 0x0C ;
					break ;
				case    2:
					if ((x4 <= sx) && (y4 >= sy)) flag |= 0x04 ;
					if ((x2 <= sx) && (y2 >= sy)) flag |= 0x08 ;
					break ;
				case    3:
					if ((x4 >= ex) && (y4 <= ey)) flag |= 0x04 ;
					if ((x2 >= ex) && (y2 <= ey)) flag |= 0x08 ;
					break ;
				case    4:
					if (((x4 <= sx) || (x4 >= ex)) && ((y4 >= sy) || (y4 <= ey))) flag |= 0x04 ;
					if (((x2 <= sx) || (x2 >= ex)) && ((y2 >= sy) || (y2 <= ey))) flag |= 0x08 ;
					break ;
				case    5:
					if ((x4 <= sx) && (x4 >= ex) && (y4 >= sy) && (y4 <= ey)) flag |= 0x04 ;
					if ((x2 <= sx) && (x2 >= ex) && (y2 >= sy) && (y2 <= ey)) flag |= 0x08 ;
					break ;
			}
											// 第３象限
			switch (pass[2]) {
				default:
					break ;
				case    1:
					flag |= 0x30 ;
					break ;
				case    2:
					if ((x2 >= sx) && (y1 >= sy)) flag |= 0x10 ;
					if ((x4 >= sx) && (y3 >= sy)) flag |= 0x20 ;
					break ;
				case    3:
					if ((x2 <= ex) && (y1 <= ey)) flag |= 0x10 ;
					if ((x4 <= ex) && (y3 <= ey)) flag |= 0x20 ;
					break ;
				case    4:
					if (((x2 >= sx) || (x2 <= ex)) && ((y1 >= sy) || (y1 <= ey))) flag |= 0x10 ;
					if (((x4 >= sx) || (x4 <= ex)) && ((y3 >= sy) || (y3 <= ey))) flag |= 0x20 ;
					break ;
				case    5:
					if ((x2 >= sx) && (x2 <= ex) && (y1 >= sy) && (y1 <= ey)) flag |= 0x10 ;
					if ((x4 >= sx) && (x4 <= ex) && (y3 >= sy) && (y3 <= ey)) flag |= 0x20 ;
					break ;
			}
											// 第４象限
			switch (pass[3]) {
				default:
					break ;
				case    1:
					flag |= 0xC0 ;
					break ;
				case    2:
					if ((x3 >= sx) && (y3 <= sy)) flag |= 0x40 ;
					if ((x1 >= sx) && (y1 <= sy)) flag |= 0x80 ;
					break ;
				case    3:
					if ((x3 <= ex) && (y3 >= ey)) flag |= 0x40 ;
					if ((x1 <= ex) && (y1 >= ey)) flag |= 0x80 ;
					break ;
				case    4:
					if (((x3 >= sx) || (x3 <= ex)) && ((y3 <= sy) || (y3 >= ey))) flag |= 0x40 ;
					if (((x1 >= sx) || (x1 <= ex)) && ((y1 <= sy) || (y1 >= ey))) flag |= 0x80 ;
					break ;
				case    5:
					if ((x3 >= sx) && (x3 <= ex) && (y3 <= sy) && (y3 >= ey)) flag |= 0x40 ;
					if ((x1 >= sx) && (x1 <= ex) && (y1 <= sy) && (y1 >= ey)) flag |= 0x80 ;
					break ;
			}

			if (flag & 0x01) mpset(win,x1,y2,wdt,wdt) ;
			if (flag & 0x02) mpset(win,x3,y4,wdt,wdt) ;
			if (flag & 0x04) mpset(win,x4,y4,wdt,wdt) ;
			if (flag & 0x08) mpset(win,x2,y2,wdt,wdt) ;
			if (flag & 0x10) mpset(win,x2,y1,wdt,wdt) ;
			if (flag & 0x20) mpset(win,x4,y3,wdt,wdt) ;
			if (flag & 0x40) mpset(win,x3,y3,wdt,wdt) ;
			if (flag & 0x80) mpset(win,x1,y1,wdt,wdt) ;

			s -= 2 * dy + 1 ;
			if (s < 0) {
				--dx ;
				s += 2 * dx ;
			}
			++dy ;
		}
	} else {
		dx = 0 ;
		s = ry ;
		dy = ry ;

		while (dy >= dx) {
			unsigned int  flag ;
			int dxx = dx*rx/ry;
			int dyy = dy*rx/ry;
			x1 = cx + dxx;
			x2 = cx - dxx;
			x3 = cx + dyy;
			x4 = cx - dyy;
			y1 = cy + dy;
			y2 = cy - dy; 
			y3 = cy + dx;
			y4 = cy - dx;
			

			flag = 0x00 ;
											// 第１象限
			switch (pass[0]) {
				default:
					break ;
				case    1:
					flag = 0x03 ;
					break ;
				case    2:
					if ((x1 <= sx) && (y2 <= sy)) flag |= 0x01 ;
					if ((x3 <= sx) && (y4 <= sy)) flag |= 0x02 ;
					break ;
				case    3:
					if ((x1 >= ex) && (y2 >= ey)) flag |= 0x01 ;
					if ((x3 >= ex) && (y4 >= ey)) flag |= 0x02 ;
					break ;
				case    4:
					if (((x1 <= sx) || (x1 >= ex)) && ((y2 <= sy) || (y2 >= ey))) flag |= 0x01 ;
					if (((x3 <= sx) || (x3 >= ex)) && ((y4 <= sy) || (y4 >= ey))) flag |= 0x02 ;
					break ;
				case    5:
					if ((x1 <= sx) && (x1 >= ex) && (y2 <= sy) && (y2 >= ey)) flag |= 0x01 ;
					if ((x3 <= sx) && (x3 >= ex) && (y4 <= sy) && (y4 >= ey)) flag |= 0x02 ;
					break ;
			}
											// 第２象限
			switch (pass[1]) {
				default:
					break ;
				case    1:
					flag |= 0x0C ;
					break ;
				case    2:
					if ((x4 <= sx) && (y4 >= sy)) flag |= 0x04 ;
					if ((x2 <= sx) && (y2 >= sy)) flag |= 0x08 ;
					break ;
				case    3:
					if ((x4 >= ex) && (y4 <= ey)) flag |= 0x04 ;
					if ((x2 >= ex) && (y2 <= ey)) flag |= 0x08 ;
					break ;
				case    4:
					if (((x4 <= sx) || (x4 >= ex)) && ((y4 >= sy) || (y4 <= ey))) flag |= 0x04 ;
					if (((x2 <= sx) || (x2 >= ex)) && ((y2 >= sy) || (y2 <= ey))) flag |= 0x08 ;
					break ;
				case    5:
					if ((x4 <= sx) && (x4 >= ex) && (y4 >= sy) && (y4 <= ey)) flag |= 0x04 ;
					if ((x2 <= sx) && (x2 >= ex) && (y2 >= sy) && (y2 <= ey)) flag |= 0x08 ;
					break ;
			}
											// 第３象限
			switch (pass[2]) {
				default:
					break ;
				case    1:
					flag |= 0x30 ;
					break ;
				case    2:
					if ((x2 >= sx) && (y1 >= sy)) flag |= 0x10 ;
					if ((x4 >= sx) && (y3 >= sy)) flag |= 0x20 ;
					break ;
				case    3:
					if ((x2 <= ex) && (y1 <= ey)) flag |= 0x10 ;
					if ((x4 <= ex) && (y3 <= ey)) flag |= 0x20 ;
					break ;
				case    4:
					if (((x2 >= sx) || (x2 <= ex)) && ((y1 >= sy) || (y1 <= ey))) flag |= 0x10 ;
					if (((x4 >= sx) || (x4 <= ex)) && ((y3 >= sy) || (y3 <= ey))) flag |= 0x20 ;
					break ;
				case    5:
					if ((x2 >= sx) && (x2 <= ex) && (y1 >= sy) && (y1 <= ey)) flag |= 0x10 ;
					if ((x4 >= sx) && (x4 <= ex) && (y3 >= sy) && (y3 <= ey)) flag |= 0x20 ;
					break ;
			}
											// 第４象限
			switch (pass[3]) {
				default:
					break ;
				case    1:
					flag |= 0xC0 ;
					break ;
				case    2:
					if ((x3 >= sx) && (y3 <= sy)) flag |= 0x40 ;
					if ((x1 >= sx) && (y1 <= sy)) flag |= 0x80 ;
					break ;
				case    3:
					if ((x3 <= ex) && (y3 >= ey)) flag |= 0x40 ;
					if ((x1 <= ex) && (y1 >= ey)) flag |= 0x80 ;
					break ;
				case    4:
					if (((x3 >= sx) || (x3 <= ex)) && ((y3 <= sy) || (y3 >= ey))) flag |= 0x40 ;
					if (((x1 >= sx) || (x1 <= ex)) && ((y1 <= sy) || (y1 >= ey))) flag |= 0x80 ;
					break ;
				case    5:
					if ((x3 >= sx) && (x3 <= ex) && (y3 <= sy) && (y3 >= ey)) flag |= 0x40 ;
					if ((x1 >= sx) && (x1 <= ex) && (y1 <= sy) && (y1 >= ey)) flag |= 0x80 ;
					break ;
			}

			if (flag & 0x01) mpset(win,x1,y2,wdt,wdt) ;
			if (flag & 0x02) mpset(win,x3,y4,wdt,wdt) ;
			if (flag & 0x04) mpset(win,x4,y4,wdt,wdt) ;
			if (flag & 0x08) mpset(win,x2,y2,wdt,wdt) ;
			if (flag & 0x10) mpset(win,x2,y1,wdt,wdt) ;
			if (flag & 0x20) mpset(win,x4,y3,wdt,wdt) ;
			if (flag & 0x40) mpset(win,x3,y3,wdt,wdt) ;
			if (flag & 0x80) mpset(win,x1,y1,wdt,wdt) ;

			s -= 2 * dx + 1 ;
			if (s < 0) {
				--dy ;
				s += 2 * dy ;
			}
			++dx ;
		}
	}
}

/*************************
 * 楕円の円弧を描く
 *  引数　int wn: ウィンドウ番号
 *      　double xcen,ycen: 円の中心座標
 * 　　　 double xrad: x方向の半径
 *       double yrad: y方向の半径
 *       double sang: 描画開始角度
 *       double eang: 描画終了角度
 *       int idir: 描画方向
 * 戻り値　なし
 */
 void drawarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir )
{
    int x0,y0,x1,y1,xsize,ysize ;

    xyconv(wn,xcen,ycen,&x0,&y0) ;
    xyconv(wn,xcen+xrad,ycen+yrad,&x1,&y1) ;
    xsize=abs(x1-x0) ;
    ysize=abs(y1-y0) ;
	sang = sang*M_PI/180.0;
	eang = eang*M_PI/180.0;
	if(idir==-1) {
		double tmp = eang;
		eang = sang;
		sang = tmp;
	}
	lowarc(wn,x0,y0,xsize,ysize,sang,eang);
}
/*
 void drawarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir )
{
	double xp[74],yp[74];

	sang = Rangle( sang*M_PI/180.0 );
	eang = Rangle( eang*M_PI/180.0 );
	if(idir==-1) {
		double tmp = eang;
		eang = sang;
		sang = tmp;
	}
	if(sang > eang) eang += 2*M_PI;
	int n=0;
	for(double a=-sang;a>-eang;a -= 5.0*M_PI/180.0) {		//楕円を72角形として描画する
		dcalcArcPoint(xcen,ycen,xrad,yrad,a,&xp[n],&yp[n]);
		n++;
	}
	dcalcArcPoint(xcen,ycen,xrad,yrad,-eang,&xp[n],&yp[n]);
	n++;
	drawlines(wn,xp,yp,n);
}
*/
/***************************
 * 2点からから直線の傾きaと切片bを求める
 * 引数  int x1:　始点x座標
 *      int y1:　始点y座標
 *      int x2:　終点x座標
 *      int y2:　終点y座標
 *      double *a:　傾き
 * 　　　int *af:　傾きが有限か
 *      double *b:　切片
 *      int *bf:　切片が有限か
 * 戻り値　なし
 */
/*
static void calclinear(int x1,int y1,int x2,int y2,double *a,int *af,double *b,int *bf)
{
	if(x1==x2) {
		*b = 0; *bf = 0;
		*a = 0; *af = 0;
	} else {
		double x1d = x1,x2d = x2,y1d = y1,y2d = y2;
		*b = (x2d*y1d - x1d*y2d) / (x2d - x1d); *bf = 1;
		*a = (y2d - y1d) / (x2d - x1d); *af = 1;
	}
} 
*/

/***************************
 * y1の高さでのx座標を計算する
 * 　
 * 戻り値　座標が存在するか
 */
/*
static int checkline(int y1,int sx1,int sy1,int sy2,double a1,double b1,int a1f,int *lx1)
{
	int lflag = 0;
	

	if( (sy1 <= y1) && (y1 <= sy2) ) {
		if( a1f == 1 ) {
			if( a1 != 0 ) {
				*lx1 = (int)( ((double)y1-b1)/a1);
				lflag = 1; 
			} else {
				*lx1 = -1;
				lflag = 0;
			}
		} else {
			*lx1 = sx1;
			lflag = 1; 
		}
	} else {
		*lx1 = -1;
		lflag = 0;
	}

	return lflag;
}
*/
/***************************
 * 点をチェックし色を塗る(線を描く)
 * 　y1
 * 　  x1 x3 x4 x2 の順で座標が並ぶ	
 * 戻り値　なし
 */
/*
void checkanddraw(int win,int y1,int x1,int fx1,int x2,int fx2,int x3,int fx3,int x4,int fx4)
{
	if(x1 > x2) { //データの並び替え
		int tmp = x1;
		x1 = x2; 
		x1 = tmp;
		tmp = fx1; 
		fx1 = fx2; 
		fx1 = tmp;
	}	
	if(x3 > x4) { //データの並び替え
		int tmp = x3;
		x3 = x4;
		x4 = tmp;
		tmp = fx3; 
		fx3 = fx4; 
		fx4 = tmp;
	}
	int dis = abs(x3-x1) + abs(x2-x4); 
	if( fx1 && fx2 && fx3 && fx4 && (dis < 3)) {
		fx3=fx4=0;
	}
	if( fx1 && fx2) {
		if( fx3 && fx4 ) {
			lowline(win,x1,y1,x3,y1);		//4点
			lowline(win,x4,y1,x2,y1);
		} else {
			lowline(win,x1,y1,x2,y1); //円の両端
		}
	} else if( fx1 ) {
		if( fx4 ) {
			lowline(win,x1,y1,x4,y1);
		} else {//if( fx4 ) 
			lowline(win,x1,y1,x3,y1);
		}
	} else if( fx2 ) {
		if( fx3 ) {
			lowline(win,x3,y1,x2,y1);
		} else {// if( fx4 )
			lowline(win,x4,y1,x2,y1);
		}
	} else if( fx3 && fx4) {
		lowline(win,x3,y1,x4,y1);
	}
}
*/

/*************************
 * フレームバッファに塗りつぶした楕円を描く
 *  引数　int cx,cy: 円の中心座標
 * 　　　 int rx: x方向の半径
 *       int ry: y方向の半径
 *       double Sangle: 描画スタート角度
 *       double Eangle: 描画終了角度
 * 戻り値　なし
 */
/*
static void lowarcf(int win,int cx, int cy, int rx,int ry, double Sangle,double Eangle)
{
	int dx, dy, s ;
	int x1, y1, x2, y2, x3, y3, x4, y4 ;
	int sx,sy,ex,ey;
	int psx, psy, pex, pey, ss, es, i, j ;
	unsigned int pass[4] ;
	double a1,a2,b1,b2;
	int a1f,a2f,b1f,b2f,sx1,sx2,sy1,sy2,ex1,ex2,ey1,ey2;

	calcArcPoint(cx,cy,rx,ry,Sangle,&sx,&sy);
	//printf("s %d %d\n",sx,sy);
	calcArcPoint(cx,cy,rx,ry,Eangle,&ex,&ey);
	//printf("e %d %d\n",ex,ey);
	if(cy>sy) {
		sy1 = sy; sy2 = cy;
		sx1 = sx; sx2 = cx;
	} else {
		sy1 = cy; sy2 = sy;
		sx1 = cx; sx2 = sx;
	}
	if(cy>ey) {
		ey1 = ey; ey2 = cy;
		ex1 = ex; ex2 = cx;
	} else {
		ey1 = cy; ey2 = ey;
		ex1 = cx; ex2 = ex;
	}
	calclinear(sx1,sy1,sx2,sy2,&a1,&a1f,&b1,&b1f);
	calclinear(ex1,ey1,ex2,ey2,&a2,&a2f,&b2,&b2f);

	psx = sx - cx ;
	pex = ex - cx ;
	psy = sy - cy ;
	pey = ey - cy ;
								// 始点の象限
	if (psx >= 0) {
		if (psy >= 0) ss = 3 ;
		else          ss = 0 ;
	} else {
		if (psy >= 0) ss = 2 ;
		else          ss = 1 ;
	}
								// 終点の象限
	if (pex >= 0) {
		if (pey >= 0) es = 3 ;
		else          es = 0 ;
	} else {
		if (pey >= 0) es = 2 ;
		else          es = 1 ;
	}

	if (ss == es) {										// 始点終点の象限が同じ
		if (((psy <  0) && (psx >= pex)) ||
			((psy >= 0) && (psx <= pex))) {
			for (i = 0 ; i < 4 ; i++) pass[i] = 0 ;		// 小円( < 90 )
			pass[ss] = 5 ;
		} else {
			for (i = 0 ; i < 4 ; i++) pass[i] = 1 ;		// 大円( > 270 )
			pass[ss] = 4 ;
		}
	} else {
		for (i = 0 ; i < 4 ; i++) pass[i] = 0 ;
		j = ss ;
		for (i = 0 ; i < 4 ; i++) {
			pass[j] = 1 ;								// 全表示
			if (j == es) break ;
			++j ;
			if (j > 3) j = 0 ;
		}
		pass[ss] = 2 ;									// 始点のみ
		pass[es] = 3 ;									// 終点のみ
	}
	
	if(rx>ry) {
		dx = rx ;
		s = rx ;
		dy = 0 ;

		while (dx >= dy) {
			unsigned int  flag ;
			int dxx = dx*ry/rx;
			int dyy = dy*ry/rx;
			x1 = cx + dx;
			x2 = cx - dx;
			x3 = cx + dy;
			x4 = cx - dy;
			y1 = cy + dyy;
			y2 = cy - dyy; 
			y3 = cy + dxx;
			y4 = cy - dxx;
			

			flag = 0x00 ;
											// 第１象限
			switch (pass[0]) {
				default:
					break ;
				case    1:
					flag = 0x03 ;
					break ;
				case    2:
					if ((y2 <= sy)) flag |= 0x01 ;
					if ((y4 <= sy)) flag |= 0x02 ;
					break ;
				case    3:
					if ((y2 >= ey)) flag |= 0x01 ;
					if ((y4 >= ey)) flag |= 0x02 ;
					break ;
				case    4:
					if (((y2 <= sy) || (y2 >= ey))) flag |= 0x01 ;
					if (((y4 <= sy) || (y4 >= ey))) flag |= 0x02 ;
					break ;
				case    5:
					if ((y2 <= sy) && (y2 >= ey)) flag |= 0x01 ;
					if ((y4 <= sy) && (y4 >= ey)) flag |= 0x02 ;
					break ;
			}
											// 第２象限
			switch (pass[1]) {
				default:
					break ;
				case    1:
					flag |= 0x0C ;
					break ;
				case    2:
					if ((y4 >= sy)) flag |= 0x04 ;
					if ((y2 >= sy)) flag |= 0x08 ;
					break ;
				case    3:
					if ((y4 <= ey)) flag |= 0x04 ;
					if ((y2 <= ey)) flag |= 0x08 ;
					break ;
				case    4:
					if ( ((y4 >= sy) || (y4 <= ey))) flag |= 0x04 ;
					if ( ((y2 >= sy) || (y2 <= ey))) flag |= 0x08 ;
					break ;
				case    5:
					if ( (y4 >= sy) && (y4 <= ey)) flag |= 0x04 ;
					if ((y2 >= sy) && (y2 <= ey)) flag |= 0x08 ;
					break ;
			}
											// 第３象限
			switch (pass[2]) {
				default:
					break ;
				case    1:
					flag |= 0x30 ;
					break ;
				case    2:
					if ((y1 >= sy)) flag |= 0x10 ;
					if ((y3 >= sy)) flag |= 0x20 ;
					break ;
				case    3:
					if ((y1 <= ey)) flag |= 0x10 ;
					if ((y3 <= ey)) flag |= 0x20 ;
					break ;
				case    4:
					if ( ((y1 >= sy) || (y1 <= ey))) flag |= 0x10 ;
					if ( ((y3 >= sy) || (y3 <= ey))) flag |= 0x20 ;
					break ;
				case    5:
					if ((y1 >= sy) && (y1 <= ey)) flag |= 0x10 ;
					if ( (y3 >= sy) && (y3 <= ey)) flag |= 0x20 ;
					break ;
			}
											// 第４象限
			switch (pass[3]) {
				default:
					break ;
				case    1:
					flag |= 0xC0 ;
					break ;
				case    2:
					if ((y3 <= sy)) flag |= 0x40 ;
					if ((y1 <= sy)) flag |= 0x80 ;
					break ;
				case    3:
					if ( (y3 >= ey)) flag |= 0x40 ;
					if ( (y1 >= ey)) flag |= 0x80 ;
					break ;
				case    4:
					if ( ((y3 <= sy) || (y3 >= ey))) flag |= 0x40 ;
					if ( ((y1 <= sy) || (y1 >= ey))) flag |= 0x80 ;
					break ;
				case    5:
					if ((y3 <= sy) && (y3 >= ey)) flag |= 0x40 ;
					if ((y1 <= sy) && (y1 >= ey)) flag |= 0x80 ;
					break ;
			}

			int lx1=0,lx2=0,lflag = 0;
			
			lflag |= checkline(y1,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y1,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y1,x2,flag&0x10,x1,flag&0x80,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y2,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y2,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y2,x2,flag&0x08,x1,flag&0x01,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y4,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y4,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y4,x4,flag&0x04,x3,flag&0x02,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y3,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y3,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y3,x4,flag&0x20,x3,flag&0x40,lx1,lflag&0x01,lx2,lflag&0x02);

			s -= 2 * dy + 1 ;
			if (s < 0) {
				--dx ;
				s += 2 * dx ;
			}
			++dy ;
		}
	} else {
		dx = 0 ;
		s = ry ;
		dy = ry ;

		while (dy >= dx) {
			unsigned int  flag ;
			int dxx = dx*rx/ry;
			int dyy = dy*rx/ry;
			x1 = cx + dxx;
			x2 = cx - dxx;
			x3 = cx + dyy;
			x4 = cx - dyy;
			y1 = cy + dy;
			y2 = cy - dy; 
			y3 = cy + dx;
			y4 = cy - dx;
			

			flag = 0x00 ;
											// 第１象限
			switch (pass[0]) {
				default:
					break ;
				case    1:
					flag = 0x03 ;
					break ;
				case    2:
					if ((y2 <= sy)) flag |= 0x01 ;
					if ((y4 <= sy)) flag |= 0x02 ;
					break ;
				case    3:
					if ((y2 >= ey)) flag |= 0x01 ;
					if ((y4 >= ey)) flag |= 0x02 ;
					break ;
				case    4:
					if (((y2 <= sy) || (y2 >= ey))) flag |= 0x01 ;
					if (((y4 <= sy) || (y4 >= ey))) flag |= 0x02 ;
					break ;
				case    5:
					if ((y2 <= sy) && (y2 >= ey)) flag |= 0x01 ;
					if ( (y4 <= sy) && (y4 >= ey)) flag |= 0x02 ;
					break ;
			}
											// 第２象限
			switch (pass[1]) {
				default:
					break ;
				case    1:
					flag |= 0x0C ;
					break ;
				case    2:
					if ((y4 >= sy)) flag |= 0x04 ;
					if ((y2 >= sy)) flag |= 0x08 ;
					break ;
				case    3:
					if ((y4 <= ey)) flag |= 0x04 ;
					if ((y2 <= ey)) flag |= 0x08 ;
					break ;
				case    4:
					if (((y4 >= sy) || (y4 <= ey))) flag |= 0x04 ;
					if (((y2 >= sy) || (y2 <= ey))) flag |= 0x08 ;
					break ;
				case    5:
					if ( (y4 >= sy) && (y4 <= ey)) flag |= 0x04 ;
					if ( (y2 >= sy) && (y2 <= ey)) flag |= 0x08 ;
					break ;
			}
											// 第３象限
			switch (pass[2]) {
				default:
					break ;
				case    1:
					flag |= 0x30 ;
					break ;
				case    2:
					if ((y1 >= sy)) flag |= 0x10 ;
					if ( (y3 >= sy)) flag |= 0x20 ;
					break ;
				case    3:
					if ((y1 <= ey)) flag |= 0x10 ;
					if ((y3 <= ey)) flag |= 0x20 ;
					break ;
				case    4:
					if ( ((y1 >= sy) || (y1 <= ey))) flag |= 0x10 ;
					if (((y3 >= sy) || (y3 <= ey))) flag |= 0x20 ;
					break ;
				case    5:
					if ((y1 >= sy) && (y1 <= ey)) flag |= 0x10 ;
					if ((y3 >= sy) && (y3 <= ey)) flag |= 0x20 ;
					break ;
			}
											// 第４象限
			switch (pass[3]) {
				default:
					break ;
				case    1:
					flag |= 0xC0 ;
					break ;
				case    2:
					if ( (y3 <= sy)) flag |= 0x40 ;
					if ((y1 <= sy)) flag |= 0x80 ;
					break ;
				case    3:
					if ((y3 >= ey)) flag |= 0x40 ;
					if ((y1 >= ey)) flag |= 0x80 ;
					break ;
				case    4:
					if ( ((y3 <= sy) || (y3 >= ey))) flag |= 0x40 ;
					if (((y1 <= sy) || (y1 >= ey))) flag |= 0x80 ;
					break ;
				case    5:
					if ((y3 <= sy) && (y3 >= ey)) flag |= 0x40 ;
					if ((y1 <= sy) && (y1 >= ey)) flag |= 0x80 ;
					break ;
			}

			int lx1=0,lx2=0,lflag = 0;
			
			lflag |= checkline(y1,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y1,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y1,x2,flag&0x10,x1,flag&0x80,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y2,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y2,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y2,x2,flag&0x08,x1,flag&0x01,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y4,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y4,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y4,x4,flag&0x04,x3,flag&0x02,lx1,lflag&0x01,lx2,lflag&0x02);
			
			lflag = 0;
			lflag |= checkline(y3,sx1,sy1,sy2,a1,b1,a1f,&lx1);
			lflag |= checkline(y3,ex1,ey1,ey2,a2,b2,a2f,&lx2)<<1;
			checkanddraw(win,y3,x4,flag&0x20,x3,flag&0x40,lx1,lflag&0x01,lx2,lflag&0x02);
			//if((y3>=200)&&(y3<350)) printf(" %d %d %d %d %d %d %d %d %d\n",y3,x4,flag&0x20,x3,flag&0x40,lx1,lflag&0x01,lx2,lflag&0x02);
			s -= 2 * dx + 1 ;
			if (s < 0) {
				--dy ;
				s += 2 * dy ;
			}
			++dx ;
		}
	}
}
*/

/*************************
 * 塗りつぶした楕円の円弧を描く
 *  引数　int wn: ウィンドウ番号
 *      　double xcen,ycen: 円の中心座標
 * 　　　 double xrad: x方向の半径
 *       double yrad: y方向の半径
 *       double sang: 描画開始角度
 *       double eang: 描画終了角度
 *       int idir: 描画方向
 * 戻り値　なし
 */
void fillarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir )
{
	double xp[72+3],yp[72+3];//楕円を72角形として描画する

	sang = Rangle( sang*M_PI/180.0 );
	eang = Rangle( eang*M_PI/180.0 );
	if(idir==-1) {
		double tmp = eang;
		eang = sang;
		sang = tmp;
	}
	if(sang > eang) eang += 2*M_PI;
	int n=0;
	xp[n  ] = xcen;
	yp[n++] = ycen;
	for(double a=-sang;a>-eang;a -= 5.0*M_PI/180.0) {		//楕円を72角形として描画する
		dcalcArcPoint(xcen,ycen,xrad,yrad,a,&xp[n],&yp[n]);
		n++;
	}
	dcalcArcPoint(xcen,ycen,xrad,yrad,-eang,&xp[n],&yp[n]);
	n++;
	fillpoly(wn,xp,yp,n,0);
}

/*void fillarc( int wn, double xcen, double ycen, double xrad, 
		double yrad, double sang, double eang, int idir )
{
	 int x0,y0,x1,y1,xsize,ysize ;
    xyconv(wn,xcen,ycen,&x0,&y0) ;
    xyconv(wn,xcen+xrad,ycen+yrad,&x1,&y1) ;
    xsize=abs(x1-x0) ;
    ysize=abs(y1-y0) ;
	sang = sang*M_PI/180.0;
	eang = eang*M_PI/180.0;
	if(idir==-1) {
		double tmp = eang;
		eang = sang;
		sang = tmp;
	}
	lowarcf(wn,x0,y0,xsize,ysize,sang,eang);

}
*/
