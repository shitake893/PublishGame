using System;

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using DG.Tweening;

public class CardMovement : MonoBehaviour, IDragHandler, IBeginDragHandler, IEndDragHandler
{
    public Transform defaultParent;//元々の親、移動の際にあらかじめ決めておく
    GameManager gameManager;
    void Start()
    {
        gameManager = GameManager.instance;//ゲームマネージャーのインスタンス生成
    }

    public void OnBeginDrag(PointerEventData eventData)//ドラッグのし始め
    {
        

        defaultParent = transform.parent;//デフォルトの親を元々の親に設定する
        transform.SetParent(defaultParent.parent, false);//このオブジェクトの親を元の親の親に設定する

        GetComponent<CanvasGroup>().blocksRaycasts = false;//ポインターの阻害を無効にする
    }

    public void OnDrag(PointerEventData eventData)//ドラッグ時
    {
        transform.position = eventData.position;//マウスに沿ってカードを移動させる
    }

    public void OnEndDrag(PointerEventData eventData)//ドラッグ終了時
    {
        transform.SetParent(defaultParent, false);//親を元々の親にする
        GetComponent<CanvasGroup>().blocksRaycasts = true;//ポインターを阻害するようにする

    }
    public void SetCardTransform(Transform parentTransform) {//敵のカードの選択(召喚用)
     
        Transform[] findchildA = parentTransform.GetComponentsInChildren<Transform>(); //ここで全子オブジェクトを全て選択
        List<Transform> findchildAA = new List<Transform>();//子オブジェクト達の入れ物、前後共に
        
        foreach (DropPlace kari in parentTransform.GetComponentsInChildren<DropPlace>())//DropPlaceが設置されているオブジェクトのみを選択
        {
            findchildAA.Add(kari.GetComponent<Transform>());//findchildAAに入れていく
         
        }
        List<Transform> frontareas = new List<Transform>();//前列とされるオブジェクトを選択
        List<Transform> backareas = new List<Transform>();//後列列とされるオブジェクトを選択
        Debug.Log("モデルの読み込み　canhront = " + GetComponent<CardController>().model.canfront);
        if (GetComponent<CardController>().model.canfront == true)
        {
            foreach (Transform kari in findchildAA)//
            {
                if (kari.GetComponent<DropPlace>().placetype == -1)//もしもplacetypeが-1(用は前列ならば)ならば追加する
                {
                    CardController[] checkexistcard = kari.GetComponentsInChildren<CardController>();
                    if (checkexistcard.Length == 0)//この場所に既にカードが無ければ
                    {
                        frontareas.Add(kari);//そこを前列のリストに加える
                    }

                }
            }
            if (frontareas.Count > 0)
            {
                Debug.Log("カードはありませんでした");
                Transform thisplace = frontareas[0];//
                defaultParent = thisplace;
                transform.SetParent(defaultParent);
            }
            else//もしも上の方に開いているスペースが無かった場合
            {
                if (GetComponent<CardController>().model.canback == true)
                {
                    foreach (Transform kari in findchildAA)//
                    {
                        if (kari.GetComponent<DropPlace>().placetype == -2)//もしもplacetypeが-1(用は前列ならば)ならば追加する
                        {
                            CardController[] checkexistcard = kari.GetComponentsInChildren<CardController>();
                            if (checkexistcard.Length == 0)//この場所に既にカードが無ければ
                            {
                                frontareas.Add(kari);
                            }

                        }
                        if (frontareas.Count > 0)
                        {
                            Debug.Log("バックに空きスペースがありました");
                            Transform thisplace = frontareas[0];//
                            defaultParent = thisplace;
                            transform.SetParent(defaultParent);
                        }
                    }
                }
            }
        }
        else//前面には置けない場合
        {
            if(GetComponent<CardController>().model.canback == true)
            {
                foreach (Transform kari in findchildAA)//
                {
                    if (kari.GetComponent<DropPlace>().placetype == -2)//もしもplacetypeが-1(用は前列ならば)ならば追加する
                    {
                        CardController[] checkexistcard = kari.GetComponentsInChildren<CardController>();
                        if (checkexistcard.Length == 0)//この場所に既にカードが無ければ
                        {
                            frontareas.Add(kari);
                        }

                    }
                    if (frontareas.Count > 0)
                    {
                        Debug.Log("バックに空きスペースがありました");
                        Transform thisplace = frontareas[0];//
                        defaultParent = thisplace;
                        transform.SetParent(defaultParent);
                    }
                }
            }
        }
       
      


        //ここから前列のみを選択する

      
        //    Transform[] findchildBa =  findchildA[1].GetComponentsInChildren<Transform>();
        // Transform[] findchildBb = findchildA[2].GetComponentsInChildren<Transform>();

        //Transform[] findchildB = new Transform[findchildBa.Length + findchildBb.Length];

        //findchildBa.CopyTo(findchildB, 0);
        //findchildBb.CopyTo(findchildB, findchildBa.Length);
        //Transform thisplace = findchildB[1].Find("RRSUPanel");


        //defaultParent = thisplace; //前のデファルトペアレントの選択
        
        //Debug.Log("元の親　:" + parentTransform);
        //Debug.Log("第一の子要素　*" + findchildA[1]);
        //Debug.Log("第2の子要素　*" + findchildB[1]);
       
      //  Debug.Log("thisplace =" + thisplace);
      //  Debug.Log("defaultParent = " +defaultParent);

        }

    //敵のカードを移動させる演出
    public IEnumerator MoveToField(Transform field)//あくまで一時的、Returnで戻せる
    {
        //一度親を変更する
        transform.SetParent(defaultParent.parent);
        //DOTweenでカードをフィールドに移動
        transform.DOMove(field.position, 0.25f);

        yield return new WaitForSeconds(0.25f);
        
        transform.SetParent(field);
    }

    public IEnumerator ReturnField()
    {
        
        //一度親を変更する
        transform.SetParent(defaultParent.parent);
        //DOTweenでカードをフィールドに移動
        transform.DOMove(defaultParent.position, 0.25f);

        yield return new WaitForSeconds(0.25f);
        transform.SetParent(defaultParent);
    }

    public IEnumerator MoveToTarget(Transform target)//ガードする際、前に出てくる敵の演出
    {

        //現在の位置と並びを記録
        Vector3 currentPosition = transform.position;
        int indexPosition = transform.GetSiblingIndex();


        transform.SetParent(defaultParent.parent);
        //DOTweenでカードをtargetに移動
        transform.DOMove(target.position, 0.25f);
        yield return new WaitForSeconds(0.25f);

        //元の位置に戻す
        transform.DOMove(currentPosition, 0.25f);
        yield return new WaitForSeconds(0.25f);
        transform.SetParent(defaultParent);
        transform.SetSiblingIndex(indexPosition);
    }

   

}
