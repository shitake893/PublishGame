using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;


//カードをクリックした際、説明パネルを使ってカードの説明が表示されるようにする。
//表示内容の詳細はGameManager経由のSHowExplainで設定する
public class OnClick : MonoBehaviour, IPointerClickHandler
{
    public void OnPointerClick(PointerEventData eventData)//カードがクリックされたら
    {
        GameManager.instance.showexplain.A(GetComponent<CardController>()) ;//ゲームマネージャーから説明処理を行う
    }
}
