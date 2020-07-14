#include <math.h>
#include <fbeggx.h>

int main(int argc,char *argv[])
{
    int x,y,win,n=0;
    double xd[80*81],yd[80*81];
    char text[] = "hello,world!\nこんにちは世界\nrotation\n回転";
    double xp[10], yp[10]; // 元の頂点位置座標配列
    double th, l; // 作業用変数
    double xi,yi;
    int i;
    xi=450.0;
    yi=450.0;
    /* 星形　各頂点座標位置を決めて配列に格納 */
    for(i=0;i<10;i++) {
        th=2.0*M_PI/10.0*(double)i;
        if(i%2==0) { // 頂点番号が偶数の時は外郭
          l=300;
        } else { // 奇数の時は内郭
          l=300 * 0.4; 
        }
        xp[i]=cos(th)*l + xi;
        yp[i]=sin(th)*l + yi;
    }

    int w,h,d,winx=800,winy=800;
    ggetdisplayinfo(&d,&w,&h);                  //ディスプレイの情報を取得
    printf("color depth %d display width %d display heigth %d\n",d,w,h);
    gsetinitialattributes(ENABLE,OFFSET_RIGHT); //表示のオフセットを有効にする
    //gsetinitialbgcolor("#00003F");
    win = gopen(winx,winy);                     //グラフィックの初期化
    fbglobal.offsetx = (w-winx)/2;              //表示オフセットの設定
    fbglobal.offsety = (h-winy)/2;
    //window(win,0,0,1600,1600);
    gsetbgcolor(win,"#00001F");                 //背景色の設定
    gclr(win);                                  //グラフィック領域クリア
    newlinewidth(win,10);                       //線の太さ設定
    for(y=0;y<800;y+=10){
        for(x=0;x<800;x+=10){
            xd[n]=x;
            yd[n]=y;
            n++;
        }
    }
    for(x=0;x<800;x++) {
        int r,g,b;
        makecolor(DS9_A,0,800,x,&r,&g,&b);      //カラーバーの生成
        newrgbcolor(win,r,g,b);                 //色の設定
        pset(win,x,400);                        //点の描画
    }
    drawline(win,10,50,400,600);                //線の描画
    drawpts(win,xd,yd,n);                       //点の集合の描画
    newrgbcolor(win,255,0,255);                 //色の設定
    fillpoly(win,xp,yp,10,1);                   //塗りつぶした多角形の描画
    newrgbcolor(win,255,0,0);                   //色の設定
    fillrect(win,400,400,100,300);              //塗りつぶした四角形の描画
    newrgbcolor(win,0,255,255);                 //色の設定
    drawcirc( win, 100, 600, 90, 190 );         //楕円の描画
    circle( win, 150, 600, 90, 190 );           //楕円の描画　上と同じ
    newrgbcolor(win,64,200,200);                //色の設定
    fillcirc( win, 100, 600, 90, 190 );         //塗りつぶした楕円の描画
    newrgbcolor(win,200,200,32);                //色の変更
    drawarc(win,100,200,90,190,45,270,1);       //円弧の描画
    drawarc(win,300,200,90,190,45,270,-1);      //円弧の描画
    fillarc(win,500,200,90,190,90,-45,1);       //塗りつぶした円弧の描画
    fillarc(win,700,200,90,190,90,-45,-1);      //塗りつぶした円弧の描画
    newrgbcolor(win,255,255,255);               //色の設定
    newlinewidth(win,1);                        //線の太さの設定
    n=0;
    for(x=10;x<800;x+=50) {
        drawsym(win,x,700,40,(n++)%10+1);       //シンボルの描画
    }
    drawpoly(win,xp,yp,10);                     //多角形の描画
    newlinewidth(win,1);                        //線の太さの設定
    newrgbcolor(win,0,255,0);                   //色の設定
    drawarrow(win,50,50,750,50,0.05,0.05,126);  //矢印の描画
    drawarrow(win,50,150,250,150,0.05,0.05,126);   //矢印の描画
    drawarrow(win,100,100,250,750,20,20,27);    //矢印の描画
    newrgbcolor(win,180,180,180);               //色の設定
    //newfontset(win,"/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc");    //フォントの設定
    drawstr(win,100,100,30,45,text);            //文字の描画
    int xsz,ysz;
    unsigned char *buf = ggetimage( win,0,100,100,199,199,&xsz,&ysz); //画像の取得
    gputimage(win,600,600,buf,xsz,ysz,1);       //画像データの出力
    free(buf);
    for(int i=0;i<10;i++) {
        gscroll(win,4,6,0);                     //画面スクロール
        msleep(50);                             //時間を待つ
    }
    for(int i=0;i<10;i++) {
        gscroll(win,-4,-6,0);                   //画面スクロール
        msleep(50);                             //時間を待つ
    }
    gsetnonblock(DISABLE);                      //キー入力でブロックする
    //gsaveimage(win,0,0,0,500,799,"ppmtojpeg",256,"sample.jpg");//画像データをファイルに出力
	//ggetch();                                 //キー入力
    gclose(win);                                //グラフィック終了

    return 0;
    
}
