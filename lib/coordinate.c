/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * 座標変換
 * xyconv() 
 * window()
 * coordinate()
 */

#include "fbeggx.h"


/************
 * 四捨五入で整数化する
 * 　引数　double v: 四捨五入する数
 * 　戻り値　四捨五入した結果
 */
int _eggx_iround( double v )
{
    if ( v < 0 ) return (int)(v-0.5);
    else return (int)(v+0.5);
}


/************
 * 座標を変換する
 * 　引数　int wn: ウィンドウ番号
 *        double x: 変換前x座標
 *        double y: 返還前y座標
 *        double *rx: 返還後y座標
 *        double *ry: 返還後y座標
 * 　戻り値　なし
 */
void xyconv( int wn, double x, double y, int *rx, int *ry )
{
    double f_rx, f_ry;
    f_rx = ((x-Pc[wn].acx0)*Pc[wn].scalex);
    f_ry = ((y-Pc[wn].acy0)*Pc[wn].scaley);
    if ( f_rx < -32767.0 ) f_rx = -32767.0;
    else if ( 32767.0 < f_rx ) f_rx = 32767.0;
    if ( f_ry < -32767.0 ) f_ry = -32767.0;
    else if ( 32767.0 < f_ry ) f_ry = 32767.0;

    *rx = _eggx_iround(f_rx);
    if ( (Pc[wn].attributes & BOTTOM_LEFT_ORIGIN) ) {
	int ry0 = Pc[wn].wszy - 1 - _eggx_iround(f_ry);
	if ( 32767 < ry0 ) *ry = 32767;
	else *ry = ry0;
    }
    else {
	*ry = _eggx_iround(f_ry);
    }
    return;
}

/************
 * アプリケーション座標系を変更する
 * 　引数　int wn: ウィンドウ番号
 *        double xs: 新座標系左下x座標
 *        double ys: 新座標系左下y座標
 *        double xe: 新座標系右上y座標
 *        double ye: 新座標系右上y座標
 * 　戻り値　なし
 */
void window( int wn, double xs, double ys, double xe, double ye )
{
    Pc[wn].acx0=xs ;
    Pc[wn].acy0=ys ;
    if( Pc[wn].wszx <= 1 ) Pc[wn].scalex = 1 ;
    else Pc[wn].scalex = (double)Pc[wn].wszx/ 
	     ((double)(Pc[wn].wszx)*(xe-Pc[wn].acx0)/(double)(Pc[wn].wszx-1)) ;
    if( Pc[wn].wszy <= 1 ) Pc[wn].scaley = 1 ;
    else Pc[wn].scaley = (double)Pc[wn].wszy/
	     ((double)(Pc[wn].wszy)*(ye-Pc[wn].acy0)/(double)(Pc[wn].wszy-1)) ;
    return;
}

/************
 * アプリケーション座標系を変更する
 * 　引数　int wn: ウィンドウ番号
 *        double xs: 新座標系左下x座標
 *        double ys: 新座標系左下y座標
 *        double xe: 新座標系右上y座標
 *        double ye: 新座標系右上y座標
 * 　戻り値　なし
 */
void coordinate( int wn, int xref, int yref,
		      double xs, double ys, double xscale, double yscale )
{
    Pc[wn].acx0 = (xs * xscale - xref) / xscale;
    Pc[wn].acy0 = (ys * yscale - yref) / yscale;
    Pc[wn].scalex = xscale;
    Pc[wn].scaley = yscale;

    return;
}
