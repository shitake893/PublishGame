#include "gamemain.h"

void GameMain() {//ここをループさせて全処理を行う
	g_count++;//進行管理のカウントを増やす


	DamageControl();//ダメージ管理
	MapEffect();//マップによるエフェクト（今回は実装なし）
	DrawHero();//主人公の動作
	DrawMikatatama();//味方の弾の動作
	DrawEnemy();//敵の動作
	DrawItems();//アイテムの動作
	DrawTekitama();//敵の弾の動作
	drawslash();//必殺技、シールドソードの動作

	clsDx();
	printfDx("主人公ライフ　　  %d", g_herolife);
	
	//Zキーをチェックして画面を切り替え
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//if (IsCKeyTrigger(key) == TRUE) {
	//	g_gamestate = GAME_OVER;
	//	g_timerstart = g_lasttime;	//タイマーセット
	//}
	if (g_herolife <= 0) {  //ライフがゼロになった時にゲームオーバー
		g_gamestate = GAME_OVER;
		g_timerstart = g_lasttime;//タイマーセット
	}
	if (g_gekihacounter == 2 && g_lasttime - g_gekihatime  >= 2000) {
		g_timerstart = g_lasttime;//タイマーセット
		g_gamestate = GAME_CLEAR;

	}
}

void DrawHero() {//主人公の動作の管理
	
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	Drawtama(key);//弾を放つ（Zボタン）
	Drawreflection(key);//シールドソードを放つ（Xボタン）
	float mv = 200.0f * g_frametime +(30 * g_hspeedrank) * g_frametime; //移動量計算
	if (g_hspeedrank >= 5) mv = 200.0f *g_frametime + 30 * 6 * g_frametime ;//スピードランクが最大を超えた場合（5以上）のスピード

	//仮の座標を代入
	float herox = g_hx;
	float heroy = g_hy;
	//仮の座標を移動させる
	if (key & PAD_INPUT_UP)		heroy = g_hy - mv;
	if (key & PAD_INPUT_DOWN)   heroy = g_hy + mv;
	if (key & PAD_INPUT_LEFT)	herox = g_hx - mv;
	if (key & PAD_INPUT_RIGHT)	herox = g_hx + mv;
	//移動可能な場所なら仮の座標を本当の座標とする
	if (heroy >= -25 && heroy <= 600 - 25) {
		g_hy = heroy;
	}
	if (herox >= -25 && herox <= 800 - 25) {
		g_hx = herox;
	}

	switch (g_herostate) {//主人公の外見の変化、通常ならなにも変化なく表示され、ダメージを受けたら一定時間点滅、及び被弾を受け付けない

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

void DamageControl() {//ダメージ状態からの回復のための関数、（主人公の状態がDAMAGEDならダメージを受け付けない、）
if (g_herostate == DAMAGED) {
	if (g_damagetimer < g_lasttime - 3000) {//ダメージを受けて3000ミリ秒経ったならNOMALの状態に戻す
		g_herostate = NOMAL;
	}
}
}


void MapEffect() {//マップによる影響を調べるための関数、今回は何もしていない

//	tekitati saisyo;
//	saisyo.x = 0;
//	saisyo.y = 0;
//	saisyo.type = 1;
	//saisyo.scale = 50;

};


	//敵を出現させる
void DrawEnemy() {
	

	for (int i = 0; i < MAX_TEKI; i++) {
		if(g_hontai[i].living == 0){
		if (g_hontai[i].dead == FALSE) {//登場させる敵の情報からまだ生きていない、かつ撃破されていないものを探す
			if (g_hontai[i].derutime != 0) {//出現時間が0以外で
				for (int j = 1; j < 10; j++) {
					//出現時間になったかどうかを確認、もしも中ボスを倒してから（gekihacounterが1以上）ならそれを倒した時間から測定を行う、
					//（それまでは10000の定数を出るまでの時間に追加して出ないようにしている（1秒で10進））
					if (g_hontai[i].derutime * 100 + g_gametime < g_lasttime ||( g_gekihacounter == j      && (g_hontai[i].derutime - j * 10000) * 100 + g_gekihatime < g_lasttime)) {
						g_hontai[i].living = 1;//生存判定にする
						g_hontai[i].counter = g_count;//作られた時間をカウントする（フレーム判定）
						g_hontai[i].timer = g_lasttime;//作られた時間をカウントする（秒判定）
						if (g_hontai[i].type == 102) {//もしもラスボスだったら取り巻きも同時に召喚させる。
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
							g_hontai[i].angles = PII / 2;//向く角度を決定させる
							}
						break;
					};
				}
				
			}
			}
		}
	}

	for (int i = 0; i < MAX_TEKI; i++) {//各タイプごとの敵の動作を実行させる。
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
			//敵の座標をX,Y軸方向のスピード分追加する
			g_hontai[i].x += g_hontai[i].speedx;
			g_hontai[i].y += g_hontai[i].speedy;
			if (g_hontai[i].x < -100 || g_hontai[i].x > 900 || g_hontai[i].y < -100 || g_hontai[i].y > 700) g_hontai[i].dead = TRUE;//画面外に出たなら敵を殺す
			if (g_hontai[i].lifecount <= 0) {//敵のライフが0になったら殺す
				g_hontai[i].dead = TRUE;
				if (g_hontai[i].type > 100) g_gekihacounter++ , g_gekihatime = g_lasttime ;//倒した敵がボスだったらg_gekihacounter（ステージのフェーズ的なものを）進行させる

			}
			if (g_hontai[i].dead == TRUE) g_hontai[i].living = 0;//もしも殺された判定なら生存判定を0（偽）にする
		}
	}
	//パラメーター表示、下のX,Yは各段落を作るためのもの
		int x = 0;
		int y = 0;
	//	for (int i = 0; i < 10; i++) {
	//		DrawFormatString(x, y, 0xFFFFFF, "%10f %10f %10d %10d %10d %10d",
	//			g_hontai[i].x, g_hontai[i].y, g_hontai[i].living, g_hontai[i].type, g_hontai[i].lifecount, g_hontai[i].derutime);// 出力

//			y += 20;
	//	}
		//DrawFormatString(600, 500, 0xFFFFF, "パワー %10d", g_shotrank);
		DrawFormatString(600, 520, 0xFFFFF, "主人公ライフ : %d", g_herolife);
		DrawFormatString(600, 540, 0xFFFFF, "ショットパワー : %d", g_shotrank);
		DrawFormatString(600, 560, 0xFFFFF, "スピードランク : %d", g_hspeedrank);
		//DrawFormatString(600, 480, 0xFFFFF, "カウント計測 %10d", ((g_count - g_drawanimation[0].time) / 10) % 3);
		


		//当たり判定及び敵の画像表現
		for (int i = 0; i <= MAX_TEKI; i++) {//全敵分判定を行う

			if (g_hontai[i].living == TRUE) {//生存している敵のみ以下の処理を実行
			//if(	g_hontai[i].type < 10){
				//敵の色を指定
				int boxcol = GetColor(100, 200, 0);
				if (g_hontai[i].type == 2) boxcol = GetColor(250, 0, 0);
				if (g_hontai[i].type == 4) boxcol = GetColor(150, 150, 250);
				if (g_hontai[i].type == 5) boxcol = GetColor(180, 180, 50);
				if (g_hontai[i].type == 6) boxcol = GetColor(250, 100, 100);
				DrawBox(g_hontai[i].x -25, g_hontai[i].y -25, g_hontai[i].x + 25, g_hontai[i].y + 25, boxcol, TRUE);//敵である四角を描写（色はboxcolの色）
				if (g_herostate == NOMAL) {//もしも主人公の状態がNOMAL（ダメージを受けた直後ではない）ならダメージ

					BOOL tekiatari = AtariinfoH(g_hontai[i].x, g_hontai[i].y, 25);//主人公の座標と敵の位置から当たっているかを判定
					if (tekiatari == TRUE) {//当たり判定が真だったらダメージを与える
						g_herostate = DAMAGED;
						g_herolife -= 1;
						g_damagetimer = g_lasttime;
					}
					}
					
				}
			//}
		}






	}


void DrawTekitama() {//敵の玉に関する処理
	int i;
	for (i = 0; i < MAX_TEKI; i++) {//全ての配列で処理を行う
		if (g_tekitama[i].living == TRUE) {//生きている弾のみ次の処理を行う
			//今の座標に各軸方向のスピード分追加する
			g_tekitama[i].x += g_tekitama[i].speedx;
			g_tekitama[i].y += g_tekitama[i].speedy;
			
			if (g_tekitama[i].x < -100 || g_tekitama[i].x > 900 || g_tekitama[i].y < -100 || g_tekitama[i].y > 700) g_tekitama[i].living = FALSE;//画面外に出たら消す
			//if (g_tekitama[i].timer < g_lasttime - 10000) g_tekitama[i].living = FALSE;

			switch (g_tekitama[i].type) {//各タイプごとに処理を行う
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
		
	
void tekitama1(int i) {//弾のパターン1、ただ与えられた方向に直進する
	int boxcol = GetColor(250, 250, 170);
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 12, boxcol, TRUE);
	//以下当たり判定処理
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

void tekitama2(int i) {//弾のパターン2、追尾弾（紫色で跳ね返せない、軌跡がある）
	int boxcol = GetColor(200, 100, 250);
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);

	if (g_herostate == NOMAL) {
		//当たり判定処理
		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}

	//for (int i = 0; i < 1000; +i++) {   //個々も無力化してた
		//if (g_kisekidata[i].living == FALSE){
	g_kisekidata[i].living = TRUE;//この弾用の軌跡のデータの配列の生存をTRUEにする
	//int kisekinumber = i;
//	break;

//		}
	//	}       //ここまで無力化してた

	//X軸、Y軸の主人公との座標の差を求め、それを元に主人公との距離を求める
	int dex = g_hx - g_tekitama[i].x;
	int dey = g_hy - g_tekitama[i].y;
	float dis = sqrt(dex * dex + dey * dey);

	//主人公の座標と弾の座標を用いて、その三角比を求める
	float sinr = dey / dis;
	float cosr = dex / dis;
	float hikiyosehi = 0.5;
	if (g_count - g_tekitama[i].counter > 200) hikiyosehi = 0;//ある程度の時間が経つと追尾を修了する
	//この弾のスピードに三角比で求めた数値*引き寄せ力（hikiyosehi）を追加する
	g_tekitama[i].speedx += hikiyosehi * cosr;
	g_tekitama[i].speedy += hikiyosehi * sinr;
	//もしも各座標のスピードが5を超えたらそこでストップする
	if (g_tekitama[i].speedx > 5) g_tekitama[i].speedx = 5;
	if (g_tekitama[i].speedy > 5) g_tekitama[i].speedy = 5;



	//もしも味方の弾とぶつかったら互いに消滅する
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
	for (int j = 0; j < 100; j++) {//軌跡を描くkisekidataで各弾に割り当て垂れた軌跡の配列、その中に実際に描く軌跡用の配列がある。
		if (g_tamakisekidata[i].id[j].living == FALSE) {
			g_tamakisekidata[i].id[j].living = TRUE;
			g_tamakisekidata[i].id[j].x = g_tekitama[i].x;
			g_tamakisekidata[i].id[j].y = g_tekitama[i].y;
			g_tamakisekidata[i].id[j].counter = g_count;
			break;
		}
	}


	//}
	for (int k = 0; k < MAX_TAMA; k++) {//こちらで軌跡で描いた円が徐々に小さくなるようにする
		if (g_tamakisekidata[i].id[k].living == TRUE) {
			int konokiseki = GetColor(200, 100, 250);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//この時の数（7）までのカウントなら大きさは一定、だがそれ以降だと徐々に小さくなる
			if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
				ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 8);
			}
			DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//ここで実際に軌跡を描写する。
			if (g_count - g_tamakisekidata[i].id[k].counter > 15) g_tamakisekidata[i].id[k].living = FALSE;
		}
	}

	if (g_tekitama[i].living == FALSE) {//弾本体が消えたなら軌跡も一斉に消す
		g_tamakisekidata[i].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[i].id[b].living = FALSE;
		}
	}
}

void tekitama3(int i) {//弾のパターン3、一定時間たつと主人公の方向に方向転換してくる。
	int boxcol = GetColor(150, 250, 150);//色の指定
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);//弾の描写

	g_kisekidata[i].living = TRUE;//軌跡用の関数の生存をTRUEにする

	if (g_herostate == NOMAL) {//ダメージ判定

		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}





	if (g_lasttime - g_tekitama[i].timer > 300) {//一定時間（300たったならreverseトリガーをONにし、方向転換を行う（このreverseトリガーが一度ONになるともう発動しない。））
		if (g_tekitama[i].reverse == FALSE) {
			g_tekitama[i].speedx = 5 * cos(kakudo(g_tekitama[i].x, g_tekitama[i].y));
			g_tekitama[i].speedy = 5 * sin(kakudo(g_tekitama[i].x, g_tekitama[i].y));
			g_tekitama[i].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//2フレームごとに軌跡を追加する
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
		for (int k = 0; k < MAX_TAMA; k++) {//軌跡の描写のための部分、一定時間たつと軌跡が小さくなり消える
			if (g_tamakisekidata[i].id[k].living == TRUE) {
				int konokiseki = GetColor(160, 230, 160);
				int ookisa = 0;
				if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//軌跡が発生してから7フレーム目までは軌跡はそのまま
				if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
					ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 16);//それ以降は徐々に小さくなる
					konokiseki = GetColor(180, 200, 180);//さらに7フレームより先は軌跡の色を変化させる
				}
				DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//軌跡を実際に描写
				if (g_count - g_tamakisekidata[i].id[k].counter > 23) g_tamakisekidata[i].id[k].living = FALSE;//23フレーム以降で軌跡の生存をFALSEにする
			}
		}

		if (g_tekitama[i].living == FALSE) {//弾本体が消えれば軌跡も同時に消す。
			g_tamakisekidata[i].living = FALSE;
			for (int b = 0; b < MAX_TAMA; b++) {
				g_tamakisekidata[i].id[b].living = FALSE;
			}
		}
}

void tekitama4(int i) {//弾のパターン4、乱数ありの一回方向追尾弾
	int boxcol = GetColor(150, 250, 150);//色指定
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 15, boxcol, TRUE);//弾を描写

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





	if (g_lasttime - g_tekitama[i].timer > 300) {//一定時間になったら主人公方向へ方向転換する
		if (g_tekitama[i].reverse == FALSE) {
			g_tekitama[i].speedx = 5 * cos(kakudo2(g_tekitama[i].x, g_tekitama[i].y, g_hx + (80 - random(160)), g_hy) );//横軸を決定、（距離における各軸の要素から決定）randomで乱数をつくる
			g_tekitama[i].speedy = 5 * sin(kakudo2(g_tekitama[i].x, g_tekitama[i].y, g_hx + (80 - random(160)), g_hy) );//縦軸を決定
			g_tekitama[i].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//軌跡の配列に情報を追加する
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
	for (int k = 0; k < MAX_TAMA; k++) {//軌跡を実際に描写する
		if (g_tamakisekidata[i].id[k].living == TRUE) {
			int konokiseki = GetColor(160, 230, 160);//軌跡の色を入力
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[i].id[k].counter) <= 7) ookisa = 14;//7フレーム目までは大きさに変化なし
			if ((g_count - g_tamakisekidata[i].id[k].counter) > 7) {
				ookisa = 14 - ((g_count - g_tamakisekidata[i].id[k].counter - 7) * 14 / 16);//7フレームより先は徐々に小さくしていく
				konokiseki = GetColor(180, 200, 180);//7フレームより先は色を変化させる。
			}
			DrawCircle(g_tamakisekidata[i].id[k].x, g_tamakisekidata[i].id[k].y, ookisa, konokiseki, TRUE);//ここで実際に描写させる
			if (g_count - g_tamakisekidata[i].id[k].counter > 23) g_tamakisekidata[i].id[k].living = FALSE;//一定時間で生存をFALSEに
		}
	}

	if (g_tekitama[i].living == FALSE) {//弾本体が消えたら軌跡も消す
		g_tamakisekidata[i].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[i].id[b].living = FALSE;
		}
	}
}

void tekitama5(int i) {//弾のパターン5、一回バウンド弾
	DrawCircle(g_tekitama[i].x, g_tekitama[i].y, 12, GetColor(200,200,150), TRUE);
	if (g_tekitama[i].reverse == FALSE) {//reverseをつかい、まだバウンドしていないもののみバウンドさせる。
		if (g_tekitama[i].x >= 800 || g_tekitama[i].x <= 0){//もしも弾のx座標が800を超えたか、X軸が0以下ならX軸方向のスピードを反転させる
			g_tekitama[i].speedx *=  -1;
			g_tekitama[i].reverse = TRUE;//reverseをTRUEにし、これ以上バウンドしないようにする
			}
		if (g_tekitama[i].y >= 600 || g_tekitama[i].y <= 0) {//もしもY軸が600以上、または0以下ならばY軸方向のスピードを反転させる。
			g_tekitama[i].speedy *= -1;
			g_tekitama[i].reverse = TRUE;//reverseをTRUEにし、これ以上バウンドしないようにする
		}
	}
if (g_herostate == NOMAL) {//主人公との当たり判定の処理
		BOOL tekiatari = AtariinfoH(g_tekitama[i].x, g_tekitama[i].y, 12);
		if (tekiatari == TRUE) {
			g_herostate = DAMAGED;
			g_herolife -= 1;
			g_damagetimer = g_lasttime;
			g_tekitama[i].living = FALSE;

		}
	}
}


void DrawType1(int cd) {//敵のタイプ1、ただ斜めに移動するだけ
	g_hontai[cd].speedx = 2.0f;
	g_hontai[cd].speedy = 1.0f;
}

void DrawType2(int cd) {//敵のタイプ2下の方に強く移動するだけ
	g_hontai[cd].speedx = 1.0f;
	g_hontai[cd].speedy = 2.0f;
}

void DrawType3(int cd) {//敵のタイプ3、約1.5秒後に弾を発射する
	 

	if (g_hontai[cd].timer + 1500 > g_lasttime) {//1.5秒前までは下に降り続ける
		g_hontai[cd].speedy = 2.0f;
	}
	
		if ((g_hontai[cd].timer + 1500 <= g_lasttime) && (g_hontai[cd].timer  + 2500 > g_lasttime)) {//登場後1.5~2.5秒の間は弾を発射する
			g_hontai[cd].speedy = 0;//速度は0に
			g_hontai[cd].counter = g_count;//カウントを設定
			g_hontai[cd].counter %= 18;//18フレーム刻みにする
			
			if(g_hontai[cd].counter == 0 ){//その18フレームの間に一回弾を放つ
				for (int i = 0; i < 1000; i++) {//左下に弾を放つ
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
				for (int i = 0; i < 1000; i++) {//真下に弾を放つ
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
				for (int i = 0; i < 1000; i++) {//右下に弾を放つ
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
	
		if (g_hontai[cd].timer + 2500 <= g_lasttime) {//2.5秒後以上では上に帰っていく
			g_hontai[cd].speedy = -2;
		}
}


void DrawType4(int cd) {//敵のタイプ4こちらに突進してくる敵
	//主人公とのX,Y軸における距離を求める
	int dex = g_hx - g_hontai[cd].x;
	int dey = g_hy - g_hontai[cd].y;
	float dis = sqrt( dex * dex + dey * dey);
	//三角関数を使い、距離のうちの各軸の要素を求める
	float sinr = dey / dis;
	float cosr = dex / dis;
	float hikiyosehi = 0.4;
	if (g_count - g_hontai[cd].counter > 300) hikiyosehi = 0;
	//各軸の要素の比分、速度を追加する
	g_hontai[cd].speedx += hikiyosehi * cosr;
	g_hontai[cd].speedy += hikiyosehi * sinr;
	//各軸の速度の上限を設定
	if (g_hontai[cd].speedx > 4) g_hontai[cd].speedx = 4;
	if (g_hontai[cd].speedy > 4) g_hontai[cd].speedy = 4;
	
	//軌跡をつくる
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
	for (int i = 0; i < 1000; i++) {//軌跡の描写制御
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
	
	if (g_hontai[cd].living == FALSE) {//本体が消えれば軌跡も消えるようにする
		g_kisekidata[cd].living = FALSE;
	}

}

void DrawType5(int cd) {//敵のタイプ5紫の弾を放つ敵
	//速度設定
	g_hontai[cd].speedx = 0;
	g_hontai[cd].speedy = 3;
	int zerocount = g_count - g_hontai[cd].counter;//ここでこのこの敵が出てきた時間を0とするカウンターをつくる
	zerocount %= 60;//おおよそ60カウントに一回弾を放つ
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

void DrawType6(int cd) {//敵のタイプ6一定時間主人公の周りを囲み、最後に一方向追尾弾（緑色の弾）を放つ
	//DrawFormatString(600, 580, 0xFFFFF, "出た時間 %10d", g_hontai[cd].timer);
	if (g_count - g_hontai[cd].counter < 50)g_hontai[cd].speedy = 3;//最初の内は下に進んでいく
	//主人公への方角を定めるためにsin、cosの値を求める
	float sin = directioninfosin(g_hontai[cd].x, g_hontai[cd].y);
	float cos = directioninfocos(g_hontai[cd].x, g_hontai[cd].y);
	//主人公との距離を求める
	float distanse = sqrt((g_hontai[cd].x - g_hx) * (g_hontai[cd].x - g_hx) + (g_hontai[cd].y - g_hy) * (g_hontai[cd].y - g_hy));
	//登場してからの時間が4500ミリ秒経ったなら主人公から離れる
	if (g_lasttime > 4500 + g_hontai[cd].timer) {
		g_hontai[cd].speedy -= sin * 0.3;
		g_hontai[cd].speedx -= cos * 0.3;
		if (g_lasttime > 5000 + g_hontai[cd].timer) {//5000ミリ秒以上で緑球を一つ放つ
			if (g_hontai[cd].reverse == FALSE) {
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {
						g_tekitama[i].living = TRUE;//ここで弾をつくる
						g_tekitama[i].type = 3;
						g_tekitama[i].x = g_hontai[cd].x;
						g_tekitama[i].y = g_hontai[cd].y;
						g_tekitama[i].speedx = 0;
						g_tekitama[i].speedy = -3.2f;
						g_tekitama[i].counter = g_count;
						g_tekitama[i].timer = g_lasttime;
						g_hontai[cd].reverse = TRUE;//最後に弾を撃ったことをreverseで記録
						break;
					}
				}
			}
		}
	}
	else {//まだ4500ミリ秒経っていない場合は主人公に一定の距離をもち追いかける
		if (distanse > 150) {//主人公との距離が150より多い場合は追いかける
			g_hontai[cd].speedy += sin * 0.3;
			g_hontai[cd].speedx += cos * 0.3;
			//速度上限の設定
			if (g_hontai[cd].speedy >= 4) g_hontai[cd].speedy = 4;
			if (g_hontai[cd].speedy <= -4) g_hontai[cd].speedy = -4;
			if (g_hontai[cd].speedx >= 4) g_hontai[cd].speedx = 4;
			if (g_hontai[cd].speedy <= -4) g_hontai[cd].speedy = -4;
		}
		if (distanse >= 110 && distanse < 160) {//距離が140~160の間ではスピードを抑えるようにする
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
		if (distanse <= 110) {//主人公との距離が110以下であれば主人公から離れる
			g_hontai[cd].speedy -= sin * 0.5;
			g_hontai[cd].speedx -= cos * 0.5;
		}
		if((g_count - g_hontai[cd].counter) % 150 == 0){//150フレームごとに通常弾を一つ主人公に向けて放つ
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

void DrawType101(int cd) {//中ボス、
	//最初の50フレームは真下に降りる
	if (g_count - g_hontai[cd].counter < 50) g_hontai[cd].speedy = 1.5, g_hontai[cd].speedx = 0 ;

		if (g_hontai[cd].y < 120) g_hontai[cd].speedy += 0.2;//y座標が120未満なら速度を下に増やす
		else {
			if (g_hontai[cd].y < 130) g_hontai[cd].speedy += 0.05;//ｙ座標が130未満なら速度を下に少し増やす
			else if (g_hontai[cd].y > 140) g_hontai[cd].speedy -= 0.05;//y座標が１４０より上なら速度を上に少し増やす
			else g_hontai[cd].speedy -= 0.2;//y座標が140以上なら速度を上に増やす
		}

		//速度上限の設定
		if (g_hontai[cd].speedy > 2) g_hontai[cd].speedy = 2;
		if (g_hontai[cd].speedy < -2) g_hontai[cd].speedy = -2;
		
		//reverseを使い、敵のx座標における移動方向を決める
		if (g_hontai[cd].x < 500 && g_hontai[cd].reverse == TRUE) g_hontai[cd].speedx += 0.2;//左から右に行くとき
		if (g_hontai[cd].x >= 500) g_hontai[cd].reverse = FALSE;//右端に来たらreverseで移動方向を反転させる
		if (g_hontai[cd].speedx > 1.5) g_hontai[cd].speedx = 1.5;//速度上限の設定
		if (g_hontai[cd].x > 50 && g_hontai[cd].reverse == FALSE) g_hontai[cd].speedx -= 0.2;//右から左に行くとき
		if(g_hontai[cd].x <= 50) g_hontai[cd].reverse = TRUE;//左端に来たら、reverseを反転、移動方向を反転させる
		if (g_hontai[cd].speedx < -1.5) g_hontai[cd].speedx = -1.5;//速度上限の設定

		//カウンターの設定（ローカル）
		

		//左下、左、右、右下の順に記述、各それぞれ同時に、15フレームに一度通常弾を放つ
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
	


	if (g_count - g_hontai[cd].counter > 500) {//この敵のカウンターが500フレームを超えたら紫の追尾弾を放つ
		
		if (g_count % 60 == 0) {
			//追尾弾は2個同時に放つ
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

void DrawType102(int cd) {//最後のボス
	
	float PI = 3.141592;//円周率の設定
	static float konoangle = PI / 2;
	static float anglespeed = 0;//回転スピード（初期化）
	static BOOL tackle = 0;//タックルしたかの判定（初期化）
	static BOOL bumping = 0;//タックル時に壁にぶつかったかの判定
	static int kyorix = 0;//X軸の主人公との距離（一応初期化）
	static int kyoriy = 0;//Y軸の主人公との距離
	static int yattaka = 0;//0,2,2とふたつごとに変わるとする　緑弾を放つタイミングを決定する
	static BOOL keepstate = 1;//ボスの行動サイクルの最初に取り巻きが居たかどうか
	static int arufa = 0;


	if (g_hontai[cd].angles - PI / 2 >= 2 * PI) g_hontai[cd].angles -= 2 * PI;//角度が360を超えたときは０に戻す
	//(g_hontai[cd + 1].living == TRUE ||g_hontai[cd + 2].living == TRUE) ||
	//ボスの行動パターンのサイクル用の関数
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;
	int count = (g_count - g_hontai[cd].counter);//このボスのカウンターを設定
	if ((g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) || keepstate == 1) {
		if (g_hontai[cd].timer + 1000 > g_lasttime) {//最初の1000ミリ秒の行動、（下に降りる）
			g_hontai[cd].speedx = 0;
			g_hontai[cd].speedy = 2;
		}
		else {
			if (cycle < 1000) {//行動1、1000ミリ秒、3回広範囲に渡り通常弾を放つ
				if (g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) keepstate = 1;
				g_hontai[cd].speedx = 0;
				g_hontai[cd].speedy = 0;
				g_hontai[cd].reverse = TRUE;
				if (count % 20 == 0) {//20フレームごとに弾を放つ
					for (int i = 0; i < 10; i++) {//10つの弾を放つ
						for (int j = 0; j < MAX_TAMA; j++) {
							if (g_tekitama[j].living == FALSE) {
								float bure = (-0.5 + random(5)* 0.2);//弾のブレをつくる
								tamaset(j, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2,
									3 * -sin(345 * g_pi / (180) - (150 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2);//弾をつくる関数、
							
								break;
							}
						}
					}
				}
			}

			if (cycle < 3000 && cycle >= 1000) {//3000以下1000以上の段階
				g_hontai[cd + 1].reverse = TRUE;//取り巻きのreverseを外し、自由に行動させる
				g_hontai[cd + 2].reverse = TRUE;
			}
			if (cycle > 2000 && cycle <= 8000) {
				
				//2000~8000までで、緑球を放つ
				if (cycle - 2000 - yattaka * 250 >= 0 || (cycle - 2000 - (yattaka - 1) * 250 - 100 >= 0 && yattaka % 2 == 1)) {
					arufa++;
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 4;
							int bata = 1;
							if (yattaka % 2 == 1)bata = -1;//yattakaで弾を放った数を数え、bataの値を決定、

							g_tekitama[j].speedx = (3 - arufa / 5) * bata;//bataが負なら左向き、正なら右向きに放つ
							g_tekitama[j].speedy = 4 - arufa/2;//arufaを用いて徐々に弾速の下要素を減らしていく
							g_tekitama[j].reverse = FALSE;//ここから弾の初期化
							g_tekitama[j].timer = g_lasttime;
							yattaka++;
							break;
						}
					}


				}
			}//角度がより大きければ小さく、小さければ大きくする
			if (cycle > 8000 && cycle <= 13000) {//8000より上で13000以下でこちらに狙いを定めれる（こっちの方向を向く）
				arufa = 0;

				yattaka = 0;//yattakaの再初期化
				anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;//こちらの向きに合わせてgnglespeedを設定する
				g_hontai[cd].angles += anglespeed;//angleにanglespeedを追加
			}
			if (cycle > 13000 && cycle <= 16000) {//13000から16000まで、主人公に向けて突進する
				if (tackle == 0) {//まだ突進していないならば突進させる
					//スピードをこちらに向けて設定する
					g_hontai[cd].speedx = directioninfocos(g_hontai[cd].x, g_hontai[cd].y) * 10;
					g_hontai[cd].speedy = directioninfosin(g_hontai[cd].x, g_hontai[cd].y) * 10;
					tackle = 1;//突進したという判定をだす
				}
				if (g_hontai[cd].x > 700 || g_hontai[cd].x < 30 || g_hontai[cd].y > 550 || g_hontai[cd].y < 30) {//画面端に来たらそこで止まる
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					bumping = 1;//ぶつかったという判定を出す

				}
				if (bumping == 1 && cycle <= 16000) {//壁にぶつかり、サイクル16000以下
					kyorix = g_hontai[cd].x - 400;//本来いるべき位置との差を記録
					kyoriy = g_hontai[cd].y - 100;
					if (count % 20 == 0) {//20サイクルごとに弾を放つ
						for (int i = 0; i < 10; i++) {//10発弾を放つ
							for (int j = 0; j < MAX_TAMA; j++) {
								if (g_tekitama[j].living == FALSE) {
									g_tekitama[j].living = TRUE;
									g_tekitama[j].x = g_hontai[cd].x;
									g_tekitama[j].y = g_hontai[cd].y;
									g_tekitama[j].type = 1;
									float bure = (-0.5 + random(5)* 0.2);//ぶつかったのと反対側に、放射状に弾を放つ
									g_tekitama[j].speedx = 3 * cos(-g_hontai[cd].angles - (155 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
									g_tekitama[j].speedy = 3 * -sin(g_hontai[cd].angles - (75 + (-5 + numberrandom(10, j)) - (150 * i / 10)) * g_pi / (180)) + bure * 2;
									break;
								}
							}
						}
					}
				}


			}
			if (cycle >= 16000 ) {//さいくる16000以降
				bumping = 0;//タックルに使った変数の再初期化
				tackle = 0;

				g_hontai[cd].speedx = -kyorix * 0.01;//約100フレームかけて本来の位置に向かうようにする
				g_hontai[cd].speedy = -kyoriy * 0.01;
				if ((g_hontai[cd].x <= 410 && g_hontai[cd].x >= 390) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {//敵の位置がほとんど目的の場所に入った場合
					g_hontai[cd].speedx = 0;//スピードを0にする
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;//位置を本来の場所で固定する
					g_hontai[cd].y = 100;


					if (count % 10 == 0) {//10カウント刻みで弾を発射する
						for (int i = 0; i < MAX_TEKI; i++) {//真下に一つ通常弾を放つ
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
					if (g_hontai[cd].angles >= PI / 2 - 0.001 && g_hontai[cd].angles <= PI / 2 + 0.001) g_hontai[cd].angles = PI / 2;//敵のアングルがほとんど真正面なら固定する
					else {//そうでないなら真正面に近づける
						if (g_hontai[cd].angles >= PI / 2)g_hontai[cd].angles -= PI / 100;//角度がありすぎる場合は引く
						else g_hontai[cd].angles += PI / 100;//無さすぎる場合は足す
					}
				}
				
				if (cycle >= 22000) {//最後に取り巻きがいるかどうかを確認する、居なかったら第二形態へ
					if (g_hontai[cd + 1].living == TRUE || g_hontai[cd + 2].living == TRUE) keepstate = 1;
					else keepstate = 0;
				}
			}
			else {
				anglespeed = 0;
				
			}
		}
	}
	else {//二つとも取り巻きがやられた場合
		if (cycle < 2000) {//サイクル2000までは位置調整、通常弾を自身からみて下に放つ
			//角度、位置を適切な所（第二形態の初期位置）に合わせる
			if (g_hontai[cd].angles > PII / 2) g_hontai[cd].angles -= PII / 300;
			if (g_hontai[cd].angles <= PII / 2)g_hontai[cd].angles = PII / 2;
			if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;
			else g_hontai[cd].speedx = 1;
			if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;
			else g_hontai[cd].speedy = 1;
			//ほぼ初期位置になったら固定する
			if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {
				g_hontai[cd].speedx = 0;
				g_hontai[cd].speedy = 0;
				g_hontai[cd].x = 400;
				g_hontai[cd].y = 300;
			}
				g_hontai[cd].reverse = TRUE;
			if (count % 20 == 0) {//通常弾を広範囲にばらまくが、中心は放たない
				for (int i = 0; i < 10; i++) {
					if (i >= 3 && i <= 6) continue;//この3~6の間は飛ばし、弾を出さないようにする、これを繰り返し、変数1で少しずつ方向を変えていく
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
			//レーザーを放つ
				int color = GetColor(255, 255, 150);//レーザーの色を設定
				float arufa = (cycle) / 50;//レーザーの太さ、10以上にはならないようにする
				if (arufa > 10) arufa = 10;
				//if (cycle > 1000)arufa = 10 - (cycle - 2000) / 100;
				if (arufa < 0) arufa = 0;//レーザーの太さが0以下にもならないようにする
				DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//下向きレーザー
				if (g_herostate == NOMAL && arufa != 0) {//主人公がダメージ直後ではなくてarufaが0出ない時
					if (abs(g_hx - g_hontai[cd].x) < arufa) {//主人公と敵のX座標の差の絶対値がarufa未満ならダメージを受ける。
						g_herolife--;
						g_herostate = DAMAGED;
						g_damagetimer = g_lasttime;
					}
				}
		}
			if (cycle > 2000 && cycle <= 12000) {//サイクル2000以上12000未満
				g_hontai[cd].angles += PII / 300;//300フレームごとに一回転するスピードで回し続ける
				if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;//X座標が400より上なら座標を引く
				else g_hontai[cd].speedx = 1;//400より下なら下に座標を追加する
				if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;//Y座標が300より上なら下に下げる
				else g_hontai[cd].speedy = 1;//300より下なら下に下げる
				if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {//最適な位置に調整
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;
					g_hontai[cd].y = 300;
				}//四角形を構築
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
					dx, dy, GetColor(255, 255, 150), TRUE);//レーザー用の四角形を描写
				BOOL hantei = atariinforey(20, g_hontai[cd].x, g_hontai[cd].y, 600 * cos(g_hontai[cd].angles), 600 * sin(g_hontai[cd].angles));//レーザーとしての当たり判定
				//DrawLine(g_hontai[cd].x, g_hontai[cd].y, g_hontai[cd].x + 600 * cos(konoangle), g_hontai[cd].y + 600 * sin(konoangle),GetColor(255,255,255));
				if (hantei == TRUE && g_herostate == NOMAL) {//当たり判定に入っていて、主人公がダメージ直後でなければダメージ
					g_herolife--;
					g_herostate = DAMAGED;
					g_damagetimer = g_lasttime;
				}

				if (g_count % 10 == 0) {//さらに10フレームごとに弾を出し続ける
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 1;
							g_tekitama[j].reverse = FALSE;

							g_tekitama[j].speedx = 2 * cos(g_hontai[cd].angles + PII) + 1 - 0.1*random(20);//自身の向きとは反対側に弾を放つ
							g_tekitama[j].speedy = -2 * sin(g_hontai[cd].angles + PII) + 1 - 0.1*random(20);
							break;
						}
					}
				}

			}
			if (cycle > 12000 && cycle < 19000) {//サイクル12000~19000、自身がバウンドしながらバウンド弾を放つ
				
				if (g_count % 12 == 0) {//12フレームごとにバウンド弾を放つ
					for (int j = 0; j < MAX_TAMA; j++) {
						if (g_tekitama[j].living == FALSE) {
							g_tekitama[j].living = TRUE;
							g_tekitama[j].x = g_hontai[cd].x;
							g_tekitama[j].y = g_hontai[cd].y;
							g_tekitama[j].type = 5;
							g_tekitama[j].reverse = FALSE;

							g_tekitama[j].speedx = 4 * cos(g_hontai[cd].angles) + g_hontai[cd].speedx  + (0.5 - 0.1*random(10));//自身のスピードを足し引きして弾を出す
							g_tekitama[j].speedy = 4 * -sin(g_hontai[cd].angles) + g_hontai[cd].speedy + (0.5 - 0.1*random(10));
							break;
						}
					}
				}

				if (cycle > 12000 && cycle < 12500) {//バウンドするための初期速度
					g_hontai[cd].speedx = 3;
					g_hontai[cd].speedy = 3;
					anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;
					g_hontai[cd].angles += anglespeed;
				}
				anglespeed = (-(g_hontai[cd].angles)+kakudo(g_hontai[cd].x, g_hontai[cd].y)) / 100;//敵が主人公の方向を向くように方向スピードを調整
				g_hontai[cd].angles +=  anglespeed;

				//カベにぶつかったらぶつかった方向に反転させる
				if ((g_hontai[cd].x > 800 && g_hontai[cd].speedx >0)|| (g_hontai[cd].x < 50 && g_hontai[cd].speedx < 0))g_hontai[cd].speedx *= -1;
				if ((g_hontai[cd].y > 600 && g_hontai[cd].speedy > 0)||( g_hontai[cd].y < 50 && g_hontai[cd].speedy < 0 ))g_hontai[cd].speedy *= -1;
			}
			if (cycle > 19000) {//サイクル19000以上、紫の追尾弾を放ち続ける
				//レーザーの予備動作、照射地点を点滅させる
				if (cycle % 200 <= 100)
					DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(255, 50, 50), TRUE);
				//向きと位置を第二形態の初期位置に戻す
				if (g_hontai[cd].angles > PII / 2) g_hontai[cd].angles -= PII / 300;
				if (g_hontai[cd].angles <= PII / 2)g_hontai[cd].angles = PII / 2;
				if (g_hontai[cd].x > 400) g_hontai[cd].speedx = -1;
				else g_hontai[cd].speedx = 1;
				if (g_hontai[cd].y > 300)g_hontai[cd].speedy = -1;
				else g_hontai[cd].speedy = 1;
				//向きがほぼ初期位置になったら固定する
				if ((g_hontai[cd].x <= 310 && g_hontai[cd].x >= 290) && (g_hontai[cd].y <= 110 && g_hontai[cd].y >= 90)) {
					g_hontai[cd].speedx = 0;
					g_hontai[cd].speedy = 0;
					g_hontai[cd].x = 400;
					g_hontai[cd].y = 300;
				}
				if (g_count % 10 == 0 && cycle < 20000) {//10フレームごとに紫玉を放つ
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

	DrawRotaGraph(g_hontai[cd].x  , g_hontai[cd].y  ,1, g_hontai[cd].angles - PI/2 , g_Boss1img, TRUE);//このボスの描写、

}

void DrawType40(int cd) {//ボスの取り巻き1
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;
	int count = (g_count - g_hontai[cd].counter);//この敵用のカウンターを設置する

	if (g_hontai[cd].reverse == FALSE) {//このreverseがFALSEならばボスの座標を追随させる
		g_hontai[cd].x = g_hontai[cd - 1].x + 30*sqrt(2) * cos(g_hontai[cd-1].angles + 2.3599449 - PII / 2);
		g_hontai[cd].y = g_hontai[cd - 1].y + 30*sqrt(2) * sin(g_hontai[cd-1].angles + 2.3599449 - PII / 2);
	}
	
		if (cycle < 1000) {//サイクルが1000未満ならreverseをFALSE
			g_hontai[cd].reverse = FALSE;
		}
		if (cycle >= 1000 && cycle < 3000) {//1000~3000の間、事前照射（赤）を行う、また少し離れて下に下がる
			g_hontai[cd].reverse = TRUE;//位置の支配をボスから離れる
			if(cycle%200 <= 100)DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(150,0,0), 1);//位置を下にずらす
			if (g_hontai[cd].x > g_hontai[cd - 1].x - 30)g_hontai[cd].speedx = -0.3;
			else g_hontai[cd].speedx = 0;
			if (g_hontai[cd].y < g_hontai[cd - 1].y + 120)g_hontai[cd].speedy = 1.5;
			else  g_hontai[cd].speedy = 0;
		}
		if(cycle>= 3000 && cycle < 8000){//3000~8000の間、レーザーの本照射を真下に行う
			
			int color = GetColor(255, 255, 150);//レーザーの色指定
			float arufa = (cycle - 3000) / 50;
			if (arufa > 10) arufa = 10;//レーザーの太さ
			if (cycle > 5000)arufa = 10 - (cycle - 5000)/100;//サイクル5000以上ならば徐々に小さくする
			if (arufa < 0) arufa = 0;//arufaは0未満にはならない
			DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//レーザーの描写
			if (g_herostate == NOMAL && arufa!= 0) {//ダメージ判定
				if (abs(g_hx - g_hontai[cd].x) < arufa) {
					g_herolife -- ;
					g_herostate = DAMAGED;
					g_damagetimer = g_lasttime;
				}
			}
			if ( cycle > 6000) {//カウント6000以上、一旦元の場所に戻す
				//DrawFormatString(600, 400, 0xFFFFF, "カウント計測 %10f", g_hontai[cd].y);
				if (g_hontai[cd].x <= g_hontai[cd-1].x - 20 )g_hontai[cd].speedx = 0.6;//元の場所に戻るまでスピードを設定する（X座標について）
				else g_hontai[cd].speedx = 0;//元の場所に戻ったならスピードを0に
					
				if (g_hontai[cd].y <= g_hontai[cd-1].y + 20)g_hontai[cd].speedy = 0;//元の場所に戻るまでスピードを設定する（Y座標について）
				else g_hontai[cd].speedy = -3; 
					if(g_hontai[cd].x >= g_hontai[cd-1].x - 25 && g_hontai[cd].y <= g_hontai[cd-1].y + 25) g_hontai[cd].reverse = FALSE;//元の場所にもどったなら
					//reverseをFALSEに、ボスの座標に依存する
			}
		}
		if (cycle >= 8000 && cycle < 13000) {//8000~130000、弾を通常弾を放ち、ボスのタックルを補助する
			g_hontai[cd].reverse = FALSE;
			if (g_count % 15 == 0) {
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {//通常弾を発射する
						tamaset(i, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x +100 - random(200), g_hontai[cd].y + 100 -numberrandom(200,g_count))), 3 * sin(kakudo(g_hontai[cd].x +100-random(200), g_hontai[cd].y + 100 -numberrandom(200,g_count))));
						break;
					}
				}
			}
		}
		if (cycle >= 14000 && cycle < 16000) {//約突進後、緑の一回方向追尾弾を放ち、弾の回避を妨害する
			if (g_count % 40 == 0) {//40フレームに一度緑球を放つ
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {//弾を放つ処理
						tamaset(i, 3, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)), 3 * sin(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)));
						break;
					}
				}
			}
		}
		if (cycle > 16000) {//6000~バウンド弾を放つ
			if (g_count % 40 == 0) {//40フレーム毎に放つ
				for (int i = 0; i < MAX_TAMA; i++) {
					if (g_tekitama[i].living == FALSE) {
						tamaset(i, 5, g_hontai[cd].x, g_hontai[cd].y,  2 - random(4),  2 - 0.1 * numberrandom(40,g_count));
						break;
					}
				}
			}
		}
	
}

void DrawType41(int cd) {//ボスの取り巻き2
	int cycle = (g_lasttime - g_hontai[cd].timer - 1000) % 22500;//このreverseがFALSEならばボスの座標を追随させる
	int count = (g_count - g_hontai[cd].counter);//この敵用のカウンターを設置する
	

	//このreverseがFALSEならばボスの座標を追随させる
	if (g_hontai[cd].reverse == FALSE) {
		g_hontai[cd].x = g_hontai[cd - 2].x + 30 * sqrt(2) * cos(g_hontai[cd-2].angles + 0.7853981634 - PII / 2);
		g_hontai[cd].y = g_hontai[cd - 2].y + 30 * sqrt(2) * sin(g_hontai[cd-2].angles + 0.7853981634 - PII / 2);
	}

	if (cycle < 1000) {//サイクルが1000未満ならreverseをFALSE
		g_hontai[cd].reverse = FALSE;
	}
	if (cycle >= 1000 && cycle < 3000) {//1000~3000の間、事前照射（赤）を行う、また少し離れて下に下がる
		g_hontai[cd].reverse = TRUE;//位置の支配をボスから離れる
		if (cycle % 200 <= 100)DrawBox(g_hontai[cd].x - 10, g_hontai[cd].y, g_hontai[cd].x + 10, g_hontai[cd].y + 600, GetColor(150, 0, 0), 1);
		if (g_hontai[cd].x < g_hontai[cd - 2].x + 30)g_hontai[cd].speedx = 0.3;
		else g_hontai[cd].speedx = 0;
		if (g_hontai[cd].y < g_hontai[cd - 2].y + 120)g_hontai[cd].speedy = 1.5;
		else  g_hontai[cd].speedy = 0;
	}
	if (cycle >= 3000 && cycle < 8000) {//3000~8000の間、レーザーの本照射を真下に行う

		int color = GetColor(255, 255, 150);//レーザーの色指定
		float arufa = (cycle - 3000) / 50;//レーザーの太さの設定
		if (arufa > 10) arufa = 10;//レーザー太さの上限を設定
		if (cycle > 5000)arufa = 10 - (cycle - 5000) / 100;//レーザーを徐々に小さくする
		if (arufa < 0) arufa = 0;//大きさは0未満にはならない
		DrawBox(g_hontai[cd].x - arufa, g_hontai[cd].y, g_hontai[cd].x + arufa, g_hontai[cd].y + 600, color, 1);//レーザーの描写
		if (g_herostate == NOMAL && arufa != 0) {//ダメージの判定、主人公がダメージ直後でなければ、当たるとダメージ
			if (abs(g_hx - g_hontai[cd].x) < arufa) {
				g_herolife--;
				g_herostate = DAMAGED;
				g_damagetimer = g_lasttime;
			}
		}
		if (cycle > 6000) {//~6000、元の場所に戻す
			//DrawFormatString(600, 400, 0xFFFFF, "カウント計測 %10f", g_hontai[cd].y);
			if (g_hontai[cd].x >= g_hontai[cd - 2].x - 20)g_hontai[cd].speedx = -0.6;//X座標において、元の場所に無ければ戻していく
			else g_hontai[cd].speedx = 0;//元に戻ったらスピードを0に

			if (g_hontai[cd].y <= g_hontai[cd - 2].y + 20)g_hontai[cd].speedy = 0;//Y軸において、元の場所に無ければ戻していく
			else g_hontai[cd].speedy = -3;
			if (g_hontai[cd].x <= g_hontai[cd - 2].x + 25 && g_hontai[cd].y <= g_hontai[cd - 2].y + 25) g_hontai[cd].reverse = FALSE;//元に戻ったら再び位置をボスに依存
		}
	}
	if (cycle >= 8000 && cycle < 13000) {//8000~13000
		g_hontai[cd].reverse = FALSE;
		if (g_count % 15 == 0) {//15フレーム毎に通常弾を放つ
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {//通常弾の発射
					tamaset(i, 1, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 100 - random(200), g_hontai[cd].y + 100 - numberrandom(200, g_count))), 3 * sin(kakudo(g_hontai[cd].x + 100 - random(200), g_hontai[cd].y + 100 - numberrandom(200, g_count))));
					break;
				}
			}
		}
	}
	if (cycle >= 14000 && cycle < 16000) {//14000~16000、緑球を放ち、主人公の回避を妨害
		if (g_count % 40 == 0) {//40フレーム毎に放つ
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {//緑球を発射する
					tamaset(i, 3, g_hontai[cd].x, g_hontai[cd].y, 3 * cos(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)), 3 * sin(kakudo(g_hontai[cd].x + 20 - random(40), g_hontai[cd].y)));
					break;
				}
			}
		}
	}
	if (cycle > 16000) {//16000、バウンド弾を下らへんに放つ
		if (g_count % 40 == 0) {
			for (int i = 0; i < MAX_TAMA; i++) {
				if (g_tekitama[i].living == FALSE) {
					tamaset(i, 5, g_hontai[cd].x, g_hontai[cd].y, 2 - random(4), 2 - 0.1 * numberrandom(40, g_count));//若干ランダムに弾を撃つ
					//random,numberrandomで指定したブレ幅の乱数を生成する
					break;
				}
			}
		}
	}


}

void Drawtama(int key) {     //弾の打ち方
	g_movecount++;//弾を撃つタイミングを指定するための変数
	if (!(key & PAD_INPUT_B)) {//Xキーを押していない時のみ弾を発射できる
		switch (g_shotrank) {//ショットランクごとのショットの出方
		case 0://ショットランク0

			if (key & PAD_INPUT_A) {//Zボタンを押したら
				if (IsAKeyTrigger(key) == TRUE) {//ボタンの押し初めに、movecountを0にする
					g_movecount = 0;
				}
				g_movecount %= 10;//10フレームごとの弾をうつ
				if (g_movecount == 0) {
					int u;
					for (u = 0; u < MAX_TAMA - 10; u++) {
						if (g_mikatatama[u + 10].living == FALSE) break;
					}
					if (u < MAX_TAMA) {//弾は正面にうつ
						g_mikatatama[u + 10].living = TRUE;
						g_mikatatama[u + 10].x = g_hx;
						g_mikatatama[u + 10].y = g_hy;
						g_mikatatama[u + 10].type = 0;
					}
				}
			}

			break;
		case 1://ショットランク1、二発同時に放つ
			if (key & PAD_INPUT_A) {//ボタンを押されたら弾を放つ
				if (IsAKeyTrigger(key) == TRUE) {//押され初めにmovecountを0にする
					g_movecount = 0;
				}
				g_movecount %= 10;//10フレーム毎に弾を放つ
				if (g_movecount == 0) {
					int u;//弾の出る場所の指定のための変数
					for (int a = 0; a < 2; a++) {//二つ分の弾を放つ
						for (u = 0; u < MAX_TAMA - 10; u++) {//-10となっているのは、uが0の時に敵を放つ状態があったため
							if (g_mikatatama[u + 10].living == FALSE) {
								g_mikatatama[u + 10].living = TRUE;
								g_mikatatama[u + 10].x = g_hx - 15 + a * 30;//aにより、放つ場所を少しずらす
								g_mikatatama[u + 10].y = g_hy;
								g_mikatatama[u + 10].type = 0;
								break;
							}
						}
					}


				}
			}
			break;

		case 2://ショットランク2(最大)、3方向に弾を放つ
			if (key & PAD_INPUT_A) {//Zボタンで弾を放つ
				if (IsAKeyTrigger(key) == TRUE) {//ボタンの押し初めにmovecountを0にする
					g_movecount = 0;
				}
				g_movecount %= 30;//30フレーム毎にサイクルで弾を放つ
				if (g_movecount - 20 == 0 || g_movecount - 10 == 0 || g_movecount == 0) {
					int u;
					for (u = 0; u < MAX_TAMA - 10; u++) {//真っすぐ進む弾
						if (g_mikatatama[u + 10].living == FALSE) {
							g_mikatatama[u + 10].living = TRUE;
							g_mikatatama[u + 10].x = g_hx ;
							g_mikatatama[u + 10].y = g_hy ;
							g_mikatatama[u + 10].type = 0;
							break;
						}
					}
					for (int a = 1; a < 4; a++) {
						if (g_movecount - 20 + (a - 1) * 10 == 0) {//右に進む弾、ここで-20で0になる場合とで分かれる、弾の発射位置をそれで変える
							for (u = 0; u < MAX_TAMA - 10; u++) {
								if (g_mikatatama[u + 10].living == FALSE) {
									g_mikatatama[u + 10].living = TRUE;
									g_mikatatama[u + 10].x = g_hx  + 15;
									g_mikatatama[u + 10].y = g_hy + (a - 1) * 5;
									g_mikatatama[u + 10].type = 1 * a;
									break;
								}
							}
							for (u = 0; u < MAX_TAMA - 10; u++) {//左に進む弾
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
		default://ショットランクが設定されていないならランク2を放つ
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

void Drawreflection(int key) {//シールドソード（必殺技）の処理
	static int tame = g_lasttime;//溜める時間のための関数、初期化
	static int tamezikan = g_lasttime;//溜めている間に光るための関数、初期化


	//if (!(key & PAD_INPUT_B)) {
	//	tame = g_lasttime;
	//	tamezikan = g_count;
	//}
	if (key & PAD_INPUT_B) {//Ｘボタンを押されていたら
	
		

		if (IsAKeyTrigger(key) == TRUE) {//Ｘボタンの押され始め
			tame = g_lasttime;//溜め始めた時間を設定
		}
	    tamezikan = g_lasttime;
		if (tamezikan > tame + 1500) {
			if (g_count % 6 <= 3) {//およそ6フレームサイクルで点滅を繰り返させる
				DrawCircle(g_hx, g_hy, 50, GetColor(150, 150, 0), TRUE);
			}
		}
			}
	else {
		if (tamezikan > tame + 1500) {//Xボタンを押していない時
			BOOL hantei = 0;//シールドソードを使うかの判定
			if (g_drawanimation[0].onoff == 0) {
				g_drawanimation[0].onoff = 1;//シールドソードアニメーション関数の判定を1にする
				g_drawanimation[0].time = g_count;//シールドソードの時間を測定し始める
			}
			for (int i = 0; i < MAX_TAMA; i++) {//跳ね返す弾を設定
				if (g_tekitama[i].living == FALSE)continue;//弾が生きていないものなら除く
				hantei = Atariinfoougi(g_tekitama[i].x, g_tekitama[i].y, g_hx, g_hy, 300);//扇形の当たり判定を主人公の周りに出す
				if (hantei == TRUE) {//もしもその判定内に敵の玉があるなら
				
					int j;
					for ( j = 10; j < MAX_TAMA; j++) {
						if (g_mikatatama[j].living == FALSE)break;
					}//その弾の属性を新しい主人公の弾に明け渡し、元の弾を消す
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

void reflectedtama101(int c) {//跳ね返した弾、タイプ1通常弾
	g_mikatatama[c].x +=g_mikatatama[c].speedx;
	g_mikatatama[c].y += g_mikatatama[c].speedy;
	
	DrawCircle(g_mikatatama[c].x, g_mikatatama[c].y, 12, GetColor(50, 250, 50), TRUE);//色を指定して描写

	for (int x = 0; x < MAX_TEKI; x++) {
		if (g_hontai[x].living == TRUE) {//全ての生きている敵に対して当たり判定を判定する

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);

			if (hantei == TRUE ) {
				g_mikatatama[c].living = FALSE;
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//当たれば敵のライフを10削る
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//敵の種類がタイプ2だったらショットランクアップアイテムを落とす
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;//使っていない配列を探す
					}
					if (u < 50) {//アイテムを作る
						g_items[u].living = TRUE;
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//敵の種類がタイプ4だったらスピードランクアップアイテムを落とす
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;//使っていない配列を探す
					}
					if (u < 50) {//アイテムを作る
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


void reflectedtama102(int c) {//タイプ2、紫色の追尾弾を反射させた場合、（弾とはならず消滅する）

	float zanzousize = 10- ((g_lasttime - g_mikatatama[c].timer) / 40);//弾の大きさを指定、発生した時間が経つごとに小さくなる
	if (zanzousize <= 0)zanzousize = 0, g_mikatatama[c].living = FALSE;//弾の大きさが0以下になれば消滅
	g_mikatatama[c].speedx = 0;//弾のスピードはZ,Y共に0とする
	g_mikatatama[c].speedy = 0;
	g_mikatatama[c].x = g_mikatatama[c].x;//場所も変更しないでおく
	g_mikatatama[c].y = g_mikatatama[c].y;

	DrawCircle(g_mikatatama[c].x, g_mikatatama[c].y, zanzousize, GetColor(100, 250, 100), TRUE);//残像の描写
}


void reflectedtama103(int c) {//タイプ3緑の弾の反射
	g_mikatatama[c].x += g_mikatatama[c].speedx;//位置に速度を追加
	g_mikatatama[c].y += g_mikatatama[c].speedy;

	int tikaino = nearestenemy(c);//最も近い敵を検索

	if (g_lasttime - g_mikatatama[c].timer > 300) {//登場してから300ミリ秒が経ったら
		if (g_mikatatama[c].reverse == FALSE) {//方向転換をまだしたことが無ければ
			g_mikatatama[c].speedx = 5 * cos(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y,g_hontai[tikaino].x,g_hontai[tikaino].y));//スピードを最も近い敵に向けて変更する
			g_tekitama[c].speedy = 5 * sin(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x, g_hontai[tikaino].y));
			g_mikatatama[c].reverse = TRUE;//方向転換したことをreverseで示す
		}
	}




	if (g_count % 2 == 0) {//軌跡を描く
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
	for (int k = 0; k < MAX_TAMA; k++) {//軌跡の変化
		if (g_tamakisekidata[c].id[k].living == TRUE) {
			int konokiseki = GetColor(120, 230, 120);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[c].id[k].counter) <= 7) ookisa = 14;//登場後7フレームまでは軌跡は変化しない
			if ((g_count - g_tamakisekidata[c].id[k].counter) > 7) {//7フレーム後から徐々に小さくなる
				ookisa = 14 - ((g_count - g_tamakisekidata[c].id[k].counter - 7) * 14 / 16);
				konokiseki = GetColor(180, 200, 180);
			}
			DrawCircle(g_tamakisekidata[c].id[k].x, g_tamakisekidata[c].id[k].y, ookisa, konokiseki, TRUE);//軌跡を実際に描写する
			if (g_count - g_tamakisekidata[c].id[k].counter > 23) g_tamakisekidata[c].id[k].living = FALSE;//一定時間で消滅
		}
	}

	if (g_mikatatama[c].living == FALSE) {//本体がなくなれば軌跡も無くなる
		g_tamakisekidata[c].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[c].id[b].living = FALSE;
		}
	}

	for (int x = 0; x < MAX_TEKI; x++) {//あたり判定を判定する
		if (g_hontai[x].living == TRUE) {

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);//各敵とこの弾とで判定

			if (hantei == TRUE) {
				g_mikatatama[c].living = FALSE;//弾を消す
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//ライフを10減らす
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//もしも敵がタイプ2,4ならばアイテムを落とす
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

void reflectedtama104(int c) {//タイプ3緑の弾の反射

	g_mikatatama[c].x += g_mikatatama[c].speedx;//位置に速度を追加
	g_mikatatama[c].y += g_mikatatama[c].speedy;

	int tikaino = nearestenemy(c);//最も近い敵を検索

	if (g_lasttime - g_mikatatama[c].timer > 300) {//登場してから300ミリ秒経ったなら
		if (g_mikatatama[c].reverse == FALSE) {//reverseが作動していない、まだ方向転換していないなら
			g_mikatatama[c].speedx = 5 * cos(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x +(80 - random(160)), g_hontai[tikaino].y));//最も近い敵に方向転換する
			g_tekitama[c].speedy = -5 * sin(kakudo2(g_mikatatama[c].x, g_mikatatama[c].y, g_hontai[tikaino].x + (80 - random(160)), g_hontai[tikaino].y));
			g_mikatatama[c].reverse = TRUE;
		}
	}




	if (g_count % 2 == 0) {//2フレーム毎にこの弾のある場所に軌跡をおく
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
	for (int k = 0; k < MAX_TAMA; k++) {//軌跡の変化を描写
		if (g_tamakisekidata[c].id[k].living == TRUE) {
			int konokiseki = GetColor(120, 230, 120);
			int ookisa = 0;
			if ((g_count - g_tamakisekidata[c].id[k].counter) <= 7) ookisa = 14;//7フレームまでなら大きさに変化はない
			if ((g_count - g_tamakisekidata[c].id[k].counter) > 7) {//7フレーム以降は徐々に小さくなる
				ookisa = 14 - ((g_count - g_tamakisekidata[c].id[k].counter - 7) * 14 / 16);
				konokiseki = GetColor(180, 200, 180);//軌跡の色を描写
			}
			DrawCircle(g_tamakisekidata[c].id[k].x, g_tamakisekidata[c].id[k].y, ookisa, konokiseki, TRUE);//実際に軌跡を描写する
			if (g_count - g_tamakisekidata[c].id[k].counter > 23) g_tamakisekidata[c].id[k].living = FALSE;//時間がたてば消滅させる
		}
	}

	if (g_mikatatama[c].living == FALSE) {//本体が消えれば軌跡も消える
		g_tamakisekidata[c].living = FALSE;
		for (int b = 0; b < MAX_TAMA; b++) {
			g_tamakisekidata[c].id[b].living = FALSE;
		}
	}

	for (int x = 0; x < MAX_TEKI; x++) {//敵との当たり判定処理
		if (g_hontai[x].living == TRUE) {//生きている敵の全てと判定を行う

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[c].x, g_mikatatama[c].y, 20);

			if (hantei == TRUE) {//もしも判定が真なら
				g_mikatatama[c].living = FALSE;//子の弾を消す
				g_hontai[x].lifecount = g_hontai[x].lifecount - 10;//敵のライフに10ダメージ
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//敵のタイプが2ならパワーアップアイテム
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

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//倒した敵のタイプが4ならスピードアップアイテム
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


void reflectedtama105(int i) {//タイプ5、バウンド弾を跳ね返した場合

	g_mikatatama[i].x += g_mikatatama[i].speedx;//座標の変数にスピードを追加
	g_mikatatama[i].y += g_mikatatama[i].speedy;

	DrawCircle(g_mikatatama[i].x, g_mikatatama[i].y, 12, GetColor(100, 250, 100), TRUE);//この弾を描写
	if (g_mikatatama[i].reverse == FALSE) {//もしもまだバウンドしていないなら、もしもreverseがFALSEなら
		if (g_mikatatama[i].x >= 800 || g_mikatatama[i].x <= 0) {//横の壁にぶつかったら
			g_mikatatama[i].speedx *= -1;//X座標のスピードを反転させる
			g_mikatatama[i].reverse = TRUE;
		}
		if (g_mikatatama[i].y >= 600 || g_mikatatama[i].y <= 0) {//上下の壁にぶつかったら
			g_mikatatama[i].speedy *= -1;//Y座標のスピードを反転させる
			g_mikatatama[i].reverse = TRUE;
		}
	}
	for (int x = 0; x < MAX_TEKI; x++) {//敵との接触の確認
		if (g_hontai[x].living == TRUE) {//生きている敵全員と判定処理を行う

			BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[i].x, g_mikatatama[i].y, 20);//この弾と敵の座標とで当たり判定を見る

			if (hantei == TRUE) {
				g_mikatatama[i].living = FALSE;//この弾を消滅させる
				g_hontai[x].lifecount = g_hontai[x].lifecount - 13;//相手ライフ-13
				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//相手を倒し、敵のタイプが2の場合
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;//パワーアップアイテムを出す
						g_items[u].x = g_hontai[x].x;
						g_items[u].y = g_hontai[x].y;
						g_items[u].speedx = 0;
						g_items[u].speedy = -0.2f;
						g_items[u].type = 1;

					}
				}

				if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//相手を倒し、敵のタイプが4の場合
					int u;
					for (u = 0; u < 50; u++) {
						if (g_items[u].living == FALSE) break;
					}
					if (u < 50) {
						g_items[u].living = TRUE;//スピードアップアイテムを出す
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

int detectNearestEnemy(int c) {//最も近い敵を検索する関数
	static int nearestenemy = 0;//最も近い敵の番号を入れれるための変数
	static float Near = 100000000;//最も近い距離、最初はものすごく大きくしておく
	for (int i = 0; i < MAX_TEKI; i++) {
		if (g_hontai[i].living == TRUE) {//生きている敵との距離を図る
		int x =	g_hontai[i].x - g_mikatatama[c].x;
		int y = g_hontai[i].y - g_mikatatama[c].y;
			if (x * x + y * y < Near) {//もしも距離が今までのNearより近ければ、その敵の番号と距離を登録
				Near = x * x + y * y; nearestenemy = i;
			}
		}


	}
	int a = nearestenemy;//aにnearestenemyをつける
	nearestenemy = 0;//各関数の再初期か
	Near = 1000000000;
		return a;
}

void DrawMikatatama() {
	//タマの描画と移動
	float mv = 600.0f * g_frametime;//移動速度の設定
	for (int i = 0; i < MAX_TAMA; i++) {
		if (g_mikatatama[i].living == FALSE) continue;



		if (g_mikatatama[i].type >= 101) {//もしも弾のtypeが101以上なら（もしも反射した弾なら）
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
		//反射していない、自身から発した弾なら
		

		//y方向に-mv移動させる
		if(g_mikatatama[i].type < 99)g_mikatatama[i].y -= mv;
		//ショットランク2の弾の場合、type1から徐々に横向きに撃っていくtypeが-なら左向き、+なら右向き
		if (g_mikatatama[i].type == 1) g_mikatatama[i].x += 0.1 * mv, g_mikatatama[i].y += 0.1 * mv;
		if (g_mikatatama[i].type == -1)g_mikatatama[i].x -= 0.1 * mv, g_mikatatama[i].y += 0.1 * mv;
		if (g_mikatatama[i].type == 2) g_mikatatama[i].x += 0.15 * mv, g_mikatatama[i].y += 0.15 * mv;
		if (g_mikatatama[i].type == -2)g_mikatatama[i].x -= 0.15 * mv, g_mikatatama[i].y += 0.15 * mv;
		if (g_mikatatama[i].type == 3) g_mikatatama[i].x += 0.2 * mv, g_mikatatama[i].y += 0.2 * mv;
		if (g_mikatatama[i].type == -3)g_mikatatama[i].x -= 0.2 * mv, g_mikatatama[i].y += 0.2 * mv;
		if (g_mikatatama[i].y < -10) g_mikatatama[i].living = FALSE;

		//弾のタイプが99までならばナイフで弾を表現
		if(g_mikatatama[i].type < 99)DrawGraph((int)g_mikatatama[i].x-25, (int)g_mikatatama[i].y-25, g_tamaimg, TRUE);//もしも弾がナイフなら
		for (int x = 0; x < MAX_TEKI; x++) {//当たり判定
			if (g_hontai[x].living == TRUE) {

				BOOL hantei = AtariinfoSo(g_hontai[x].x, g_hontai[x].y, 25, g_mikatatama[i].x, g_mikatatama[i].y, 20);//敵との座標から当たったかを求める

				if (hantei == TRUE && g_mikatatama[i].type < 99) {
					g_mikatatama[i].living = FALSE;//弾を消滅させる
					g_hontai[x].lifecount = g_hontai[x].lifecount - 1;//敵に1ダメージを与える
					if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 2) {//相手を倒して、かつそれがタイプ2ならパワーアップアイテムを与える
						int u;
						for (u = 0; u < 50; u++) {
							if (g_items[u].living == FALSE) break;
						}
						if (u < 50) {
							g_items[u].living = TRUE;//アイテムが生きているという判定にする
							g_items[u].x = g_hontai[x].x;
							g_items[u].y = g_hontai[x].y;
							g_items[u].speedx = 0;
							g_items[u].speedy = -0.2f;
							g_items[u].type = 1;//type1でパワーアップアイテム

						}
					}

					if (g_hontai[x].lifecount <= 0 && g_hontai[x].type == 4) {//相手を倒して、かつそれがタイプ4ならスピードアップアイテムを与える
						int u;
						for (u = 0; u < 50; u++) {
							if (g_items[u].living == FALSE) break;
						}
						if (u < 50) {
							g_items[u].living = TRUE;//アイテムが生きていることにする
							g_items[u].x = g_hontai[x].x;
							g_items[u].y = g_hontai[x].y;
							g_items[u].speedx = 0;
							g_items[u].speedy = -0.2f;
							g_items[u].type = 2;//type2でスピードアップアイテム

						}
					}

				}

			}
		}
		//弾が画面外に出たら生存を取り消す
		if (g_mikatatama[i].x > 800 || g_mikatatama[i].x < -50 || g_mikatatama[i].y > 700 || g_mikatatama[i].y < -50)g_mikatatama[i].living = FALSE;
	}
	
	
	}

void DrawItems() {    //アイテム描写
	for (int i = 0; i < 50; i++) {
		if (g_items[i].living == FALSE) continue;
		if (g_items[i].y < -10) g_mikatatama[i].living = FALSE;//アイテムが上に行き過ぎれば消す
			g_items[i].speedy += 0.02;//アイテムのy方向のスピード、等加速落下運動
			g_items[i].y += g_items[i].speedy;
			g_items[i].x += g_items[i].speedx;


		
			if (g_items[i].type == 1) {//アイテムのtypeが1なら
				
				DrawGraph(g_items[i].x - 25, g_items[i].y - 25, g_PowerSterimg, TRUE);//パワーアップスターの画像を表示する
			}
			if (g_items[i].type == 2) {//アイテムのtypeが2なら
							
				DrawGraph(g_items[i].x - 25, g_items[i].y - 25, g_SpeedSterimg, TRUE);//スピードアップスターの画像を表示する
			}
			

			BOOL atariitem = AtariinfoH(g_items[i].x, g_items[i].y , 25);//当たり判定
			if (atariitem == TRUE) {//当たり判定がTRUEなら
				if(g_items[i].type == 1)  g_shotrank ++;//アイテムのtypeが1ならショットランクの上昇
				if (g_items[i].type == 2) g_hspeedrank ++;//type2ならスピードランクの上昇
				g_items[i].living = FALSE;//生存を取り消す


			}


			

	};
}

void drawslash() {//シールドソードのアニメーション
	if (g_drawanimation[0].onoff == 1) {//もしもシールドソードのアニメーション判定が1ならば
		int time = ((g_count - g_drawanimation[0].time)) % 4;//このg_drawanimation（シールドソードを発動した時間を使って、経過をカウントする）を4で割る
		if (time >= 3) {
			g_drawanimation[0].onoff = 0;
		}
		else {
		DrawGraph(g_hx - 251, g_hy - 346, g_slashimg[time], TRUE);//アニメーションの描画、timeで描写するコマを設定
	}
	}
}

BOOL AtariinfoH(int sx, int sy,int size) {//主人公に対する当たり判定

	float a = sx - g_hx;//主人公と相手の位置のX座標の差
	float b = sy - g_hy;//主人公と相手のY座標の差
	float c = a * a + b * b;//位置の差を軽く見る
	float sum_radius = size + 15;
	if (c <= sum_radius * sum_radius) {//もしも位置の差が、主人公と敵の半径より大きければ、TRUEを返す
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL atariinforey(float r, float ax, float ay, float vx, float vy) {//当たり判定、レーザー用
	if (r < 0)return false;//太さが0以下ならFALSEを返す

	if (vx == 0.0f, vy == 0.0f) return false;//0,0ならファルスを返る
	

	ax -= g_hx;
	ay -= g_hy;//円の中心点が原点になるように開始点をオフセット(補正する)

	float len = sqrtf(vx * vx + vy * vy);
	vx /= len;
	vy /= len;

	// 計数tを算出
	float dotAV = ax * vx + ay * vy;
	float dotAA = ax * ax + ay * ay;
	float s = dotAV * dotAV - dotAA + r * r;
	if (fabs(s) < 0.000001f) s = 0;

	if (s < 0.0f) return false;//衝突していない

	float sq = sqrtf(s);
	float t1 = -dotAV - sq;
	float t2 = -dotAV + sq;

	//もしt1及びt2がマイナスだったら始点が円内にめり込んでるのでエラーになる
	if (t1 < 0.0f || t2 < 0.0f)return false;
	 return true;






}

BOOL AtariinfoSo(int ax, int ay, int as, int bx, int by, int bs) {//主人公以外の当たり判定、x,yはx座標、y座標
	float a = ax - bx;//それぞれの座標の差を求める
	float b = ay - by;
	float c = a * a + b * b;//大まかな距離を求める
	float sum_radius = as + bs;
	if (c <= sum_radius * sum_radius) return TRUE;//もしもその大まかな距離が二つの半径より小さければTRUE
	else {
		return FALSE;
	}
}

BOOL Atariinfoougi(int ax, int ay, int bx, int by, int lengh) {//扇形の当たり判定、シールドソードの判定に使う
	float a = ax - bx;
	float b = ay - by;
	float c = a * a + b * b;//大まかな距離がこちらの二つの半径より小さければ、ヒット
	if (c <= lengh * lengh) {

		float kakuhantei = kakudo2(ax, ay, bx, by);//二つの間の角度を求める。その角度で扇の中に入っていれば当たり
		if (kakuhantei >= PII / 4 * (-1) && kakuhantei <= PII * 5 / 4) {//ヒットした中で、扇に入っているもののみを選別
			return TRUE;
		}
	}
	return FALSE;
}


float directioninfosin(int x, int y) {//距離情報（sin）
	int dex = g_hx - x;//各座標の差を求める
	int dey = g_hy - y;
	float dis = sqrt(dex * dex + dey * dey);//実際の距離を求める

	float sinr = dey / dis;//Y座標の差を距離で割り、sinを求める
	return sinr;
	

}

float directioninfocos(int x, int y) {//距離情報（cos）
	int dex = g_hx - x;//各座標の差を求める
	int dey = g_hy - y;
	float dis = sqrt(dex * dex + dey * dey);//実際の距離を求める

	float cosr = dex / dis;//X座標の差を距離で割り、cosを求める
	return cosr;
}


float kakudo(int x, int y) {//主人公との角度を求める
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = g_hx - x;//主人公との座標の差を求める
	dey = g_hy - y;
	angle = acos(dex / sqrt(dex * dex + dey * dey));//ここで角度を求める
	//angle = angle   * 180 / PI;

	if (dey < 0) angle = PI * 2 - angle;

	return angle;

}


float kakudo2(int x, int y, int aitex, int aitey) {//二つのものとの角度を求める
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = aitex - x;//二つのものの座標の差を求める
	dey = aitey - y;
	angle = acos(dex / sqrt(dex * dex + dey * dey));//ここで角度を求める

	if (dey < 0) angle = PI * 2 - angle;

	return angle;
}

float angle1(int x, int y) {//角度を求める
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = g_hx - x;//主人公との座標の差を求める
	dey = y - g_hy;
	angle = acos(dex / sqrt(dex * dex + dey * dey));
	//angle = angle   * 180 / PI;

	if (dey < 0) angle = PI * 2 - angle;//ここで角度を求める

	return angle;

}


float angle2(int x, int y, int aitex, int aitey) {//二つのものとの角度を求める
	float dex, dey, angle, PI;
	PI = 3.141592;
	dex = aitex - x;//二つのものの座標の差を求める
	dey = y - aitey;
	angle = acos(dex / sqrt(dex * dex + dey * dey));

	if (dey < 0) angle = PI * 2 - angle;//ここで角度を求める

	return angle;
}


int nearestenemy(int cd) {//最も近い敵を検索する
	static float nearest = 1000000000;
	static int nearestnumber = 10005;
	static float a = 0;
	static float b = 0;
	static float c = 0;
	int i = 0;
	for ( i = 0; i < MAX_TEKI; i++) {//敵毎の距離を測定、最も近いものを記録する
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

int random(int lengh) {//ランダムな値を返す（時間を元にしてタネを作る）
	int r = g_lasttime % lengh;
	return r;
}

int numberrandom(int lengh, int seed) {//ランダムな値を返す（こちらでタネをセットし、それと時間とで乱数を作る）
	float r = seed * seed *(sin(g_lasttime)*sin(seed * g_lasttime) / cos(seed * g_lasttime));
	int seir = r;
	int kaeri = seir % lengh;
	return kaeri;
}


void tamaset(int number, int  type, int x, int y, float speedx, float speedy) {//弾を生み出すための関数
	g_tekitama[number].living = TRUE;//弾の生存をONにする
	g_tekitama[number].type = type;//弾のタイプの設定
	g_tekitama[number].x = x;//弾の初期のX座標
	g_tekitama[number].y = y;//弾の初期のY座標
	g_tekitama[number].speedx = speedx;//弾のX軸方向でのスピード
	g_tekitama[number].speedy = speedy;//弾のX軸方向でのスピード
	g_tekitama[number].timer = g_lasttime;//弾が生存している時間を測定するための変数
	g_tekitama[number].counter = g_count;//弾の生存しているフレームを設定するための変数
	g_tekitama[number].reverse = FALSE;//弾のトリガー（reverse）をFALSEにする

}