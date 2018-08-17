#include "DxLib.h"


IPDATA ip;
int Nethandle;
int width = 640;
int height = 640;



class ball{
public:
	ball(int x,int y,int s,int red ,int green,int blue);
	~ball();
	void Reflection();
	void Connect();
	void Move();
	void Input();
	void Draw();
	void All();

private:
	int x1,y1,x2,y2,m_s,r;
	int mycolor,yourcolor;
	int vecX,vecY;
};

ball::ball(int x,int y,int s,int red ,int green,int blue){
	ip.d1 = 172;
	ip.d2 = 16;
	ip.d3 = 8;
	ip.d4 = 25;

	vecX = 2;
	vecY = 1;
	x1 = x;
	y1 = y;
	m_s = s;
	r = 20;
	mycolor = GetColor(red,green,blue);

	PreparationListenNetWork(4444);
	while( !ProcessMessage() ){
		Nethandle = GetNewAcceptNetWork() ;
		if( Nethandle != -1 ) break;
	}
	if (Nethandle != -1){
		StopListenNetWork();

		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&x2,sizeof(x2));
		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&y2,sizeof(y2));
		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&yourcolor,sizeof(yourcolor));

		//送信
		NetWorkSend(Nethandle,&x1,sizeof(x1));
		NetWorkSend(Nethandle,&y1,sizeof(y1));
		NetWorkSend(Nethandle,&mycolor,sizeof(mycolor));

		
		CloseNetWork(Nethandle);
	}
}
ball::~ball(){
	DeleteUDPSocket(Nethandle);
}

void ball::Reflection(){
	if((x1 + r) > width){
		vecX *= -1;
		x1 = width - r;
	}
	if((x1 - r) < 0){
		vecX *= -1;
		x1 = r;
	}
	if((y1 - r) < 0){
		vecY *= -1;
		y1 = r;
	}
	if((y1 + r) > height){
		vecY *= -1;
		y1 = height - r;
	}
}

void ball::Connect(){
	NetWorkSendUDP( Nethandle, ip, 6666, &x1, sizeof(x1) );
	NetWorkSendUDP( Nethandle, ip, 6666, &y1, sizeof(y1) );
	NetWorkRecvUDP( Nethandle, NULL, NULL, &x2, sizeof(x2), FALSE );
	NetWorkRecvUDP( Nethandle, NULL, NULL, &y2, sizeof(y2), FALSE );
}
void ball::Move(){
	x1 += m_s*vecX;
	y1 += m_s*vecY;
}
void ball::Input(){
	if(CheckHitKey(KEY_INPUT_RIGHT)){
		x1 += 5;
	}
	if(CheckHitKey(KEY_INPUT_LEFT)){
		x1 -= 5;
	}
}
void ball::Draw(){
	DrawCircle(x2,y2,r,yourcolor,TRUE);
	DrawCircle(x1,y1,r,mycolor,TRUE);
}
void ball::All(){
	Draw();
	Move();
	Input();
	Reflection();
	Connect();
}


class game{
public:
	game();
	~game();
	void All();
private:
	ball* Ball1;
	ball* Ball2;
};

game::game(){
	Ball1 = new ball(0,0,4,255,165,0);
	Ball2 = new ball(600,600,4,255,48,48);
	Nethandle = MakeUDPSocket(5555);
}

game::~game(){
	delete Ball1;
	delete Ball2;
	DeleteUDPSocket(Nethandle);
}
void game::All(){
	ScreenFlip();
	ClearDrawScreen();
	Ball1->All();
	Ball2->All();
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	
	//初期設定
	ChangeWindowMode(TRUE);//ウィンドウを開く
	SetGraphMode(width,height,32);//ウィンドウを指定の大きさとカラービット数にする
	if( DxLib_Init() == -1 )return -1; //dxlibの初期化に失敗した場合は-1を返してゲーム終了
	SetDrawScreen(DX_SCREEN_BACK);//裏画面で画像を描き、完成するたびに表示する形式
    
	game* Game = new game();

    while(!ProcessMessage()){

		Game->All();

		if(CheckHitKey(KEY_INPUT_RETURN))break;


    }//while
	DeleteUDPSocket(Nethandle);
	DxLib_End() ;//dxlibを閉じる
	return 0 ;
}