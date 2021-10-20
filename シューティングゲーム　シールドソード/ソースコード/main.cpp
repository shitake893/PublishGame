#include "main.h"

//�O���[�o���ϐ�
//���Ԍv���p�ϐ�
int g_lasttime = 0 ;		//���O�̌v������
float g_frametime = 0;	//1���[�v�ɂ�����������
int g_timerstart;		//�^�C�}�[�p�ϐ�
int g_damagetimer; //�_�[���[�W���ԗp�^�C�}�[
int g_gametime; //�Q�[���̎���
int g_movecount;//���o�v�Z�p
int g_count;
int g_gekihacounter;  //�{�X���j�󋵊m�F
float g_gekihatime;   //�{�X���j����
float g_pi = 3.14159; //�~�����̐ݒ�

kisekimoto g_kisekidata[1000];//�e�̋O�Ղ�`���̂Ɏg�p
kisekimoto g_tamakisekidata[MAX_TAMA];//�G�̒e�����邽�߂̔z��


//�����ݒ�
GameState g_gamestate = GAME_TITLE;//�Q�[���̏�Ԃ��^�C�g����
HeroState g_herostate = NOMAL;//��l���̏�Ԃ�ʏ��Ԃ�
int g_gametitleimg;			//�^�C�g���C���[�W
int g_heroimg[3];			//�摜
int g_tamaimg;        //��l���ʉ摜
int g_slashimg[3];//�X���b�V���摜
int g_PowerSterimg;//�p���[�X�^�[�摜
int g_SpeedSterimg;//�X�s�[�h�X�^�[�摜
int g_Boss1img;//�{�X�̉摜
float g_hx = 375, g_hy = 550;//��l���̍��W

int g_herolife;//�q�[���̃��C�t
int g_shotrank;//��l���̃V���b�g�̃����N
int g_hspeedrank;//��l���̃X�s�[�h�̃����N
							 //�{�^��
BOOL g_akey_prev;	//���O��A�{�^���̏��(Z�{�^��)
BOOL g_bkey_prev; //���O��B�{�^���̏�ԁiX�{�^���j
BOOL g_ckey_prev; //���O��C�{�^���̏�ԁiC�{�^���j


//�t�H���g
int g_middlefont;			//���T�C�Y�t�H���g�n���h��
int g_largefont;			//��T�C�Y�t�H���g�n���h��



int WINAPI WinMain(HINSTANCE h1, HINSTANCE hP, LPSTR lpC, int nC){
	//�E�B���h�E���[�h�ɂ���
	ChangeWindowMode(TRUE);
	//�E�B���h�E�T�C�Y��ύX����
	SetGraphMode(800, 600, 32);
	//DX���C�u����������
	if (DxLib_Init() == -1) return -1;
	if (Loadtekidata() == FALSE) return -1;
	

	//�摜��ǂݍ���
	g_gametitleimg = LoadGraph("media\\�^�C�g���摜.png");
	//g_heroimg = LoadGraph("media\\smp1_chara01.png");
	if(LoadDivGraph("media\\�E�҂���.png", 3, 3, 1, 50, 50, g_heroimg) == -1){
	return FALSE;
}
	g_PowerSterimg = LoadGraph("media\\PowerSter.png");
	g_SpeedSterimg = LoadGraph("media\\SpeedSter.png");
	if (LoadDivGraph("media\\�X���b�V��.png", 3, 3, 1, 500, 400, g_slashimg) == -1)return FALSE;
	g_tamaimg = LoadGraph("media\\smp2_knife.png");
	g_Boss1img = LoadGraph("media\\�{�X�̏K��.png");
	g_middlefont = CreateFontToHandle("���C���I", 42, -1, DX_FONTTYPE_ANTIALIASING);//�����̃t�H���g���̐ݒ�
	g_largefont = CreateFontToHandle("���C���I", 90, -1, DX_FONTTYPE_ANTIALIASING);

	SetDrawScreen(DX_SCREEN_BACK);//�X�N���[���̐ݒ�
	g_lasttime = GetNowCount() & INT_MAX;	//���ݎ����̋L�^
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1���[�v�ɂ����������Ԃ��v��
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
		
		ClearDrawScreen();
		//��ʕ`��֐��ɐ؂�ւ�
		switch (g_gamestate)//�Q�[���̊e���
		{
		case GAME_TITLE:
			DrawGameTitle();
			break;
		case GAME_MAIN:
			DrawGameMain();
			break;
		case GAME_CLEAR:
			DrawGameClear();
			break;
		case GAME_OVER:
			DrawGameOver();
			break;
		default:
			break;
		}
		ScreenFlip();
	}

	//�ҋ@
	WaitKey();
	//DX���C�u�����̏I������
	DxLib_End();
	return 0;
}

//�^�C�g����ʕ`��
void DrawGameTitle() {
	DrawBox(0, 0, 800, 600, GetColor(255, 255, 255), TRUE);
	DrawGraph(0, 0, g_gametitleimg, TRUE);

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);//Z�{�^���������ƊJ�n�i���C���Q�[���Ɉڍs�j
	if (IsAKeyTrigger(key) == TRUE) {
		InitStage();
		Loadtekidata();
			
	
		
			g_gamestate = GAME_MAIN;
		}
	}


//�Q�[���{�ҕ`��
void DrawGameMain(){
	
	
GameMain();
}
//�Q�[���N���A��ʕ`��
void DrawGameClear(){
	DrawBox(0, 0, 800, 600, GetColor(255, 255, 255), TRUE);
	DrawStringToHandle(100, 200, "�Q�[���N���A�I�I�I",
		GetColor(100, 255, 100), g_largefont);
	//5�b�o������^�C�g����ʂ�
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (g_lasttime - g_timerstart > 5000 || key & PAD_INPUT_A) g_gamestate = GAME_TITLE;
}
//�Q�[���I�[�o�[��ʕ`��
void DrawGameOver(){
	
	//�e�L�X�g�\��
	
	DrawStringToHandle(100, 200, "�Q�[���I�[�o�[",
		GetColor(255, 0, 0), g_largefont);
	//5�b�o������^�C�g����ʂ�
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (g_lasttime - g_timerstart > 5000 ||key &PAD_INPUT_A) g_gamestate = GAME_TITLE;
}

//�L�[�g���K�[����
BOOL IsAKeyTrigger(int key){//Z�{�^������������
	if (key & PAD_INPUT_A){
		if (g_akey_prev == FALSE){
			g_akey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_akey_prev = FALSE;
	}
	return FALSE;
}

//�L�[�g���K�[�����iB)X�{�^������������
BOOL IsBKeyTrigger(int key) {
	if (key & PAD_INPUT_B) {
		if (g_bkey_prev == FALSE) {
			g_bkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_bkey_prev = FALSE;
	}
	return FALSE;
}

BOOL IsCKeyTrigger(int key) {//C�{�^������������
	if (key & PAD_INPUT_C) {
		if (g_bkey_prev == FALSE) {
			g_bkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_bkey_prev = FALSE;
	}
	return FALSE;
}

//������
void InitStage() {
	g_herostate = DAMAGED;
	g_damagetimer = g_lasttime;
	g_gametime = g_lasttime;
	g_herolife = 10;
	g_movecount = 0;
	g_shotrank = 0;
	g_hspeedrank = 0;
	g_count = 0;
	g_gekihacounter = 0;
	g_gekihatime = 0;


};


	

