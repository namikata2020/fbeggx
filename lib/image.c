/*
 * fbeggxlib
 * グラフィックスライブラリeggxをフレームバッファで使用可能にする
 * https://www.ir.isas.jaxa.jp/~cyamauch/eggx_procall/index.ja.html
 *  
 * イメージ関連関数
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fbeggx.h"

#define ch_is_space(ch) isspace((int)(ch))
/* 一時利用のための子プロセス */
static volatile pid_t Cpid_tmp = 0 ;
static int Pc_eggxhandlers_are_set = 0;

/* "cmd arg arg" といった文字列を分割して argv な形式にする */
/* 第二，第三引数で返される領域は，呼び出し側で開放が必要 */
static int create_args( const char *args_string, 
			char **args_buf_r, char ***args_r )
{
    char **args = NULL;
    char *f_ptr;
    int i, nargs;
    /* argsの作成 */
    *args_buf_r = _eggx_xstrdup(args_string);
    f_ptr = *args_buf_r;
    i = 0;
    while ( 1 ) {
	int spn;
	spn = strspn(f_ptr," ");
	if ( 0 < spn ) {
	    *f_ptr = '\0';
	    f_ptr += spn;
	}
	spn = strcspn(f_ptr," ");
	args = (char **)_eggx_xrealloc(args,sizeof(char *)*(i+1));
	if ( 0 < spn ) {
	    args[i++] = f_ptr;
	    f_ptr += spn;
	}
	else {
	    args[i] = NULL;
	    break;
	}
    }
    nargs = i;

    *args_r = args;
    return nargs;
}

/* pnm ファイルのヘッダ部を1行読む */
static char *read_next( FILE *fp, char *buf, size_t sz )
{
    char *rt = NULL;
    while ( 1 ) {
	int i;
	rt = fgets(buf,sz,fp);
	if ( rt == NULL ) break;
	if ( strchr(buf,'\n') == NULL ) {
	    while ( 1 ) {
		int ch = fgetc(fp);
		if ( ch == '\n' || ch == EOF ) break;
	    }
	}
	for ( i=0 ; buf[i] == ' ' || buf[i] == '\t' ; i++ );
	if ( buf[i] != '#' ) {
	    rt = buf + i;
	    break;
	}
    }
    return rt;
}

/*************************
 *　ファイルの画像データをコンバータを通してメモリバッファに取り込む
 *  引数　
 *      　const char *filter: 画像コンバータ
 * 　　　 const char *filename: 画像のファイル名
 *      　int *r_width: 取り込む　画像の幅
 *        int *r_height: 取り込む　画像の高さ
 *        int *r_msk: 画像のマスク
 * 戻り値　画像データのポインタ
 */
/* バイナリ pbm,pgm,ppm,pam 形式のファイルを読む．その他の形式は pngtopnm 等 */
/* の変換プログラムで読み込みが可能．変換プログラムは filter で指定する */
/* 返り値は確保され画像が保存されたバッファで，呼び出し側で開放が必要 */
unsigned char *readimage( const char *filter, const char *filename,
			       int *r_width, int *r_height, int *r_msk )
{
    unsigned char *rt_buf = NULL;
    char *fn = NULL;
    char *tmp1 = NULL;
    char **args = NULL;
    unsigned char *line_buf = NULL;
    char txt_line_buf[256];
    char pam_stdout[] = "PAM:-";		/* for convert */
    const char *pos;
    FILE *fp = NULL;
    int width = 0, height = 0, maxv = 0, nchnl = 0, nbytes = 0, msk = 0;
    size_t ii, sz_line = 0;
    int i, pnm_format;				/* fmt: '4'～'7' のいずれか */
    int found_alpha = 0;

    if ( filename == NULL ) goto quit;

    if ( filter != NULL &&				/* ImageMagick */
	 (strcmp(filter,"convert")==0 || strcmp(filter,"convert.exe")==0) ) {
		int nargs;
		int pfds[2];
		/* */
		nargs = create_args(filter, &tmp1, &args);
		args = (char **)_eggx_xrealloc(args,sizeof(char *)*(nargs+3));
		fn = _eggx_xstrdup(filename);
		args[nargs] = fn;
		nargs++;
		args[nargs] = pam_stdout;
		nargs++;
		args[nargs] = NULL;
		nargs++;
		//{for(i=0;i<nargs;i++){fprintf(stderr,"DEBUG: [%s]\n",args[i]);}}
		/* */
		if ( pipe(pfds) < 0 ) {
			fprintf(stderr,"EGGX: [ERROR] pipe() failed\n");
			goto quit;
		}
		if ( (Cpid_tmp=fork()) < 0 ) {
			fprintf(stderr,"EGGX: [ERROR] fork() failed\n");
			goto quit;
		}
		if ( Cpid_tmp == 0 ) {	/* 子プロセス */
			dup2( pfds[1],1 );	/* 読み込み用 */
			close( pfds[1] );
			close( pfds[0] );
			execvp( *args, args );
			fprintf(stderr,"EGGX: [ERROR] Cannot exec '%s' command.\n",*args);
			_exit(-1);
		}
		/* 親 */
		close( pfds[1] );
		/* */
		fp = fdopen(pfds[0],"rb");
    }
    else if ( filter != NULL && *filter != '\0' ) {	/* netpbm */
		int nargs;
		int pfds[2];
		nargs = create_args(filter, &tmp1, &args);
		args = (char **)_eggx_xrealloc(args,sizeof(char *)*(nargs+2));
		fn = _eggx_xstrdup(filename);
		args[nargs] = fn;
		nargs++;
		args[nargs] = NULL;
		nargs++;
		//{for(i=0;i<nargs;i++){fprintf(stderr,"DEBUG: [%s]\n",args[i]);}}
		/* */
		if ( pipe(pfds) < 0 ) {
			fprintf(stderr,"EGGX: [ERROR] pipe() failed\n");
			goto quit;
		}
		if ( (Cpid_tmp=fork()) < 0 ) {
			fprintf(stderr,"EGGX: [ERROR] fork() failed\n");
			goto quit;
		}
		if ( Cpid_tmp == 0 ) {	/* 子プロセス */
			dup2( pfds[1],1 );	/* 読み込み用 */
			close( pfds[1] );
			close( pfds[0] );
			execvp( *args, args );
			fprintf(stderr,"EGGX: [ERROR] Cannot exec '%s' command.\n",*args);
			_exit(-1);
		}
		/* 親 */
		close( pfds[1] );
		/* */
		fp = fdopen(pfds[0],"rb");
    } else {
		fp = fopen(filename,"rb");
    }

    if ( fp == NULL ) {
		fprintf(stderr,"EGGX: [ERROR] Cannot open file: %s\n",filename);
		goto quit;
    }

    /* 読み込み開始 */
    pos = read_next(fp,txt_line_buf,256);
    if ( pos == NULL || pos[0] != 'P' ) {
		fprintf(stderr,"EGGX: [ERROR] Invalid stream (1)\n");
		goto quit;
    }

    pnm_format = pos[1];

    if ( '4' <= pnm_format && pnm_format <= '6' ) {	/* binary p[bgp]m */
	int n;
	pos = read_next(fp,txt_line_buf,256);
	if ( pos == NULL ) {
	    fprintf(stderr,"EGGX: [ERROR] Invalid stream (2)\n");
	    goto quit;
	}
	n = sscanf(pos,"%d %d\n",&width,&height);
	if ( n != 2 ) {
	    fprintf(stderr,"EGGX: [ERROR] Invalid stream (3)\n");
	    goto quit;
	}
	if ( pnm_format != '4' ) {			/* pgm,ppm */
	    pos = read_next(fp,txt_line_buf,256);
	    if ( pos == NULL ) {
		fprintf(stderr,"EGGX: [ERROR] Invalid stream (4)\n");
		goto quit;
	    }
	    n = sscanf(pos,"%d\n",&maxv);
	    if ( n != 1 ) {
		fprintf(stderr,"EGGX: [ERROR] Invalid stream (5)\n");
		goto quit;
	    }
	    nbytes = (maxv < 256) ? 1 : 2;
	    if ( pnm_format == '5' ) nchnl = 1;
	    else nchnl = 3;
	    msk = 0;
	}
	else {						/* pbm */
	    nchnl = 0;
	    msk = 0;
	    maxv = 1;
	}
    }
    else if ( pnm_format == '7' ) {			/* pam */
		nchnl = -1;
		while ( 1 ) {
			int n = 1;
			pos = read_next(fp,txt_line_buf,256);
			if ( pos == NULL ) {
				fprintf(stderr,"EGGX: [ERROR] Invalid stream (2)\n");
				goto quit;
			}
			if ( strncmp(pos,"WIDTH",5)==0 && (ch_is_space(pos[5])) ) {
				n = sscanf(pos,"%*s %d",&width);
			}
			else if ( strncmp(pos,"HEIGHT",6)==0 && (ch_is_space(pos[6])) ) {
				n = sscanf(pos,"%*s %d",&height);
			}
			else if ( strncmp(pos,"DEPTH",5)==0 && (ch_is_space(pos[5])) ) {
				n = sscanf(pos,"%*s %d",&nchnl);
			}
			else if ( strncmp(pos,"MAXVAL",6)==0 && (ch_is_space(pos[6])) ) {
				n = sscanf(pos,"%*s %d",&maxv);
			}
			else if ( strncmp(pos,"ENDHDR",6)==0 &&
				(ch_is_space(pos[6]) != 0 || pos[6] == '\0') ) {
				break;
			}
			if ( n != 1 ) {
				fprintf(stderr,"EGGX: [ERROR] Invalid stream (3)\n");
				goto quit;
			}
		}
		nbytes = (maxv < 256) ? 1 : 2;
		if ( nchnl == 2 || nchnl == 4 ) msk = 1;
		else msk = 0;
	}
    else {
		fprintf(stderr,"EGGX: [ERROR] Unsupported type: P%c\n",pnm_format);
		goto quit;
    }

    if ( width < 1 ) {
		fprintf(stderr,"EGGX: [ERROR] Invalid width: %d\n",width);
		goto quit;
    }
    if ( height < 1 ) {
		fprintf(stderr,"EGGX: [ERROR] Invalid height: %d\n",height);
		goto quit;
    }
    if ( maxv < 1 || 65535 < maxv ) {
		fprintf(stderr,"EGGX: [ERROR] Unsupported depth: %d\n",maxv+1);
		goto quit;
    }
    if ( nchnl < 0 || 4 < nchnl ) {
		fprintf(stderr,"EGGX: [ERROR] Unsupported type\n");
		goto quit;
    }

    if ( pnm_format == '4' ) {					/* pbm */
		sz_line = (width + 7) / 8;
    }
    else {
		sz_line = nbytes * nchnl * width;
    }

    //fprintf(stderr,"DEBUG: format : P%c\n",pnm_format);

    line_buf = (unsigned char *)_eggx_xmalloc(sz_line);
    rt_buf = (unsigned char *)_eggx_xmalloc((4 * width) * height);

    ii = 0;
	for ( i=0 ; i < height ; i++ ) {																																			
		if ( fread(line_buf, 1, sz_line, fp) != sz_line ) {
			fprintf(stderr,"EGGX: [ERROR] Invalid bitmap stream\n");
			free(rt_buf);
			rt_buf = NULL;
			goto quit;
		}
		if ( nchnl == 3 ) {			/* r,g,b */
			size_t j;
			if ( nbytes == 1 ) {
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii++] = 255;
				rt_buf[ii++] = 255 * line_buf[j++] / maxv;
				rt_buf[ii++] = 255 * line_buf[j++] / maxv;
				rt_buf[ii++] = 255 * line_buf[j++] / maxv;
			}
			}
			else {	/* nbytes == 2 */
			int vl;
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii++] = 255;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii++] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii++] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii++] = 255 * vl / maxv;
			}
			}
		}
		else if ( nchnl == 4 ) {		/* r,g,b,a */
			size_t j;
			if ( nbytes == 1 ) {
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii+1] = 255 * line_buf[j++] / maxv;
				rt_buf[ii+2] = 255 * line_buf[j++] / maxv;
				rt_buf[ii+3] = 255 * line_buf[j++] / maxv;
				rt_buf[ii+0] = 255 * line_buf[j++] / maxv;
				if (0 < rt_buf[ii+0] && rt_buf[ii+0] < 255) found_alpha=1;
				ii += 4;
			}
			}
			else {	/* nbytes == 2 */
			int vl;
			for ( j=0 ; j < sz_line ; ) {
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+1] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+2] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+3] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+0] = 255 * vl / maxv;
				if (0 < rt_buf[ii+0] && rt_buf[ii+0] < 255) found_alpha=1;
				ii += 4;
			}
			}
		}
		else if ( nchnl == 1 ) {		/* pgm */
			size_t j;
			if ( nbytes == 1 ) {
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii++] = 255;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				j++;
			}
			}
			else {
			int vl;
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii++] = 255;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii++] = 255 * vl / maxv;
				rt_buf[ii++] = 255 * vl / maxv;
				rt_buf[ii++] = 255 * vl / maxv;
			}
			}
		}
		else if ( nchnl == 2 ) {		/* grey + a */
			size_t j;
			if ( nbytes == 1 ) {
			for ( j=0 ; j < sz_line ; ) {
				rt_buf[ii] = 255 * line_buf[j+1] / maxv;
				if (0 < rt_buf[ii] && rt_buf[ii] < 255) found_alpha=1;
				ii++;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				rt_buf[ii++] = 255 * line_buf[j] / maxv;
				j += 2;
			}
			}
			else {
			int vl;
			for ( j=0 ; j < sz_line ; ) {
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+1] = 255 * vl / maxv;
				rt_buf[ii+2] = 255 * vl / maxv;
				rt_buf[ii+3] = 255 * vl / maxv;
				vl = line_buf[j++];  vl <<= 8;  vl |= line_buf[j++];
				rt_buf[ii+0] = 255 * vl / maxv;
				if (0 < rt_buf[ii+0] && rt_buf[ii+0] < 255) found_alpha=1;
				ii += 4;
			}
			}
		}
		else if ( nchnl == 0 ) {		/* pbm */
			int j;
			for ( j=0 ; j < width ; ) {
			int v = line_buf[j/8];
			v &= (1 << (7 - (j % 8)));
			if ( v != 0 ) v = 0;
			else v = 255;
			rt_buf[ii++] = 255;
			rt_buf[ii++] = v;
			rt_buf[ii++] = v;
			rt_buf[ii++] = v;
			j++;
			}
		}
    }

    if ( found_alpha != 0 ) {
		msk = 256;
    }

 quit:
    if ( 0 < Cpid_tmp ) {
		if ( Pc_eggxhandlers_are_set ) {
			while ( 0 < Cpid_tmp ) {
				msleep(10);
			}
		}
		else {
			int status;
			while ( wait(&status) != Cpid_tmp );
			Cpid_tmp = 0;
		}
    }
    if ( fp != NULL ) fclose(fp);
    if ( args != NULL ) free(args);
    if ( tmp1 != NULL ) free(tmp1);
    if ( fn != NULL ) free(fn);
    if ( line_buf != NULL ) free(line_buf);
    if ( rt_buf != NULL ) {
		if ( r_width != NULL ) *r_width = width;
		if ( r_height != NULL ) *r_height = height;
		if ( r_msk != NULL ) *r_msk = msk;
    }
    return rt_buf;
}


/*************************
 *　メモリバッファの画像データをコンバータを通してファイルに保存する
 *  引数　
 *        const unsigned char *buf: 画像データ
 *        int width:　画像データの幅
 *        int height:　画像データの高さ
 *        int msk:　画像データのマスク
 *      　const char *filter: 画像コンバータ
 *        int depth:　減色パラメータ
 * 　　　 const char *argsformat, ... : 画像のファイル名
 * 戻り値　成否
 */
/* ppm or pam 形式のファイルを書く．filter を指定して pnmtopng 等で変換する */
/* 事で，様々な形式で保存が可能．msk の値がノンゼロかファイル名が *.pam なら */
/* pam 形式を出力する */
/* 返り値は正常終了なら 0 で，異常終了なら負の値 */
int writeimage( const unsigned char *buf, int width, int height, int msk,
		   const char *filter, int depth, const char *argsformat, ... )
{
    int rtv = -1;
    char *out_fname = NULL;
    char *tmp1 = NULL;
    char **args = NULL;
    /* for convert */
    char rgb_stdin[] = "RGB:-";
    char rgba_stdin[] = "RGBA:-";
    char ag_size[] = "-size";
    char *ag_size_vl = NULL;
    char ag_depth[] = "-depth";
    char ag_depth_vl[] = "8";
    unsigned char *line_buf = NULL;
    const char *p0;
    FILE *fp = NULL;
    int out_is_pam;
    va_list ap;

    if ( argsformat == NULL ) goto quit;
    if ( buf == NULL ) goto quit;
    if ( width < 1 ) goto quit;
    if ( height < 1 ) goto quit;

    /* ファイル名を作る */
    va_start(ap, argsformat);
    out_fname = _eggx_vasprintf(argsformat,ap);
    va_end(ap);

    if ( depth < 2 ) depth = 2;
    else if ( 256 < depth ) depth = 256;

    p0 = strstr(out_fname,".pam");
    if ( p0 == NULL ) p0 = strstr(out_fname,".PAM");

    out_is_pam = 0;
    if ( p0 != NULL && p0[4] == '\0' ) out_is_pam = 1;
    if ( msk != 0 ) out_is_pam = 1;

    if ( filter != NULL &&				/* ImageMagick */
	 (strcmp(filter,"convert")==0 || strcmp(filter,"convert.exe")==0) ) {
	int nargs;
	int pfds[2];
	/* 引数 */
	nargs = create_args(filter, &tmp1, &args);
	/* */
	args = (char **)_eggx_xrealloc(args,sizeof(char *)*(nargs+2+2+3));
	args[nargs] = ag_size;
	nargs++;
	ag_size_vl = _eggx_asprintf("%dx%d",width,height);
	args[nargs] = ag_size_vl;
	nargs++;
	args[nargs] = ag_depth;
	nargs++;
	args[nargs] = ag_depth_vl;
	nargs++;
	if ( msk != 0 ) args[nargs] = rgba_stdin;
	else args[nargs] = rgb_stdin;
	nargs++;
	args[nargs] = out_fname;
	nargs++;
	args[nargs] = NULL;
	nargs++;

	//{int i;for(i=0;i<nargs;i++){fprintf(stderr,"DEBUG: [%s]\n",args[i]);}}
	/* コンバータを起動する */
	if ( pipe(pfds) < 0 ) {
	    fprintf(stderr,"EGGX: [ERROR] pipe() failed.\n");
	    goto quit;
	}
	if ( (Cpid_tmp=fork()) < 0 ) {
	    fprintf(stderr,"EGGX: [ERROR] fork() failed.\n");
	    close( pfds[1] );
	    close( pfds[0] );
	    goto quit;
	}
	if ( Cpid_tmp == 0 ) {		/* 子プロセス */
	    dup2( pfds[0], 0 );
	    close( pfds[1] );
	    close( pfds[0] );
	    execvp( args[0], args );
	    fprintf(stderr,"EGGX: [ERROR] Cannot exec '%s'.\n",args[0]);
	    _exit(-1);
	}
	close(pfds[0]);
	fp = fdopen( pfds[1], "wb" );	/* 送信用 */
    }
    else if ( filter != NULL && *filter != '\0' ) {	/* netpbm */
	//int nargs;
	int fd, pfds[2];
	/* 引数 */
	//nargs = create_args(filter, &tmp1, &args);
	create_args(filter, &tmp1, &args);
	/* fileのcreate */
	fd = open( out_fname, O_WRONLY|O_CREAT|O_TRUNC,
		   S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH );
	if ( fd == -1 ) {
	    fprintf(stderr,"EGGX: [ERROR] Cannot create a file.\n");
	    goto quit;
	}
	/* コンバータを起動する */
	if ( pipe(pfds) < 0 ) {
	    fprintf(stderr,"EGGX: [ERROR] pipe() failed.\n");
	    close(fd);
	    goto quit;
	}
	if ( (Cpid_tmp=fork()) < 0 ) {
	    fprintf(stderr,"EGGX: [ERROR] fork() failed.\n");
	    close(fd);
	    close( pfds[1] );
	    close( pfds[0] );
	    goto quit;
	}
	if ( Cpid_tmp == 0 ) {		/* 子プロセス */
	    dup2( fd, 1 );
	    dup2( pfds[0], 0 );
	    close( pfds[1] );
	    close( pfds[0] );
	    execvp( args[0], args );
	    fprintf(stderr,"EGGX: [ERROR] Cannot exec '%s'.\n",args[0]);
	    _exit(-1);
	}
	close(fd);
	close(pfds[0]);
	fp = fdopen( pfds[1], "wb" );	/* 送信用 */
    } else {
	fp = fopen(out_fname, "wb");
    }

    if ( fp == NULL ) {
		fprintf(stderr,"EGGX: [ERROR] cannot open file or fd.\n");
		goto quit;
    }

    if ( ag_size_vl == NULL ) {				/* not "convert" */
	if ( out_is_pam ) {
	    fprintf(fp,"P7\n");				/* pam */
	    fprintf(fp,"WIDTH %d\n",width);
	    fprintf(fp,"HEIGHT %d\n",height);
	    if ( msk == 0 ) {
		fprintf(fp,"DEPTH 3\n");		/* r,b,g */
	    }
	    else {
		fprintf(fp,"DEPTH 4\n");		/* r,b,g,alpha */
	    }
	    fprintf(fp,"MAXVAL %d\n",depth-1);
	    fprintf(fp,"ENDHDR\n");
	}
	else {
	    fprintf(fp,"P6\n");				/* binary ppm */
	    fprintf(fp,"%d %d\n",width,height);
	    fprintf(fp,"%d\n",depth-1);
	}
    }

    if ( msk == 0 ) {
	int i;
	size_t ii;
	line_buf = (unsigned char *)_eggx_xmalloc(width * 3);
	ii = 0;
	for ( i=0 ; i < height ; i++ ) {
	    unsigned char (*dst)[3] = (unsigned char (*)[3])line_buf;
	    int j;
	    for ( j=0 ; j < width ; j++ ) {
		ii++;
		dst[j][0] = depth * buf[ii++] / 256;
		dst[j][1] = depth * buf[ii++] / 256;
		dst[j][2] = depth * buf[ii++] / 256;
	    }
	    fwrite(line_buf, 1, 3 * width, fp);
	}
    }
    else {
	int i;
	size_t ii;
	line_buf = (unsigned char *)_eggx_xmalloc(width * 4);
	ii = 0;
	for ( i=0 ; i < height ; i++ ) {
	    unsigned char (*dst)[4] = (unsigned char (*)[4])line_buf;
	    int j;
	    for ( j=0 ; j < width ; j++ ) {
		dst[j][0] = depth * buf[ii+1] / 256;
		dst[j][1] = depth * buf[ii+2] / 256;
		dst[j][2] = depth * buf[ii+3] / 256;
		dst[j][3] = depth * buf[ii+0] / 256;
		ii += 4;
	    }
	    fwrite(line_buf, 1, 4 * width, fp);
	}
    }

    fclose(fp);
    fp = NULL;

    rtv = 0;
 quit:
    if ( 0 < Cpid_tmp ) {
		if ( Pc_eggxhandlers_are_set ) {
			while ( 0 < Cpid_tmp ) {
				msleep(10);
			}
		}
		else {
			int status;
			while ( wait(&status) != Cpid_tmp );
			Cpid_tmp = 0;
		}
    }
    if ( fp != NULL ) fclose(fp);
    if ( args != NULL ) free(args);
    if ( tmp1 != NULL ) free(tmp1);
    if ( out_fname != NULL ) free(out_fname);
    if ( line_buf != NULL ) free(line_buf);
    if ( ag_size_vl != NULL ) free(ag_size_vl);
    return rtv;
}

/* マスクなし */
int eggx_putimg24( int wn, double x, double y,
		   int width, int height, unsigned char *buf )
{
    int xx, yy;
	struct color tmp = Pc[wn].fg;

    //if ( fbglobal.cpp >= 16 ) return -1;
    if ( width <= 0 ) return 0;
    if ( height <= 0 ) return 0;
    if ( buf == NULL ) return -1;

    xyconv( wn, x,y, &xx,&yy );
    if ( (Pc[wn].attributes & BOTTOM_LEFT_ORIGIN) ) yy -= height - 1;

	for(int y=0;y<height;y++) {
		for(int x=0;x<width;x++) {
			int r,g,b;
			//a = buf[4*(y*width+x)];
			r = buf[4*(y*width+x)+1];
			g = buf[4*(y*width+x)+2];
			b = buf[4*(y*width+x)+3];
			newrgbcolor(wn,r,g,b);
			lowpset(wn,x+xx,y+yy);
		}
	}
	newrgbcolor(wn,tmp.r,tmp.g,tmp.b);
    return 0;
}

/* マスク付き */
int eggx_putimg24m( int wn, double x, double y,
		    int width, int height, unsigned char *buf )
{
    int xx, yy;
    struct color tmp = Pc[wn].fg;

    //if ( fbglobal.cpp >= 16 ) return -1;
    if ( width <= 0 ) return 0;
    if ( height <= 0 ) return 0;
    if ( buf == NULL ) return -1;

    xyconv( wn, x,y, &xx,&yy );
    if ( (Pc[wn].attributes & BOTTOM_LEFT_ORIGIN) ) yy -= height - 1;

    for(int y=0;y<height;y++) {
	for(int x=0;x<width;x++) {
	    int a,r,g,b;
	    a = buf[4*(y*width+x)];
	    r = buf[4*(y*width+x)+1];
	    g = buf[4*(y*width+x)+2];
	    b = buf[4*(y*width+x)+3];
	    if(a!=0) {
		newrgbcolor(wn,r,g,b);
		lowpset(wn,x+xx,y+yy);
	    }
	}
    }
    newrgbcolor(wn,tmp.r,tmp.g,tmp.b);

    return 0;
}

/*************************
 *　メモリバッファの画像データを描画レイヤーに一括転送する
 *  引数　int wn: ウィンドウ番号
 *      　double x: コピー先x座標
 * 　　　 double y: コピー先y座標
 *        unsigned char *buf: 画像データ
 *        int width:　画像データの幅
 *        int height:　画像データの高さ
 *        int msk:　コピー時のマスク
 * 戻り値　なし
 */
int gputimage( int wn, double x, double y,
		    unsigned char *buf, int width, int height, int msk )
{
    if ( msk == 0 ) return eggx_putimg24(wn,x,y,width,height,buf);
    else return eggx_putimg24m(wn,x,y,width,height,buf);
}

/* 2点の座標 → [始点,大きさ] に変換 */
static void cnv_2p_to_pwh( int wn,
			   double xs, double ys, double xe, double ye,
			   int *r_sx, int *r_sy, int *r_width, int *r_height )
{
    int sx0, sy0, sx1, sy1, width, height;

    /* 座標変換 */
    xyconv(wn, xs,ys, &sx0,&sy0);
    xyconv(wn, xe,ye, &sx1,&sy1);

    /* 座標チェック */
    if ( Pc[wn].wszx <= sx0 ) sx0 = Pc[wn].wszx-1;
    if ( Pc[wn].wszx <= sx1 ) sx1 = Pc[wn].wszx-1;
    if ( Pc[wn].wszy <= sy0 ) sy0 = Pc[wn].wszy-1;
    if ( Pc[wn].wszy <= sy1 ) sy1 = Pc[wn].wszy-1;
    if ( sx0<0 ) sx0 = 0;
    if ( sx1<0 ) sx1 = 0;
    if ( sy0<0 ) sy0 = 0;
    if ( sy1<0 ) sy1 = 0;
    width  = abs(sx1 - sx0) + 1;
    height = abs(sy1 - sy0) + 1;
    if ( sx1 < sx0 ) sx0 = sx1;
    if ( sy1 < sy0 ) sy0 = sy1;

    *r_sx = sx0;
    *r_sy = sy0;
    *r_width = width;
    *r_height = height;

    return;
}

/*************************
 *　任意のウィンドウ・レイヤ・領域の画像データをメモリバッファに取り込む
 *  引数　int wn: ウィンドウ番号
 *      　double xs: 取り込む　スタートx座標
 * 　　　 double ys: 取り込む　スタートy座標
 *      　double xe: 取り込む　エンドx座標
 * 　　　 double ye: 取り込む　エンドy座標
 *        unsigned char *buf: 画像データ
 *        int *r_width:　取り込んだ画像データの幅
 *        int *r_height:　取り込んだ画像データの高さ
 * 戻り値　画像データのポインタ
 */
/* ウィンドゥ wn，レイヤ ly 上にある画像をバッファに取得 */
/* 返り値は画像バッファで，ユーザ側で開放が必要 */
unsigned char *ggetimage( int wn, int ly, 
			       double xs, double ys, double xe, double ye,
			       int *r_width, int *r_height )
{
    unsigned char *rt_buf = NULL;
    int sx0, sy0, width, height, i, j;

    if ( ly < 0 || MAX_NLAYER <= ly ) {
		fprintf(stderr,"Invalid layer index at ggetimage()\n");
		return NULL;
    }

    /* 2点の座標 → [始点,大きさ] に変換 */
    cnv_2p_to_pwh( wn, xs, ys, xe, ye, &sx0, &sy0, &width, &height );

    rt_buf = (unsigned char *)_eggx_xmalloc(4 * width * height);

    for ( i=0 ; i < height ; i++ ) {
		for ( j=0 ; j < width ; j++ ) {
			struct color c = lowpget(wn,ly,j+sx0,i+sy0);
			rt_buf[(i*width+j)*4 + 0] = 0xFF;
			rt_buf[(i*width+j)*4 + 1] = c.r;
			rt_buf[(i*width+j)*4 + 2] = c.g;
			rt_buf[(i*width+j)*4 + 3] = c.b;
		}
    }
	*r_width = width;
	*r_height = height;
	 return rt_buf;
}

/*************************
 *　ピクセル単位で描画レイヤをスクロールする
 *  引数　int wn: ウィンドウ番号
 *      　int x: x方向スクロール量
 * 　　　 int y: y方向スクロール量
 *       int clr: スクロール後塗りつぶすか
 * 戻り値　なし
 */
void gscroll( int wn, int scrx, int scry, int clr )
{
    int ac_x, ac_y;
    struct color tmp = Pc[wn].fg;

    ac_x = scrx;
    if ( (Pc[wn].attributes & BOTTOM_LEFT_ORIGIN) ) ac_y = -scry;
    else ac_y = scry;

    unsigned char *buf = (unsigned char *)_eggx_xmalloc(3 * Pc[wn].wszx * Pc[wn].wszy);
    //イメージ取得
    for (int y=0 ; y < Pc[wn].wszy ; y++ ) {
		for (int x=0 ; x < Pc[wn].wszx ; x++ ) {
			//printf("%d  %d %d %d\n",Pc[wn].wszx,Pc[wn].wszy,x,y);
			struct color c = lowpget(wn,Pc[wn].wly,x,y);
			buf[(y*Pc[wn].wszx+x)*3 + 0] = c.r;
			buf[(y*Pc[wn].wszx+x)*3 + 1] = c.g;
			buf[(y*Pc[wn].wszx+x)*3 + 2] = c.b;
		}
    }
    //イメージ書き出し
    for (int y=0 ; y < Pc[wn].wszy ; y++ ) {
		for (int x=0 ; x < Pc[wn].wszx ; x++ ) {
			int newx = x + ac_x;
			int newy = y + ac_y;
			if(newx < 0) newx = Pc[wn].wszx + newx;
			if(newy < 0) newy = Pc[wn].wszy + newy;
			newx = newx % Pc[wn].wszx;
			newy = newy % Pc[wn].wszy;
			newrgbcolor(wn,buf[(y*Pc[wn].wszx+x)*3 + 0]
			,buf[(y*Pc[wn].wszx+x)*3 + 1] ,buf[(y*Pc[wn].wszx+x)*3 + 2] );
			lowpset(wn,newx,newy);
		}
    }
    //背景色で塗りつぶし
    if ( clr ) {
		int xx, yy;

		xx = (0 < ac_x) ? 0 : (Pc[wn].wszx + ac_x);
		yy = (0 < ac_y) ? 0 : (Pc[wn].wszy + ac_y);
		newrgbcolor(wn,Pc[wn].bg.r,Pc[wn].bg.g,Pc[wn].bg.b);
		int x1 = xx;
		int y1 = 0;
		int x2 = xx+abs(ac_x);
		int y2 = Pc[wn].wszy;
		if(x1>x2) {
			int tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		for(int i=x1;i<=x2;i++) {
			lowline(wn,i,y1,i,y2);
		}
		x1 = 0;
		y1 = yy;
		x2 = Pc[wn].wszx;
		y2 = yy+abs(ac_y);
		if(x1>x2) {
			int tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		for(int i=x1;i<=x2;i++) {
			lowline(wn,i,y1,i,y2);
		}
    }
    free(buf);
    newrgbcolor(wn,tmp.r,tmp.g,tmp.b);
}

/*************************
 *　任意のウィンドウ・レイヤ・領域の画像データをコンバータを通してファイルに保存する
 *  引数　int wn: ウィンドウ番号
 *      　double xs: 取り込む　スタートx座標
 * 　　　 double ys: 取り込む　スタートy座標
 *      　double xe: 取り込む　エンドx座標
 *        double ye: 取り込む　エンドy座標
 *        const char *filter: コンバータの指定
 *        int depth:　減色パラメータ
 *        const char *argsformat, ...:　ファイル名
 * 戻り値　成否
 */
/* バックグラウンドで画像をsaveする(C言語) */
int gsaveimage( int wn, int ly, double xs,double ys, double xe,double ye,
		   const char *filter, int depth, const char *argsformat, ... )
{
    int r_w,r_h,rt;
    unsigned char *buf;
    char *fname;
    va_list ap;
    
    buf = ggetimage(wn,ly,xs,ys,xe,ye,&r_w,&r_h);
    va_start(ap, argsformat);
    fname = _eggx_vasprintf(argsformat,ap);
    va_end(ap);
    rt = writeimage(buf,r_w,r_h,0,filter,depth,fname);
    free(buf);
    free(fname);
    return rt;
}


