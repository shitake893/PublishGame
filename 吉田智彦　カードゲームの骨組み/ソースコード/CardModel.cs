using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//カードデータそのものとその処理
public class CardModel 
{

    public string name;//カードの名前
       
    public int pw;//パワー
    public int pluspw;//このユニットのパワー増減
    public int lank;//ランク
    public Sprite icon;//アイコン
    public bool canfront;//前におけるか
    public bool canback;//後ろにおけるか
    public bool state;//状態
    public string explain;//説明
    public int effectA;//効果A
    public int effectB;//効果B
    public GameManager.Phase effectC;//どのフェーズで発動できるか

    public bool selectable;//選択可能か
    public bool effected;//効果の選択対象になっているか

    public bool Apointing;//攻撃時に表示されるもの
    public bool Tpointing;//攻撃対象になったときに表示されるもの
    public CardModel(int cardID)
    {
        CardEntity cardEntity = Resources.Load<CardEntity>("CardEntityList/Card" +  cardID);//CardEntityのIDから目的のエンティティーを読み込む（初期化）
        name = cardEntity.name;
        pw = cardEntity.pw;
        pluspw = 0;

        lank = cardEntity.lank;

        icon = cardEntity.icon;
        canfront = cardEntity.frontplace;
        canback = cardEntity.backplcace;
        explain = cardEntity.explaintext;
        effectA = cardEntity.effectA;
        effectB = cardEntity.effectB;
        effectC = cardEntity.effectC;
        state = true;
        selectable = false;
        effected = false;
        Apointing = false;
        Tpointing = false;
    }

    void Battleresult(int comepower)
    {
        if(pw <= comepower)
        {
            
        }
    }

}
