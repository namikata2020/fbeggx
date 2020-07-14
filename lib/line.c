/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 * 
 * 線を引く関連
 */

#include "fbeggx.h"

/*************************
 * ライン幅を指定する
 * 　引数　int wn: ウィンドウ番号
 * 　　　　int width: 線の太さ
 * 　戻り値　なし
 */
void newlinewidth( int wn, int width )
{

    Pc[wn].linewidth = width ;

    return;
}

/*************************
 * フレームバッファに直線を描く
 *  引数　int wn: ウィンドウ番号
 *        int x1,y1: 描画開始座標
 * 　　　 int x2,y2: 描画終了座標
 * 戻り値　なし
 */
void lowline(int wn,int x1,int y1,int x2,int y2)
{
	int s;
	int dy;
	int wx=abs(x2-x1),wy=abs(y2-y1);
	
	if(x1<x2 && y1<y2)
		dy = 1;
	else
		dy = -1;

	if(wx>wy)  {
		if(x1>x2)
		{
			s=x1,x1=x2,x2=s;
			if(y1>y2) dy=1;	
			y1=y2;
		}
		s=wx/2;
		for(;x1<=x2;x1++)
		{
			lowpset(wn,x1,y1);
			s-=wy;
			if(s<0) s+=wx,y1+=dy;
		}
	}
	else {
		if(y1>y2)
		{
			s=y1,y1=y2,y2=s;
			if(x1>x2) dy=1;	
			x1=x2;
		}
		s=wy/2;
		for(;y1<=y2;y1++)
		{
			lowpset(wn,x1,y1);
			s-=wx;
			if(s<0) s+=wy,x1+=dy;
		}
	}
}


/*************************
 * フレームバッファに直線を描く
 *  引数　int wn: ウィンドウ番号
 *        int x1,y1: 描画開始座標
 * 　　　 int x2,y2: 描画終了座標
 * 戻り値　なし
 */
void mlowline(int wn,int x1,int y1,int x2,int y2)
{
	int s;
	int dy;
	int wx=abs(x2-x1),wy=abs(y2-y1);
	
	if(x1<x2 && y1<y2)
		dy = 1;
	else
		dy = -1;

	if(wx>wy)  {
		if(x1>x2)
		{
			s=x1,x1=x2,x2=s;
			if(y1>y2) dy=1;	
			y1=y2;
		}
		s=wx/2;
		for(;x1<=x2;x1++)
		{
			mpset(wn,x1,y1,Pc[wn].linewidth,Pc[wn].linewidth);
			s-=wy;
			if(s<0) s+=wx,y1+=dy;
		}
	}
	else {
		if(y1>y2)
		{
			s=y1,y1=y2,y2=s;
			if(x1>x2) dy=1;	
			x1=x2;
		}
		s=wy/2;
		for(;y1<=y2;y1++)
		{
			mpset(wn,x1,y1,Pc[wn].linewidth,Pc[wn].linewidth);
			s-=wx;
			if(s<0) s+=wy,x1+=dy;
		}
	}
}

/*************************
 * 線を引く
 * 　引数　int wn: ウィンドウ番号
 * 　　　　 double xg0: 始点x座標
 *         double yg0: 始点y座標
 *         double xg1: 終点x座標
 *         double yg1: 終点y座標
 * 　戻り値　なし
 */
void drawline( int wn, double xg0, double yg0, double xg1, double yg1 )
{
    int xx0,yy0,xx1,yy1;

    xyconv( wn,xg0,yg0,&xx0,&yy0 ) ;
    xyconv( wn,xg1,yg1,&xx1,&yy1 ) ;

    mlowline( wn, xx0, yy0, xx1, yy1 ) ;

    return;
}

/*************************
 * 連続的に線を引く
 * 　引数　int wn: ウィンドウ番号
 * 　　　　 double xg: x座標
 *         double yg: y座標
 *         int mode: モード 移動 描画 セット
 * 　戻り値　なし
 */
void line( int wn, double xg, double yg, int mode )
{
    int xxg,yyg ;
    xyconv(wn,xg,yg,&xxg,&yyg) ;
    
    switch( mode ) {
    case PENDOWN:
	    mlowline( wn, Pc[wn].prevx, Pc[wn].prevy, xxg, yyg ) ;
	    Pc[wn].prevx=xxg ;
	    Pc[wn].prevy=yyg ;
	    break ;
    case PENUP:
	    Pc[wn].prevx=xxg ;
	    Pc[wn].prevy=yyg ;
	    break ;
    case PSET:
	    Pc[wn].prevx=xxg ;	/* 0.74 */
	    Pc[wn].prevy=yyg ;	/* 0.74 */
    }
}

/*************************
 * 描画する位置を移動する
 * 　引数　int wn: ウィンドウ番号
 * 　　　　 double xg: x座標
 *         double yg: y座標
 * 　戻り値　なし
 */
void moveto( int wn, double xg, double yg )
{
    line(wn,xg,yg,PENUP);
}

/*************************
 * 連続的に線を引く
 * 　引数　int wn: ウィンドウ番号
 * 　　　　 double xg: x座標
 *         double yg: y座標
 * 　戻り値　なし
 */
void lineto( int wn, double xg, double yg )
{
    line(wn,xg,yg,PENDOWN);
}

/************
 *  折線を描く
 * 　引数　int win: ウィンドウ番号
 *        double x[]: x座標の配列
 *        double y[]: y座標の配列
 *        int n: 描画する線の数
 * 　戻り値　なし
 */
void drawlines( int wn, const double x[], const double y[], int n )
{
    int i;
    int xxg = Pc[wn].prevx;
    int yyg = Pc[wn].prevy;

    moveto(wn,x[0],y[0]);
    for ( i=1 ; i < n ; i++ ) {
        lineto(wn,x[i],y[i]);
    }

	Pc[wn].prevx=xxg ;
	Pc[wn].prevy=yyg ;
}

