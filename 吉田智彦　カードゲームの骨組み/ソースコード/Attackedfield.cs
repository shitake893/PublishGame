using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.EventSystems;


public class Attackedfield : MonoBehaviour, IDropHandler//左右のフィールド全体で戦闘を行う
{

   

    public void OnDrop(PointerEventData eventData)//プレイヤーがフィールドを相手の向かい合ったフィールドにぶつけたら
    {
        if (GameManager.instance.phase != GameManager.Phase.BATTLE) return;//バトルフェーズでなければ何もしない
        Debug.Log("AttackedFieldが開始されました");
        //attackerを選択（ドロップしたところ））
        Transform attacker = eventData.pointerDrag.GetComponent<Transform>();//攻撃側は重ねてきたフィールド

        //defenderを選択（これがあるカード)
        Transform defender = GetComponent<Transform>();//ディフェンダーは重ねられた方、（このスクリプトはフィールド全部に貼り付けられる。つまり発動したフィールド）

        //戦闘処理
        if (attacker == null || defender == null)//もしもアタッカーかディフェンダーが無かったら
        {
            return;
        }
        //戦闘処理
        Debug.Log("attackerの名前　=" + attacker);
        Debug.Log("defenderの名前　=" + defender);

        CardController[] attackercheck = attacker.GetComponentsInChildren<CardController>();//アタッカーのフィールド内の全カードを確認する
        if(attacker.gameObject.tag == defender.gameObject.tag)
        {
            return;
        }

        int number = 0;
        foreach(CardController cards in attackercheck)//アタッカーのカードの枚数を数える
        {
            
            if(cards.model.state == true)number++;//存在するカードの中で、攻撃可能なカードの数を数える
        }
        if(number != 4)//もしも4枚揃っていなければ何もしない
        {
            return;
        }
            if (attacker.name == defender.name)
            {

                GameManager.instance.battle.FieldBattle(attacker, defender);//ゲームマネージャーのフィールドバトル処理を行う
            }
    }


}





