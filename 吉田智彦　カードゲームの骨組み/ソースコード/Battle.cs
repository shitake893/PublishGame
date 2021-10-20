using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Battle : MonoBehaviour//バトルに関する処理
{


    GameManager gameManager;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = GameManager.instance;
    }

    public void CardsBattle(CardController attacker, CardController defender, int supportpw)//カードバトル（個々のカードごとのバトル）
    {
        Debug.Log("カードバトル処理に行きました");
        if (attacker.model.pw + attacker.model.pluspw >= defender.model.pw + defender.model.pluspw + supportpw)//攻撃側が防御側のパワー以上だった場合
        {
            if (defender.place.placenumber == 3)//リーダーへの攻撃の場合
            {
                Debug.Log("リーダーへの攻撃が届きました");
                if (defender.place.placearea < 0)//敵側が攻撃された場合
                {
                    gameManager.enemyLife -= (attacker.model.pw + attacker.model.pluspw - (defender.model.pw + defender.model.pluspw)) / 1000;//上回った分/1000のダメージを敵に与える
                }
                else//プレイヤーが攻撃された場合
                {
                    gameManager.playerLife -= (attacker.model.pw + attacker.model.pluspw - (defender.model.pw + defender.model.pluspw)) / 1000;//上回った分/1000のダメージをプレイヤーに与える
                }
                gameManager.ui.ShowLife();//ライフの表示
                gameManager.ui.CheckLife();//ライフがゼロになったかの確認
            }
            else // リーダー以外への攻撃の場合
            {
                defender.GotoDrop();//防御側を破壊する
                Debug.Log("ディフェンダーが破壊される条件を満たしました");
            }
        }
        else//ディフェンス力がアタックを上回った場合はアタッカーが返り討ちに合う
        {
            if (supportpw > 0 && attacker.place.placenumber != 3)//サポートがあり、アタッカーがリーダーでない場合
            {
                attacker.GotoDrop();//アタッカーを破壊
                Debug.Log("アタッカーが破壊される条件を満たしました");
            }
            if (supportpw > 0 && attacker.place.placenumber == 3)//リーダーだった場合は2ダメージを与える
            {
                gameManager.enemyLife -= 2;
                gameManager.ui.ShowLife();
                gameManager.ui.CheckLife();
                Debug.Log("リーダーが返り討ちに会いました");
            }
        }


        attacker.view.SetApointer(false);//攻撃の表示をはずす
        defender.view.SetTpointer(false);//標的表示をはずす

        //  Debug.Log("処理が行われる前の向き" + attacker.transform.rotation);
        if (attacker.place.placenumber != 0)//もしも攻撃側がフィールドをはなれていなければ
        {
            attacker.Changestate(false);//横向きにする（攻撃できないようにする）
        }
        




        //ここからフィールドのカードの選択可能を一掃する！
        CardController[] EMcardFieldList = gameManager.enemyFieldTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in EMcardFieldList)
        {
            card.model.selectable = false;
            card.view.SetActiveSelectablePanel(false);
        }

        CardController[] PLcardFieldList = gameManager.playerFieldTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in PLcardFieldList)
        {
            card.model.selectable = false;
            card.view.SetActiveSelectablePanel(false);
        }
    }


    public void FieldBattle(Transform attacker, Transform defender)//フィールドバトルの処理
    {
        CardController[] attackers = attacker.GetComponentsInChildren<CardController>();

        CardController[] defenders = defender.GetComponentsInChildren<CardController>();

        int attackpw = 0;//攻撃側の合計パワー
        int defendpw = 0;//防御側の合計パワー

        foreach (CardController acard in attackers)
        {
            attackpw += acard.model.pw;//攻撃側に用いたカードのパワーを加算する
        }

        foreach (CardController acard in defenders)
        {
            defendpw += acard.model.pw;//防御側もそのフィールドのカードをのパワーを加算する
        }

        if (attackpw >= defendpw)//もしも攻撃側のパワーが勝ったら
        {
            foreach (CardController destroyedcard in defenders)
            {
                //このカードがリーダーかplaceareaとplacenumberを用いて分析
                if (destroyedcard.place.placenumber == 3 && 1 == Mathf.Abs(destroyedcard.place.placearea))//もしもdestroydcardがリーダーだったら
                {
                    if (destroyedcard.place.placearea < 0)//破壊されるのがプレイヤーか敵かを判断、これでどちらのライフを削るか確認
                    {
                        gameManager.enemyLife -= 1;//敵ライフを1減らす
                        gameManager.ui.ShowLife();
                        gameManager.ui.CheckLife();
                    }
                    else
                    {
                        gameManager.playerLife -= 1;
                        gameManager.ui.ShowLife();
                        gameManager.ui.CheckLife();
                    }
                }
                else
                {//もしもリーダーでなければ普通に破壊
                    destroyedcard.GotoDrop();
                   // Destroy(destroyedcard.gameObject);//防御側を破壊

                }
            }

            foreach (CardController sleepcard in attackers)//攻撃終了時、攻撃に用いたカードは全て横向きになる
            {
                sleepcard.Changestate( false);
            }
        }
        gameManager.ui.ShowLife();
        gameManager.ui.CheckLife();
    }

}
