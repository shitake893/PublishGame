#include "gamemain.h"

void GameMain() {//���������[�v�����đS�������s��
	g_count++;//�i�s�Ǘ��̃J�E���g�𑝂₷


	DamageControl();//�_���[�W�Ǘ�
	MapEffect();//�}�b�v�ɂ��G�t�F�N�g�i����͎����Ȃ��j
	DrawHero();//��l���̓���
	DrawMikatatama();//�����̒e�̓���
	DrawEnemy();//�G�̓���
	DrawItems();//�A�C�e���̓���
	DrawTekitama();//�G�̒e�̓���
	drawslash();//�K�E�Z�A�V�[���h�\�[�h�̓���

	clsDx();
	printfDx("��l�����C�t�@�@  %d", g_herolife);
	
	//Z�L�[���`�F�b�N���ĉ�ʂ�؂�ւ�
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//if (IsCKeyTrigger(key) == TRUE) {
	//	g_gamestate = GAME_OVER;
	//	g_timerstart = g_lasttime;	//�^�C�}�[�Z�b�g
	//}
	if (g_herolife <= 0) {  //���C�t���[���ɂȂ������ɃQ�[���I�[�o�[
		g_gamestate = GAME_OVER;
		g_timerstart = g_lasttime;//�^�C�}�[�Z�b�g
	}
	if (g_gekihacounter == 2 && g_lasttime - g_gekihatime  >= 2000) {
		g_timerstart = g_lasttime;//�^�C�}�[�Z�b�g
		g_gamestate = GAME_CLEAR;

	}
}

void DrawHero() {//��l���̓���̊Ǘ�
	
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	Drawtama(key);//�e����iZ�{�^���j
	Drawreflection(key);//�V�[���h�\�[�h����iX�{�^���j
	float mv = 200.0f * g_frametime +(30 * g_hspeedrank) * g_frametime; //�ړ��ʌv�Z
	if (g_hspeedrank >= 5) mv = 200.0f *g_frametime + 30 * 6 * g_frametime ;//�X�s�[�h�����N���ő�𒴂����ꍇ�i5�ȏ�j�̃X�s�[�h

	//���̍��W����
	float herox = g_hx;
	float heroy = g_hy;
	//���̍��W���ړ�������
	if (key & PAD_INPUT_UP)		heroy = g_hy - mv;
	if (key & PAD_INPUT_DOWN)   heroy = g_hy + mv;
	if (key & PAD_INPUT_LEFT)	herox = g_hx - mv;
	if (key & PAD_INPUT_RIGHT)	herox = g_hx + mv;
	//�ړ��\�ȏꏊ�Ȃ牼�̍��W��{���̍��W�Ƃ���
	if (heroy >= -25 && heroy <= 600 - 25) {
		g_hy = heroy;
	}
	if (herox >= -25 && herox <= 800 - 25) {
		g_hx = herox;
	}

	switch (g_herostate) {//��l���̊O���̕ω��A�ʏ�Ȃ�Ȃɂ��ω��Ȃ��\������A�_���[�W���󂯂����莞�ԓ_�ŁA�y�є�e���󂯕t���Ȃ�

	case NOMAL:
		DrawGraph(g_hx - 25, g_hy - 25, g_heroimg[(g_count /8)%3], TRUE);
		break;
	case DAMAGED:
		if (g_lasttime % 100 <= 50) {
			DrawGraph(g_hx - 25, g_hy - 25, g_heroimg[(g_count / 8) % 3], TRUE);
		}
		break;

	}
	
	
}

void DamageControl() {//�_���[�W��Ԃ���̉񕜂̂��߂̊֐��A�i��l���̏�Ԃ�DAMAGED�Ȃ�_���[�W���󂯕t���Ȃ��A�j
if (g_herostate == DAMAGED) {
	if (g_damagetimer < g_lasttime - 3000) {//�_���[�W���󂯂�3000�~���b�o�����Ȃ�NOMAL�̏�Ԃɖ߂�
		g_herostate = NOMAL;
	}
}
}


void MapEffect() {//�}�b�v�ɂ��e���𒲂ׂ邽�߂̊֐��A����͉������Ă��Ȃ�

//	tekitati saisyo;
//	saisyo.x = 0;
//	saisyo.y = 0;
//	saisyo.type = 1;
	//saisyo.scale = 50;

};


	//�G���o��������
void DrawEnemy() {
	

	for (int i = 0; i < MAX_TEKI; i++) {
		if(g_hontai[i].living == 0){
		if (g_hontai[i].dead == FALSE) {//�o�ꂳ����G�̏�񂩂�܂������Ă��Ȃ��A�����j����Ă��Ȃ����̂�T��
			if (g_hontai[i].derutime != 0) {//�o�����Ԃ�0�ȊO��
				for (int j = 1; j < 10; j++) {
					//�o�����ԂɂȂ������ǂ������m�F�A���������{�X��|���Ă���igekihacounter��1�ȏ�j�Ȃ炻���|�������Ԃ��瑪����s���A
					//�i����܂ł�10000�̒萔���o��܂ł̎��Ԃɒǉ����ďo�Ȃ��悤�ɂ��Ă���i1�b��10�i�j�j
					if (g_hontai[i].derutime * 100 + g_gametime < g_lasttime ||( g_gekihacounter == j      && (g_hontai[i].derutime - j * 10000) * 100 + g_gekihatime < g_lasttime)) {
						g_hontai[i].living = 1;//��������ɂ���
						g_hontai[i].counter = g_count;//���ꂽ���Ԃ��J�E���g����i�t���[������j
						g_hontai[i].timer = g_lasttime;//���ꂽ���Ԃ��J�E���g����i�b����j
						if (g_hontai[i].type == 102) {//���������X�{�X���������芪���������ɏ���������B
							g_hontai[i + 1].living = TRUE;
							g_hontai[i + 2].living = TRUE;
							g_hontai[i + 1].x = g_hontai[i].x - 30;
							g_hontai[i + 2].x = g_hontai[i].x + 30;
							g_hontai[i + 1].y = g_hontai[i].y + 30;
							g_hontai[i + 2].y = g_hontai[i].y + 30;
							g_hontai[i + 2].counter = g_count;
							g_hontai[i + 1].counter = g_count;
							g_hontai[i + 1].timer = g_lasttime;
							g_hontai[i + 2].timer = g_lasttime;
							g_hontai[i].angles = PII / 2;//�����p�x�����肳����
							}
						break;
					};
				}
				
			}
			}
		}
	}

	for (int i = 0; i < MAX_TEKI; i++) {//�e�^�C�v���Ƃ̓G�̓�������s������B
		if (g_hontai[i].living == 1) {
			switch (g_hontai[i].type) {
			case 1:
				DrawType1(i);

				break;
			case 2:
				DrawType2(i);
				break;

			case 3:
				DrawType3(i);

				break;
			case 4:



				DrawType4(i);

				break;

			case 5:
				DrawType5(i);
				break;

			case 6:
				DrawType6(i);
				break;

			case 40:
				DrawType40(i);
				break;

			case 41:
				DrawType41(i);
				break;

			case 101:
				DrawType101(i);
				break;

			case 102:
				DrawType102(i);
			}
			//�G�̍��W��X,Y�������̃X�s�[�h���ǉ�����
			g_hontai[i].x += g_hontai[i].speedx;
			g_hontai[i].y += g_hontai[i].speedy;
			if (g_hontai[i].x < -100 || g_hontai[i].x > 900 || g_hontai[i].y < -100 || g_hontai[i].y > 700) g_hontai[i].dead = TRUE;//��ʊO�ɏo���Ȃ�G���E��
			if (g_hontai[i].lifecount <= 0) {//�G�̃��C�t��0�ɂȂ�����E��
				g_hontai[i].dead = TRUE;
				if (g_hontai[i].type > 100) g_gekihacounter++ , g_gekihatime = g_lasttime ;//�|�����G���{�X��������g_gekihacounter�i�X�e�[�W�̃t�F�[�Y�I�Ȃ��̂��j�i�s������

			}
			if (g_hontai[i].dead == TRUE) g_hontai[i].living = 0;//�������E���ꂽ����Ȃ琶�������0�i�U�j�ɂ���
		}
	}
	//�p�����[�^�[�\���A����X,Y�͊e�i������邽�߂̂���
		int x = 0;
		int y = 0;
	//	for (int i = 0; i < 10; i++) {
	//		DrawFormatString(x, y, 0xFFFFFF, "%10f %10f %10d %10d %10d %10d",
	//			g_hontai[i].x, g_hontai[i].y, g_hontai[i].living, g_hontai[i].type, g_hontai[i].lifecount, g_hontai[i].derutime);// �o��

//			y += 20;
	//	}
		//DrawFormatString(600, 500, 0xFFFFF, "�p���[ %10d", g_shotrank);
		DrawFormatString(600, 520, 0xFFFFF, "��l�����C�t : %d", g_herolife);
		DrawFormatString(600, 540, 0xFFFFF, "�V���b�g�p���[ : %d", g_shotrank);
		DrawFormatString(600, 560, 0xFFFFF, "�X�s�[�h�����N : %d", g_hspeedrank);
		//DrawFormatString(600, 480, 0xFFFFF, "�J�E���g�v�� %10d", ((g_count - g_drawanimation[0].time) / 10) % 3);
		


		//�����蔻��y�ѓG�̉摜�\��
		for (int i = 0; i <= MAX_TEKI; i++) {//�S�G��������s��

			if (g_hontai[i].living == TRUE) {//�������Ă���G�݈̂ȉ��̏��������s
			//if(	g_hontai[i].type < 10){
				//�G�̐F���w��
				int boxcol = GetColor(100, 200, 0);
				if (g_hontai[i].type == 2) boxcol = GetColor(250, 0, 0);
				if (g_hontai[i].type == 4) boxcol = GetColor(150, 150, 250);
				if (g_hontai[i].type == 5) boxcol = GetColor(180, 180, 50);
				if (g_hontai[i].type == 6) boxcol = GetColor(250, 100, 100);
				DrawBox(g_hontai[i].x -25, g_hontai[i].y -25, g_hontai[i].x + 25, g_hontai[i].y + 25, boxcol, TRUE);//�G�ł���l�p��`�ʁi�F��boxcol�̐F�j
				if (g_herostate == NOMAL) {//��������l���̏�Ԃ�NOMAL�i�_���[�W���󂯂�����ł͂Ȃ��j�Ȃ�_���[�W

					BOOL tekiatari = AtariinfoH(g_hontai[i].x, g_hontai[i].y, 25);//��l���̍��W�ƓG�̈ʒu���瓖�����Ă��邩�𔻒�
					if (tekiatari == TRUE) {//�����蔻�肪�^��������_���[�W��^����
						g_herostate = DAMAGED;
						g_herolife -= 1;
						g_damagetimer = g_lasttime;
					}
					}
					
				}
			//}
		}






	}


void DrawTekitama() {//�G�̋ʂɊւ��鏈��
	int i;
	for (i = 0; i < MAX_TEKI; i++) {//�S�Ă̔z��ŏ������s��
		if (g_tekitama[i].living == TRUE) {//�����Ă���e�̂ݎ��̏������s��
			//���̍��W�Ɋe�������̃X�s�[�h���ǉ�����
			g_tekitama[i].x += g_tekitama[i].speedx;
			g_tekitama[i].y += g_tekitama[i].speedy;
			
			if (g_tekitama[i].x < -100 || g_tekitama[i].x > 900 || g_tekitama[i].y < -100 || g_tekitama[i].y > 700) g_tekitama[i].living = FALSE;//��ʊO�ɏo�������
			//if (g_tekitama[i].timer < g_lasttime - 10000) g_tekitama[i].living = FALSE;

			switch (g_tekitama[i].type) {//�e�^�C�v���Ƃɏ������s��
			case 1:
				tekitama1(i);
							break;

						case 2:
							tekitama2(i);
                          	break;
						case 3:
							tekitama3(i);
						
							break;
						case 4:
							tekitama4(i);

							break;

						case 5:
							tekitama5(i);
							break;
						}
					
					
		}
	}
}
		
	
void tekitama1(int i) {//�e�̃p�^�[��1�A�����^����ꂽ�����ɒ��i����
	int boxcol = GetColor(250, 250, 170);
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 12, boxcol, TRUE);
	//�ȉ������蔻�菈��
	if (g_herostate == NOMAL) {

		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}
}

void tekitama2(int i) {//�e�̃p�^�[��2�A�ǔ��e�i���F�Œ��˕Ԃ��Ȃ��A�O�Ղ�����j
	int boxcol = GetColor(200, 100, 250);
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);

	if (g_herostate == NOMAL) {
		//�����蔻�菈��
		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}

	//for (int i = 0; i < 1000; +i++) {   //�X�����͉����Ă�
		//if (g_kisekidata[i].living == FALSE){
	g_kisekidata[i].living = TRUE;//���̒e�p�̋O�Ղ̃f�[�^�̔z��̐�����TRUE�ɂ���
	//int kisekinumber = i;
//	break;

//		}
	//	}       //�����܂Ŗ��͉����Ă�

	//X���AY���̎�l���Ƃ̍��W�̍������߁A��������Ɏ�l���Ƃ̋��������߂�
	int dex = g_hx - g_tekitama[i].x;
	int dey = g_hy - g_tekitama[i].y;
	float dis = sqrt(dex * dex + dey * dey);

	//��l���̍��W�ƒe�̍��W��p���āA���̎O�p������߂�
	float sinr = dey / dis;
	float cosr = dex / dis;
	float hikiyosehi = 0.5;
	if (g_count - g_tekitama[i].counter > 200) hikiyosehi = 0;//������x�̎��Ԃ��o�ƒǔ����C������
	//���̒e�̃X�s�[�h�ɎO�p��ŋ��߂����l*�����񂹗́ihikiyosehi�j��ǉ�����
	g_tekitama[i].speedx += hikiyosehi * cosr;
	g_tekitama[i].speedy += hikiyosehi * sinr;
	//�������e���W�̃X�s�[�h��5�𒴂����炻���ŃX�g�b�v����
	if (g_tekitama[i].speedx > 5) g_tekitama[i].speedx = 5;
	if (g_tekitama[i].speedy > 5) g_tekitama[i].speedy = 5;



	//�����������̒e�ƂԂ�������݂��ɏ��ł���
	for (int a = 0; a < MAX_TAMA; a++) {
		if (g_mikatatama[a].living == TRUE) {
			BOOL hantei = AtariinfoSo(g_tekitama[i].x, g_tekitama[i].y, 15, g_mikatatama[a].x, g_mikatatama[a].y, 10);
			if (hantei == TRUE) {
				g_tekitama[i].living = FALSE;
				g_mikatatama[a].living = FALSE;
			}
		}
	}


	//if (g_count % 3 == 0) {
	for (int j = 0; j < 100; j++) {//�O�Ղ�`��kisekidata�Ŋe�e�Ɋ��蓖�Đ��ꂽ�O�Ղ̔z��A���̒��Ɏ��ۂɕ`���O�՗p�̔z�񂪂���B
		if (g_tamakisekidata[i].id[j].living == FALSE) {
			g_tamakisekidata[i].id[j].living = TRUE;
			g_tamakisekidata[i].id[j].x = g_tekitama[i].x;
			g_tamakisekidata[i].id[j].y = g_tekitama[i].y;
			g_tamakisekidata[i].id[j].counter = g_count;
			break;
		}
	}


	//}
	for (int k = 0; k < MAX_TAMA; k++) {//������ŋO�Ղŕ`�����~�����X�ɏ������Ȃ�悤�ɂ���
		if (g_tamakisekidata[i].id[k].living == TRUE) {
			int konokiseki = GetColor(200, 100, 250);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//���̎��̐��i7�j�܂ł̃J�E���g�Ȃ�傫���͈��A��������ȍ~���Ə��X�ɏ������Ȃ�
			if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
				ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 8);
			}
			DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//�����Ŏ��ۂɋO�Ղ�`�ʂ���B
			if (g_count - g_tamakisekidata[i].id[k].counter > 15) g_tamakisekidata[i].id[k].living = FALSE;
		}
	}

	if (g_tekitama[i].living == FALSE) {//�e�{�̂��������Ȃ�O�Ղ���Ăɏ���
		g_tamakisekidata[i].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[i].id[b].living = FALSE;
		}
	}
}

void tekitama3(int i) {//�e�̃p�^�[��3�A��莞�Ԃ��Ǝ�l���̕����ɕ����]�����Ă���B
	int boxcol = GetColor(150, 250, 150);//�F�̎w��
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);//�e�̕`��

	g_kisekidata[i].living = TRUE;//�O�՗p�̊֐��̐�����TRUE�ɂ���

	if (g_herostate == NOMAL) {//�_���[�W����

		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}





	if (g_lasttime - g_tekitama[i].timer > 300) {//��莞�ԁi300�������Ȃ�reverse�g���K�[��ON�ɂ��A�����]�����s���i����reverse�g���K�[����xON�ɂȂ�Ƃ����������Ȃ��B�j�j
		if (g_tekitama[i].reverse == FALSE) {
			g_tekitama[i].speedx = 5 * cos(kakudo(g_tekitama[i].x, g_tekitama[i].y));
			g_tekitama[i].speedy = 5 * sin(kakudo(g_tekitama[i].x, g_tekitama[i].y));
			g_tekitama[i].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//2�t���[�����ƂɋO�Ղ�ǉ�����
		for (int j = 0; j < 100; j++) {
			if (g_tamakisekidata[i].id[j].living == FALSE) {
				g_tamakisekidata[i].id[j].living = TRUE;
				g_tamakisekidata[i].id[j].x = g_tekitama[i].x;
				g_tamakisekidata[i].id[j].y = g_tekitama[i].y;
				g_tamakisekidata[i].id[j].counter = g_count;
				break;
			}
		}


	}
		for (int k = 0; k < MAX_TAMA; k++) {//�O�Ղ̕`�ʂ̂��߂̕����A��莞�Ԃ��ƋO�Ղ��������Ȃ������
			if (g_tamakisekidata[i].id[k].living == TRUE) {
				int konokiseki = GetColor(160, 230, 160);
				int ookisa = 0;
				if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//�O�Ղ��������Ă���7�t���[���ڂ܂ł͋O�Ղ͂��̂܂�
				if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
					ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 16);//����ȍ~�͏��X�ɏ������Ȃ�
					konokiseki = GetColor(180, 200, 180);//�����7�t���[������͋O�Ղ̐F��ω�������
				}
				DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//�O�Ղ����ۂɕ`��
				if (g_count - g_tamakisekidata[i].id[k].counter > 23) g_tamakisekidata[i].id[k].living = FALSE;//23�t���[���ȍ~�ŋO�Ղ̐�����FALSE�ɂ���
			}
		}

		if (g_tekitama[i].living == FALSE) {//�e�{�̂�������΋O�Ղ������ɏ����B
			g_tamakisekidata[i].living = FALSE;
			for (int b = 0; b < MAX_TAMA; b++) {
				g_tamakisekidata[i].id[b].living = FALSE;
			}
		}
}

void tekitama4(int i) {//�e�̃p�^�[��4�A��������̈������ǔ��e
	int boxcol = GetColor(150, 250, 150);//�F�w��
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);//�e��`��

	g_kisekidata[i].living = TRUE;

	if (g_herostate == NOMAL) {

		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}





	if (g_lasttime - g_tekitama[i].timer > 300) {//��莞�ԂɂȂ������l�������֕����]������
		if (g_tekitama[i].reverse == FALSE) {
			g_tekitama[i].speedx = 5 * cos(kakudo2(g_tekitama[i].x, g_tekitama[i].y, g_hx + (80 - random(160)), g_hy) );//����������A�i�����ɂ�����e���̗v�f���猈��jrandom�ŗ���������
			g_tekitama[i].speedy = 5 * sin(kakudo2(g_tekitama[i].x, g_tekitama[i].y, g_hx + (80 - random(160)), g_hy) );//�c��������
			g_tekitama[i].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//�O�Ղ̔z��ɏ���ǉ�����
		for (int j = 0; j < 100; j++) {
			if (g_tamakisekidata[i].id[j].living == FALSE) {
				g_tamakisekidata[i].id[j].living = TRUE;
				g_tamakisekidata[i].id[j].x = g_tekitama[i].x;
				g_tamakisekidata[i].id[j].y = g_tekitama[i].y;
				g_tamakisekidata[i].id[j].counter = g_count;
				break;
			}
		}


	}
	for (int k = 0; k < MAX_TAMA; k++) {//�O�Ղ����ۂɕ`�ʂ���
		if (g_tamakisekidata[i].id[k].living == TRUE) {
			int konokiseki = GetColor(160, 230, 160);//�O�Ղ̐F�����
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//7�t���[���ڂ܂ł͑傫���ɕω��Ȃ�
			if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
				ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 16);//7�t���[������͏��X�ɏ��������Ă���
				konokiseki = GetColor(180, 200, 180);//7�t���[������͐F��ω�������B
			}
			DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//�����Ŏ��ۂɕ`�ʂ�����
			if (g_count - g_tamakisekidata[i].id[k].counter > 23) g_tamakisekidata[i].id[k].living = FALSE;//��莞�ԂŐ�����FALSE��
		}
	}

	if (g_tekitama[i].living == FALSE) {//�e�{�̂���������O�Ղ�����
		g_tamakisekidata[i].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[i].id[b].living = FALSE;
		}
	}
}

void tekitama5(int i) {//�e�̃p�^�[��5�A���o�E���h�e
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 12, GetColor(200,200,150), TRUE);
	if (g_tekitama[i].reverse == FALSE) {//reverse�������A�܂��o�E���h���Ă��Ȃ����̂̂݃o�E���h������B
		if (g_tekitama[i].x >= 800 || g_tekitama[i].x <= 0){//�������e��x���W��800�𒴂������AX����0�ȉ��Ȃ�X�������̃X�s�[�h�𔽓]������
			g_tekitama[i].speedx *=  -1;
			g_tekitama[i].reverse = TRUE;//reverse��TRUE�ɂ��A����ȏ�o�E���h���Ȃ��悤�ɂ���
			}
		if (g_tekitama[i].y >= 600 || g_tekitama[i].y <= 0) {//������Y����600�ȏ�A�܂���0�ȉ��Ȃ��Y�������̃X�s�[�h�𔽓]������B
			g_tekitama[i].speedy *= -1;
			g_tekitama[i].reverse = TRUE;//reverse��TRUE�ɂ��A����ȏ�o�E���h���Ȃ��悤�ɂ���
		}
	}
if (g_herostate == NOMAL) {//��l���Ƃ̓����蔻��̏���
		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}
}


void DrawType1(int cd) {//�G�̃^�C�v1�A�����΂߂Ɉړ����邾��
	g_hontai[cd].speedx = 2.0f;
	g_hontai[cd].speedy = 1.0f;
}

void DrawType2(int cd) {//�G�̃^�C�v2���̕��ɋ����ړ����邾��
	g_hontai[cd].speedx = 1.0f;
	g_hontai[cd].speedy = 2.0f;
}

void DrawType3(int cd) {//�G�̃^�C�v3�A��1.5�b��ɒe�𔭎˂���
	 

	if (g_hontai[cd].timer + 1500 > g_lasttime) {//1.5�b�O�܂ł͉��ɍ~�葱����
		g_hontai[cd].speedy = 2.0f;
	}
	
		if ((g_hontai[cd].timer + 1500 <= g_lasttime) && (g_hontai[cd].timer  + 2500 > g_lasttime)) {//�o���1.5~2.5�b�̊Ԃ͒e�𔭎˂���
			g_hontai[cd].speedy = 0;//���x��0��
			g_hontai[cd].counter = g_count;//�J�E���g��ݒ�
			g_hontai[cd].counter %= 18;//18�t���[�����݂ɂ���
			
			if(g_hontai[cd].counter == 0 ){//����18�t���[���̊ԂɈ��e�����
				for (int i = 0; i < 1000; i++) {//�����ɒe�����
					if (g_tekitama[i].living == FALSE) {
						g_tekitama[i].living = TRUE;
						g_tekitama[i].type = 1;
						g_tekitama[i].x = g_hontai[cd].x;
						g_tekitama[i].y = g_hontai[cd].y;
						g_tekitama[i].speedx = -0.5f;
						g_tekitama[i].speedy = 1.0f;
						break;
					}
				}
				for (int i = 0; i < 1000; i++) {//�^���ɒe�����
					if (g_tekitama[i].living == FALSE) {
						g_tekitama[i].living = TRUE;
						g_tekitama[i].type = 1;
						g_tekitama[i].x = g_hontai[cd].x;
						g_tekitama[i].y = g_hontai[cd].y;
						g_tekitama[i].speedx = 0;
						g_tekitama[i].speedy = 1.2f;
						break;
					}
				}
				for (int i = 0; i < 1000; i++) {//�E���ɒe�����
					if (g_tekitama[i].living == FALSE) {
						g_tekitama[i].living = TRUE;
						g_tekitama[i].type = 1;
						g_tekitama[i].x = g_hontai[cd].x;
						g_tekitama[i].y = g_hontai[cd].y;
						g_tekitama[i].speedx = 0.5f;
						g_tekitama[i].speedy = 1.0f;
						break;
					}
				}
				}
			}
	
		if (g_hontai[cd].timer + 2500 <= g_lasttime) {//2.5�b��ȏ�ł͏�ɋA���Ă���
			g_hontai[cd].speedy = -2;
		}
}


void DrawType4(int cd) {//�G�̃^�C�v4������ɓːi���Ă���G
	//��l���Ƃ�X,Y���ɂ����鋗�������߂�
	int dex = g_hx - g_hontai[cd].x;
	int dey = g_hy - g_hontai[cd].y;
	float dis = sqrt( dex * dex + dey * dey);
	//�O�p�֐����g���A�����̂����̊e���̗v�f�����߂�
	float sinr = dey / dis;
	float cosr = dex / dis;
	float hikiyosehi = 0.4;
	if (g_count - g_hontai[cd].counter > 300) hikiyosehi = 0;
	//�e���̗v�f�̔䕪�A���x��ǉ�����
	g_hontai[cd].speedx += hikiyosehi * cosr;
	g_hontai[cd].speedy += hikiyosehi * sinr;
	//�e���̑��x�̏����ݒ�
	if (g_hontai[cd].speedx > 4) g_hontai[cd].speedx = 4;
	if (g_hontai[cd].speedy > 4) g_hontai[cd].speedy = 4;
	
	//�O�Ղ�����
	if (g_count%3 == 0){
		for (int j = 0; j < 100; j++) {
			if (g_kisekidata[cd].id[j].living == FALSE) {
				g_kisekidata[cd].id[j].living = TRUE;
				g_kisekidata[cd].id[j].x = g_hontai[cd].x;
				g_kisekidata[cd].id[j].y = g_hontai[cd].y;
				g_kisekidata[cd].id[j].counter = g_count;
				break;
			}
		}
		
		
	}
	for (int i = 0; i < 1000; i++) {//�O�Ղ̕`�ʐ���
		if (g_kisekidata[cd].id[i].living == TRUE) {
			int boxcol = GetColor(250, 250, 170);
			int ookisa = 0;
			if ((g_count - g_kisekidata[cd].id[i].counter) <= 7) ookisa = 10;
			if ((g_count - g_kisekidata[cd].id[i].counter) > 7) {
				ookisa = 10 - ((g_count - g_kisekidata[cd].id[i].counter - 7) * 10 /8);
			}
			DrawCircle(g_kisekidata[cd].id[i].x, g_kisekidata[cd].id[i].y, ookisa, boxcol, TRUE);
			if (g_count - g_kisekidata[cd].id[i].counter > 15) g_kisekidata[cd].id[i].living = FALSE;
		}
	}
	
	if (g_hontai[cd].living == FALSE) {//�{�̂�������΋O�Ղ�������悤�ɂ���
		g_kisekidata[cd].living = FALSE;
	}

}

void DrawType5(int cd) {//�G�̃^�C�v5���̒e����G
	//���x�ݒ�
	g_hontai[cd].speedx = 0;
	g_hontai[cd].speedy = 3;
	int zerocount = g_count - g_hontai[cd].counter;//�����ł��̂��̓G���o�Ă������Ԃ�0�Ƃ���J�E���^�[������
	zerocount %= 60;//�����悻60�J�E���g�Ɉ��e�����
	if (zerocount == 0) {
		for (int i = 0; i < 1000; i++) {
			if (g_tekitama[i].living == FALSE) {
				g_tekitama[i].living = TRUE;
				g_tekitama[i].type = 2;
				g_tekitama[i].x = g_hontai[cd].x;
				g_tekitama[i].y = g_hontai[cd].y;
				g_tekitama[i].speedx = 0;
				g_tekitama[i].speedy = -3.2f;
				g_tekitama[i].counter = g_count;
				break;
			}
		}
	}

}

void DrawType6(int cd) {//�G�̃^�C�v6��莞�Ԏ�l���̎�����͂݁A�Ō�Ɉ�����ǔ��e�i�ΐF�̒e�j�����
	//DrawFormatString(600, 580, 0xFFFFF, "�o������ %10d", g_hontai[cd].timer);
	if (g_count - g_hontai[cd].counter < 50)g_hontai[cd].speedy = 3;//�ŏ��̓��͉��ɐi��ł���
	//��l���ւ̕��p���߂邽�߂�sin�Acos�̒l�����߂�
	float sin = directioninfosin(g_hontai[cd].x, g_hontai[cd].y);
	float cos = directioninfocos(g_hontai[cd].x, g_hontai[cd].y);
	//��l���Ƃ̋��������߂�
	float distanse = sqrt((g_hontai[cd].x - g_hx) * (g_hontai[cd].x - g_hx) + (g_hontai[cd].y - g_hy) * (g_hontai[cd].y - g_hy));
	//�o�ꂵ�Ă���̎��Ԃ�4500�~���b�o�����Ȃ��l�����痣���
	if (g_lasttime > 4500 + g_hontai[cd].timer) {
		g_hontai[cd].speedy -= sin * 0.3;
		g_hontai[cd].speedx -= cos * 0.3;
		if (g_lasttime > 5000 + g_hontai[cd].timer) {//5000�~���b�ȏ�ŗ΋��������
			if (g_hontai[cd].reverse == FALSE) {
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {
						g_tekitama[i].living = TRUE;//�����Œe������
						g_tekitama[i].type = 3;
						g_tekitama[i].x = g_hontai[cd].x;
						g_tekitama[i].y = g_hontai[cd].y;
						g_tekitama[i].speedx = 0;
						g_tekitama[i].speedy = -3.2f;
						g_tekitama[i].counter = g_count;
						g_tekitama[i].timer = g_lasttime;
						g_hontai[cd].reverse = TRUE;//�Ō�ɒe�����������Ƃ�reverse�ŋL�^
						break;
					}
				}
			}
		}
	}
	else {//�܂�4500�~���b�o���Ă��Ȃ��ꍇ�͎�l���Ɉ��̋����������ǂ�������
		if (distanse > 150) {//��l���Ƃ̋�����150��葽���ꍇ�͒ǂ�������
			g_hontai[cd].speedy += sin * 0.3;
			g_hontai[cd].speedx += cos * 0.3;
			//���x����̐ݒ�
			if (g_hontai[cd].speedy >= 4) g_hontai[cd].speedy = 4;
			if (g_hontai[cd].speedy <= -4) g_hontai[cd].speedy = -4;
			if (g_hontai[cd].speedx >= 4) g_hontai[cd].speedx = 4;
			if (g_hontai[cd].speedy <= -4) g_hontai[cd].speedy = -4;
		}
		if (distanse >= 110 && distanse < 160) {//������140~160�̊Ԃł̓X�s�[�h��}����悤�ɂ���
			if (g_hontai[cd].speedx >= 0)g_hontai[cd].speedx -= 0.2;
			else {
				g_hontai[cd].speedx += 0.2;
			}
			if (g_hontai[cd].speedy >= 0) g_hontai[cd].speedy -= 0.2;
			else {
				g_hontai[cd].speedy += 0.2;
			}
			//if (abs(g_hontai[cd].speedx) < 0.4) g_hontai[cd].speedx = 0;
			//if (abs(g_hontai[cd].speedy) < 0.4) g_hontai[cd].speedy = 0;
		}
		if (distanse <= 110) {//��l���Ƃ̋�����110�ȉ��ł���Ύ�l�����痣���
			g_hontai[cd].speedy -= sin * 0.5;
			g_hontai[cd].speedx -= cos * 0.5;
		}
		if((g_count - g_hontai[cd].counter) % 150 == 0){//150�t���[�����Ƃɒʏ�e�����l���Ɍ����ĕ���
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 1;
					g_tekitama[i].x = g_hontai[cd].x;
					g_tekitama[i].y = g_hontai[cd].y;
					g_tekitama[i].speedx = 0.5 * cos;
					g_tekitama[i].speedy = 0.5 * sin;
					break;
				}
			}
		}
		
	}

}

void DrawType101(int cd) {//���{�X�A
	//�ŏ���50�t���[���͐^���ɍ~���
	if (g_count - g_hontai[cd].counter < 50) g_hontai[cd].speedy = 1.5, g_hontai[cd].speedx = 0 ;

		if (g_hontai[cd].y < 120) g_hontai[cd].speedy += 0.2;//y���W��120�����Ȃ瑬�x�����ɑ��₷
		else {
			if (g_hontai[cd].y < 130) g_hontai[cd].speedy += 0.05;//�����W��130�����Ȃ瑬�x�����ɏ������₷
			else if (g_hontai[cd].y > 140) g_hontai[cd].speedy -= 0.05;//y���W���P�S�O����Ȃ瑬�x����ɏ������₷
			else g_hontai[cd].speedy -= 0.2;//y���W��140�ȏ�Ȃ瑬�x����ɑ��₷
		}

		//���x����̐ݒ�
		if (g_hontai[cd].speedy > 2) g_hontai[cd].speedy = 2;
		if (g_hontai[cd].speedy < -2) g_hontai[cd].speedy = -2;
		
		//reverse���g���A�G��x���W�ɂ�����ړ����������߂�
		if (g_hontai[cd].x < 500 && g_hontai[cd].reverse == TRUE) g_hontai[cd].speedx += 0.2;//������E�ɍs���Ƃ�
		if (g_hontai[cd].x >= 500) g_hontai[cd].reverse = FALSE;//�E�[�ɗ�����reverse�ňړ������𔽓]������
		if (g_hontai[cd].speedx > 1.5) g_hontai[cd].speedx = 1.5;//���x����̐ݒ�
		if (g_hontai[cd].x > 50 && g_hontai[cd].reverse == FALSE) g_hontai[cd].speedx -= 0.2;//�E���獶�ɍs���Ƃ�
		if(g_hontai[cd].x <= 50) g_hontai[cd].reverse = TRUE;//���[�ɗ�����Areverse�𔽓]�A�ړ������𔽓]������
		if (g_hontai[cd].speedx < -1.5) g_hontai[cd].speedx = -1.5;//���x����̐ݒ�

		//�J�E���^�[�̐ݒ�i���[�J���j
		

		//�����A���A�E�A�E���̏��ɋL�q�A�e���ꂼ�ꓯ���ɁA15�t���[���Ɉ�x�ʏ�e�����
		if (g_count % 15 == 0) {
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 1;
					g_tekitama[i].x = g_hontai[cd].x;
					g_tekitama[i].y = g_hontai[cd].y;
					g_tekitama[i].speedx = -1.5f;
					g_tekitama[i].speedy = 3.0f;
					break;
				}
			}
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 1;
					g_tekitama[i].x = g_hontai[cd].x;
					g_tekitama[i].y = g_hontai[cd].y;
					g_tekitama[i].speedx = -2.5f;
					g_tekitama[i].speedy = 2.5f;
					break;
				}
			}
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 1;
					g_tekitama[i].x = g_hontai[cd].x;
					g_tekitama[i].y = g_hontai[cd].y;
					g_tekitama[i].speedx = 1.5f;
					g_tekitama[i].speedy = 3.0f;
					break;
				}
			}
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 1;
					g_tekitama[i].x = g_hontai[cd].x;
					g_tekitama[i].y = g_hontai[cd].y;
					g_tekitama[i].speedx = 2.5f;
					g_tekitama[i].speedy = 2.5f;
					break;
				}
			}
		}
	


	if (g_count - g_hontai[cd].counter > 500) {//���̓G�̃J�E���^�[��500�t���[���𒴂����率�̒ǔ��e�����
		
		if (g_count % 60 == 0) {
			//�ǔ��e��2�����ɕ���
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 2;
					g_tekitama[i].x = g_hontai[cd].x -20;
					g_tekitama[i].y = g_hontai[cd].y +20;
					g_tekitama[i].speedx = -3.2;
					g_tekitama[i].speedy = -3.2f;
					g_tekitama[i].counter = g_count;
					break;
				}
			}
			for (int i = 0; i < 1000; i++) {
				if (g_tekitama[i].living == FALSE) {
					g_tekitama[i].living = TRUE;
					g_tekitama[i].type = 2;
					g_tekitama[i].x = g_hontai[cd].x + 20;
					g_tekitama[i].y = g_hontai[cd].y + 20;
					g_tekitama[i].speedx = 3.2;
					g_tekitama[i].speedy = -3.2f;
					g_tekitama[i].counter = g_count;
					break;
				}
			}
		}
	}
}

void DrawType102(int cd) {//�Ō�̃{�X
	
	float PI = 3.141592;//�~�����̐ݒ�
	static float konoangle = PI / 2;
	static float anglespeed = 0;//��]�X�s�[�h�i�������j
	static BOOL tackle = 0;//�^�b�N���������̔���i�������j
	static BOOL bumping = 0;//�^�b�N�����ɕǂɂԂ��������̔���
	static int kyorix = 0;//X���̎�l���Ƃ̋����i�ꉞ�������j
	static int kyoriy = 0;//Y���̎�l���Ƃ̋���
	static int yattaka = 0;//0,2,2�Ƃӂ����Ƃɕς��Ƃ���@�Βe����^�C�~���O�����肷��
	static BOOL keepstate = 1;//�{�X�̍s���T�C�N���̍ŏ��Ɏ�芪�����������ǂ���
	static int arufa = 0;


	if (g_hontai[cd].angles - PI / 2 >= 2 * PI) g_hontai[cd].angles -= 2 * PI;//�p�x��360�𒴂����Ƃ��͂O�ɖ߂�
	//(g_hontai[cd + 1].living == TRUE ||g_hontai[cd + 2].living == TRUE) ||
	//�{�X�̍s���p�^�[���̃T�C�N���p�̊֐�
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;
	int count = (g_count - g_hontai[cd].counter);//���̃{�X�̃J�E���^�[��ݒ�
	if ((g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) || keepstate == 1) {
		if (g_hontai[cd].timer + 1000 > g_lasttime) {//�ŏ���1000�~���b�̍s���A�i���ɍ~���j
			g_hontai[cd].speedx = 0;
			g_hontai[cd].speedy = 2;
		}
		else {
			if (cycle < 1000) {//�s��1�A1000�~���b�A3��L�͈͂ɓn��ʏ�e�����
				if (g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) keepstate = 1;
				g_hontai[cd].speedx = 0;
				g_hontai[cd].speedy = 0;
				g_hontai[cd].reverse = TRUE;
				if (count % 20 == 0) {//20�t���[�����Ƃɒe�����
					for (int i = 0; i < 10; i++) {//10�̒e�����
						for (int j = 0; j < MAX_TAMA; j++) {
							if (g_tekitama[j].living == FALSE) {
								float bure = (-0.5 + random(5)* 0.2);//�e�̃u��������
								tamaset(j, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2,
									3 * -sin(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2);//�e������֐��A
							
								break;
							}
						}
					}
				}
			}

			if (cycle < 3000 && cycle >= 1000) {//3000�ȉ�1000�ȏ�̒i�K
				g_hontai[cd + 1].reverse = TRUE;//��芪����reverse���O���A���R�ɍs��������
				g_hontai[cd + 2].reverse = TRUE;
			}
			if (cycle > 2000 && cycle <= 8000) {
				
				//2000~8000�܂łŁA�΋������
				if (cycle - 2000 - yattaka * 250 >= 0 || (cycle - 2000 - (yattaka - 1) * 250 - 100 >= 0 && yattaka % 2 == 1)) {
					arufa++;
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 4;
							int bata = 1;
							if (yattaka % 2 == 1)bata = -1;//yattaka�Œe����������𐔂��Abata�̒l������A

							g_tekitama[j].speedx = (3 - arufa / 5) * bata;//bata�����Ȃ獶�����A���Ȃ�E�����ɕ���
							g_tekitama[j].speedy = 4 - arufa/2;//arufa��p���ď��X�ɒe���̉��v�f�����炵�Ă���
							g_tekitama[j].reverse = FALSE;//��������e�̏�����
							g_tekitama[j].timer = g_lasttime;
							yattaka++;
							break;
						}
					}


				}
			}//�p�x�����傫����Ώ������A��������Α傫������
			if (cycle > 8000 && cycle <= 13000) {//8000�����13000�ȉ��ł�����ɑ_�����߂��i�������̕����������j
				arufa = 0;

				yattaka = 0;//yattaka�̍ď�����
				anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;//������̌����ɍ��킹��gnglespeed��ݒ肷��
				g_hontai[cd].angles += anglespeed;//angle��anglespeed��ǉ�
			}
			if (cycle > 13000 && cycle <= 16000) {//13000����16000�܂ŁA��l���Ɍ����ēːi����
				if (tackle == 0) {//�܂��ːi���Ă��Ȃ��Ȃ�Γːi������
					//�X�s�[�h��������Ɍ����Đݒ肷��
					g_hontai[cd].speedx = directioninfocos(g_hontai[cd].x, g_hontai[cd].y) * 10;
					g_hontai[cd].speedy = directioninfosin(g_hontai[cd].x, g_hontai[cd].y) * 10;
					tackle = 1;//�ːi�����Ƃ������������
				}
				if (g_hontai[cd].x > 700 || g_hontai[cd].x < 30 || g_hontai[cd].y > 550 || g_hontai[cd].y < 30) {//��ʒ[�ɗ����炻���Ŏ~�܂�
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					bumping = 1;//�Ԃ������Ƃ���������o��

				}
				if (bumping == 1 && cycle <= 16000) {//�ǂɂԂ���A�T�C�N��16000�ȉ�
					kyorix = g_hontai[cd].x - 400;//�{������ׂ��ʒu�Ƃ̍����L�^
					kyoriy = g_hontai[cd].y - 100;
					if (count % 20 == 0) {//20�T�C�N�����Ƃɒe�����
						for (int i = 0; i < 10; i++) {//10���e�����
							for (int j = 0; j < MAX_TAMA; j++) {
								if (g_tekitama[j].living == FALSE) {
									g_tekitama[j].living = TRUE;
									g_tekitama[j].x = g_hontai[cd].x;
									g_tekitama[j].y = g_hontai[cd].y;
									g_tekitama[j].type = 1;
									float bure = (-0.5 + random(5)* 0.2);//�Ԃ������̂Ɣ��Α��ɁA���ˏ�ɒe�����
									g_tekitama[j].speedx = 3 * cos(-g_hontai[cd].angles - (155 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
									g_tekitama[j].speedy = 3 * -sin(g_hontai[cd].angles - (75 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
									break;
								}
							}
						}
					}
				}


			}
			if (cycle >= 16000 ) {//��������16000�ȍ~
				bumping = 0;//�^�b�N���Ɏg�����ϐ��̍ď�����
				tackle = 0;

				g_hontai[cd].speedx = -kyorix * 0.01;//��100�t���[�������Ė{���̈ʒu�Ɍ������悤�ɂ���
				g_hontai[cd].speedy = -kyoriy * 0.01;
				if ((g_hontai[cd].x <= 410 && g_hontai[cd].x >= 390) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {//�G�̈ʒu���قƂ�ǖړI�̏ꏊ�ɓ������ꍇ
					g_hontai[cd].speedx = 0;//�X�s�[�h��0�ɂ���
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;//�ʒu��{���̏ꏊ�ŌŒ肷��
					g_hontai[cd].y = 100;


					if (count % 10 == 0) {//10�J�E���g���݂Œe�𔭎˂���
						for (int i = 0; i < MAX_TEKI; i++) {//�^���Ɉ�ʏ�e�����
							if (g_tekitama[i].living == FALSE) {
								
							g_tekitama[i].living = TRUE;
							g_tekitama[i].type = 1;

							g_tekitama[i].x = g_hontai[cd].x;
							g_tekitama[i].y = g_hontai[cd].y;
							g_tekitama[i].speedx = cos(g_hontai[cd].angles) * 3;
							g_tekitama[i].speedy = sin(g_hontai[cd].angles) * 3;
							break;
							}

						}
					}
					if (g_hontai[cd].angles >= PI / 2 - 0.001 && g_hontai[cd].angles <= PI / 2 + 0.001) g_hontai[cd].angles = PI / 2;//�G�̃A���O�����قƂ�ǐ^���ʂȂ�Œ肷��
					else {//�����łȂ��Ȃ�^���ʂɋ߂Â���
						if (g_hontai[cd].angles >= PI / 2)g_hontai[cd].angles -= PI / 100;//�p�x�����肷����ꍇ�͈���
						else g_hontai[cd].angles += PI / 100;//����������ꍇ�͑���
					}
				}
				
				if (cycle >= 22000) {//�Ō�Ɏ�芪�������邩�ǂ������m�F����A���Ȃ���������`�Ԃ�
					if (g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) keepstate = 1;
					else keepstate = 0;
				}
			}
			else {
				anglespeed = 0;
				
			}
		}
	}
	else {//��Ƃ���芪�������ꂽ�ꍇ
		if (cycle < 2000) {//�T�C�N��2000�܂ł͈ʒu�����A�ʏ�e�����g����݂ĉ��ɕ���
			//�p�x�A�ʒu��K�؂ȏ��i���`�Ԃ̏����ʒu�j�ɍ��킹��
			if (g_hontai[cd].angles > PII / 2) g_hontai[cd].angles -= PII / 300;
			if (g_hontai[cd].angles <= PII / 2)g_hontai[cd].angles = PII / 2;
			if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;
			else g_hontai[cd].speedx = 1;
			if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;
			else g_hontai[cd].speedy = 1;
			//�قڏ����ʒu�ɂȂ�����Œ肷��
			if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {
				g_hontai[cd].speedx = 0;
				g_hontai[cd].speedy = 0;
				g_hontai[cd].x = 400;
				g_hontai[cd].y = 300;
			}
				g_hontai[cd].reverse = TRUE;
			if (count % 20 == 0) {//�ʏ�e���L�͈͂ɂ΂�܂����A���S�͕����Ȃ�
				for (int i = 0; i < 10; i++) {
					if (i >= 3 && i <= 6) continue;//����3~6�̊Ԃ͔�΂��A�e���o���Ȃ��悤�ɂ���A������J��Ԃ��A�ϐ�1�ŏ�����������ς��Ă���
						for (int j = 0; j < MAX_TAMA; j++) {
								if (g_tekitama[j].living == FALSE) {
								g_tekitama[j].living = TRUE;
								g_tekitama[j].x = g_hontai[cd].x;
								g_tekitama[j].y = g_hontai[cd].y;
								g_tekitama[j].type = 1;
								float bure = (-0.5 + random(5)* 0.2);
								g_tekitama[j].speedx = 3 * cos(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
								g_tekitama[j].speedy = 3 * -sin(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
								break;
								}
						}
					

				}
			}
			//���[�U�[�����
				int color = GetColor(255, 255, 150);//���[�U�[�̐F��ݒ�
				float arufa = (cycle) / 50;//���[�U�[�̑����A10�ȏ�ɂ͂Ȃ�Ȃ��悤�ɂ���
				if (arufa > 10) arufa = 10;
				//if (cycle > 1000)arufa = 10 - (cycle - 2000) / 100;
				if (arufa < 0) arufa = 0;//���[�U�[�̑�����0�ȉ��ɂ��Ȃ�Ȃ��悤�ɂ���
				DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//���������[�U�[
				if (g_herostate == NOMAL && arufa != 0) {//��l�����_���[�W����ł͂Ȃ���arufa��0�o�Ȃ���
					if (abs(g_hx - g_hontai[cd].x) < arufa) {//��l���ƓG��X���W�̍��̐�Βl��arufa�����Ȃ�_���[�W���󂯂�B
						g_herolife--;
						g_herostate = DAMAGED;
						g_damagetimer = g_lasttime;
					}
				}
		}
			if (cycle > 2000 && cycle <= 12000) {//�T�C�N��2000�ȏ�12000����
				g_hontai[cd].angles += PII / 300;//300�t���[�����ƂɈ��]����X�s�[�h�ŉ񂵑�����
				if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;//X���W��400����Ȃ���W������
				else g_hontai[cd].speedx = 1;//400��艺�Ȃ牺�ɍ��W��ǉ�����
				if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;//Y���W��300����Ȃ牺�ɉ�����
				else g_hontai[cd].speedy = 1;//300��艺�Ȃ牺�ɉ�����
				if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {//�œK�Ȉʒu�ɒ���
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;
					g_hontai[cd].y = 300;
				}//�l�p�`���\�z
				int ax = g_hontai[cd].x + 14.14 * cos(g_hontai[cd].angles + 2.3599449 - PII / 2);
				int ay = g_hontai[cd].y + 14.14 * sin(g_hontai[cd].angles + 2.3599449 - PII / 2);
				int bx = g_hontai[cd].x + 14.14 * cos(g_hontai[cd].angles + 0.7853981634 - PII / 2);
				int by = g_hontai[cd].y + 14.14 * sin(g_hontai[cd].angles + 0.7853981634 - PII / 2);
				int cx = g_hontai[cd].x + 600.08 * cos(g_hontai[cd].angles + 1.554131203 - PII / 2);//1.5693981634
				int cy = g_hontai[cd].y + 600.08 * sin(g_hontai[cd].angles + 1.554131203 - PII / 2);
				int dx = g_hontai[cd].x + 600.08 * cos(g_hontai[cd].angles + 1.587461451 - PII / 2);//1.572462992
				int dy = g_hontai[cd].y + 600.08 * sin(g_hontai[cd].angles + 1.587461451 - PII / 2);
				//	DrawQuadrangle(g_hontai[cd].x - 10 * cos(konoangle), g_hontai[cd].y + 10 * sin(konoangle)
				//		, g_hontai[cd].x + 10 * cos(konoangle), g_hontai[cd].y + 10 * sin(konoangle)
				//		, g_hontai[cd].x + 610 * cos(konoangle), g_hontai[cd].y + 600 * sin(konoangle)
				//		, g_hontai[cd].x + 590 * cos(konoangle), g_hontai[cd].y + 600 * sin(konoangle),GetColor(255,255,150),TRUE);
				DrawQuadrangle(ax, ay,
					bx, by,
					cx, cy,
					dx, dy, GetColor(255, 255, 150), TRUE);//���[�U�[�p�̎l�p�`��`��
				BOOL hantei = atariinforey(20, g_hontai[cd].x, g_hontai[cd].y, 600 * cos(g_hontai[cd].angles), 600 * sin(g_hontai[cd].angles));//���[�U�[�Ƃ��Ă̓����蔻��
				//DrawLine(g_hontai[cd].x, g_hontai[cd].y, g_hontai[cd].x + 600 * cos(konoangle), g_hontai[cd].y + 600 * sin(konoangle),GetColor(255,255,255));
				if (hantei == TRUE && g_herostate == NOMAL) {//�����蔻��ɓ����Ă��āA��l�����_���[�W����łȂ���΃_���[�W
					g_herolife--;
					g_herostate = DAMAGED;
					g_damagetimer = g_lasttime;
				}

				if (g_count % 10 == 0) {//�����10�t���[�����Ƃɒe���o��������
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 1;
							g_tekitama[j].reverse = FALSE;

							g_tekitama[j].speedx = 2 * cos(g_hontai[cd].angles + PII) + 1 - 0.1*random(20);//���g�̌����Ƃ͔��Α��ɒe�����
							g_tekitama[j].speedy = -2 * sin(g_hontai[cd].angles + PII) + 1 - 0.1*random(20);
							break;
						}
					}
				}

			}
			if (cycle > 12000 && cycle < 19000) {//�T�C�N��12000~19000�A���g���o�E���h���Ȃ���o�E���h�e�����
				
				if (g_count % 12 == 0) {//12�t���[�����ƂɃo�E���h�e�����
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 5;
							g_tekitama[j].reverse = FALSE;

							g_tekitama[j].speedx = 4 * cos(g_hontai[cd].angles) + g_hontai[cd].speedx  + (0.5 - 0.1*random(10));//���g�̃X�s�[�h�𑫂��������Ēe���o��
							g_tekitama[j].speedy = 4 * -sin(g_hontai[cd].angles) + g_hontai[cd].speedy + (0.5 - 0.1*random(10));
							break;
						}
					}
				}

				if (cycle > 12000 && cycle < 12500) {//�o�E���h���邽�߂̏������x
					g_hontai[cd].speedx = 3;
					g_hontai[cd].speedy = 3;
					anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;
					g_hontai[cd].angles += anglespeed;
				}
				anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;//�G����l���̕����������悤�ɕ����X�s�[�h�𒲐�
				g_hontai[cd].angles +=  anglespeed;

				//�J�x�ɂԂ�������Ԃ����������ɔ��]������
				if ((g_hontai[cd].x > 800 && g_hontai[cd].speedx >0)|| (g_hontai[cd].x < 50 && g_hontai[cd].speedx < 0))g_hontai[cd].speedx *= -1;
				if ((g_hontai[cd].y > 600 && g_hontai[cd].speedy > 0)||( g_hontai[cd].y < 50 && g_hontai[cd].speedy < 0 ))g_hontai[cd].speedy *= -1;
			}
			if (cycle > 19000) {//�T�C�N��19000�ȏ�A���̒ǔ��e�����������
				//���[�U�[�̗\������A�Ǝ˒n�_��_�ł�����
				if (cycle % 200 <= 100)
					DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(255, 50, 50), TRUE);
				//�����ƈʒu����`�Ԃ̏����ʒu�ɖ߂�
				if (g_hontai[cd].angles > PII / 2) g_hontai[cd].angles -= PII / 300;
				if (g_hontai[cd].angles <= PII / 2)g_hontai[cd].angles = PII / 2;
				if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;
				else g_hontai[cd].speedx = 1;
				if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;
				else g_hontai[cd].speedy = 1;
				//�������قڏ����ʒu�ɂȂ�����Œ肷��
				if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;
					g_hontai[cd].y = 300;
				}
				if (g_count % 10 == 0 && cycle < 20000) {//10�t���[�����ƂɎ��ʂ����
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 2;
							g_tekitama[j].reverse = FALSE;
							g_tekitama[j].counter = g_lasttime;

							g_tekitama[j].speedx = 3 - 0.1*random(100);
							g_tekitama[j].speedy = 3 - 0.1*random(100);
							break;
						}
					}
					
				}
			}
			
		}

	DrawRotaGraph(g_hontai[cd].x  , g_hontai[cd].y  ,1, g_hontai[cd].angles - PI/2 , g_Boss1img, TRUE);//���̃{�X�̕`�ʁA

}

void DrawType40(int cd) {//�{�X�̎�芪��1
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;
	int count = (g_count - g_hontai[cd].counter);//���̓G�p�̃J�E���^�[��ݒu����

	if (g_hontai[cd].reverse == FALSE) {//����reverse��FALSE�Ȃ�΃{�X�̍��W��ǐ�������
		g_hontai[cd].x = g_hontai[cd - 1].x + 30*sqrt(2) * cos(g_hontai[cd-1].angles + 2.3599449 - PII / 2);
		g_hontai[cd].y = g_hontai[cd - 1].y + 30*sqrt(2) * sin(g_hontai[cd-1].angles + 2.3599449 - PII / 2);
	}
	
		if (cycle < 1000) {//�T�C�N����1000�����Ȃ�reverse��FALSE
			g_hontai[cd].reverse = FALSE;
		}
		if (cycle >= 1000 && cycle < 3000) {//1000~3000�̊ԁA���O�Ǝˁi�ԁj���s���A�܂���������ĉ��ɉ�����
			g_hontai[cd].reverse = TRUE;//�ʒu�̎x�z���{�X���痣���
			if(cycle%200 <= 100)DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(150,0,0), 1);//�ʒu�����ɂ��炷
			if (g_hontai[cd].x > g_hontai[cd - 1].x - 30)g_hontai[cd].speedx = -0.3;
			else g_hontai[cd].speedx = 0;
			if (g_hontai[cd].y < g_hontai[cd - 1].y + 120)g_hontai[cd].speedy = 1.5;
			else  g_hontai[cd].speedy = 0;
		}
		if(cycle>= 3000 && cycle < 8000){//3000~8000�̊ԁA���[�U�[�̖{�Ǝ˂�^���ɍs��
			
			int color = GetColor(255, 255, 150);//���[�U�[�̐F�w��
			float arufa = (cycle - 3000) / 50;
			if (arufa > 10) arufa = 10;//���[�U�[�̑���
			if (cycle > 5000)arufa = 10 - (cycle - 5000)/100;//�T�C�N��5000�ȏ�Ȃ�Ώ��X�ɏ���������
			if (arufa < 0) arufa = 0;//arufa��0�����ɂ͂Ȃ�Ȃ�
			DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//���[�U�[�̕`��
			if (g_herostate == NOMAL && arufa!= 0) {//�_���[�W����
				if (abs(g_hx - g_hontai[cd].x) < arufa) {
					g_herolife -- ;
					g_herostate = DAMAGED;
					g_damagetimer = g_lasttime;
				}
			}
			if ( cycle > 6000) {//�J�E���g6000�ȏ�A��U���̏ꏊ�ɖ߂�
				//DrawFormatString(600, 400, 0xFFFFF, "�J�E���g�v�� %10f", g_hontai[cd].y);
				if (g_hontai[cd].x <= g_hontai[cd-1].x - 20 )g_hontai[cd].speedx = 0.6;//���̏ꏊ�ɖ߂�܂ŃX�s�[�h��ݒ肷��iX���W�ɂ��āj
				else g_hontai[cd].speedx = 0;//���̏ꏊ�ɖ߂����Ȃ�X�s�[�h��0��
					
				if (g_hontai[cd].y <= g_hontai[cd-1].y + 20)g_hontai[cd].speedy = 0;//���̏ꏊ�ɖ߂�܂ŃX�s�[�h��ݒ肷��iY���W�ɂ��āj
				else g_hontai[cd].speedy = -3; 
					if(g_hontai[cd].x >= g_hontai[cd-1].x - 25 && g_hontai[cd].y <= g_hontai[cd-1].y + 25) g_hontai[cd].reverse = FALSE;//���̏ꏊ�ɂ��ǂ����Ȃ�
					//reverse��FALSE�ɁA�{�X�̍��W�Ɉˑ�����
			}
		}
		if (cycle >= 8000 && cycle < 13000) {//8000~130000�A�e��ʏ�e������A�{�X�̃^�b�N����⏕����
			g_hontai[cd].reverse = FALSE;
			if (g_count % 15 == 0) {
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {//�ʏ�e�𔭎˂���
						tamaset(i, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x +100 - random(200), g_hontai[cd].y + 100 -numberrandom(200,g_count))), 3 * sin(kakudo(g_hontai[cd].x +100-random(200), g_hontai[cd].y + 100 -numberrandom(200,g_count))));
						break;
					}
				}
			}
		}
		if (cycle >= 14000 && cycle < 16000) {//��ːi��A�΂̈������ǔ��e������A�e�̉����W�Q����
			if (g_count % 40 == 0) {//40�t���[���Ɉ�x�΋������
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {//�e�������
						tamaset(i, 3, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)), 3 * sin(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)));
						break;
					}
				}
			}
		}
		if (cycle > 16000) {//6000~�o�E���h�e�����
			if (g_count % 40 == 0) {//40�t���[�����ɕ���
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {
						tamaset(i, 5, g_hontai[cd].x, g_hontai[cd].y,  2 - random(4),  2 - 0.1 * numberrandom(40,g_count));
						break;
					}
				}
			}
		}
	
}

void DrawType41(int cd) {//�{�X�̎�芪��2
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;//����reverse��FALSE�Ȃ�΃{�X�̍��W��ǐ�������
	int count = (g_count - g_hontai[cd].counter);//���̓G�p�̃J�E���^�[��ݒu����
	

	//����reverse��FALSE�Ȃ�΃{�X�̍��W��ǐ�������
	if (g_hontai[cd].reverse == FALSE) {
		g_hontai[cd].x = g_hontai[cd - 2].x + 30 * sqrt(2) * cos(g_hontai[cd-2].angles + 0.7853981634 - PII / 2);
		g_hontai[cd].y = g_hontai[cd - 2].y + 30 * sqrt(2) * sin(g_hontai[cd-2].angles + 0.7853981634 - PII / 2);
	}

	if (cycle < 1000) {//�T�C�N����1000�����Ȃ�reverse��FALSE
		g_hontai[cd].reverse = FALSE;
	}
	if (cycle >= 1000 && cycle < 3000) {//1000~3000�̊ԁA���O�Ǝˁi�ԁj���s���A�܂���������ĉ��ɉ�����
		g_hontai[cd].reverse = TRUE;//�ʒu�̎x�z���{�X���痣���
		if (cycle % 200 <= 100)DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(150, 0, 0), 1);
		if (g_hontai[cd].x < g_hontai[cd - 2].x + 30)g_hontai[cd].speedx = 0.3;
		else g_hontai[cd].speedx = 0;
		if (g_hontai[cd].y < g_hontai[cd - 2].y + 120)g_hontai[cd].speedy = 1.5;
		else  g_hontai[cd].speedy = 0;
	}
	if (cycle >= 3000 && cycle < 8000) {//3000~8000�̊ԁA���[�U�[�̖{�Ǝ˂�^���ɍs��

		int color = GetColor(255, 255, 150);//���[�U�[�̐F�w��
		float arufa = (cycle - 3000) / 50;//���[�U�[�̑����̐ݒ�
		if (arufa > 10) arufa = 10;//���[�U�[�����̏����ݒ�
		if (cycle > 5000)arufa = 10 - (cycle - 5000) / 100;//���[�U�[�����X�ɏ���������
		if (arufa < 0) arufa = 0;//�傫����0�����ɂ͂Ȃ�Ȃ�
		DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//���[�U�[�̕`��
		if (g_herostate == NOMAL && arufa != 0) {//�_���[�W�̔���A��l�����_���[�W����łȂ���΁A������ƃ_���[�W
			if (abs(g_hx - g_hontai[cd].x) < arufa) {
				g_herolife--;
				g_herostate = DAMAGED;
				g_damagetimer = g_lasttime;
			}
		}
		if (cycle > 6000) {//~6000�A���̏ꏊ�ɖ߂�
			//DrawFormatString(600, 400, 0xFFFFF, "�J�E���g�v�� %10f", g_hontai[cd].y);
			if (g_hontai[cd].x >= g_hontai[cd - 2].x - 20)g_hontai[cd].speedx = -0.6;//X���W�ɂ����āA���̏ꏊ�ɖ�����Ζ߂��Ă���
			else g_hontai[cd].speedx = 0;//���ɖ߂�����X�s�[�h��0��

			if (g_hontai[cd].y <= g_hontai[cd - 2].y + 20)g_hontai[cd].speedy = 0;//Y���ɂ����āA���̏ꏊ�ɖ�����Ζ߂��Ă���
			else g_hontai[cd].speedy = -3;
			if (g_hontai[cd].x <= g_hontai[cd - 2].x + 25 && g_hontai[cd].y <= g_hontai[cd - 2].y + 25) g_hontai[cd].reverse = FALSE;//���ɖ߂�����Ăшʒu���{�X�Ɉˑ�
		}
	}
	if (cycle >= 8000 && cycle < 13000) {//8000~13000
		g_hontai[cd].reverse = FALSE;
		if (g_count % 15 == 0) {//15�t���[�����ɒʏ�e�����
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {//�ʏ�e�̔���
					tamaset(i, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 100 - random(200), g_hontai[cd].y + 100 - numberrandom(200, g_count))), 3 * sin(kakudo(g_hontai[cd].x + 100 - random(200), g_hontai[cd].y + 100 - numberrandom(200, g_count))));
					break;
				}
			}
		}
	}
	if (cycle >= 14000 && cycle < 16000) {//14000~16000�A�΋�������A��l���̉����W�Q
		if (g_count % 40 == 0) {//40�t���[�����ɕ���
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {//�΋��𔭎˂���
					tamaset(i, 3, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)), 3 * sin(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)));
					break;
				}
			}
		}
	}
	if (cycle > 16000) {//16000�A�o�E���h�e������ւ�ɕ���
		if (g_count % 40 == 0) {
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {
					tamaset(i, 5, g_hontai[cd].x, g_hontai[cd].y, 2 - random(4), 2 - 0.1 * numberrandom(40, g_count));//�኱�����_���ɒe������
					//random,numberrandom�Ŏw�肵���u�����̗����𐶐�����
					break;
				}
			}
		}
	}


}

void Drawtama(int key) {     //�e�̑ł���
	g_movecount++;//�e�����^�C�~���O���w�肷�邽�߂̕ϐ�
	if (!(key & PAD_INPUT_B)) {//X�L�[�������Ă��Ȃ����̂ݒe�𔭎˂ł���
		switch (g_shotrank) {//�V���b�g�����N���Ƃ̃V���b�g�̏o��
		case 0://�V���b�g�����N0

			if (key & PAD_INPUT_A) {//Z�{�^������������
				if (IsAKeyTrigger(key) == TRUE) {//�{�^���̉������߂ɁAmovecount��0�ɂ���
					g_movecount = 0;
				}
				g_movecount %= 10;//10�t���[�����Ƃ̒e������
				if (g_movecount == 0) {
					int u;
					for (u = 0; u < MAX_TAMA - 10; u++) {
						if (g_mikatatama[u + 10].living == FALSE) break;
					}
					if (u < MAX_TAMA) {//�e�͐��ʂɂ���
						g_mikatatama[u + 10].living = TRUE;
						g_mikatatama[u + 10].x = g_hx;
						g_mikatatama[u + 10].y = g_hy;
						g_mikatatama[u + 10].type = 0;
					}
				}
			}

			break;
		case 1://�V���b�g�����N1�A�񔭓����ɕ���
			if (key & PAD_INPUT_A) {//�{�^���������ꂽ��e�����
				if (IsAKeyTrigger(key) == TRUE) {//�����ꏉ�߂�movecount��0�ɂ���
					g_movecount = 0;
				}
				g_movecount %= 10;//10�t���[�����ɒe�����
				if (g_movecount == 0) {
					int u;//�e�̏o��ꏊ�̎w��̂��߂̕ϐ�
					for (int a = 0; a < 2; a++) {//����̒e�����
						for (u = 0; u < MAX_TAMA - 10; u++) {//-10�ƂȂ��Ă���̂́Au��0�̎��ɓG�����Ԃ�����������
							if (g_mikatatama[u + 10].living == FALSE) {
								g_mikatatama[u + 10].living = TRUE;
								g_mikatatama[u + 10].x = g_hx - 15 + a * 30;//a�ɂ��A���ꏊ���������炷
								g_mikatatama[u + 10].y = g_hy;
								g_mikatatama[u + 10].type = 0;
								break;
							}
						}
					}


				}
			}
			break;

		case 2://�V���b�g�����N2(�ő�)�A3�����ɒe�����
			if (key & PAD_INPUT_A) {//Z�{�^���Œe�����
				if (IsAKeyTrigger(key) == TRUE) {//�{�^���̉������߂�movecount��0�ɂ���
					g_movecount = 0;
				}
				g_movecount %= 30;//30�t���[�����ɃT�C�N���Œe�����
				if (g_movecount - 20 == 0 || g_movecount - 10 == 0 || g_movecount == 0) {
					int u;
					for (u = 0; u < MAX_TAMA - 10; u++) {//�^�������i�ޒe
						if (g_mikatatama[u + 10].living == FALSE) {
							g_mikatatama[u + 10].living = TRUE;
							g_mikatatama[u + 10].x = g_hx ;
							g_mikatatama[u + 10].y = g_hy ;
							g_mikatatama[u + 10].type = 0;
							break;
						}
					}
					for (int a = 1; a < 4; a++) {
						if (g_movecount - 20 + (a - 1) * 10 == 0) {//�E�ɐi�ޒe�A������-20��0�ɂȂ�ꍇ�Ƃŕ������A�e�̔��ˈʒu������ŕς���
							for (u = 0; u < MAX_TAMA - 10; u++) {
								if (g_mikatatama[u + 10].living == FALSE) {
									g_mikatatama[u + 10].living = TRUE;
									g_mikatatama[u + 10].x = g_hx  + 15;
									g_mikatatama[u + 10].y = g_hy + (a - 1) * 5;
									g_mikatatama[u + 10].type = 1 * a;
									break;
								}
							}
							for (u = 0; u < MAX_TAMA - 10; u++) {//���ɐi�ޒe
								if (g_mikatatama[u + 10].living == FALSE) {
									g_mikatatama[u + 10].living = TRUE;
									g_mikatatama[u + 10].x = g_hx  - 15;
									g_mikatatama[u + 10].y = g_hy + (a - 1) * 5;
									g_mikatatama[u + 10].type = -1 * a;
									break;
								}

							}




							break;
						}
					}
				}
			}
		default://�V���b�g�����N���ݒ肳��Ă��Ȃ��Ȃ烉���N2�����
			if (key & PAD_INPUT_A) {
				if (IsAKeyTrigger(key) == TRUE) {
					g_movecount = 0;
				}
				g_movecount %= 30;
				if (g_movecount - 20 == 0 || g_movecount - 10 == 0 || g_movecount == 0) {
					int u;
					for (u = 0; u < MAX_TAMA - 10; u++) {
						if (g_mikatatama[u + 10].living == FALSE) {
							g_mikatatama[u + 10].living = TRUE;
							g_mikatatama[u + 10].x = g_hx ;
							g_mikatatama[u + 10].y = g_hy ;
							g_mikatatama[u + 10].type = 0 ;
							break;
						}
					}
					if (g_movecount - 20 == 0) {
						for (u = 0; u < MAX_TAMA - 10; u++) {
							if (g_mikatatama[u + 10].living == FALSE) {
								g_mikatatama[u + 10].living = TRUE;
								g_mikatatama[u + 10].x = g_hx  + 15;
								g_mikatatama[u + 10].y = g_hy ;
								g_mikatatama[u + 10].type = 1;
								break;
							}
						}
						for (u = 0; u < MAX_TAMA - 10; u++) {
							if (g_mikatatama[u + 10].living == FALSE) {
								g_mikatatama[u + 10].living = TRUE;
								g_mikatatama[u + 10].x = g_hx  - 15;
								g_mikatatama[u + 10].y = g_hy ;
								g_mikatatama[u + 10].type = -1;
								break;
							}

						}
					}


					else
						if (g_movecount - 10 == 0) {
							for (u = 0; u < MAX_TAMA - 10; u++) {
								if (g_mikatatama[u + 10].living == FALSE) {
									g_mikatatama[u + 10].living = TRUE;
									g_mikatatama[u + 10].x = g_hx  + 15;
									g_mikatatama[u + 10].y = g_hy  + 5;
									g_mikatatama[u + 10].type = 2;
									break;
								}
							}
							for (int u = 0; u < MAX_TAMA - 10; u++) {
								if (g_mikatatama[u + 10].living == FALSE) {
									g_mikatatama[u + 10].living = TRUE;
									g_mikatatama[u + 10].x = g_hx  - 15;
									g_mikatatama[u + 10].y = g_hy  + 5;
									g_mikatatama[u + 10].type = -2;
									break;
								}

							}
						}
						else
							if (g_movecount == 0) {
								for (u = 0; u < MAX_TAMA - 10; u++) {
									if (g_mikatatama[u + 10].living == FALSE) {
										g_mikatatama[u + 10].living = TRUE;
										g_mikatatama[u + 10].x = g_hx  + 15;
										g_mikatatama[u + 10].y = g_hy  + 10;
										g_mikatatama[u + 10].type = 3;
										break;
									}
								}
								for (u = 0; u < MAX_TAMA - 10; u++) {
									if (g_mikatatama[u + 10].living == FALSE) {
										g_mikatatama[u + 10].living = TRUE;
										g_mikatatama[u + 10].x = g_hx - 15;
										g_mikatatama[u + 10].y = g_hy  + 10;
										g_mikatatama[u + 10].type = -3;
										break;
									}

								}
							}
					break;
				}
			}
		}

	}
	
}

void Drawreflection(int key) {//�V�[���h�\�[�h�i�K�E�Z�j�̏���
	static int tame = g_lasttime;//���߂鎞�Ԃ̂��߂̊֐��A������
	static int tamezikan = g_lasttime;//���߂Ă���ԂɌ��邽�߂̊֐��A������


	//if (!(key & PAD_INPUT_B)) {
	//	tame = g_lasttime;
	//	tamezikan = g_count;
	//}
	if (key & PAD_INPUT_B) {//�w�{�^����������Ă�����
	
		

		if (IsAKeyTrigger(key) == TRUE) {//�w�{�^���̉�����n��
			tame = g_lasttime;//���ߎn�߂����Ԃ�ݒ�
		}
	    tamezikan = g_lasttime;
		if (tamezikan > tame + 1500) {
			if (g_count % 6 <= 3) {//���悻6�t���[���T�C�N���œ_�ł��J��Ԃ�����
				DrawCircle(g_hx, g_hy, 50, GetColor(150, 150, 0), TRUE);
			}
		}
			}
	else {
		if (tamezikan > tame + 1500) {//X�{�^���������Ă��Ȃ���
			BOOL hantei = 0;//�V�[���h�\�[�h���g�����̔���
			if (g_drawanimation[0].onoff == 0) {
				g_drawanimation[0].onoff = 1;//�V�[���h�\�[�h�A�j���[�V�����֐��̔����1�ɂ���
				g_drawanimation[0].time = g_count;//�V�[���h�\�[�h�̎��Ԃ𑪒肵�n�߂�
			}
			for (int i = 0; i < MAX_TAMA; i++) {//���˕Ԃ��e��ݒ�
				if (g_tekitama[i].living == FALSE)continue;//�e�������Ă��Ȃ����̂Ȃ珜��
				hantei = Atariinfoougi(g_tekitama[i].x, g_tekitama[i].y, g_hx, g_hy, 300);//��`�̓����蔻�����l���̎���ɏo��
				if (hantei == TRUE) {//���������̔�����ɓG�̋ʂ�����Ȃ�
				
					int j;
					for ( j = 10; j < MAX_TAMA; j++) {
						if (g_mikatatama[j].living == FALSE)break;
					}//���̒e�̑�����V������l���̒e�ɖ����n���A���̒e������
							g_mikatatama[j].living = TRUE;
							g_mikatatama[j].x = g_tekitama[i].x;
							g_mikatatama[j].y = g_tekitama[i].y;
							g_mikatatama[j].type = g_tekitama[i].type + 100;
							g_mikatatama[j].speedx = cos(angle1(g_mikatatama[j].x, g_mikatatama[j].y)) * -3;
							g_mikatatama[j].speedy = sin(angle1(g_mikatatama[j].x, g_mikatatama[j].y)) * +3;
							g_mikatatama[j].timer = g_lasttime;
							g_mikatatama[j].counter = g_count;
							g_mikatatama[j].reverse = FALSE;
							g_tekitama[i].living = FALSE;
						
					
					
				}
			}
			//DrawTriangle(g_hx, g_hy, g_hx + 100 * cos(PII / 4), g_hy - 100 * sin(PII / 4), g_hx + 100 * cos(PII * 3 / 4), g_hy - 100 * sin(PII * 3 / 4), GetColor(255, 255, 255), TRUE);
			//DrawRotaGraph(g_hx - 25, g_hy, 1, 3.14, g_Boss1img, TRUE);
			
		}
		tame = g_lasttime;
		tamezikan = g_lasttime;
	
	}
}

void reflectedtama101(int c) {//���˕Ԃ����e�A�^�C�v1�ʏ�e
	g_mikatatama[c].x +=g_mikatatama[c].speedx;
	g_mikatatama[c].y += g_mikatatama[c].speedy;
	
	DrawCircle(g_mikatatama[c].x, g_mikatatama[c].y, 12, GetColor(50, 250, 50), TRUE);//�F���w�肵�ĕ`��

	for (int x = 0; x < MAX_TEKI; x++) {
		if (g_hontai[x].living == TRUE) {//�S�Ă̐����Ă���G�ɑ΂��ē����蔻��𔻒肷��

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);

			if (hantei == TRUE ) {
				g_mikatatama[c].living = FALSE;
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//������ΓG�̃��C�t��10���
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//�G�̎�ނ��^�C�v2��������V���b�g�����N�A�b�v�A�C�e���𗎂Ƃ�
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;//�g���Ă��Ȃ��z���T��
					}
					if (u < 50) {//�A�C�e�������
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//�G�̎�ނ��^�C�v4��������X�s�[�h�����N�A�b�v�A�C�e���𗎂Ƃ�
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;//�g���Ă��Ȃ��z���T��
					}
					if (u < 50) {//�A�C�e�������
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 2;

					}
				}

			}

		}
	}
}


void reflectedtama102(int c) {//�^�C�v2�A���F�̒ǔ��e�𔽎˂������ꍇ�A�i�e�Ƃ͂Ȃ炸���ł���j

	float zanzousize = 10- ((g_lasttime - g_mikatatama[c].timer) / 40);//�e�̑傫�����w��A�����������Ԃ��o���Ƃɏ������Ȃ�
	if (zanzousize <= 0)zanzousize = 0, g_mikatatama[c].living = FALSE;//�e�̑傫����0�ȉ��ɂȂ�Ώ���
	g_mikatatama[c].speedx = 0;//�e�̃X�s�[�h��Z,Y����0�Ƃ���
	g_mikatatama[c].speedy = 0;
	g_mikatatama[c].x = g_mikatatama[c].x;//�ꏊ���ύX���Ȃ��ł���
	g_mikatatama[c].y = g_mikatatama[c].y;

	DrawCircle(g_mikatatama[c].x, g_mikatatama[c].y, zanzousize, GetColor(100, 250, 100), TRUE);//�c���̕`��
}


void reflectedtama103(int c) {//�^�C�v3�΂̒e�̔���
	g_mikatatama[c].x += g_mikatatama[c].speedx;//�ʒu�ɑ��x��ǉ�
	g_mikatatama[c].y += g_mikatatama[c].speedy;

	int tikaino = nearestenemy(c);//�ł��߂��G������

	if (g_lasttime - g_mikatatama[c].timer > 300) {//�o�ꂵ�Ă���300�~���b���o������
		if (g_mikatatama[c].reverse == FALSE) {//�����]�����܂��������Ƃ��������
			g_mikatatama[c].speedx = 5 * cos(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y,g_hontai[tikaino].x,g_hontai[tikaino].y));//�X�s�[�h���ł��߂��G�Ɍ����ĕύX����
			g_tekitama[c].speedy = 5 * sin(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x, g_hontai[tikaino].y));
			g_mikatatama[c].reverse = TRUE;//�����]���������Ƃ�reverse�Ŏ���
		}
	}




	if (g_count % 2 == 0) {//�O�Ղ�`��
		for (int j = 0; j < 100; j++) {
			if (g_tamakisekidata[c].id[j].living == FALSE) {
				g_tamakisekidata[c].id[j].living = TRUE;
				g_tamakisekidata[c].id[j].x = g_mikatatama[c].x;
				g_tamakisekidata[c].id[j].y = g_mikatatama[c].y;
				g_tamakisekidata[c].id[j].counter = g_count;
				break;
			}
		}


	}
	for (int k = 0; k < MAX_TAMA; k++) {//�O�Ղ̕ω�
		if (g_tamakisekidata[c].id[k].living == TRUE) {
			int konokiseki = GetColor(120, 230, 120);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[c].id[k].counter) <= 7) ookisa = 14;//�o���7�t���[���܂ł͋O�Ղ͕ω����Ȃ�
			if ((g_count - g_tamakisekidata[c].id[k].counter) > 7) {//7�t���[���ォ�珙�X�ɏ������Ȃ�
				ookisa = 14 - ((g_count - g_tamakisekidata[c].id[k].counter - 7) * 14 / 16);
				konokiseki = GetColor(180, 200, 180);
			}
			DrawCircle(g_tamakisekidata[c].id[k].x, g_tamakisekidata[c].id[k].y, ookisa, konokiseki, TRUE);//�O�Ղ����ۂɕ`�ʂ���
			if (g_count - g_tamakisekidata[c].id[k].counter > 23) g_tamakisekidata[c].id[k].living = FALSE;//��莞�Ԃŏ���
		}
	}

	if (g_mikatatama[c].living == FALSE) {//�{�̂��Ȃ��Ȃ�΋O�Ղ������Ȃ�
		g_tamakisekidata[c].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[c].id[b].living = FALSE;
		}
	}

	for (int x = 0; x < MAX_TEKI; x++) {//�����蔻��𔻒肷��
		if (g_hontai[x].living == TRUE) {

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);//�e�G�Ƃ��̒e�ƂŔ���

			if (hantei == TRUE) {
				g_mikatatama[c].living = FALSE;//�e������
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//���C�t��10���炷
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//�������G���^�C�v2,4�Ȃ�΃A�C�e���𗎂Ƃ�
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 2;

					}
				}

			}

		}
	}

}

void reflectedtama104(int c) {//�^�C�v3�΂̒e�̔���

	g_mikatatama[c].x += g_mikatatama[c].speedx;//�ʒu�ɑ��x��ǉ�
	g_mikatatama[c].y += g_mikatatama[c].speedy;

	int tikaino = nearestenemy(c);//�ł��߂��G������

	if (g_lasttime - g_mikatatama[c].timer > 300) {//�o�ꂵ�Ă���300�~���b�o�����Ȃ�
		if (g_mikatatama[c].reverse == FALSE) {//reverse���쓮���Ă��Ȃ��A�܂������]�����Ă��Ȃ��Ȃ�
			g_mikatatama[c].speedx = 5 * cos(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x +(80 - random(160)), g_hontai[tikaino].y));//�ł��߂��G�ɕ����]������
			g_tekitama[c].speedy = -5 * sin(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x + (80 - random(160)), g_hontai[tikaino].y));
			g_mikatatama[c].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//2�t���[�����ɂ��̒e�̂���ꏊ�ɋO�Ղ�����
		for (int j = 0; j < 100; j++) {
			if (g_tamakisekidata[c].id[j].living == FALSE) {
				g_tamakisekidata[c].id[j].living = TRUE;
				g_tamakisekidata[c].id[j].x = g_mikatatama[c].x;
				g_tamakisekidata[c].id[j].y = g_mikatatama[c].y;
				g_tamakisekidata[c].id[j].counter = g_count;
				break;
			}
		}


	}
	for (int k = 0; k < MAX_TAMA; k++) {//�O�Ղ̕ω���`��
		if (g_tamakisekidata[c].id[k].living == TRUE) {
			int konokiseki = GetColor(120, 230, 120);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[c].id[k].counter) <= 7) ookisa = 14;//7�t���[���܂łȂ�傫���ɕω��͂Ȃ�
			if ((g_count - g_tamakisekidata[c].id[k].counter) > 7) {//7�t���[���ȍ~�͏��X�ɏ������Ȃ�
				ookisa = 14 - ((g_count - g_tamakisekidata[c].id[k].counter - 7) * 14 / 16);
				konokiseki = GetColor(180, 200, 180);//�O�Ղ̐F��`��
			}
			DrawCircle(g_tamakisekidata[c].id[k].x, g_tamakisekidata[c].id[k].y, ookisa, konokiseki, TRUE);//���ۂɋO�Ղ�`�ʂ���
			if (g_count - g_tamakisekidata[c].id[k].counter > 23) g_tamakisekidata[c].id[k].living = FALSE;//���Ԃ����ĂΏ��ł�����
		}
	}

	if (g_mikatatama[c].living == FALSE) {//�{�̂�������΋O�Ղ�������
		g_tamakisekidata[c].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[c].id[b].living = FALSE;
		}
	}

	for (int x = 0; x < MAX_TEKI; x++) {//�G�Ƃ̓����蔻�菈��
		if (g_hontai[x].living == TRUE) {//�����Ă���G�̑S�ĂƔ�����s��

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);

			if (hantei == TRUE) {//���������肪�^�Ȃ�
				g_mikatatama[c].living = FALSE;//�q�̒e������
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//�G�̃��C�t��10�_���[�W
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//�G�̃^�C�v��2�Ȃ�p���[�A�b�v�A�C�e��
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//�|�����G�̃^�C�v��4�Ȃ�X�s�[�h�A�b�v�A�C�e��
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 2;

					}
				}

			}

		}
	}
}


void reflectedtama105(int i) {//�^�C�v5�A�o�E���h�e�𒵂˕Ԃ����ꍇ

	g_mikatatama[i].x += g_mikatatama[i].speedx;//���W�̕ϐ��ɃX�s�[�h��ǉ�
	g_mikatatama[i].y += g_mikatatama[i].speedy;

	DrawCircle(g_mikatatama[i].x, g_mikatatama[i].y, 12, GetColor(100, 250, 100), TRUE);//���̒e��`��
	if (g_mikatatama[i].reverse == FALSE) {//�������܂��o�E���h���Ă��Ȃ��Ȃ�A������reverse��FALSE�Ȃ�
		if (g_mikatatama[i].x >= 800 || g_mikatatama[i].x <= 0) {//���̕ǂɂԂ�������
			g_mikatatama[i].speedx *= -1;//X���W�̃X�s�[�h�𔽓]������
			g_mikatatama[i].reverse = TRUE;
		}
		if (g_mikatatama[i].y >= 600 || g_mikatatama[i].y <= 0) {//�㉺�̕ǂɂԂ�������
			g_mikatatama[i].speedy *= -1;//Y���W�̃X�s�[�h�𔽓]������
			g_mikatatama[i].reverse = TRUE;
		}
	}
	for (int x = 0; x < MAX_TEKI; x++) {//�G�Ƃ̐ڐG�̊m�F
		if (g_hontai[x].living == TRUE) {//�����Ă���G�S���Ɣ��菈�����s��

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[i].x, g_mikatatama[i].y, 20);//���̒e�ƓG�̍��W�Ƃœ����蔻�������

			if (hantei == TRUE) {
				g_mikatatama[i].living = FALSE;//���̒e�����ł�����
				g_hontai[x].lifecount = g_hontai[x].lifecount - 13;//���胉�C�t-13
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//�����|���A�G�̃^�C�v��2�̏ꍇ
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;//�p���[�A�b�v�A�C�e�����o��
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//�����|���A�G�̃^�C�v��4�̏ꍇ
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;//�X�s�[�h�A�b�v�A�C�e�����o��
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 2;

					}
				}

			}

		}
	}
}

int detectNearestEnemy(int c) {//�ł��߂��G����������֐�
	static int nearestenemy = 0;//�ł��߂��G�̔ԍ�������邽�߂̕ϐ�
	static float Near = 100000000;//�ł��߂������A�ŏ��͂��̂������傫�����Ă���
	for (int i = 0; i < MAX_TEKI; i++) {
		if (g_hontai[i].living == TRUE) {//�����Ă���G�Ƃ̋�����}��
		int x =	g_hontai[i].x - g_mikatatama[c].x;
		int y = g_hontai[i].y - g_mikatatama[c].y;
			if (x * x + y * y < Near) {//���������������܂ł�Near���߂���΁A���̓G�̔ԍ��Ƌ�����o�^
				Near = x * x + y * y; nearestenemy = i;
			}
		}


	}
	int a = nearestenemy;//a��nearestenemy������
	nearestenemy = 0;//�e�֐��̍ď�����
	Near = 1000000000;
		return a;
}

void DrawMikatatama() {
	//�^�}�̕`��ƈړ�
	float mv = 600.0f * g_frametime;//�ړ����x�̐ݒ�
	for (int i = 0; i < MAX_TAMA; i++) {
		if (g_mikatatama[i].living == FALSE) continue;



		if (g_mikatatama[i].type >= 101) {//�������e��type��101�ȏ�Ȃ�i���������˂����e�Ȃ�j
			switch (g_mikatatama[i].type) {
			case 101:
				reflectedtama101(i);
				
				break;

			case 102:
				reflectedtama102(i);
				break;

			case 103:
				reflectedtama103(i);
				break;
			case 104:
				reflectedtama104(i);
				break;

			case 105:
				reflectedtama105(i);
				break;
			default:
				break;
			}
		}
		//���˂��Ă��Ȃ��A���g���甭�����e�Ȃ�
		

		//y������-mv�ړ�������
		if(g_mikatatama[i].type < 99)g_mikatatama[i].y -= mv;
		//�V���b�g�����N2�̒e�̏ꍇ�Atype1���珙�X�ɉ������Ɍ����Ă���type��-�Ȃ獶�����A+�Ȃ�E����
		if (g_mikatatama[i].type == 1) g_mikatatama[i].x += 0.1 * mv, g_mikatatama[i].y += 0.1 * mv;
		if (g_mikatatama[i].type == -1)g_mikatatama[i].x -= 0.1 * mv, g_mikatatama[i].y += 0.1 * mv;
		if (g_mikatatama[i].type == 2) g_mikatatama[i].x += 0.15 * mv, g_mikatatama[i].y += 0.15 * mv;
		if (g_mikatatama[i].type == -2)g_mikatatama[i].x -= 0.15 * mv, g_mikatatama[i].y += 0.15 * mv;
		if (g_mikatatama[i].type == 3) g_mikatatama[i].x += 0.2 * mv, g_mikatatama[i].y += 0.2 * mv;
		if (g_mikatatama[i].type == -3)g_mikatatama[i].x -= 0.2 * mv, g_mikatatama[i].y += 0.2 * mv;
		if (g_mikatatama[i].y < -10) g_mikatatama[i].living = FALSE;

		//�e�̃^�C�v��99�܂łȂ�΃i�C�t�Œe��\��
		if(g_mikatatama[i].type < 99)DrawGraph((int)g_mikatatama[i].x-25, (int)g_mikatatama[i].y-25, g_tamaimg, TRUE);//�������e���i�C�t�Ȃ�
		for (int x = 0; x < MAX_TEKI; x++) {//�����蔻��
			if (g_hontai[x].living == TRUE) {

				BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[i].x, g_mikatatama[i].y, 20);//�G�Ƃ̍��W���瓖�������������߂�

				if (hantei == TRUE && g_mikatatama[i].type < 99) {
					g_mikatatama[i].living = FALSE;//�e�����ł�����
					g_hontai[x].lifecount = g_hontai[x].lifecount - 1;//�G��1�_���[�W��^����
					if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//�����|���āA�����ꂪ�^�C�v2�Ȃ�p���[�A�b�v�A�C�e����^����
						int u;
						for (u = 0; u < 50; u++) {
							if (g_items[u].living == FALSE) break;
						}
						if (u < 50) {
							g_items[u].living = TRUE;//�A�C�e���������Ă���Ƃ�������ɂ���
							g_items[u].x = g_hontai[x].x;
							g_items[u].y = g_hontai[x].y;
							g_items[u].speedx = 0;
							g_items[u].speedy = -0.2f;
							g_items[u].type = 1;//type1�Ńp���[�A�b�v�A�C�e��

						}
					}

					if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//�����|���āA�����ꂪ�^�C�v4�Ȃ�X�s�[�h�A�b�v�A�C�e����^����
						int u;
						for (u = 0; u < 50; u++) {
							if (g_items[u].living == FALSE) break;
						}
						if (u < 50) {
							g_items[u].living = TRUE;//�A�C�e���������Ă��邱�Ƃɂ���
							g_items[u].x = g_hontai[x].x;
							g_items[u].y = g_hontai[x].y;
							g_items[u].speedx = 0;
							g_items[u].speedy = -0.2f;
							g_items[u].type = 2;//type2�ŃX�s�[�h�A�b�v�A�C�e��

						}
					}

				}

			}
		}
		//�e����ʊO�ɏo���琶����������
		if (g_mikatatama[i].x > 800 || g_mikatatama[i].x < -50 || g_mikatatama[i].y > 700 || g_mikatatama[i].y < -50)g_mikatatama[i].living = FALSE;
	}
	
	
	}

void DrawItems() {    //�A�C�e���`��
	for (int i = 0; i < 50; i++) {
		if (g_items[i].living == FALSE) continue;
		if (g_items[i].y < -10) g_mikatatama[i].living = FALSE;//�A�C�e������ɍs���߂���Ώ���
			g_items[i].speedy += 0.02;//�A�C�e����y�����̃X�s�[�h�A�����������^��
			g_items[i].y += g_items[i].speedy;
			g_items[i].x += g_items[i].speedx;


		
			if (g_items[i].type == 1) {//�A�C�e����type��1�Ȃ�
				
				DrawGraph(g_items[i].x - 25, g_items[i].y - 25, g_PowerSterimg, TRUE);//�p���[�A�b�v�X�^�[�̉摜��\������
			}
			if (g_items[i].type == 2) {//�A�C�e����type��2�Ȃ�
							
				DrawGraph(g_items[i].x - 25, g_items[i].y - 25, g_SpeedSterimg, TRUE);//�X�s�[�h�A�b�v�X�^�[�̉摜��\������
			}
			

			BOOL atariitem = AtariinfoH(g_items[i].x, g_items[i].y , 25);//�����蔻��
			if (atariitem == TRUE) {//�����蔻�肪TRUE�Ȃ�
				if(g_items[i].type == 1)  g_shotrank ++;//�A�C�e����type��1�Ȃ�V���b�g�����N�̏㏸
				if (g_items[i].type == 2) g_hspeedrank ++;//type2�Ȃ�X�s�[�h�����N�̏㏸
				g_items[i].living = FALSE;//������������


			}


			

	};
}

void drawslash() {//�V�[���h�\�[�h�̃A�j���[�V����
	if (g_drawanimation[0].onoff == 1) {//�������V�[���h�\�[�h�̃A�j���[�V�������肪1�Ȃ��
		int time = ((g_count - g_drawanimation[0].time)) % 4;//����g_drawanimation�i�V�[���h�\�[�h�𔭓��������Ԃ��g���āA�o�߂��J�E���g����j��4�Ŋ���
		if (time >= 3) {
			g_drawanimation[0].onoff = 0;
		}
		else {
		DrawGraph(g_hx - 251, g_hy - 346, g_slashimg[time], TRUE);//�A�j���[�V�����̕`��Atime�ŕ`�ʂ���R�}��ݒ�
	}
	}
}

BOOL AtariinfoH(int sx, int sy,int size) {//��l���ɑ΂��铖���蔻��

	float a = sx - g_hx;//��l���Ƒ���̈ʒu��X���W�̍�
	float b = sy - g_hy;//��l���Ƒ����Y���W�̍�
	float c = a * a + b * b;//�ʒu�̍����y������
	float sum_radius = size + 15;
	if (c <= sum_radius * sum_radius) {//�������ʒu�̍����A��l���ƓG�̔��a���傫����΁ATRUE��Ԃ�
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL atariinforey(float r, float ax, float ay, float vx, float vy) {//�����蔻��A���[�U�[�p
	if (r < 0)return false;//������0�ȉ��Ȃ�FALSE��Ԃ�

	if (vx == 0.0f, vy == 0.0f) return false;//0,0�Ȃ�t�@���X��Ԃ�
	

	ax -= g_hx;
	ay -= g_hy;//�~�̒��S�_�����_�ɂȂ�悤�ɊJ�n�_���I�t�Z�b�g(�␳����)

	float len = sqrtf(vx * vx + vy * vy);
	vx /= len;
	vy /= len;

	// �v��t���Z�o
	float dotAV = ax * vx + ay * vy;
	float dotAA = ax * ax + ay * ay;
	float s = dotAV * dotAV - dotAA + r * r;
	if (fabs(s) < 0.000001f) s = 0;

	if (s < 0.0f) return false;//�Փ˂��Ă��Ȃ�

	float sq = sqrtf(s);
	float t1 = -dotAV - sq;
	float t2 = -dotAV + sq;

	//����t1�y��t2���}�C�i�X��������n�_���~���ɂ߂荞��ł�̂ŃG���[�ɂȂ�
	if (t1 < 0.0f || t2 < 0.0f)return false;
	 return true;






}

BOOL AtariinfoSo(int ax, int ay, int as, int bx, int by, int bs) {//��l���ȊO�̓����蔻��Ax,y��x���W�Ay���W
	float a = ax - bx;//���ꂼ��̍��W�̍������߂�
	float b = ay - by;
	float c = a * a + b * b;//��܂��ȋ��������߂�
	float sum_radius = as + bs;
	if (c <= sum_radius * sum_radius) return TRUE;//���������̑�܂��ȋ�������̔��a��菬�������TRUE
	else {
		return FALSE;
	}
}

BOOL Atariinfoougi(int ax, int ay, int bx, int by, int lengh) {//��`�̓����蔻��A�V�[���h�\�[�h�̔���Ɏg��
	float a = ax - bx;
	float b = ay - by;
	float c = a * a + b * b;//��܂��ȋ�����������̓�̔��a��菬������΁A�q�b�g
	if (c <= lengh * lengh) {

		float kakuhantei = kakudo2(ax, ay, bx, by);//��̊Ԃ̊p�x�����߂�B���̊p�x�Ő�̒��ɓ����Ă���Γ�����
		if (kakuhantei >= PII / 4 * (-1) && kakuhantei <= PII * 5 / 4) {//�q�b�g�������ŁA��ɓ����Ă�����݂̂̂�I��
			return TRUE;
		}
	}
	return FALSE;
}


float directioninfosin(int x, int y) {//�������isin�j
	int dex = g_hx - x;//�e���W�̍������߂�
	int dey = g_hy - y;
	float dis = sqrt(dex * dex + dey * dey);//���ۂ̋��������߂�

	float sinr = dey / dis;//Y���W�̍��������Ŋ���Asin�����߂�
	return sinr;
	

}

float directioninfocos(int x, int y) {//�������icos�j
	int dex = g_hx - x;//�e���W�̍������߂�
	int dey = g_hy - y;
	float dis = sqrt(dex * dex + dey * dey);//���ۂ̋��������߂�

	float cosr = dex / dis;//X���W�̍��������Ŋ���Acos�����߂�
	return cosr;
}


float kakudo(int x, int y) {//��l���Ƃ̊p�x�����߂�
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = g_hx - x;//��l���Ƃ̍��W�̍������߂�
	dey = g_hy - y;
	angle = acos(dex / sqrt(dex * dex + dey * dey));//�����Ŋp�x�����߂�
	//angle = angle   * 180 / PI;

	if (dey < 0) angle = PI * 2 - angle;

	return angle;

}


float kakudo2(int x, int y, int aitex, int aitey) {//��̂��̂Ƃ̊p�x�����߂�
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = aitex - x;//��̂��̂̍��W�̍������߂�
	dey = aitey - y;
	angle = acos(dex / sqrt(dex * dex + dey * dey));//�����Ŋp�x�����߂�

	if (dey < 0) angle = PI * 2 - angle;

	return angle;
}

float angle1(int x, int y) {//�p�x�����߂�
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = g_hx - x;//��l���Ƃ̍��W�̍������߂�
	dey = y - g_hy;
	angle = acos(dex / sqrt(dex * dex + dey * dey));
	//angle = angle   * 180 / PI;

	if (dey < 0) angle = PI * 2 - angle;//�����Ŋp�x�����߂�

	return angle;

}


float angle2(int x, int y, int aitex, int aitey) {//��̂��̂Ƃ̊p�x�����߂�
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = aitex - x;//��̂��̂̍��W�̍������߂�
	dey = y - aitey;
	angle = acos(dex / sqrt(dex * dex + dey * dey));

	if (dey < 0) angle = PI * 2 - angle;//�����Ŋp�x�����߂�

	return angle;
}


int nearestenemy(int cd) {//�ł��߂��G����������
	static float nearest = 1000000000;
	static int nearestnumber = 10005;
	static float a = 0;
	static float b = 0;
	static float c = 0;
	int i = 0;
	for ( i = 0; i < MAX_TEKI; i++) {//�G���̋����𑪒�A�ł��߂����̂��L�^����
		if (g_hontai[i].living == FALSE)continue;
		float a =abs(g_mikatatama[cd].x - g_hontai[i].x);
		float b = abs(g_mikatatama[cd].y - g_hontai[i].y);
		float c = a  +  b;
		if (c <= nearest) {
			nearest = c;
			nearestnumber = i;
		}
	}
	return nearestnumber;
}

int random(int lengh) {//�����_���Ȓl��Ԃ��i���Ԃ����ɂ��ă^�l�����j
	int r = g_lasttime % lengh;
	return r;
}

int numberrandom(int lengh, int seed) {//�����_���Ȓl��Ԃ��i������Ń^�l���Z�b�g���A����Ǝ��ԂƂŗ��������j
	float r = seed * seed *(sin(g_lasttime)*sin(seed * g_lasttime) / cos(seed * g_lasttime));
	int seir = r;
	int kaeri = seir % lengh;
	return kaeri;
}


void tamaset(int number, int  type, int x, int y, float speedx, float speedy) {//�e�𐶂ݏo�����߂̊֐�
	g_tekitama[number].living = TRUE;//�e�̐�����ON�ɂ���
	g_tekitama[number].type = type;//�e�̃^�C�v�̐ݒ�
	g_tekitama[number].x = x;//�e�̏�����X���W
	g_tekitama[number].y = y;//�e�̏�����Y���W
	g_tekitama[number].speedx = speedx;//�e��X�������ł̃X�s�[�h
	g_tekitama[number].speedy = speedy;//�e��X�������ł̃X�s�[�h
	g_tekitama[number].timer = g_lasttime;//�e���������Ă��鎞�Ԃ𑪒肷�邽�߂̕ϐ�
	g_tekitama[number].counter = g_count;//�e�̐������Ă���t���[����ݒ肷�邽�߂̕ϐ�
	g_tekitama[number].reverse = FALSE;//�e�̃g���K�[�ireverse�j��FALSE�ɂ���

}