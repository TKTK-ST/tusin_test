#include "DxLib.h"


//���C���֐� 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	
	//�����ݒ�
	ChangeWindowMode(TRUE);//�E�B���h�E���J��
	SetGraphMode(600,640,32);//�E�B���h�E���w��̑傫���ƃJ���[�r�b�g���ɂ���
    if( DxLib_Init() == -1 )return -1; //dxlib�̏������Ɏ��s�����ꍇ��-1��Ԃ��ăQ�[���I��
    SetDrawScreen(DX_SCREEN_BACK);//����ʂŉ摜��`���A�������邽�тɕ\������`��
    

	//�{�[���̑傫���ƈړ��̌����̐ݒ�
	int x1 = 40,y1 = 200;//�ŏ��̉~�̒��S���W
	int x2 = 0;
	int y2 = 0;
	IPDATA ip;
	int Nethandle;
	int DataLength;

	ip.d1 = 172;
	ip.d2 = 16;
	ip.d3 = 8;
	ip.d4 = 25;

	Nethandle = MakeUDPSocket(5555);
	NetWorkSendUDP(Nethandle,ip,6666,&x1,sizeof(x1));
	NetWorkSendUDP(Nethandle,ip,6666,&y1,sizeof(y1));
	while(CheckNetWorkRecvUDP(Nethandle) != TRUE){
		if(ProcessMessage < 0) break;
	}
	NetWorkRecvUDP(Nethandle,NULL,NULL,&x2,sizeof(x2),FALSE);
	while(CheckNetWorkRecvUDP(Nethandle) != TRUE){
		if(ProcessMessage < 0) break;
	}
	NetWorkRecvUDP(Nethandle,NULL,NULL,&y2,sizeof(y2),FALSE);


	int vecX = 1,vecY = 1;//�ŏ��̉~�̈ړ��̕���


    while(!ProcessMessage()){

		NetWorkSendUDP(Nethandle,ip,6666,&x1,sizeof(x1));
		NetWorkSendUDP(Nethandle,ip,6666,&y1,sizeof(y1));
		NetWorkRecvUDP(Nethandle,NULL,NULL,&x2,sizeof(x2),FALSE);
		NetWorkRecvUDP(Nethandle,NULL,NULL,&y2,sizeof(y2),FALSE);

		DrawCircle(x1,y1,20,GetColor(0,0,255),TRUE);
		DrawCircle(x2,y2,20,GetColor(255,255,255),TRUE);

		ScreenFlip();//���ŕ`�����G��\�ɏo��
		ClearDrawScreen();//��ʂɏ����ꂽ�G������

		x1 += 5*vecX;//x�����̑���
		y1 += 5*vecY;//y�����̑���


		if(CheckHitKey(KEY_INPUT_RIGHT)) x1 += 10;
		if(CheckHitKey(KEY_INPUT_LEFT)) x1 -= 10;

		//�ǂɓ��������Ƃ��̔��˂�ݒ�
		if(x1>600)vecX = -2;//x���W640���Ŕ��]
		if(x1<0)vecX = 2;//x���W0���Ŕ��]
		if(y1<0)vecY = 1;//y���W0���Ŕ��]
		if(y1>600)vecY = -1;//x���W480���Ŕ��]


		if(CheckHitKey(KEY_INPUT_RETURN))break;


    }//���[�v�͂����܂�
	CloseNetWork(Nethandle);
	DxLib_End() ;//dxlib�����
	return 0 ;
}