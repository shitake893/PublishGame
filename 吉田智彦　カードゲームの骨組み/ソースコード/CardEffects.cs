using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardEffects : MonoBehaviour
{

    public static CardEffects instance;
    [SerializeField] GameObject multipurposepanel;//多機能パネルを取得する
    [SerializeField] Text multipurposetext;//多機能パネルのテキストを取得する
    GameManager gameManager;

    void Awake()
    {


        if (instance == null)
        {
            instance = this;
            gameManager = GameManager.instance;//ゲームマネージャーのインスタンス生成
        }
    }

    public IEnumerator Aeffects(CardController card)//カードの効果を発動させる
    {
       
        if(card.model.effectC == GameManager.instance.phase)//そのカードの効果の発動フェーズがあっていれば発動する
        {
            yield return StartCoroutine( gameManager.ui.ViewEffectedcard(card));//カードの発動エフェクト、巨大なカードが一瞬表示されて消える
            switch (card.model.effectA)
            {
                case 1://カードを2枚ドローする！

                    if (card.place.placenumber == 3 && Mathf.Abs(card.place.placearea) == 1)//もしもリーダーだったら
                    {
                        if (card.place.placearea > 0)//もしもプレイヤーなら、プレイヤーがカードを一枚ドローする
                        {
                           yield return StartCoroutine( GameManager.instance.GiveCardToHand(GameManager.instance.g_playerDeck, GameManager.instance.playerHandTransform));
                        }
                        else//もしも敵ーなら、敵がカードを一枚ドローする
                        {
                            yield return StartCoroutine(GameManager.instance.GiveCardToHand(GameManager.instance.g_enemyDeck, GameManager.instance.enemyHandTransform));

                        }
                        break;
                    }
                    else//もしもリーダーではなかった場合
                    {
                        if (card.place.placearea > 0)//プレイヤーの場合、プレイヤーが2枚ドロー
                        {
                            Debug.Log("みかたのカードがドローエフェクトされました");

                            yield return StartCoroutine(GameManager.instance.GiveCardToHand(GameManager.instance.g_playerDeck, GameManager.instance.playerHandTransform));
                            yield return StartCoroutine(GameManager.instance.GiveCardToHand(GameManager.instance.g_playerDeck, GameManager.instance.playerHandTransform));
                        }
                        else//敵の場合、敵がカードを2枚ドロー
                        {
                            Debug.Log("敵のカードがドローエフェクトされました");
                            yield return StartCoroutine(GameManager.instance.GiveCardToHand(GameManager.instance.g_enemyDeck, GameManager.instance.enemyHandTransform));
                            yield return StartCoroutine(GameManager.instance.GiveCardToHand(GameManager.instance.g_enemyDeck, GameManager.instance.enemyHandTransform));
                        }
                        card.GotoDrop();//効果を発動したカードは破壊される
                        
                    }
                    
                    break;

                case 2://ライフを1回復、自動処理とする

                    
                    if (card.place.placearea > 0)//もしもプレイヤーならプレイヤーのライフが1回復
                    {
                        GameManager.instance.playerLife++;
                    }
                    else//もしも敵なら敵のライフが1回復
                    {
                        GameManager.instance.enemyLife++;
                    }
                    GameManager.instance.ui.ShowLife();//ライフを表示
                    GameManager.instance.ui.CheckLife();//ライフが0になっていないか確認
                    break;

                case 3://相手を一体破壊(任意処理とする)
                    Debug.Log("効果3を発動！");
                    List<CardController> target = new List<CardController>();//ターゲットとなるカードを収納するリストを作る

                    // yield return new WaitWhile(() => multipurposepanel.activeSelf);元々バトルフェイズ終了とかで待つのでこれはOK
                    yield return new WaitForSeconds(1);//一瞬で終わらないように1秒待つ
                    CardController[] Icards;//カードをふるいに分ける前のリスト
                    CardController[] cards;//ターゲット候補のカードを収納する
                    if (card.place.placearea > 0)//効果を発動したのががプレイヤーのカードなら
                    {
                         Icards = GameManager.instance.GetFieldcards(false);//敵のフィールドのカードを収納する
                        cards = Array.FindAll(Icards, cardA => cardA.place.placearea == -2);//この中で破壊できるカード（後列のカード）を選択する
                    }
                    else
                    {
                         Icards = GameManager.instance.GetFieldcards(true);//味方のフィールドのカードを収納する
                        cards = Array.FindAll(Icards, cardA => cardA.place.placearea == 2);//この中で破壊できるカード（後列のカード）を選択する
                    }

                    foreach(CardController C in cards)
                    {
                        if(C.place.placenumber != 3 && Mathf.Abs(C.place.placearea) != 1)//リーダーのカードは除外する
                        {
                            C.effected(true);//効果の対象となるカードとする
                            target.Add(C);//ターゲットに追加する
                        }
                      
                        
                    }
                    if (target.Count < 1)//もしも破壊できるカードが無ければ、処理を中断する
                    {
                        Debug.Log("破壊できるカードがありません");
                        break;
                    }
                
                   if(card.place.placearea >= 0) yield return StartCoroutine(selectDestroycard(1));//主人公側ならマルチパネルを出す（任意の数を破壊するための骨組みを使う）
                    else
                    {
                       yield return StartCoroutine( gameManager.enemyai.selectDestroy(target, 1));
                    }

                   foreach (CardController C in target)//全て終わったら選択可能判定を消す
                    {
                        C.effected(false);
                    }
                    break;
            }
            if (card.place.placearea != 0)
            {


                Debug.Log("カードを選択できないようにします");
                card.model.selectable = false;//カードをこれ以上選択できないようにする
                card.view.SetActiveSelectablePanel(false);
            }
        }
    }

    IEnumerator selectDestroycard(int count)//ここで破壊するカードを選択する
    {
        multipurposepanel.SetActive(true);//ここにカードを入力されたら、こっちに返ってくる
        multipurposetext.text = "破壊するカードを" + count.ToString() + "枚選択してください";
        
        Debug.Log("SelectDestroycardに移ります");
        yield return new WaitForSeconds(1);
        while (count > 0)//ここのカウントはドロップに送らなければならない数
        {

            yield return new WaitUntil(() => multipurposepanel.GetComponent<DropMultipurpose>().cardin);//このパネルにカードがドロップされるまでまつ
            {
                CardController card = multipurposepanel.GetComponent<DropMultipurpose>().returning();
                if (card.model.effected == true)//もしも選択可能だったカードならば
                {
                    card.GotoDrop();//ドロップゾーンに送る
                    count--;//選択したので、カウントを減らす。
                }


            }
        }
        uneffectedfield();//全て終わったら、選択対象判定を全フィールドのかーどにおいて外す

        multipurposepanel.SetActive(false);//ここにカードを入力されたら、こっちに返ってくる

    }

    private void uneffectedfield()
    {
        //フィールド上のカードのeffectedを解除
        CardController[] effectedA = GameManager.instance.playerFieldTransform.GetComponentsInChildren<CardController>();//プレイヤーのフィールドのカードの解除
        foreach (CardController card in effectedA)
        {
            card.effected(false);
        }
        CardController[] effectedB = GameManager.instance.playerHandTransform.GetComponentsInChildren<CardController>();//プレイヤーの手札のカードの解除
        foreach (CardController card in effectedB)
        {
            card.effected(false);
        }
        CardController[] effectedC = GameManager.instance.enemyHandTransform.GetComponentsInChildren<CardController>();//敵の手札のカードの解除
        foreach (CardController card in effectedC)
        {
            card.effected(false);
        }
        CardController[] effectedD = GameManager.instance.enemyFieldTransform.GetComponentsInChildren<CardController>();//敵の手札のカードの解除

        foreach (CardController card in effectedD)
        {
            card.effected(false);
        }
    }


}
