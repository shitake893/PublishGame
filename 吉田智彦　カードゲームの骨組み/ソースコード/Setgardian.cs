using System;

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
//ガードをするための処理
public class Setgardian : MonoBehaviour, IDropHandler
{
    [SerializeField] Text attacktext;//攻撃側のパワーを表示するためのテキスト
    [SerializeField] Text defensetext;//防御側のパワーを表示するためのテキスト

    public int attackednumber = 0;//
    public CardController attacker;//攻撃側のカードを保存する
    public CardController defender;//防御側のカードを保存する
    int supportpw;//サポートで追加されたパワーを保存する
   

    CardController[] Precanguardcards;//最初にこの配列にカードを入れる
    CardController[] g_canguardcards;//上の配列からガードが可能なものを選択する
    void Awake()
    {

    }


    public void Setbattle(CardController A,CardController B)//バトルで最初に呼び出される、引数から攻撃側、防御側のカードを読み取る
    {
        attacker = A;
        defender = B;

        supportpw = 0;

        attacktext.text = (A.model.pw + A.model.pluspw).ToString();//ここの部分で攻撃側、防御側のパワーを左右に表示する
        defensetext.text = (B.model.pw + B.model.pluspw).ToString();

        //ここで攻撃側と対象側にアイコンを出す（Cardcontrollerに入れておく）
        A.view.SetApointer(true);//攻撃！のアイコンを攻撃側に出す
        B.view.SetTpointer(true);//標的！のアイコンを防御側に出す




        CardController[] Precanguardcards = GameManager.instance.playerFieldTransform.GetComponentsInChildren<CardController>();//プレイヤーのフィールドからカードを配列に集める
       CardController[] g_canguardcards = Array.FindAll(Precanguardcards, card => (card.place.placenumber == defender.place.placenumber - 1
       ||  card.place.placenumber == defender.place.placenumber + 1) && card.place.placearea == 1);//ガードに使えるカード（左右の一つ隣）をこちらの配列に入れる

        for(int i = 0; i < g_canguardcards.Length; i++)//ここでガード可能なカードに青い枠を出す。そして選択可能である状態にする
        {
         
                g_canguardcards[i].model.selectable = true;//選択可能な判定にする
                g_canguardcards[i].view.SetActiveSelectablePanel(true);//選択可能（青い枠）を出す

        }
        B.model.selectable = false;//防御側のカードは選択可能な表示をはずず
        B.view.SetActiveSelectablePanel(false);//選択可能判定もはずす

    }

    public void OnDrop(PointerEventData eventData)//このパネルにカードをドロップすればそのカードでガードができる
    {
        //attackerを選択（ドロップしたところ））
        CardController supporter  = eventData.pointerDrag.GetComponent<CardController>();//サポーター（防御側に加勢するカードを記録
                                                                                         //）

     
            if (supporter.model.selectable == false ) return;//そのカードが選択可能でなければ何もしない
        
        if(supporter.place.placearea == 1 && supporter.place.placenumber <= defender.place.placenumber + 1 && supporter.place.placenumber >= defender.place.placenumber - 1)//サポーターの位置が防御側のカードの隣であることを確認
        {
            supportpw += supporter.model.pw;//サポーターのパワーをsupportpwに加える
            defensetext.text = (supportpw + defender.model.pw + defender.model.pluspw).ToString();//防御側のテキストを更新する
            if (supporter.model.state == true)//もしもサポーターの状態が攻撃可能だったならば
            {
                supporter.Changestate(false);//攻撃不可能な状態にする
                supporter.model.selectable = false;
                supporter.view.SetActiveSelectablePanel(false);//このカードをこれ以上選択できないようにする
            }
            else//攻撃可能な状態でないカードをガードとして出した場合
            {
                if(supporter.place.placenumber == 3)//それがリーダーの場合
                {
                    supporter.model.selectable = false;//選択可能な表示を消す
                    supporter.view.SetActiveSelectablePanel(false);//このカードをこれ以上選択できないようにする
                    GameManager.instance.lifemove(-2);//ライフを2減らす
                    GameManager.instance.ui.ShowLife();
                    Debug.Log("ガードによるライフ増減を行いました");
                }
                else
                {
                    supporter.GotoDrop();//リーダーでないなら、ドロップに送る

                }
            }
        }
       
    
   
        

    }
    public void attackbotton()//ガードOKのボタンを押した場合、バトルに移る
    {
        GameManager.instance.battle.CardsBattle(attacker, defender,supportpw);//battleにてのカードバトルの処理を行う
      //  Debug.Log("canアタックのカード群の数 = " + g_canguardcards.Length);
     //   for (int i = 0; i < g_canguardcards.Length; i++)
    //    {
    //        g_canguardcards[i].model.selectable = false;
    //        g_canguardcards[i].view.SetActiveSelectablePanel(false);
    //    }
        gameObject.SetActive(false);//パネルの表示をオフにする
    }
}

