#include "DxLib.h"


//メイン関数 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

	//初期設定
	ChangeWindowMode(TRUE);//ウィンドウを開く
	SetGraphMode(600,640,32);//ウィンドウを指定の大きさとカラービット数にする
	if( DxLib_Init() == -1 )return -1; //dxlibの初期化に失敗した場合は-1を返してゲーム終了
	SetDrawScreen(DX_SCREEN_BACK);//裏画面で画像を描き、完成するたびに表示する形式
    

	//ボールの大きさと移動の向きの設定
	int x1 = 200,y1 = 40;//最初の円の中心座標
	int x2 = 0;
	int y2 = 0;
	IPDATA ip;
	int Nethandle;
	int DataLength;

	ip.d1 = 172;
	ip.d2 = 16;
	ip.d3 = 8;
	ip.d4 = 26;

	Nethandle = MakeUDPSocket(6666);

	while(CheckNetWorkRecvUDP(Nethandle) != TRUE ){
		if( ProcessMessage() < 0 ) break;
	}
	NetWorkRecvUDP( Nethandle, NULL, NULL, &x2, sizeof(x2), FALSE );

	while(CheckNetWorkRecvUDP(Nethandle) != TRUE ){
	    if( ProcessMessage() < 0 ) break;
	}
	NetWorkRecvUDP( Nethandle, NULL, NULL, &y2, sizeof(y2), FALSE );

	NetWorkSendUDP(Nethandle,ip,5555,&x1,sizeof(x1));
	NetWorkSendUDP(Nethandle,ip,5555,&y1,sizeof(x1));



    int vecX = 1,vecY = 1;//最初の円の移動の方向


    while(!ProcessMessage()){//エラーが出るまで以下を繰り返す
		
		
		NetWorkSendUDP(Nethandle,ip,5555,&x1,sizeof(x1));
		NetWorkSendUDP(Nethandle,ip,5555,&y1,sizeof(x1));
		NetWorkRecvUDP( Nethandle, NULL, NULL, &x2, sizeof(x2), FALSE );
		NetWorkRecvUDP( Nethandle, NULL, NULL, &y2, sizeof(y2), FALSE );


		//裏画面に円を描く
        DrawCircle(x1,y1,20,GetColor(255,255,255),TRUE);
        DrawCircle(x2,y2,20,GetColor(0,0,255),TRUE);

		ScreenFlip();//裏で描いた絵を表に出す
		ClearDrawScreen();//画面に書かれた絵を消す

		//円の中心座標を変化させる
		x1 += 5*vecX;//x方向の速さ
		y1 += 5*vecY;//y方向の速さ

		if(CheckHitKey(KEY_INPUT_RIGHT)) x1 += 10;
		if(CheckHitKey(KEY_INPUT_LEFT)) x1 -= 10;

		//壁に当たったときの反射を設定
        if(x1>600)vecX = -2;//x座標640超で反転
        if(x1<0)vecX = 2;//x座標0超で反転
        if(y1<0)vecY = 1;//y座標0超で反転
        if(y1>600)vecY = -1;//x座標480超で反転


        if(CheckHitKey(KEY_INPUT_RETURN))break;

    }//while
	DeleteUDPSocket(Nethandle);
    DxLib_End() ;//dxlibを閉じる
    return 0 ;
}