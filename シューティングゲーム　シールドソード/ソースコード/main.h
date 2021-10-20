#ifndef __MAIN_H__ 
#define __MAIN_H__
#define PII 3.14159


#include<cmath>
#include <DxLib.h>
#include "gamemain.h"
#include "load.h"



//�O���[�o���ϐ�
//���Ԍv���p�ϐ�
extern int g_lasttime ;		//���O�̌v������
extern float g_frametime ;	//1���[�v�ɂ�����������
extern int g_timerstart;		//�^�C�}�[�p�ϐ�
extern int g_damagetimer; //�_���[�W���ԗp�^�C�}�[
extern int g_gametime;    //�Q�[�������Ԑݒ�
extern int g_movecount;//�Ԋu�v�Z�p
extern int g_count;
extern int g_gekihacounter; //���{�X���j��
extern float g_gekihatime; //�{�X���j����
extern float g_pi;
enum GameState {			//�Q�[�����
	GAME_TITLE, GAME_MAIN,
	GAME_CLEAR, GAME_OVER
};

enum HeroState {
	NOMAL, DAMAGED
};
extern HeroState g_herostate;

extern GameState g_gamestate;
extern int g_gametitleimg;			//�^�C�g���C���[�W
extern int g_heroimg[3];	//�摜
extern int g_tamaimg;    //��l���̋ʂ̉摜
extern int g_Boss1img;   //�{�X�̉摜
extern int g_slashimg[3];//�X���b�V���摜
extern int g_PowerSterimg;//�p���[�X�^�[�摜
extern int g_SpeedSterimg;//�X�s�[�h�X�^�[�摜

extern float g_hx, g_hy;	//���W
extern int g_herolife;     //��l�����C�t
extern int g_shotrank;    //��l���V���b�g�̋���
extern int g_hspeedrank;  //��l���̃X�s�[�h






//�{�^��
extern  BOOL g_akey_prev;	//���O��A�{�^���̏��

extern BOOL g_bkey_prev;   //���O��B�{�^���̏��
//�t�H���g
extern int g_middlefont;			//���T�C�Y�t�H���g�n���h��
extern int g_largefont;			//��T�C�Y�t�H���g�n���h��

//�֐��v���g�^�C�v�錾
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();
void InitStage();
BOOL IsAKeyTrigger(int key);
BOOL IsCKeyTrigger(int key);
BOOL IsBKeyTrigger(int key);





#endif