#ifndef __GAMEMAIN_H__ 
#define __GAMEMAIN_H__


#include <DxLib.h>
#include "main.h"
void GameMain();//GameMain�̊֐�
void DrawHero();//��l���̕`��
void DamageControl();//�_���[�W���󂯂�����Ɋւ��鏈��
void MapEffect();//�܂��Ղɂ��M�~�b�N�i����͖������j
void DrawEnemy();//�G�̕`��
void DrawTekitama();//�G�̒e�̕`��
//�G�̒e�̎��
void tekitama1(int i);
void tekitama2(int i);
void tekitama3(int i);
void tekitama4(int i);
void tekitama5(int i);
//�����̍U����
void Drawreflection(int key);
void Drawtama(int key);
void DrawMikatatama();
//���Ւe
void reflectedtama101(int c);
void reflectedtama102(int c);
void reflectedtama103(int c);
void reflectedtama104(int c);
void reflectedtama105(int i);
//�G�̎��
void DrawType1(int cd);
void DrawType2(int cd);
void DrawType3(int cd);
void DrawType4(int cd);
void DrawType5(int cd);
void DrawType6(int cd);
void DrawType40(int cd);
void DrawType41(int cd);
void DrawType101(int cd);
void DrawType102(int cd);


//�A�C�e���̕`��
void DrawItems();
//�K�E�Z�i�V�[���h�\�[�h�̕`�ʁj
void drawslash();


//�p�x���́A�ǔ��p�̊֐�
float directioninfosin(int x, int y);
float directioninfocos(int x, int y);
float kakudo(int x, int y);
float kakudo2(int x, int y, int aitex, int aitey);
float angle1(int x, int y);
float angle2(int, int y, int aitex, int aitey);
int nearestenemy(int cd);

//�����蔻��
BOOL AtariinfoH(int sx,int sy,int size);
BOOL AtariinfoSo(int ax, int ay, int as, int bx, int by, int bs);
BOOL atariinforey(float r, float ax, float ay, float vx, float vy);
BOOL Atariinfoougi(int ax, int ay, int bx, int by, int lengh);
int detectNearestEnemy(int c);//�ł��߂��G�̌���Ɋւ��銪�q

BOOL IsBKeyTrigger(int key);//X�{�^���𐄂��n�߂��Ƃ�������֐�
int random(int lengh);//�����_���֐�
int numberrandom(int lengh, int seed);//�������V�[�h�Ƃ��������_���֐�




extern int movecount;//�e���������ۂ̎��ԕω��̑���



//extern tekitati saisyo;
//extern int tekiscale;

void tamaset(int number, int  type, int x, int y, float speedx, float speedy);//�e���Z�b�g����


#endif
