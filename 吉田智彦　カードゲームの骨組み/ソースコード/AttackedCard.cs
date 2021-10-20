
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class AttackedCard : MonoBehaviour , IDropHandler//プレイヤーが攻撃したカードが適応される
{
    public void OnDrop(PointerEventData eventData)
    {
        if (GameManager.instance.phase != GameManager.Phase.BATTLE) return;
        //attackerを選択（ドロップしたところ））
        CardController attacker = eventData.pointerDrag.GetComponent<CardController>();

        //defenderを選択（これがあるカード)
        CardController defender = GetComponent<CardController>();

        //戦闘処理
        if(attacker == null || defender == null)
        {
            return;
        }
        Debug.Log("アタッカーのステート = " + attacker.model.state);
        if (attacker.model.state == false) return;//アタッカーが不活化状態なら何もできない

        if(attacker.place.placearea == 2 && defender.place.placearea == 1)//味方をサポートする場合（味方同士で後列から前列に攻撃した場合はパワーを加算する）
        {
            defender.model.pluspw += attacker.model.pw + attacker.model.pluspw;
            defender.view.Show(defender.model);
            attacker.Changestate( false);//
        }
        else//普通に戦闘する
        {
            if (Mathf.Abs(attacker.place.placearea) == 1 && Mathf.Abs(defender.place.placearea) == 1)
            {
                if (attacker.place.placenumber <= 5 - defender.place.placenumber + 1 && attacker.place.placenumber >= 5 - defender.place.placenumber - 1)
                {
                    //Debug.Log("attacker = " + attacker);
                    //Debug.Log("defender = " + defender);
                    //Debug.Log("defenderspw = " + defender.model.pw);
                    //Debug.Log("defenderspw = " + attacker.model.pw);
                    //Debug.Log("attacker place =" + attacker.place.placenumber);
                    //Debug.Log("defender place =" + defender.place.placenumber);

                    StartCoroutine(GameManager.instance.enemyai.enemygardsed(attacker, defender));
                }
            }
            else
            {
                return;
            }
        }
       
        
    }
        

  
}
