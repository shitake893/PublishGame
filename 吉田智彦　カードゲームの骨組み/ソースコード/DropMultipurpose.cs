using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;


public class DropMultipurpose : MonoBehaviour, IDropHandler//多目的パネルを表示、処理するためのスクリプトです
{
   public bool cardin;//多目的パネルにカードが入ったかの判定
    void Awake()
    {
        cardin = false;//最初はcardinはfalseにしておく
    }

    public bool cardOK()//使用する側が使うもの、caidinにカードが入ったかを返す
    {
        return cardin;
    }
    CardController card;//入力されたカードをこの中に入れる
    // Start is called before the first frame update
    public CardController returning()//やりたいこと、一旦こっちで止まって、入力されるまで何もしない
    {//呼び出す側にカードを送る関数。cardOkでカードがあることを確認した後に、これを呼び出しカードを読み込む
        cardin = false;//カードは入っていないという判定にする
        return card;//入っていたカードを返す
    }

    //void IDropHandler.OnDrop(PointerEventData eventData)
    //{
    //    throw new System.NotImplementedException();
    //}
    void IDropHandler.OnDrop(PointerEventData eventData)//ドロップされた時、ここでカードをこの関数に入れる
    {
        card = eventData.pointerDrag.GetComponent<CardController>();//このパネルに落とされたカードを記録する
        cardin = true;//cardinをtrueにし、カードが入っていることを伝える準備をする
    }
}
