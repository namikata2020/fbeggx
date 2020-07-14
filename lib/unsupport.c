/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * サポートしていない関数
 */

#include "fbeggx.h"

/************
 * ウィンドウのタイトルを変更する
 * 　引数　int win: ウィンドウ番号
 *        const char *argformat,...: 変更する文字列
 * 　戻り値　表示した文字数
 */
int winname( int wn, const char *argsformat, ... )
{
	fprintf(stderr,"winname is not implemented.\n");
	return 0;
}

/*************************
 * 線のスタイルを指定する
 * 　引数　int wn: ウィンドウ番号
 * 　　　　int style: 線のスタイル
 * 　戻り値　なし
 */
void newlinestyle( int wn, int style )
{
	fprintf(stderr,"newlinestyle is not implemented.\n");
    return;
}

/*************************
 * GC function を指定する
 * 　引数　int wn: ウィンドウ番号
 * 　　　　int fnc: 設定
 * 　戻り値　なし
 */
void newgcfunction( int wn, int fnc )
{
	fprintf(stderr,"newgcfunction is not implemented.\n");
    return;
}

/*************************
 *　任意のウィンドウ・レイヤ・領域の画像を描画レイヤにコピーする
 *  引数　int wn: ウィンドウ番号
 *      　double x: コピー先x座標
 * 　　　 double y: コピー先y座標
 *        int src_wn: コピー元ウィンドウ番号
 *        int src_ly:　コピー元レイヤー番号
 *        double xs:　コピー元　スタートx座標
 *        double ys:　コピー元　スタートy座標
 *        double xe:　コピー元　エンドx座標
 *        double ye:　コピー元　エンドy座標
 * 戻り値　なし
 */
void gputarea( int wn, double x, double y,
	   int src_wn, int src_ly, double xs, double ys, double xe, double ye )
{
	fprintf(stderr,"gputarea is not implemented.\n");
	return;   
}

/*************************
 *　マウスやキーボードからの入力情報を返す
 */
int ggetevent( int *type_r, int *code_r, double *x_r, double *y_r )
{
	fprintf(stderr,"ggetevent is not implemented.\n");
	return -1;
}

/*************************
 *　マウスからのボタンクリック、キーボードからの入力情報を返す
 */
int ggetxpress( int *type_r, int *code_r, double *x_r, double *y_r )
{
	fprintf(stderr,"ggetxpress is not implemented.\n");
	return -1;
}
