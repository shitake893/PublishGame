using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardController : MonoBehaviour
{//カードの情報管理、
    GameManager gameManager;

    //見かけに関することの操作
    public CardView view;
    //データに関することの操作
    public CardModel model;

    //カードがどこにあるかの確認
    public Chackplace place;
    //カードの移動に関する操作
    public CardMovement movement;
   


    private void Awake()//起動時、みかけや移動に関するスクリプトをを読み込む
    {
        view = GetComponent<CardView>();
        movement = GetComponent<CardMovement>();
        gameManager = GameManager.instance;
    }
    public void Init(int cardID)//カードの初期化、
    {
        model = new CardModel(cardID);
        view.Show(model);
        place = GetComponent<Chackplace>();
        if(place.GetComponentInParent<DropPlace>() != null)
        {
            place.placearea = GetComponentInParent<DropPlace>().placetype;//最初にはドロップ（使用済みカード置き場）に位置を置く
            place.placenumber = GetComponentInParent<DropPlace>().placenumber;
        }

       
        
 
    }

    public void SetCanAttack(bool state)//攻撃できるかどうかのみかけ、攻撃できないと横になる
    {
        model.state = state;
        //見た目の繁栄

    }

    public void effected(bool Q)//効果の対象になったときの表現、オレンジ色の枠が表示される
    {
        model.effected = Q;//対象になっていることを記録する
        view.SetActiveEffectedPanel(Q);//オレンジ色の枠
    }


    public void Apointer(bool Q)//攻撃する際に出てくる表示
    {
        model.Apointing = Q;//攻撃することを示す変数
        view.SetApointer(Q);//ここで表示するかを確認
    }

    public void Tpointer(bool Q)//攻撃対象になった際に出てくる表示
    {
        model.Tpointing = Q;//標的になっていることを管理する変数
        view.SetTpointer(Q);//ここで表示するかを決定する
    }

    public void Changestate( bool state)//カードの状態（攻撃可能かどうか）を決める
    {
        if (state == true)//state==trueならこうげきできる
        {
            model.state = true;

            view.SetActive(model.state);//ここでカードの向きを変更する
        }

        else
        {
            model.state = false;

            view.SetActive(model.state);//ここでカードの向きを変更する
        }



    }

   

    public void GotoDrop()//カードが破壊された際、ドロップ（使用済み置き場）に
    {

        Debug.Log("GotoDropに行きました");

        movement.defaultParent = gameManager.playerdroparea;//デフォルトの親をドロップエリアに変更する
        if (place.placearea < 0) movement.defaultParent = gameManager.enemydroparea;//もしも敵のカードなら、敵のドロップエリアに移動させる
        transform.parent = movement.defaultParent;//その親になったドロップエリアに移動させる。
        Debug.Log("対象の親　＝ " + transform.parent);

        transform.position = movement.defaultParent.position;//ここで座標を親に合わせる
        place.placearea = 0;//場所の判定を初期化
        place.placenumber = 0;
        model.pluspw = 0;
        Changestate( true);
        model.selectable = false;//セレクタブルを解除
        view.SetActiveSelectablePanel(false);
        view.Show(model);

        effected(false);//効果の対象になっていた場合は外す
        if (transform.parent == gameManager.playerdroparea)//どちらのドロップエリアに入ったかの確認
        {

            gameManager.drops.Add(this);//味方のドロップリストに追加する

        }
        else
        {
            gameManager.enemydrops.Add(this);//敵のドロップリストに追加する
        }



    }

}
