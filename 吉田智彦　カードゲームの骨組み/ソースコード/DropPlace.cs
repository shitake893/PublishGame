using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;



//おかれる側（フィールド）のスクリプト

public class DropPlace : MonoBehaviour, IDropHandler
{
    [SerializeField] public int placetype;//この場所がどのポジションか（前列か後列か）

    [SerializeField] public int placenumber;//この場所の座標(左から何番目か、攻撃可能かの判定に用いる)

    public void OnDrop(PointerEventData eventData)//ここにカードが置かれた時の処理
    {

        if (GameManager.instance.phase != GameManager.Phase.MAIN) return;//ゲームの状態がMAIN出なければ何もしない
        CardMovement card = eventData.pointerDrag.GetComponent<CardMovement>();//ドロップしたカードを記録する
        CardController controller = eventData.pointerDrag.GetComponent<CardController>();
        if (card != null)
        {
            if(placetype < 0)//プレイヤーフィールドでなければ何もしない
            {
                return;
            }

          if(controller.model.lank > GameManager.instance.playerLeaderarea.GetComponentInChildren<CardController>().model.lank)
            {
                Debug.Log("リーダーのランク　" + GameManager.instance.playerLeaderarea.GetComponentInChildren<CardController>().model.lank);
                Debug.Log("召喚するカードのランク　" + controller.model.lank);
                return;//カードがこちらのリーダーよりランクが上なら何もしない。
            }

            switch (placetype)//フィールドの場所ごとに置くかを決める
            {
                case 0://ケース0、何でも置けるところ
                    SummonCard();//カードを召喚する際の処理
                    break;

                case 1://ケース1、前面
                    if(controller.model.canfront == true)//そのカードが前面に置けるならば置く
                    {
                        SummonCard();//カードを召喚する際の処理

                    }
                    break;
                case 2://ケース2、後面
                    if(controller.model.canback == true)//そのカードが後面に置けるならば置く
                    {
                        SummonCard();


                    }
                    break;
                default:
                    break;
            }
        
        }

        void SummonCard()//カードの召喚処理
        {//ここでフィールドのカードに必要なデータを入力していく
            card.defaultParent = this.transform;//元々の親を置かれた場所にしておく
            controller.place.placenumber = placenumber;//場所のナンバーをカードに記入
            Debug.Log(controller.place.placenumber);
            controller.place.placearea = placetype ;//場所のタイプをカードに記入
            Debug.Log(controller.place.placearea);
        }
    }
}
