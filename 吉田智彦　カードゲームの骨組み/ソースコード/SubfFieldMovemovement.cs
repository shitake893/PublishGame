using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

//サブフィールドを移動する
public class SubfFieldMovemovement : MonoBehaviour, IDragHandler, IBeginDragHandler, IEndDragHandler
{

    [SerializeField] GameObject pointer;//ポインターに使うもの、今回はフィールドのコピー
    [SerializeField] Transform parent;//親の設定


    public Transform defaultParent;
    GameObject Pointer;

    public void OnBeginDrag(PointerEventData eventData)
    {
        Pointer =  Instantiate(pointer ,parent , false );
        Pointer.GetComponent<CanvasGroup>().blocksRaycasts = false;//持っている物がポインターの邪魔をしないようにする
    }

    public void OnDrag(PointerEventData eventData)
    {
        Pointer.transform.position = eventData.position;//ポインターのポジションをドラッグしている所にする
    }

    public void OnEndDrag(PointerEventData eventData)//ドロップすると消失する
    {
        Debug.Log("オンエンドドラッグ");
      
        Debug.Log("LAST =" + transform.parent);
        Destroy(Pointer);
       // Pointer.GetComponent<CanvasGroup>().blocksRaycasts = true;
        

    }


   

   
}
