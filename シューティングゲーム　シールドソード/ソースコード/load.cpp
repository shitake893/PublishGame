#include "load.h"
tekidata g_hontai[MAX_TEKI];//敵の本体用の配列
tekidata g_mikatatama[MAX_TAMA];//味方の弾用の配列
tekidata g_items[50];//アイテム用の配列
tekidata g_tekitama[1000];//敵の弾用の配列
graphiceffects g_drawanimation[2];//シールドソードのアニメーション








BOOL Loadtekidata() {
	int FileHandle;
	
	char gomi[8][256];//CSVファイルを読み取るのに確認用の配列
	FileHandle = FileRead_open("fileopen.csv");//このファイルから敵情報を読み取る
	if (FileHandle == NULL) return -1;

	for (int i = 0; i < 2; i++) {//最初の行を捨てる
		FileRead_scanf(FileHandle, "%[^,],%[^,],%[^,],%[^,], %[^,],%s",
			gomi[0], gomi[1], gomi[2], gomi[3], gomi[4], gomi[5]); //読み込む

	}

	int n = MAX_TEKI;//データの数
	for (int i = 0; i < MAX_TEKI; i++) {//敵データをファイルから読み取る

		FileRead_scanf(FileHandle, "%f,%f,%d,%d,%d,%d",
			&g_hontai[i].x, &g_hontai[i].y, &g_hontai[i].living, &g_hontai[i].type, &g_hontai[i].lifecount, &g_hontai[i].derutime);//読み込み（最初の座標や敵体力、敵のタイプなど）
		g_hontai[i].dead = FALSE;//全ての敵で共通の要素を決定する//これは倒されたか
		g_hontai[i].speedx = 0;//X軸のスピード
		g_hontai[i].speedy = 0;//Y軸のスピード（これらは内部のスクリプトで定義されている）
		g_hontai[i].counter = 0;

		

}
	for (int i = 0; i < MAX_TAMA; i++) {//味方の弾を入れる配列、初期か
		g_mikatatama[i].living = FALSE;
		g_mikatatama[i].x = 0;
		g_mikatatama[i].y = 0;
		g_mikatatama[i].type = 0;
		g_mikatatama[i].lifecount = 0;

	}

	for (int i = 0;i < 50; i++) {//アイテムを収納するための配列
		g_items[i].x = 0;
		g_items[i].y = 0;
		g_items[i].living = FALSE;
		g_items[i].type = 0;
		g_items[i].lifecount = 0;
		g_items[i].derutime = 0;
	}

	for (int i = 0; i < 1000; i++) {//敵の弾を収納するための配列
		g_tekitama[i].x = 0;
		g_tekitama[i].y = 0;
		g_tekitama[i].living = FALSE;
		g_tekitama[i].type = 0;
		g_tekitama[i].lifecount = 0;
		g_tekitama[i].derutime = 0;
	}

	for (int i = 0; i < 1000; i++) {//弾や敵の軌跡を収納するための配列
		g_kisekidata[i].living = FALSE;
		for (int j = 0; j < 100; j++) {
			g_kisekidata[i].id[j].x = 0;
			g_kisekidata[i].id[j].y = 0;
			g_kisekidata[i].id[j].x = 0;
			g_kisekidata[i].id[j].living = FALSE;
			g_kisekidata[i].id[j].counter = 0;
		}
	}

	for (int i = 0; i < 2; i++) {//シールドソードのアニメーション用の配列
		g_drawanimation[i].onoff = 0;
		g_drawanimation[i].time = 0;

	}
	
	
	return TRUE;
}