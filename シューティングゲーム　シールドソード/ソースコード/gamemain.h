#ifndef __GAMEMAIN_H__ 
#define __GAMEMAIN_H__


#include <DxLib.h>
#include "main.h"
void GameMain();//GameMainの関数
void DrawHero();//主人公の描写
void DamageControl();//ダメージを受けた直後に関する処理
void MapEffect();//まっぷによるギミック（今回は未実装）
void DrawEnemy();//敵の描写
void DrawTekitama();//敵の弾の描写
//敵の弾の種類
void tekitama1(int i);
void tekitama2(int i);
void tekitama3(int i);
void tekitama4(int i);
void tekitama5(int i);
//味方の攻撃等
void Drawreflection(int key);
void Drawtama(int key);
void DrawMikatatama();
//半遮弾
void reflectedtama101(int c);
void reflectedtama102(int c);
void reflectedtama103(int c);
void reflectedtama104(int c);
void reflectedtama105(int i);
//敵の種類
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


//アイテムの描写
void DrawItems();
//必殺技（シールドソードの描写）
void drawslash();


//角度分析、追尾用の関数
float directioninfosin(int x, int y);
float directioninfocos(int x, int y);
float kakudo(int x, int y);
float kakudo2(int x, int y, int aitex, int aitey);
float angle1(int x, int y);
float angle2(int, int y, int aitex, int aitey);
int nearestenemy(int cd);

//当たり判定
BOOL AtariinfoH(int sx,int sy,int size);
BOOL AtariinfoSo(int ax, int ay, int as, int bx, int by, int bs);
BOOL atariinforey(float r, float ax, float ay, float vx, float vy);
BOOL Atariinfoougi(int ax, int ay, int bx, int by, int lengh);
int detectNearestEnemy(int c);//最も近い敵の決定に関する巻子

BOOL IsBKeyTrigger(int key);//Xボタンを推し始めたときを見る関数
int random(int lengh);//ランダム関数
int numberrandom(int lengh, int seed);//数字もシードとしたランダム関数




extern int movecount;//弾を撃った際の時間変化の測定



//extern tekitati saisyo;
//extern int tekiscale;

void tamaset(int number, int  type, int x, int y, float speedx, float speedy);//弾をセットする


#endif
