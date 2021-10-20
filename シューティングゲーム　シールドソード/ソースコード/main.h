#ifndef __MAIN_H__ 
#define __MAIN_H__
#define PII 3.14159


#include<cmath>
#include <DxLib.h>
#include "gamemain.h"
#include "load.h"



//グローバル変数
//時間計測用変数
extern int g_lasttime ;		//直前の計測時間
extern float g_frametime ;	//1ループにかかった時間
extern int g_timerstart;		//タイマー用変数
extern int g_damagetimer; //ダメージ時間用タイマー
extern int g_gametime;    //ゲーム内時間設定
extern int g_movecount;//間隔計算用
extern int g_count;
extern int g_gekihacounter; //中ボス撃破状況
extern float g_gekihatime; //ボス撃破時間
extern float g_pi;
enum GameState {			//ゲーム状態
	GAME_TITLE, GAME_MAIN,
	GAME_CLEAR, GAME_OVER
};

enum HeroState {
	NOMAL, DAMAGED
};
extern HeroState g_herostate;

extern GameState g_gamestate;
extern int g_gametitleimg;			//タイトルイメージ
extern int g_heroimg[3];	//画像
extern int g_tamaimg;    //主人公の玉の画像
extern int g_Boss1img;   //ボスの画像
extern int g_slashimg[3];//スラッシュ画像
extern int g_PowerSterimg;//パワースター画像
extern int g_SpeedSterimg;//スピードスター画像

extern float g_hx, g_hy;	//座標
extern int g_herolife;     //主人公ライフ
extern int g_shotrank;    //主人公ショットの強さ
extern int g_hspeedrank;  //主人公のスピード






//ボタン
extern  BOOL g_akey_prev;	//直前のAボタンの状態

extern BOOL g_bkey_prev;   //直前のBボタンの状態
//フォント
extern int g_middlefont;			//中サイズフォントハンドル
extern int g_largefont;			//大サイズフォントハンドル

//関数プロトタイプ宣言
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();
void InitStage();
BOOL IsAKeyTrigger(int key);
BOOL IsCKeyTrigger(int key);
BOOL IsBKeyTrigger(int key);





#endif