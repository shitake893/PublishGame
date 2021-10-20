using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;




public class Activateeffect : MonoBehaviour, IDropHandler
{
    [SerializeField] GameObject OKbutton;//OKボタンのオブジェクト
    GameManager gameManager;

    void Start()
    {
        gameManager = GameManager.instance;//ゲームマネージャーのインスタンス生成
    }

    // Start is called before the first frame update
    public void OnDrop(PointerEventData eventData)//効果カードをドロップ
    {
        
        CardController effecter = eventData.pointerDrag.GetComponent<CardController>();
        if (effecter.model.selectable != true) return;//もしも選択可能でなければ何もしない
        Debug.Log("選択パネルにカードが運ばれました");
        StartCoroutine(CardEffects.instance.Aeffects(effecter)) ;//カードの効果処理


    }

    public void pushOKbutton()//OKボタンを押されたら
    {

        CardController[] EMcardFieldList = gameManager.enemyFieldTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in EMcardFieldList)//敵のカードの選択可能を解除、及びそのためのパネルを消す
        {
            card.model.selectable = false;
            card.view.SetActiveSelectablePanel(false);
        }

        CardController[] PLcardFieldList = gameManager.playerFieldTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in PLcardFieldList)//プレイヤーのカードにおいて、選択を解除
        {
            card.model.selectable = false;
            card.view.SetActiveSelectablePanel(false);
        }

        this.gameObject.SetActive(false);//選択用のパネルを消す
    }
}
