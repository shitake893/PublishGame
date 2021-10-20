#include "main.h"

//グローバル変数
//時間計測用変数
int g_lasttime = 0 ;		//直前の計測時間
float g_frametime = 0;	//1ループにかかった時間
int g_timerstart;		//タイマー用変数
int g_damagetimer; //ダーメージ時間用タイマー
int g_gametime; //ゲームの時間
int g_movecount;//感覚計算用
int g_count;
int g_gekihacounter;  //ボス撃破状況確認
float g_gekihatime;   //ボス撃破時間
float g_pi = 3.14159; //円周率の設定

kisekimoto g_kisekidata[1000];//弾の軌跡を描くのに使用
kisekimoto g_tamakisekidata[MAX_TAMA];//敵の弾を入れるための配列


//初期設定
GameState g_gamestate = GAME_TITLE;//ゲームの状態をタイトルに
HeroState g_herostate = NOMAL;//主人公の状態を通常状態に
int g_gametitleimg;			//タイトルイメージ
int g_heroimg[3];			//画像
int g_tamaimg;        //主人公玉画像
int g_slashimg[3];//スラッシュ画像
int g_PowerSterimg;//パワースター画像
int g_SpeedSterimg;//スピードスター画像
int g_Boss1img;//ボスの画像
float g_hx = 375, g_hy = 550;//主人公の座標

int g_herolife;//ヒーロのライフ
int g_shotrank;//主人公のショットのランク
int g_hspeedrank;//主人公のスピードのランク
							 //ボタン
BOOL g_akey_prev;	//直前のAボタンの状態(Zボタン)
BOOL g_bkey_prev; //直前のBボタンの状態（Xボタン）
BOOL g_ckey_prev; //直前のCボタンの状態（Cボタン）


//フォント
int g_middlefont;			//中サイズフォントハンドル
int g_largefont;			//大サイズフォントハンドル



int WINAPI WinMain(HINSTANCE h1, HINSTANCE hP, LPSTR lpC, int nC){
	//ウィンドウモードにする
	ChangeWindowMode(TRUE);
	//ウィンドウサイズを変更する
	SetGraphMode(800, 600, 32);
	//DXライブラリ初期化
	if (DxLib_Init() == -1) return -1;
	if (Loadtekidata() == FALSE) return -1;
	

	//画像を読み込み
	g_gametitleimg = LoadGraph("media\\タイトル画像.png");
	//g_heroimg = LoadGraph("media\\smp1_chara01.png");
	if(LoadDivGraph("media\\勇者だよ.png", 3, 3, 1, 50, 50, g_heroimg) == -1){
	return FALSE;
}
	g_PowerSterimg = LoadGraph("media\\PowerSter.png");
	g_SpeedSterimg = LoadGraph("media\\SpeedSter.png");
	if (LoadDivGraph("media\\スラッシュ.png", 3, 3, 1, 500, 400, g_slashimg) == -1)return FALSE;
	g_tamaimg = LoadGraph("media\\smp2_knife.png");
	g_Boss1img = LoadGraph("media\\ボスの習作.png");
	g_middlefont = CreateFontToHandle("メイリオ", 42, -1, DX_FONTTYPE_ANTIALIASING);//文字のフォント等の設定
	g_largefont = CreateFontToHandle("メイリオ", 90, -1, DX_FONTTYPE_ANTIALIASING);

	SetDrawScreen(DX_SCREEN_BACK);//スクリーンの設定
	g_lasttime = GetNowCount() & INT_MAX;	//現在時刻の記録
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1ループにかかった時間を計測
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
		
		ClearDrawScreen();
		//画面描画関数に切り替え
		switch (g_gamestate)//ゲームの各状態
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

	//待機
	WaitKey();
	//DXライブラリの終了処理
	DxLib_End();
	return 0;
}

//タイトル画面描画
void DrawGameTitle() {
	DrawBox(0, 0, 800, 600, GetColor(255, 255, 255), TRUE);
	DrawGraph(0, 0, g_gametitleimg, TRUE);

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);//Zボタンを押すと開始（メインゲームに移行）
	if (IsAKeyTrigger(key) == TRUE) {
		InitStage();
		Loadtekidata();
			
	
		
			g_gamestate = GAME_MAIN;
		}
	}


//ゲーム本編描画
void DrawGameMain(){
	
	
GameMain();
}
//ゲームクリア画面描画
void DrawGameClear(){
	DrawBox(0, 0, 800, 600, GetColor(255, 255, 255), TRUE);
	DrawStringToHandle(100, 200, "ゲームクリア！！！",
		GetColor(100, 255, 100), g_largefont);
	//5秒経ったらタイトル画面へ
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (g_lasttime - g_timerstart > 5000 || key & PAD_INPUT_A) g_gamestate = GAME_TITLE;
}
//ゲームオーバー画面描画
void DrawGameOver(){
	
	//テキスト表示
	
	DrawStringToHandle(100, 200, "ゲームオーバー",
		GetColor(255, 0, 0), g_largefont);
	//5秒経ったらタイトル画面へ
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (g_lasttime - g_timerstart > 5000 ||key &PAD_INPUT_A) g_gamestate = GAME_TITLE;
}

//キートリガー処理
BOOL IsAKeyTrigger(int key){//Zボタンを押したら
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

//キートリガー処理（B)Xボタンを押したら
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

BOOL IsCKeyTrigger(int key) {//Cボタンを押したら
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

//初期化
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


	

