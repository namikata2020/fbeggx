/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * レイヤの管理
 * layer()
 * copylayer() 
 */

#include <string.h>
#include "fbeggx.h"

/************
 * 表示，書き込むレイヤを選択する
 * 　引数　int wn: ウィンドウ番号
 *        int sl: 表示するレイヤ
 *        int wl: 書き込むレイヤ
 * 　戻り値　なし
 (Pc[win].fb16[layer])[y*fbglobal.width+x] = Pc[win].fgc15;
 */
void layer( int wn, int sl, int wl )
{
    if ( sl < 0 || MAX_NLAYER <= sl ) sl = Pc[wn].sly;
    if ( wl < 0 || MAX_NLAYER <= wl ) wl = Pc[wn].wly;
    /* */
    if ( Pc[wn].sly != sl  ) {
		switch(fbglobal.colorperpixel)
		{
		case 8:
			if(Pc[wn].fb8[sl]!=NULL) {
				free(Pc[wn].fb8[sl]);	
			} 
			Pc[wn].fb8[sl] = Pc[wn].fb8[Pc[wn].sly];
			Pc[wn].fb8[Pc[wn].sly] = NULL;
			break;
		case 15:
		case 16:
			if(Pc[wn].fb16[sl]!=NULL) {
				free(Pc[wn].fb16[sl]);	
			} 
			Pc[wn].fb16[sl] = Pc[wn].fb16[Pc[wn].sly];
			Pc[wn].fb16[Pc[wn].sly] = NULL;
			break;
		case 24:
		case 32:
			if(Pc[wn].fb32[sl]!=NULL) {
				free(Pc[wn].fb32[sl]);	
			} 
			Pc[wn].fb32[sl] = Pc[wn].fb32[Pc[wn].sly];
			Pc[wn].fb32[Pc[wn].sly] = NULL;
			break;
		}
	}
    if ( Pc[wn].wly != wl  ) {
		switch(fbglobal.colorperpixel)
		{
		case 8:
			if(Pc[wn].fb8[wl]==NULL) {
				Pc[wn].fb8[wl] = (uint8_t *)malloc(fbglobal.width * fbglobal.height * fbglobal.cpp);
			}
			break;
		case 15:
		case 16:
			if(Pc[wn].fb16[wl]==NULL) {
				Pc[wn].fb16[wl] = (uint16_t *)malloc(fbglobal.width * fbglobal.height * fbglobal.cpp);
			}
			break;
		case 24:
		case 32:
			if(Pc[wn].fb32[wl]==NULL) {
				Pc[wn].fb32[wl] = (uint32_t *)malloc(fbglobal.width * fbglobal.height * fbglobal.cpp);
			}
			break;
		}
	}
	//gclr(wn);
    Pc[wn].sly = sl ;
    Pc[wn].wly = wl ;
}

/************
 * レイヤのコピーを行う
 * 　引数　int wn: ウィンドウ番号
 *        int src: コピー元レイヤ
 *        int dst: コピー先レイヤ
 * 　戻り値　なしmemcpy(void *dest， const void *src， size_t n);
 (Pc[win].fb16[layer])[y*fbglobal.width+x] = Pc[win].fgc15;
 */
 /* gopen()で設定した範囲をコピーする　1dot毎にコピー */
/*void copylayer( int wn, int src, int dst )
{
	switch(fbglobal.colorperpixel)
	{
		case 8:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb8[src] == NULL || Pc[wn].fb8[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				for(int x=0; x<Pc[wn].wszx; x++){
					(Pc[wn].fb8[dst])[y*fbglobal.width+x] = (Pc[wn].fb8[src])[y*fbglobal.width+x];
				}
			}
			break;
		case 15:
		case 16:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb16[src] == NULL || Pc[wn].fb16[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				for(int x=0; x<Pc[wn].wszx; x++){
					(Pc[wn].fb16[dst])[y*fbglobal.width+x] = (Pc[wn].fb16[src])[y*fbglobal.width+x];
				}
			}
			break;
		case 24:
		case 32:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb32[src] == NULL || Pc[wn].fb32[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				for(int x=0; x<Pc[wn].wszx; x++){
					(Pc[wn].fb32[dst])[y*fbglobal.width+x] = (Pc[wn].fb32[src])[y*fbglobal.width+x];
				}
			}
			break;
	}
}
*/
 /* gopen()で設定した範囲をコピーする　ライン毎にmemcpy() */

void copylayer( int wn, int src, int dst )
{
	int offx=0,offy=0;

	if(Pc[wn].attributes&OFFSET_RIGHT) {
		offx =  fbglobal.offsetx;
		offy =  fbglobal.offsety;
	}

	switch(fbglobal.colorperpixel)
	{
		case 8:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb8[src] == NULL || Pc[wn].fb8[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				int cpsize = Pc[wn].wszx*fbglobal.cpp;
				uint8_t *dstaddr = &(Pc[wn].fb8[dst])[(y+offy)*fbglobal.width+offx];
				uint8_t *srcaddr = &(Pc[wn].fb8[src])[(y+offy)*fbglobal.width+offx];
				memcpy(dstaddr,srcaddr,cpsize);
			}
			break;
		case 15:
		case 16:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb16[src] == NULL || Pc[wn].fb16[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				int cpsize = Pc[wn].wszx*fbglobal.cpp;
				uint16_t *dstaddr = &(Pc[wn].fb16[dst])[(y+offy)*fbglobal.width+offx];
				uint16_t *srcaddr = &(Pc[wn].fb16[src])[(y+offy)*fbglobal.width+offx];
				memcpy(dstaddr,srcaddr,cpsize);
			}
			break;
		case 24:
		case 32:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb32[src] == NULL || Pc[wn].fb32[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			for(int y=0; y<Pc[wn].wszy; y++) {
				int cpsize = Pc[wn].wszx*fbglobal.cpp;
				uint32_t *dstaddr = &(Pc[wn].fb32[dst])[(y+offy)*fbglobal.width+offx];
				uint32_t *srcaddr = &(Pc[wn].fb32[src])[(y+offy)*fbglobal.width+offx];
				memcpy(dstaddr,srcaddr,cpsize);
			}
			break;
	}
}


/* gopen()で設定した範囲を無視して全画面でコピーする　memcpy()使用 */
/*
void copylayer( int wn, int src, int dst )
{
	switch(fbglobal.colorperpixel)
	{
		case 8:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb8[src] == NULL || Pc[wn].fb8[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			memcpy(Pc[wn].fb8[dst],Pc[wn].fb8[src],fbglobal.width * fbglobal.height * fbglobal.cpp);
			break;
		case 15:
		case 16:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb16[src] == NULL || Pc[wn].fb16[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			memcpy(Pc[wn].fb16[dst],Pc[wn].fb16[src],fbglobal.width * fbglobal.height * fbglobal.cpp);
			break;
		case 24:
		case 32:
		    if ( src < 0 || MAX_NLAYER <= src || dst < 0 || MAX_NLAYER <= dst ||
	 			Pc[wn].fb32[src] == NULL || Pc[wn].fb32[dst] == NULL ) {
				fprintf(stderr,"Invalid layer index at copylayer()\n");
				return;
			}
			memcpy(Pc[wn].fb32[dst],Pc[wn].fb32[src],fbglobal.width * fbglobal.height * fbglobal.cpp);
			break;
	}
}
*/