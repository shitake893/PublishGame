#ifndef ___LOAD_H__/#define __LOAD_H__

#include <DxLib.h>

//extern int g_count;
//extern int g_lasttime;
#define MAX_TAMA 1000
#define MAX_TEKI 500
class tekidata {//�G�A�y�уA�C�e���Ȃǂ̃f�[�^
public:
	float x, y;//x,y���W
	int living;//�����Ă��邩�̔���
	int type;//���̂��̂̃^�C�v
	int lifecount;//���̂��̂̃��C�t
int derutime;//�o������
BOOL dead;//�E���ꂽ���ǂ����̔���
float speedx, speedy;//���̂��̂̃X�s�[�h
int counter;//���Ԍv���p�i�t���[���ɂ��j�J�E���^�[
int timer;//���Ԍv���p�i�~���b�j�̃^�C�}�[
BOOL reverse;//�M�~�b�N�����p��BOOL�ϐ�
float angles;//�p�x�����߂邽�߂̕ϐ�




};



extern tekidata g_hontai[MAX_TEKI];
struct kisekihasi {//�O�՗p�̃f�[�^
	float x, y;
	BOOL living;
	int type;
	int counter;
	
};

struct kisekimoto {//�O�Ղ̌�
	kisekihasi id[1000];
	BOOL living;

};

extern kisekimoto g_kisekidata[1000];//�O�Ղ�`�����߂Ɏg���A�`��p�̃f�[�^
extern kisekimoto g_tamakisekidata[MAX_TAMA];//��̒e�A�G�ɂ��g���O�Ճf�[�^�̔z��

struct tamadata {//�e�̃f�[�^
	int x, y;
	BOOL living;
	int type;
	int lifecount;
};

extern tekidata g_mikatatama[MAX_TAMA];//�����̒e�̔z��

extern tekidata g_items[50];//�A�C�e���p�̔z��
extern tekidata g_tekitama[1000];//�G�̃f�[�^�z��


BOOL Loadtekidata();
//void graphiceffects::slasheffect;
struct graphiceffects {//�O���t�B�b�N�̃G�t�F�N�g�p�̔z��
public:
	int onoff;
	int time;

	//int animationrate;
	//int animationvolume;
	//int gazou;


	
	



};
//�悱251.����346
extern graphiceffects g_drawanimation[2];


#endif
