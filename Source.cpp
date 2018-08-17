#include "DxLib.h"


IPDATA ip;
int Nethandle;
int x = 0;
int y = 0;
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
	int m_x,m_y,m_s,r;
	int mycolor,yourcolor;
	int vecX,vecY;
};

ball::ball(int x,int y,int s,int red ,int green,int blue){
	ip.d1 = 172;
	ip.d2 = 16;
	ip.d3 = 8;
	ip.d4 = 26;

	vecX = 1;
	vecY = 1;
	m_x = x;
	m_y = y;
	m_s = s;
	r = 20;
	mycolor = GetColor(red,green,blue);

	Nethandle = ConnectNetWork(ip,4444);
	if (Nethandle != -1){
		NetWorkSend(Nethandle,&x,sizeof(x));
		NetWorkSend(Nethandle,&y,sizeof(y));
		NetWorkSend(Nethandle,&mycolor,sizeof(mycolor));

		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&x,sizeof(x));
		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&y,sizeof(y));
		while(!ProcessMessage()){
			// 取得していない受信データ量が０以外のときはループから抜ける
			if( GetNetWorkDataLength( Nethandle ) > 0 ) break;
		}
		NetWorkRecv(Nethandle,&yourcolor,sizeof(yourcolor));
		CloseNetWork(Nethandle);
		Nethandle = MakeUDPSocket(6666);
	}
}
ball::~ball(){
	DeleteUDPSocket(Nethandle);
}
void ball::Reflection(){
	if((m_x + r) > width){
		vecX = -2;//x座標640超で反転
		m_x = width - r;
	}
	if((m_x - r) < 0){
		vecX = 2;//x座標0超で反転
		m_x = r;
	}
	if((m_y - r) < 0){
		vecY = 1;//y座標0超で反転
		m_y = r;
	}
	if((m_y + r) > height){
		vecY = -1;//x座標480超で反転
		m_y = height - r;
	}
}
void ball::Connect(){
	NetWorkSendUDP( Nethandle, ip, 5555, &m_x, sizeof(m_x) );
	NetWorkSendUDP( Nethandle, ip, 5555 ,&m_y, sizeof(m_y) );
	NetWorkRecvUDP( Nethandle, NULL, NULL, &x, sizeof(x), FALSE );
	NetWorkRecvUDP( Nethandle, NULL, NULL, &y, sizeof(y), FALSE );
}
void ball::Move(){
	m_x += m_s*vecX;
	m_y += m_s*vecY;
}
void ball::Input(){
	if(CheckHitKey(KEY_INPUT_RIGHT)){
		m_x += 5;
	}
	if(CheckHitKey(KEY_INPUT_LEFT)){
		m_x -= 5;
	}
}
void ball::Draw(){
	DrawCircle(m_x,m_y,r,mycolor,TRUE);
	DrawCircle(x,y,r,yourcolor,TRUE);
}
void ball::All(){
	Draw();
	ScreenFlip();
	ClearDrawScreen();
	Move();
	Input();
	Reflection();
	Connect();
}

//メイン関数 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

	//初期設定
	ChangeWindowMode(TRUE);//ウィンドウを開く
	SetGraphMode(width,height,32);//ウィンドウを指定の大きさとカラービット数にする
	if( DxLib_Init() == -1 )return -1; //dxlibの初期化に失敗した場合は-1を返してゲーム終了
	SetDrawScreen(DX_SCREEN_BACK);//裏画面で画像を描き、完成するたびに表示する形式

	
	ball* Ball = new ball(200,40,4,255,0,0);


	while(!ProcessMessage()){//エラーが出るまで以下を繰り返す
	
		
		Ball->All();

		//if(CheckHitKey(KEY_INPUT_RETURN))break;

    }//while
	delete Ball;
	DxLib_End() ;//dxlibを閉じる
	return 0 ;
}