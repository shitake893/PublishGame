#include "load.h"
tekidata g_hontai[MAX_TEKI];//�G�̖{�̗p�̔z��
tekidata g_mikatatama[MAX_TAMA];//�����̒e�p�̔z��
tekidata g_items[50];//�A�C�e���p�̔z��
tekidata g_tekitama[1000];//�G�̒e�p�̔z��
graphiceffects g_drawanimation[2];//�V�[���h�\�[�h�̃A�j���[�V����








BOOL Loadtekidata() {
	int FileHandle;
	
	char gomi[8][256];//CSV�t�@�C����ǂݎ��̂Ɋm�F�p�̔z��
	FileHandle = FileRead_open("fileopen.csv");//���̃t�@�C������G����ǂݎ��
	if (FileHandle == NULL) return -1;

	for (int i = 0; i < 2; i++) {//�ŏ��̍s���̂Ă�
		FileRead_scanf(FileHandle, "%[^,],%[^,],%[^,],%[^,], %[^,],%s",
			gomi[0], gomi[1], gomi[2], gomi[3], gomi[4], gomi[5]); //�ǂݍ���

	}

	int n = MAX_TEKI;//�f�[�^�̐�
	for (int i = 0; i < MAX_TEKI; i++) {//�G�f�[�^���t�@�C������ǂݎ��

		FileRead_scanf(FileHandle, "%f,%f,%d,%d,%d,%d",
			&g_hontai[i].x, &g_hontai[i].y, &g_hontai[i].living, &g_hontai[i].type, &g_hontai[i].lifecount, &g_hontai[i].derutime);//�ǂݍ��݁i�ŏ��̍��W��G�̗́A�G�̃^�C�v�Ȃǁj
		g_hontai[i].dead = FALSE;//�S�Ă̓G�ŋ��ʂ̗v�f�����肷��//����͓|���ꂽ��
		g_hontai[i].speedx = 0;//X���̃X�s�[�h
		g_hontai[i].speedy = 0;//Y���̃X�s�[�h�i�����͓����̃X�N���v�g�Œ�`����Ă���j
		g_hontai[i].counter = 0;

		

}
	for (int i = 0; i < MAX_TAMA; i++) {//�����̒e������z��A������
		g_mikatatama[i].living = FALSE;
		g_mikatatama[i].x = 0;
		g_mikatatama[i].y = 0;
		g_mikatatama[i].type = 0;
		g_mikatatama[i].lifecount = 0;

	}

	for (int i = 0;i < 50; i++) {//�A�C�e�������[���邽�߂̔z��
		g_items[i].x = 0;
		g_items[i].y = 0;
		g_items[i].living = FALSE;
		g_items[i].type = 0;
		g_items[i].lifecount = 0;
		g_items[i].derutime = 0;
	}

	for (int i = 0; i < 1000; i++) {//�G�̒e�����[���邽�߂̔z��
		g_tekitama[i].x = 0;
		g_tekitama[i].y = 0;
		g_tekitama[i].living = FALSE;
		g_tekitama[i].type = 0;
		g_tekitama[i].lifecount = 0;
		g_tekitama[i].derutime = 0;
	}

	for (int i = 0; i < 1000; i++) {//�e��G�̋O�Ղ����[���邽�߂̔z��
		g_kisekidata[i].living = FALSE;
		for (int j = 0; j < 100; j++) {
			g_kisekidata[i].id[j].x = 0;
			g_kisekidata[i].id[j].y = 0;
			g_kisekidata[i].id[j].x = 0;
			g_kisekidata[i].id[j].living = FALSE;
			g_kisekidata[i].id[j].counter = 0;
		}
	}

	for (int i = 0; i < 2; i++) {//�V�[���h�\�[�h�̃A�j���[�V�����p�̔z��
		g_drawanimation[i].onoff = 0;
		g_drawanimation[i].time = 0;

	}
	
	
	return TRUE;
}