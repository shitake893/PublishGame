using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardView : MonoBehaviour
{
    [SerializeField] Text nameText;//名前のテキスト
    [SerializeField] Text pwText;//パワーのテキスト
    [SerializeField] Text lankText;//ランクのテキスト
    [SerializeField] Image iconImage;//アイコンのイメージ
    [SerializeField] Transform rect;//向いている方向
    [SerializeField] GameObject Front;//前に置けるという印
    [SerializeField] GameObject Back;//後ろに置けるという印

    [SerializeField] GameObject selectablePanel;//選択可能であることを示すパネル
    [SerializeField] GameObject effectedPanel;//効果の対象になっていることを示すパネル
    public GameObject Apointer;//攻撃時に表示される印
    public GameObject Tpointer;//攻撃の標的になった時に表示される印



    public void Show(CardModel cardModel)
    {//各項目にmodelから文字、画像を入力する
        nameText.text = cardModel.name;
        pwText.text = (cardModel.pw + cardModel.pluspw).ToString();
        lankText.text = cardModel.lank.ToString();
        iconImage.sprite =  cardModel.icon;

        Front.SetActive(false);//初めは、前、後ろの印は表示しない
        Back.SetActive(false);
        if (cardModel.canfront == true) {//だがmodelのcanflontがtrueなら前を表示
            Front.SetActive(true);
        }
        else
        {
            Front.SetActive(false);
        }
        if (cardModel.canback == true) //modelのcanbackがtrueなら後ろを表示
        { Back.SetActive(true);
        }
        else
        {
            Back.SetActive(false);
        }
        
        

    }

    public void SetActive(bool state)//ここでカードをレスト(スリープ)させる
    {
        if (!state)
        {
            rect.rotation = Quaternion.AngleAxis(-90, new Vector3(0, 0, 1));//不活化状態（攻撃できない）なら右90度回転
           
        }
        else
        {
            rect.rotation = Quaternion.AngleAxis(0, new Vector3(0, 0, 0));//活性化状態なら、回転はなし

        }
        
    }

    public void SetActiveSelectablePanel(bool flag)//もしも選択可能になったら、選択可能パネルを表示する
    {
        selectablePanel.SetActive(flag);

    }

    public void SetActiveEffectedPanel(bool flag)//もしも効果の対象に選択できるなら、効果対象パネルを表示する
    {
        effectedPanel.SetActive(flag);

    }

    public void SetApointer(bool juge)//A判定を出す（攻撃時に印を出す）
    {
        Apointer.SetActive(juge);
    }

    public void SetTpointer(bool juge)//T判定を出す（攻撃の標的になった場合はその印を出す）
    {
        Tpointer.SetActive(juge);
    }
}
