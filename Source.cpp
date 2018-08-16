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
    ip.d4 = 26;

	Nethandle = -1;
	PreparationListenNetWork( 6666 );
	while (!ProcessMessage()){
		Nethandle = GetNewAcceptNetWork() ;
        if( Nethandle != -1 ){
			StopListenNetWork();
			break;
		}
	}
	NetWorkSend(Nethandle, &x1,sizeof(x1));
	NetWorkSend(Nethandle, &y1,sizeof(y1));
	while( !ProcessMessage()){
            // �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
            DataLength = GetNetWorkDataLength( Nethandle );
            // �擾���ĂȂ���M�f�[�^�ʂ��O����Ȃ��ꍇ�̓��[�v�𔲂���
            if( DataLength != 0 ) break ;
	}
	NetWorkRecv(Nethandle, &x2,sizeof(x2));
	while( !ProcessMessage()){
            // �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
            DataLength = GetNetWorkDataLength( Nethandle );

            // �擾���ĂȂ���M�f�[�^�ʂ��O����Ȃ��ꍇ�̓��[�v�𔲂���
            if( DataLength != 0 ) break ;
	}
	NetWorkRecv(Nethandle, &y2,sizeof(y2));


    int vecX = 1,vecY = 1;//�ŏ��̉~�̈ړ��̕���


    while(!ProcessMessage()){//�G���[���o��܂ňȉ����J��Ԃ�

		NetWorkSend(Nethandle, &x1,sizeof(x1));
		NetWorkSend(Nethandle, &y1,sizeof(y1));
		NetWorkRecv(Nethandle, &x2,sizeof(x2));
		NetWorkRecv(Nethandle, &y2,sizeof(y2));

		//����ʂɉ~��`��
        DrawCircle(x1,y1,20,GetColor(0,0,255),TRUE);
        DrawCircle(x2,y2,20,GetColor(255,255,255),TRUE);

		ScreenFlip();//���ŕ`�����G��\�ɏo��
		ClearDrawScreen();//��ʂɏ����ꂽ�G������

		//�~�̒��S���W��ω�������
        x1 += 5*vecX;//x�����̑���
        y1 += 5*vecY;//y�����̑���


		if(CheckHitKey(KEY_INPUT_RIGHT)) x1 += 15;
		if(CheckHitKey(KEY_INPUT_LEFT)) x1 -= 15;

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