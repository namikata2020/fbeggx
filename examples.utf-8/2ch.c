/* egg 2ch.c -o 2ch -Wall */
/* 2chアスキーアートのサンプル */
#include <fbeggx.h>

int main()
{
    int win ;
    int x,y ;
    win = gopen(560,120) ;

    newfontset(win,"/usr/share/fonts/opentype/ipaexfont-mincho/ipaexm.ttf") ;
    x=0 ;
    y=120-32 ;

    drawstr(win,x,y, 12,0,
	    "　　 ∧＿∧　　／‾‾‾‾‾\n"
	    "　　（　´∀｀）＜　オマエモナー\n"
	    "　　（　　　　） 　＼＿＿＿＿＿\n"
	    "　　｜ ｜　|\n"
	    "　　（_＿）＿）") ;
    
    x=210 ;
    y=120-32 ;
    drawstr(win,x,y, 12,0,
	    "　　　　　　　　　　　　　　　　　　∧∧\n"
	    "　　　　　　　　　　　　∧∧　　　(,,゜Д゜)　　　　∧∧\n"
	    "　　　　　　∧∧　　　(,,゜Д゜)　 ⊂　　つ　　　 (　　,,)\n"
	    "〜′‾‾(,,゜Д゜)　　 / つつ　 〜　　|　　　　/　　|\n"
	    "　　UU‾U U　　 〜（＿＿）　　 し｀Ｊ　　〜（＿＿）") ;
    
    ggetch() ;
    gclose(win) ;
    return(0) ;
}
