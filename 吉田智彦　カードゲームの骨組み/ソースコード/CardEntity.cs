using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(fileName = "CardEntity",menuName = "Create Card Entity")]
public class CardEntity : ScriptableObject
{
    public new string name;//カードの名前

    public int pw;//パワー
    public int lank;//ランク
    public Sprite icon;//アイコン
    public bool frontplace;//前に置けるかの判定
    public bool backplcace;//後ろに置けるかの判定
    public int effectA;  //効果の種類、各カードごとの効果
    public int effectB;//0なら何もなし、1なら起動効果、2なら任意効果、3なら永続効果
    public GameManager.Phase effectC;
    /*0なら何もなし、1ならドローフェイズ、2ならメインフェイズ、
      3ならバトルフェイズ、4なら戦闘時、5ならエンドフェイズ！
     */
   [TextArea(1,7)] public string explaintext;//このカードの説明テキスト
  
}
