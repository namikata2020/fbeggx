/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 * 
 * 多角形を引く関連
 */

#include <stdio.h>
#include <stdlib.h>
#include "fbeggx.h"

/*
 * ポリゴン描画サンプルプログラム
 * 
 * 参考
 * http://uno036.starfree.jp/PRGmanual/algo1/algo6-1.htm
 * http://uno036.starfree.jp/PRGmanual/algo1/algo6-2.htm
 * 
 * http://fussy.web.fc2.com/algo/polygon3_misc.htm
 * http://fussy.web.fc2.com/algo/polygon2_vertex.htm
 * http://fussy.web.fc2.com/algo/polygon1_clipping.htm
 * http://www.f.waseda.jp/moriya/PUBLIC_HTML/education/classes/infomath6/applet/fractal/vector_product/
 */

/*
 * ポリゴンを構成する直線を表す構造体
 */
struct EDGBUF {
	double X;   /* 交点のX座標(初期値は始点のX座標) */
	int Y0;  /* 始点のY座標 */
	int Y1;  /* 終点のY座標 */
	double a;    /* 傾き(dX/dY) */
	int downward ;	/* 方向 */
};

/************************** 
 * 頂点のリストより辺リストの作成する 
 * 引数　int dataN:頂点の数
 *      signed short int * ApexList;  頂点リスト
 *      struct EDGBUF * Edge;         辺リスト
 * 戻り値　辺リスト
 */
static int GenerateEdgeList(int dataN,int *ApexList, struct EDGBUF *Edge )
{
	signed short int x,y;
	int i,edg=0;

	for ( i = 0 ; i < dataN ; i++ ) {
		x = ApexList[2*i+2] - ApexList[2*i];
		y = ApexList[2*i+3] - ApexList[2*i+1];
		if ( y != 0 ) {
			Edge[edg].downward = ( y >= 0 ) ? 1:0;
			Edge[edg].X =  ( y > 0 ) ? ApexList[2*i] : ApexList[2*i+2];
			Edge[edg].a = (double) x /(double) y;
			Edge[edg].Y0 = ( y > 0 ) ? ApexList[2*i+1] : ApexList[2*i+3];
			Edge[edg].Y1 = ( y > 0 ) ? ApexList[2*i+3] : ApexList[2*i+1];
			edg++;
		}
	}
	return edg;
}

/***************************** 
 * X座標のソート(単純挿入法) 
 * 引数　struct EDGBUF *DataStart: 並び替えの先頭ポインタ
 *      struct EDGBUF *DataEnd:   並び替える最終ポインタ
 * 戻り値　なし
 */
static void InsertSort(struct EDGBUF *DataStart,struct EDGBUF *DataEnd)
{
	struct EDGBUF *GetData, *DataAddr;
	struct EDGBUF p;

	for ( GetData = DataEnd - 1 ; GetData >= DataStart ; GetData-- ) {
		p = *GetData;
		for ( DataAddr = GetData + 1 ; ( *DataAddr ).X < p.X ; DataAddr++ )
			*( DataAddr - 1 ) = *DataAddr;
		*( DataAddr - 1 ) = p;
	}
}

/**************
 *  ポリゴン描画メインルーチン 
 * Crossing Number (Even-Odd) Test アルゴリズム
 * 引数 int StartEdge: 辺リストのアドレス
 *     int EndEdge: 辺リストの終了アドレス
 *     struct EDGBUF *EdgBuf:  辺リストアドレス格納用配列
 * 戻り値　なし
 */
void PolyFill1(int wn,int StartEdge,int EndEdge,struct EDGBUF *EdgBuf)
{
	int ActiveEdgePointer, NonActiveEdgePointer;
	struct EDGBUF ep, eof;
	int x,y;
	int x0,x1;
	int MINY=0,MAXY=Pc[wn].wszy-1; //画面のサイズY方向
	int MINX=0,MAXX=Pc[wn].wszx-1; //画面のサイズx方向

	eof.X = 0x7FFFFFFF;

	//全ての辺リストを「未アクティブ」にする 
	//NonActiveEdgePointer = EdgBufPointer;
	for (int dp = StartEdge ;dp < EndEdge ;dp++ ) {
		EdgBuf[dp-StartEdge] = EdgBuf[dp];
	}
	ActiveEdgePointer = NonActiveEdgePointer = EndEdge - StartEdge;
	// 「アクティブ」な辺リストの末尾に番人を設ける(ソート時に必要) 
	EdgBuf[ActiveEdgePointer + 1 ] = eof;
	for ( y = MINY ; y <= MAXY ; y++ ) {
		// yと始点が一致した「未アクティブ」な辺を「アクティブ」にする 
		for ( int i = 0 ; i <= NonActiveEdgePointer ; ) {
			if ( EdgBuf[i].Y0 == y ) {
				ep = EdgBuf[i];
				EdgBuf[i] = EdgBuf[NonActiveEdgePointer];
				EdgBuf[NonActiveEdgePointer--] = ep;
			} else i++;
		}

		// ｙと終点が一致した「アクティブ」な辺を削除する 
		for ( int i = NonActiveEdgePointer + 1 ; i <= ActiveEdgePointer ; ) {
			if ( EdgBuf[i].Y1 == y ) {
				EdgBuf[i] = EdgBuf[ActiveEdgePointer];
				EdgBuf[ActiveEdgePointer--] = eof;
			} else i++;
		}

		// 交点のソート 
		InsertSort( &EdgBuf[NonActiveEdgePointer + 1] , &EdgBuf[ActiveEdgePointer] );
		// クリッピング・エリア外の交点をチェックしながらライン描画 
		for ( int i = NonActiveEdgePointer + 1 ; i < ActiveEdgePointer ; i += 2 ) {
			x0 = EdgBuf[i].X;
			x1 = EdgBuf[i+1].X;
			if ( x1 < MINX || x0 > MAXX ) continue;
			if ( x0 < MINX ) x0 = MINX;
			if ( x1 > MAXX ) x1 = MAXX;
			for ( x = x0 ; x <= x1 ; x++ ) lowpset(wn,x,y);
			// X座標の更新 
			EdgBuf[i].X += EdgBuf[i].a;
			EdgBuf[i+1].X += EdgBuf[i+1].a;
		}
	}
}


/**************
 *  ポリゴン描画メインルーチン 
 * Winding Number アルゴリズム
 * 引数 int StartEdge: 辺リストのアドレス
 *     int EndEdge: 辺リストの終了アドレス
 *     struct EDGBUF *EdgBuf:  辺リストアドレス格納用配列
 * 戻り値　なし
 */
void PolyFill2(int wn,int StartEdge,int EndEdge,struct EDGBUF *EdgBuf)
{
	int ActiveEdgePointer, NonActiveEdgePointer;
	struct EDGBUF ep, eof;
	int x,y;
	int x0,x1;
	int MINY=0,MAXY=Pc[wn].wszy-1; //画面のサイズY方向
	int MINX=0,MAXX=Pc[wn].wszx-1; //画面のサイズx方向

	eof.X = 0x7FFFFFFF;

	/* 全ての辺リストを「未アクティブ」にする */
	//NonActiveEdgePointer = EdgBufPointer;
	for (int dp = StartEdge ;dp < EndEdge ;dp++ ) {
		EdgBuf[dp-StartEdge] = EdgBuf[dp];
	}
	ActiveEdgePointer = NonActiveEdgePointer = EndEdge - StartEdge;
	/* 「アクティブ」な辺リストの末尾に番人を設ける(ソート時に必要) */
	EdgBuf[ActiveEdgePointer + 1 ] = eof;
	for ( y = MINY ; y <= MAXY ; y++ ) {
		/* yと始点が一致した「未アクティブ」な辺を「アクティブ」にする */
		for ( int i = 0 ; i <= NonActiveEdgePointer ; ) {
			if ( EdgBuf[i].Y0 == y ) {
				ep = EdgBuf[i];
				EdgBuf[i] = EdgBuf[NonActiveEdgePointer];
				EdgBuf[NonActiveEdgePointer--] = ep;
			} else i++;
		}

		/* ｙと終点が一致した「アクティブ」な辺を削除する */
		for ( int i = NonActiveEdgePointer + 1 ; i <= ActiveEdgePointer ; ) {
			if ( EdgBuf[i].Y1 == y ) {
				EdgBuf[i] = EdgBuf[ActiveEdgePointer];
				EdgBuf[ActiveEdgePointer--] = eof;
			} else i++;
		}

		/* 交点のソート */
		InsertSort( &EdgBuf[NonActiveEdgePointer + 1] , &EdgBuf[ActiveEdgePointer] );
		/* クリッピング・エリア外の交点をチェックしながらライン描画 */
		for ( int i = NonActiveEdgePointer + 1; i < ActiveEdgePointer ;i++) {
			int starti = i;
			int wnn = (EdgBuf[starti].downward==1)? 1:-1; 
			for(i++;i<=ActiveEdgePointer;i++) {
				wnn += (EdgBuf[i].downward==1) ? 1:-1;
				if(wnn==0) {
					break;
				} else {
					EdgBuf[i].X += EdgBuf[i].a;
				}
			}
			if(wnn != 0) break;
			x0 = EdgBuf[starti].X;
			x1 = EdgBuf[i].X;
			if ( x1 < MINX || x0 > MAXX ) continue;
			if ( x0 < MINX ) x0 = MINX;
			if ( x1 > MAXX ) x1 = MAXX;
			for ( x = x0 ; x <= x1 ; x++ ) lowpset(wn,x,y);
			/* X座標の更新 */
			EdgBuf[starti].X += EdgBuf[starti].a;
			EdgBuf[i].X += EdgBuf[i].a;
		}
	}
}

/**************
 *  ポリゴンを描画する 
 * 引数 int vnum: 頂点の数
 *     int *data: 頂点のリスト{ x0,y0,x1,y1,x2,y2...}
 * 戻り値　なし
 */
void lowpolygonf(int win,int vnum,int *data, int shape)
{
	struct EDGBUF *buf;
	
	buf = (struct EDGBUF *)_eggx_xmalloc(sizeof(struct EDGBUF)*(vnum+1));//終了記号を入れるため+1
	int num = GenerateEdgeList(vnum,data,buf);
	if(shape==0) {
		PolyFill1(win,0,num-1,buf);
	} else {
		PolyFill2(win,0,num-1,buf);
	}
	free(buf);
}

/************
 *  多角形を描く
 * 　引数　int win: ウィンドウ番号
 *        double x[]: x座標の配列
 *        double y[]: y座標の配列
 *        int n: 描画する角の数
 *        int shape: 描画するモード
 * 　戻り値　なし
 */
void fillpoly(int wn, const double x[], const double y[], int n, int shape)
{
    int xx, yy, i;
    int *points = NULL;

    if ( n < 1 ) return;

    points = (int *)_eggx_xmalloc(sizeof(int)*(n+1)*2);
    for ( i=0 ; i < n ; i++ ) {
	    xyconv( wn,x[i],y[i],&xx,&yy );
	    points[2*i] = xx;
	    points[2*i+1] = yy;
    }
    points[2*n] = points[0];	//先頭の点に戻る
    points[2*n+1] = points[1];
    lowpolygonf(wn,n,points,shape);
    if(points != NULL) free(points);
}

/************
 *  多角形を描く
 * 　引数　int win: ウィンドウ番号
 *        double x[]: x座標の配列
 *        double y[]: y座標の配列
 *        int n: 描画する角の数
 * 　戻り値　なし
 */
void drawpoly( int wn, const double x[], const double y[], int n )
{
    int i;
    int xxg = Pc[wn].prevx;
    int yyg = Pc[wn].prevy;

    moveto(wn,x[0],y[0]);
    for ( i=1 ; i < n ; i++ ) {
        lineto(wn,x[i],y[i]);
    }
    lineto(wn,x[0],y[0]);
	Pc[wn].prevx=xxg ;
	Pc[wn].prevy=yyg ;
}

/*************************
 * 長方形を描く
 *  引数　int win: ウィンドウ番号　
 *        double x,y: 描画する座標
 * 　　　 double w,h: 幅と高さ
 * 戻り値　なし
 */
void drawrect(int wn,double x,double y,double w,double h)
{
    int x1,y1,x2,y2;
    xyconv( wn,x,y,&x1,&y1 ) ;
    xyconv( wn,x+w,y+h,&x2,&y2 ) ;
    mlowline(wn,x1,y1,x2,y1);
	mlowline(wn,x2,y1,x2,y2);
	mlowline(wn,x2,y2,x1,y2);
	mlowline(wn,x1,y2,x1,y1);
}

/*************************
 * 塗りつぶした長方形を描く
 *  引数　int win: ウィンドウ番号　
 *        double x,y: 描画する座標
 * 　　　 double w,h: 幅と高さ
 * 戻り値　なし
 */
void fillrect(int wn,double x,double y,double w,double h)
{
	int tmp,i;
    int x1,y1,x2,y2;
    xyconv( wn,x,y,&x1,&y1 ) ;
    xyconv( wn,x+w,y+h,&x2,&y2 ) ;
	
	if(x1>x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	for(i=x1;i<=x2;i++) {
		lowline(wn,i,y1,i,y2);
	}
}
