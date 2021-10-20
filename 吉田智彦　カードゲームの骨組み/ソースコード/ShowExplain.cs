using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ShowExplain : MonoBehaviour
{
    [SerializeField] GameObject explainpanel;
    [SerializeField] Transform cardpanel;

    [SerializeField] Text ranktext;
    [SerializeField] Text powertext;
    [SerializeField] Text fronttext;
    [SerializeField] Text backtext;
    [SerializeField] Text explaintext;
    [SerializeField] Text nametext;


    CardModel cardModel;

    public void A(CardController card)//ここから説明をする
    {
        
        explainpanel.SetActive(true);//パネルを表示する

        //ここからカードの情報を表示する
        nametext.text = card.model.name;//名前、ランク、パワーのテキストを表示する
        ranktext.text = card.model.lank.ToString();
        powertext.text = card.model.pw.ToString();
             if(card.model.canfront == true)//もしも前面に置けるなら、前面に〇と、ダメならば×と表示する
           {
            fronttext.text = "○";
           }
            else
            {
              fronttext.text = "×";
               }
            if (card.model.canback == true)
            {
                backtext.text = "○";
            }
            else
            {
                backtext.text = "×";
            }

        explaintext.text = card.model.explain;//説明欄には説明用のテキストを読み込む
        
        foreach(Transform child in cardpanel)//ここで元々あったカードを一度破壊する
        {
            Destroy(child.gameObject);
        }

        GameObject plaincard = Instantiate(card.gameObject);//ここで新たに選択されたカードを生成する。
        plaincard.transform.position = cardpanel.transform.position;
        plaincard.transform.parent = cardpanel.transform;
    }

    public void ExplainEndButton()//エンドボタンを押されたら、表示を消す
    {
        explainpanel.SetActive(false);
    }
}
