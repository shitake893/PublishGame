#ifndef ___LOAD_H__/#define __LOAD_H__

#include <DxLib.h>

//extern int g_count;
//extern int g_lasttime;
#define MAX_TAMA 1000
#define MAX_TEKI 500
class tekidata {//敵、及びアイテムなどのデータ
public:
	float x, y;//x,y座標
	int living;//生きているかの判定
	int type;//そのもののタイプ
	int lifecount;//そのもののライフ
int derutime;//出現時間
BOOL dead;//殺されたかどうかの判定
float speedx, speedy;//そのもののスピード
int counter;//時間計測用（フレームによる）カウンター
int timer;//時間計測用（ミリ秒）のタイマー
BOOL reverse;//ギミック発動用のBOOL変数
float angles;//角度を決めるための変数




};



extern tekidata g_hontai[MAX_TEKI];
struct kisekihasi {//軌跡用のデータ
	float x, y;
	BOOL living;
	int type;
	int counter;
	
};

struct kisekimoto {//軌跡の元
	kisekihasi id[1000];
	BOOL living;

};

extern kisekimoto g_kisekidata[1000];//軌跡を描くために使う、描画用のデータ
extern kisekimoto g_tamakisekidata[MAX_TAMA];//一つの弾、敵につき使う軌跡データの配列

struct tamadata {//弾のデータ
	int x, y;
	BOOL living;
	int type;
	int lifecount;
};

extern tekidata g_mikatatama[MAX_TAMA];//味方の弾の配列

extern tekidata g_items[50];//アイテム用の配列
extern tekidata g_tekitama[1000];//敵のデータ配列


BOOL Loadtekidata();
//void graphiceffects::slasheffect;
struct graphiceffects {//グラフィックのエフェクト用の配列
public:
	int onoff;
	int time;

	//int animationrate;
	//int animationvolume;
	//int gazou;


	
	



};
//よこ251.たて346
extern graphiceffects g_drawanimation[2];


#endif
