/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * シンボル・矢印の描画
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fbeggx.h"

/* 連続した直線を描く */
static void X_DrawLines(int wn,struct point *xpts, int n)
{
    int i;
    int xxg = xpts[0].x;
    int yyg = xpts[0].y;

    for ( i=1 ; i < n ; i++ ) {
        mlowline(wn,xxg,yyg,xpts[i].x,xpts[i].y);
        xxg = xpts[i].x;
        yyg = xpts[i].y;
    }
}

/* シンボルを描く下請け */
static int draw_a_symbol( int wn, double xg, double yg, int size, int sym )
{
    int x0,y0,x1,y1 ;
    int xx0=0,yy0=0,xx1=0,yy1=0 ;
    int hs,f=0 ;
    int xxg,yyg ;
    double fhs;
    const double factors[10] = { 0.52,				/* 10 */
				0.125, 0.5, 0.55, 0.44, 0.42,	/* 1-5 */
				0.62, 0.68, 0.43, 0.42 };	/* 6-9 */
    struct point xpts[8];

    if ( sym <= 0 ) return f;	/* 0 以下は却下 */
    else sym = sym % 10;	/* 10 番は 0 番になる */
    
    xyconv(wn,xg,yg,&xxg,&yyg) ;

    /* Version 0.90 で各シンボルの大きさのバランスを整えた */
    fhs = size * factors[sym] ;
    hs = _eggx_iround(fhs) ;
    if ( hs < 0 ) hs = 0 ;

    y0 = yyg - hs ;
    if ( sym == 7 ) hs = _eggx_iround(size * 0.4) ;
    x0 = xxg - hs ;
    x1 = xxg + hs ;
    y1 = yyg + hs ;
    if ( sym == 3 || sym == 6 ) {
	hs = _eggx_iround(fhs * 0.7) ;
	xx0 = xxg - hs ;
	yy0 = yyg - hs ;
	xx1 = xxg + hs ;
	yy1 = yyg + hs ;
    }

    switch ( sym ) {
        case 1:
            xpts[0].x = xxg;  xpts[0].y = y0;
            xpts[1].x = x1;   xpts[1].y = yyg;
            xpts[2].x = xxg;  xpts[2].y = y1;
            xpts[3].x = x0;   xpts[3].y = yyg;
            xpts[4].x = xxg;  xpts[4].y = y0;
            X_DrawLines(  wn, xpts, 5 );
            break;
        case 2:
            mlowline( wn, xxg, y0, xxg, y1 );
            mlowline( wn, x0, yyg, x1, yyg );
            break;
        case 3:
            mlowline( wn, xxg, y0, xxg, y1 );
            mlowline( wn, x0, yyg, x1, yyg );
            mlowline( wn, xx0, yy0, xx1, yy1 );
            mlowline( wn, xx1, yy0, xx0, yy1 );
            break;
        case 4:
            lowcircle( wn, xxg, yyg, (x1-x0)/2, (y1-y0)/2);
            break;
        case 5:
            mlowline( wn, x0, y0, x1, y1 );
            mlowline( wn, x1, y0, x0, y1 );
            break;
        case 6:
            mlowline( wn, xx0, yy0, xxg, yyg );
            mlowline( wn, xx1, yy0, xxg, yyg );
            mlowline( wn, xxg, y1, xxg, yyg );
            break;
        case 7:
            xpts[0].x = xxg;  xpts[0].y = yyg;
            xpts[1].x = xxg;  xpts[1].y = y0;
            xpts[2].x = x0;   xpts[2].y = y1;
            xpts[3].x = x1;   xpts[3].y = y1;
            xpts[4].x = xxg;  xpts[4].y = y0;
            X_DrawLines(  wn, xpts, 5 );
            break;
        case 8:
            xpts[0].x = xxg;  xpts[0].y = yyg;
            xpts[1].x = xxg;  xpts[1].y = y0;
            xpts[2].x = x1;   xpts[2].y = y0;
            xpts[3].x = x1;   xpts[3].y = y1;
            xpts[4].x = x0;   xpts[4].y = y1;
            xpts[5].x = x0;   xpts[5].y = y0;
            xpts[6].x = xxg;  xpts[6].y = y0;
            X_DrawLines(  wn, xpts, 7 );
            break;
        case 9:
            mlowline( wn, x0, y0, x1, y1 );
            mlowline( wn, x1, y0, x0, y1 );
            mlowline( wn, x0, y0, x1, y0 );
            break;
        case 0:
            xpts[0].x = xxg;  xpts[0].y = yyg;
            xpts[1].x = xxg;  xpts[1].y = y0;
            xpts[2].x = x1;   xpts[2].y = yyg;
            xpts[3].x = xxg;  xpts[3].y = y1;
            xpts[4].x = x0;   xpts[4].y = yyg;
            xpts[5].x = xxg;  xpts[5].y = y0;
            X_DrawLines( wn, xpts, 6 );
            break;
    }
    return f;
}

/************
 *  センターシンボルを描く
 * 　引数　int wn: ウィンドウ番号
 *        double xg: x座標の配列
 *        double yg: y座標の配列
 *        int size: 大きさ
 *        int sym: 形　1～10
 * 　戻り値　なし
 */
void drawsym( int wn, double xg, double yg, int size, int sym )
{

    draw_a_symbol(wn,xg,yg,size,sym);

}

/************
 *  複数のセンターシンボルを描く
 * 　引数　int wn: ウィンドウ番号
 *        double x[]: x座標の配列
 *        double y[]: y座標の配列
 *        int n: シンボルの数
 *        int size: 大きさ
 *        int sym: 形　1～10
 * 　戻り値　なし
 *//* 複数のシンボルを描く */
void drawsyms( int wn, const double x[], const double y[], int n,
		    int size, int sym )
{
    for(int i=0;i<n;i++) {
        drawsym(wn,x[i],y[i], size, sym );
    }
}

/* 矢印の先端を描く */
static int dwtop( int wn, int si, int sj,
		  int xx[], int yy[], 
		  int x1[], int y1[],
		  int x2[], int y2[],
		  int x3[], int y3[] )
{
    int i=1 ;
    int points[8] ;
    do{
        switch(sj){
        case 1:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline( wn, xx[i],yy[i],x2[i],y2[i] ) ;
            mlowline( wn, x1[i],y1[i],x2[i],y2[i] ) ;
            break ;
        case 2:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline( wn, xx[i],yy[i],x2[i],y2[i] ) ;
            mlowline( wn, x1[i],y1[i],x2[i],y2[i] ) ;
            if( si==0 ) mlowline( wn, xx[i],yy[i],x3[i],y3[i] ) ;
            break ;
        case 3:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline( wn, x1[i],y1[i],x3[i],y3[i] ) ;
            if( si==0 ) mlowline( wn, xx[i],yy[i],x3[i],y3[i] ) ;
            break ;
        case 4:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline(  wn, xx[i],yy[i],x2[i],y2[i] ) ;
            if( si==0 ) mlowline( wn, xx[i],yy[i],x3[i],y3[i] ) ;
            break ;
        case 5:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            if( si==0 ) mlowline( wn, xx[i],yy[i],x3[i],y3[i] ) ;
            break ;
        case 6:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline( wn, xx[i],yy[i],x2[i],y2[i] ) ;
            mlowline( wn, x1[i],y1[i],x2[i],y2[i] ) ;
            points[0]=points[6]=xx[i] ;
            points[1]=points[7]=yy[i] ;
            points[2]=x1[i] ;
            points[3]=y1[i] ;
            points[4]=x2[i] ;
            points[5]=y2[i] ;
            lowpolygonf(wn,3,points,0);
            break ;
        case 7:
            mlowline( wn, xx[i],yy[i],x1[i],y1[i] ) ;
            mlowline( wn, x1[i],y1[i],x3[i],y3[i] ) ;
            if( si==0 ) mlowline( wn, xx[i],yy[i],x3[i],y3[i] ) ;
            points[0]=points[6]=xx[i] ;
            points[1]=points[7]=yy[i] ;
            points[2]=x1[i] ;
            points[3]=y1[i] ;
            points[4]=x3[i] ;
            points[5]=y3[i] ;
            lowpolygonf(wn,3,points,0);;
            break ;
        }
    } while( --i > 1-si ) ;
    return 0;
}

/************
 *  矢印を描く
 * 　引数　int wn: ウィンドウ番号
 *        double xs: 開始x座標
 *        double ys: 開始y座標
 *        double xt: 終点x座標
 *        double yt: 終点y座標
 *        double s: 矢印の長さの比率　0～1
 *        double w: 矢印の大きさの比率　0～1
 *        int shape: 矢印の形　10*i+j
 * 　戻り値　なし
 */
void drawarrow( int wn, double xs, double ys, double xt, double yt, 
		     double s, double w, int shape )
{
    int xx[2],yy[2] ;
    int si,sj ;		
    double r ;
    double ss,ww ;
    double theta,phi ;
    double xl,yl ;
    double frx1,frx2,fry1,fry2 ;
    int x1[2],x2[2],y1[2],y2[2],x3[2],y3[2] ;
    int rx1,rx2,ry1,ry2,rx3,ry3 ;

    xyconv( wn,xs,ys,xx,yy ) ;
    xyconv( wn,xt,yt,xx+1,yy+1 ) ;
    xl = xx[0]-xx[1] ;
    yl = yy[0]-yy[1] ;
    if( shape<100 ){
        ss = s ;
        ww = w * 0.5 ;
        si = shape/10 ;
    }
    else {
        ww = sqrt(xl*xl+yl*yl) ;
        ss = ww * s ;
        ww *= w ;
        ww /= 2.0 ;
        si = shape/10-10 ;
    }
	sj = shape % 10 ;
    r  = sqrt( ss*ss+ww*ww ) ;
    theta = atan2(yl,xl) ;
    phi = atan2(ww,ss) ;
    frx1 = r*cos(theta-phi);
    fry1 = r*sin(theta-phi);
    frx2 = r*cos(theta+phi);
    fry2 = r*sin(theta+phi);
    rx1 = _eggx_iround(frx1);
    ry1 = _eggx_iround(fry1);
    rx2 = _eggx_iround(frx2);
    ry2 = _eggx_iround(fry2);
    x1[1] = xx[1] + rx1 ;
    y1[1] = yy[1] + ry1 ;
    x2[1] = xx[1] + rx2 ;
    y2[1] = yy[1] + ry2 ;
    x3[1] = xx[1] + (rx3=_eggx_iround((frx1+frx2)*0.5)) ;
    y3[1] = yy[1] + (ry3=_eggx_iround((fry1+fry2)*0.5)) ;
    if( si==2 ){
        x1[0] = xx[0] - rx1 ;
        y1[0] = yy[0] - ry1 ;
        x2[0] = xx[0] - rx2 ;
        y2[0] = yy[0] - ry2 ;
        x3[0] = xx[0] - rx3 ;
        y3[0] = yy[0] - ry3 ;
    }
    dwtop( wn, si, sj, xx,yy,x1,y1,x2,y2,x3,y3 );
    switch( si ){
        case 1:
            if( 2<=sj ){
                mlowline( wn, xx[0],yy[0],xx[1],yy[1] );
            }
            else mlowline( wn, xx[0],yy[0],x3[1],y3[1] );
            break ;
        case 2:
            if( 2<=sj ){
                mlowline( wn, xx[0],yy[0],xx[1],yy[1] );
            }
            else mlowline( wn, x3[0],y3[0],x3[1],y3[1] );
            break ;
    }
}
