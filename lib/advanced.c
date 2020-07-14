/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * Advanced関数
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fbeggx.h"

/*************************
 * ディスプレイの情報を取得する
 *  引数　int *rt_depth: 使用可能色数
 *       int *root_width:　ディスプレイの幅
 *       int *root_height:　ディスプレイの高さ
 * 戻り値　0で成功
 */
int ggetdisplayinfo( int *rt_depth, int *root_width, int *root_height )
{
	int fd,d,w,h;

	fd = openFB(NULL);
	getFBSize(fd,&w,&h,&d);
    if(rt_depth!=NULL) *rt_depth = d;
    if(root_width!=NULL) *root_width = w;
    if(root_height!=NULL) *root_height = h;
    close(fd);
    return 0 ;
}

/*************************
 * 描画関数におけるフラッシュの設定
 *  引数　int flag: ENABLEかDISABLEか
 * 戻り値　なし
 */
void gsetnonflush( int flag )
{
    if ( flag != DISABLE ) {
	    fbglobal.Pc_nonflush = ENABLE;
    } else {
	    fbglobal.Pc_nonflush = DISABLE;
    }
    return;
}
/*************************
 * 描画関数におけるフラッシュの設定の取得
 *  引数　なし
 * 戻り値　ENABLEかDISABLEか
 */
int ggetnonflush( void )
{
    return fbglobal.Pc_nonflush;
}

/*************************
 * 手動flushを行なう
 */
void gflush( void )
{
    fprintf(stderr,"gflush has no effect.\n");
    return;
}

/*************************
 * gopen()で開かれるウィンドウ属性を指定する
 * 引数  int values: ENABLEかDISABLEか
 *       int att_msk: 各属性の変更フラグ
 * 戻り値　なし
 */
void gsetinitialattributes( int values, int att_msk )
{
    if ( values == ENABLE ) {
	    fbglobal.Pc_attributes |= att_msk;
    } else if ( values == DISABLE ) {
	    fbglobal.Pc_attributes &= ~att_msk;
    }
    else {
	    fbglobal.Pc_attributes |= (values & att_msk) ;
	    fbglobal.Pc_attributes &= (values | ~att_msk) ;
    }

    return;
}

/*************************
 * gopen()で開かれるウィンドウ属性を取得する
 *　戻り値　設定値
 */
int ggetinitialattributes( void )
{
    return fbglobal.Pc_attributes;
}

/*************************
 * gopen()で開かれるウィンドウの背景色を指定する
 *  引数　const char *argsformat, ... : 色名
 *　戻り値　なし
 */void gsetinitialbgcolor(const char *argsformat, ... )
{
    char *color = NULL;
    va_list ap;

    if ( argsformat == NULL ) return;
	va_start(ap, argsformat);
	color = _eggx_vasprintf(argsformat,ap);
	va_end(ap);

	fbglobal.Pc_bg = get_color_pixel(color);
    if(color != NULL) free(color);
    return;
}

/*************************
 * ウインドウのボーダーの色を指定する
 */
void gsetborder( int wn, int width, const char *argsformat, ... )
{
    fprintf(stderr,"gsetborder has no effect.\n");
    return;
}

/*************************
 * gopen()で開かれるウィンドウのボーダーを指定する
 */
void gsetinitialborder( int width, const char *argsformat, ... )
{
    fprintf(stderr,"gsetinitialborder has no effect.\n");
    return;
}

/*************************
 * gopen()で開かれるウィンドウの大きさと出現座標を指定する
 */
void gsetinitialgeometry( const char *argsformat, ... )
{
    fprintf(stderr,"gsetinitialgeometry has no effect.\n");
    return;
}

/*************************
 * gopen()でのウィンドウ名、アイコン名、リソース名、クラス名を指定する
 */
void gsetinitialwinname( const char *storename, const char *iconname,
			      const char *resname, const char *classname )
{
    fprintf(stderr,"gsetinitialwinname has no effect.\n");
    return;
}

/*************************
 * EGGXによるスクロースバーのキー操作のためのキーマスクを設定する
 */
void gsetscrollbarkeymask( int wn, unsigned int key_mask )
{
    fprintf(stderr,"gsetscrollbarkeymask has no effect.\n");
    return;
} 

