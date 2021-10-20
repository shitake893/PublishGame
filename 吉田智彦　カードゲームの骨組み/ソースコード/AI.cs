using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AI : MonoBehaviour
{
    GameManager gameManager;
    private void Start()
    {
        gameManager = GameManager.instance;
    }
    public IEnumerator EnemyTurn()
    {
        //  Debug.Log("敵のターン");
        // CreateCard(enemyHandTransform);もしも先行どろーさせたいならこっち

        //ここでドローフェイズの処理が終わっているか確認する
        //そのためにyield return StartCoroutine(〇●（）)で、その非同期処理を完了するまで待つことが出来る
        //gameManager.ChangePhase(GameManager.Phase.DRAW);  エンド、ドローからメインまではChangePhase内でできるので待つ

        yield return new WaitWhile(() => gameManager.phase == GameManager.Phase.MAIN);

        /*場にカードを出す*/
        //手札のカードリストを選択




       
        CardController[] handcardList = gameManager.enemyHandTransform.GetComponentsInChildren<CardController>();
        // Debug.Log(enemyHandTransform.GetComponentsInChildren<CardController>());
        //場に出すカードを選択
        //ここで場に空きがある限り、かつ手札に出せるカードがある限り何度でも召喚する
        foreach(CardController enemyCard in handcardList)
        {
            yield return new WaitForSeconds(1);
            Debug.Log(gameManager.enemyHandTransform);
            Debug.Log(handcardList[0]);
            //カードを移動
            if(gameManager.enemyLeaderarea.GetComponentInChildren<CardController>().model.lank >= enemyCard.model.lank)//リーダーのランクよりもリーダーデッキのカードがランクが上ならば
            enemyCard.movement.SetCardTransform(gameManager.enemyFieldTransform);
            if (enemyCard.transform.parent.transform.GetComponent<DropPlace>() != null)//もしも親にDropPlaceがあれば
            {
                Transform oya = enemyCard.transform.parent.transform;//親の変更
                enemyCard.place.placenumber = oya.GetComponent<DropPlace>().placenumber;//場所のデータをDropに合わせる
                enemyCard.place.placearea = oya.GetComponent<DropPlace>().placetype;
            }
        }
       



        yield return StartCoroutine( gameManager.MAINroutine());//ここで使ったカードに効果がないか確認
        gameManager.ChangePhase(GameManager.Phase.BATTLE);
        /*攻撃してくる*/
        //フィールドのカードリストを取得
        List<CardController> cardFieldList = new List<CardController>(gameManager.enemyFieldTransform.GetComponentsInChildren<CardController>());
        //attackerカードを取得
        CardController attacker = cardFieldList[0];
        //defenderカードを選択

        List<CardController> playerFieldCardList = new List<CardController>(gameManager.playerFieldTransform.GetComponentsInChildren<CardController>());

        //後方のカードでブーストさせる。
        for(int i = 0;i < cardFieldList.Count; i++)
        {
            if(cardFieldList[i].place.placearea == -2)//ブーストする側
            {
                Debug.Log("ブーストするカードが決まりました");
                for(int j = 0; j < cardFieldList.Count; j++)//ブーストされる側
                {
                    Debug.Log("ブーストされる相手を選ぶところまでいきました");
                    if(cardFieldList[i].place.placenumber == cardFieldList[j].place.placenumber && i != j)//もしもする側とされる側のplacenumberが同じで、カードが同じでないなら
                    {
                        cardFieldList[j].model.pluspw += cardFieldList[i].model.pw + cardFieldList[i].model.pluspw;
                        cardFieldList[i].Changestate(false);//ブーストしたカードを横向きにする
                        cardFieldList[j].view.Show(cardFieldList[j].model);//ブーストされたカードの表示を更新する
                    }
                }
            }
        }

        if (playerFieldCardList.Count >= 1)//プレイヤーのカードリストが一つ以上あるならばここから攻撃する
        {

            for (int i = 0; i < cardFieldList.Count; i++)//敵のカードを探索する
            {
                for (int j = 0; j < playerFieldCardList.Count; j++)//プレイヤーのフィールドのカードを全て確認
                {
                    if (cardFieldList[i].place.placenumber <= 5 - playerFieldCardList[j].place.placenumber + 1//まず、場所が右に一つだけずれている以下か、
                        && cardFieldList[i].place.placenumber >= 5 - playerFieldCardList[j].place.placenumber - 1//そして左に一つだけずれている以上か
                        && cardFieldList[i].place.placearea == -1 && playerFieldCardList[j].place.placearea == 1 && cardFieldList[i].model.state == true)
                    {//そして敵が前列、味方も前列ならば、さらに敵のステートが攻撃可能ならば、攻撃する
                        while (gameManager.guardianerea.activeSelf)//プレイヤーのガードパネルが出ている間は待つ
                        {
                            Debug.Log("一秒待ちます");
                            yield return new WaitForSeconds(1);
                        }
                        Debug.Log("セットアクティブかの確認" + gameManager.guardianerea.activeSelf);


                        gameManager.guardianerea.SetActive(true);//味方側にガード判定を出す
                        Setgardian gard = gameManager.guardianerea.GetComponent<Setgardian>();
                        gard.Setbattle(cardFieldList[i], playerFieldCardList[j]);//攻撃側と防御側を指定する

                        while (gameManager.guardianerea.activeSelf)
                        {
                            Debug.Log("一秒待ちます");
                            yield return new WaitForSeconds(1);
                        }
                        if (playerFieldCardList[j] == null) playerFieldCardList.RemoveAt(j);//もうjにカードが無ければ、リストからも除外
                        if (cardFieldList[i] == null) cardFieldList.RemoveAt(i);//もうi（敵カード）が無くなっていれば、リストから除外

                        //Setbattle(cardFieldList[i], playerFieldCardList[j]);//注意、すこしオブジェクトの指定が危険
                        //CardsBattle(cardFieldList[i], playerFieldCardList[j],0);//一時的にサポートを0にしている
                    }
                }
            }



            Debug.Log("forループからは抜けています");
            while (gameManager.guardianerea.activeSelf)
            {
                Debug.Log("一秒待ちます");
                yield return new WaitForSeconds(1);
            }

            //  CardController defender = playerFieldCardList[0];
            //attackerとdefenderを戦わせる
            //Debug.Log("attacerの場所ナンバー　=" + attacker.place.placenumber);
            //Debug.Log("defenderの場所ナンバー　=" + defender.place.placenumber);
            //  if (attacker.place.placenumber <= 5 - defender.place.placenumber + 1 && attacker.place.placenumber >= 5 - defender.place.placenumber - 1)
            // {
            //    CardsBattle(attacker, defender);
            //  }
        }
        //gameManager.ChangePhase(GameManager.Phase.END);

        gameManager.phase = GameManager.Phase.END;//フェーズの変更、END（戦闘の終了）
        gameManager.ui.Sphase(gameManager.phase);//フェーズ表示の変更
        yield return StartCoroutine( gameManager.ENDroutine());//エンドフェーズのルーチンを発動


       // gameManager.ChangeTurn();
    }

    public IEnumerator enemygardsed(CardController attacker, CardController defender)//敵のガード時のAI
    {
        Debug.Log("敵側のガード処理");
        yield return new WaitForSeconds(1);//少し待つ、一瞬で終わらないように
        CardController[] Precanguardcards = gameManager.enemyFieldTransform.GetComponentsInChildren<CardController>();//敵フィールドから仮にカードリストに納める
        CardController[] canguardcards = Array.FindAll(Precanguardcards, card => (card.place.placenumber == defender.place.placenumber - 1
        || card.place.placenumber == defender.place.placenumber + 1) && card.place.placearea == -1);//そのリストから、前列で、なおかつ被攻撃者から左右のとなりのものを選択

        bool onegard = false;//一つのカードだけでガード可能かの判定
        int cardmount = 0;//カードの数
        int thiscard = 0;//ガード用のカード

        for (int i = 0; i < canguardcards.Length; i++)//総当たりをかけ、ガードが可能なカードを探る
        {
            if (attacker.model.pw + attacker.model.pluspw < defender.model.pw + defender.model.pluspw + canguardcards[i].model.pw + canguardcards[i].model.pluspw)
            {
                onegard = true;
                cardmount++;
                thiscard = i;
            };

        }
        Debug.Log("カードカウント　=" + cardmount);
        Debug.Log("一つだけでガード可能か?=" + onegard);
        if (onegard == true)//もしも一つでガードできるなら
        {
            CardController selected;
            if (cardmount == 1)//ちょうど一つのカードでガードできるならそれでガードする
            {
                Debug.Log("パターン1、ちょうど一つのカードだけでガード");
                selected = canguardcards[thiscard];

            }
            else//二つ以上一つだけでガードできるカードがある場合
            {
                Debug.Log("パターン2、一つのカードでガードできるのが二つ以上");
                if (canguardcards[0].model.lank == canguardcards[1].model.lank)//もしも競合が同ランクだったら
                {
                    //カードのパワーでどちらを使うかを決める(後のガードの事を考え、パワーが低い方を出す（同じ場合は初めのものを出す）)
                    if (canguardcards[0].model.pw + canguardcards[0].model.pluspw >= canguardcards[1].model.pw + canguardcards[1].model.pluspw)
                    {
                        selected = canguardcards[1];
                    }
                    else
                    {
                        selected = canguardcards[0];
                    }
                }
                else
                {//ランクが違ったら低い方でガードする
                    if (canguardcards[0].model.lank >= canguardcards[1].model.lank)
                    {
                        selected = canguardcards[1];
                    }
                    else
                    {
                        selected = canguardcards[0];
                    }
                }



            }
            //このカードをサポーターとして出す
            if (selected.model.state == true)//ガードに出したカードの状態が攻撃可能な場合
            {

                StartCoroutine(selected.movement.MoveToField(gameManager.enemyguard));
                yield return new WaitForSeconds(0.25f);
                gameManager.battle.CardsBattle(attacker, defender, selected.model.pw + selected.model.pluspw);//ガード値を足した状態で戦闘を行う

                StartCoroutine(selected.movement.ReturnField());
                yield return new WaitForSeconds(0.25f);
                selected.Changestate( false);//攻撃を行った状態にする
            }

            else
            {

                StartCoroutine(selected.movement.MoveToField(gameManager.enemyguard));//カードが既に攻撃を行った場合はガードしたカードは破壊される
                yield return new WaitForSeconds(0.25f);
                gameManager.battle.CardsBattle(attacker, defender, selected.model.pw + selected.model.pluspw);
                if (selected.place.placenumber == 3)
                {
                    gameManager.enemyLife -= 2;

                    StartCoroutine(selected.movement.ReturnField());
                    yield return new WaitForSeconds(0.25f);
                }
                else
                {
                    selected.GotoDrop();
                  
                }

            }
            gameManager.ui.ShowLife();//ライフの表示を変更
            gameManager.ui.CheckLife();//どちらかのライフがゼロになったかを確認

        }
        else//一つのカードではガードできない場合
        {
            Debug.Log("一つではガード出来ない場合");
            int add = 0;//合計値
            foreach (CardController card in canguardcards)
            {//合計値を足し続ける
                add += card.model.pw + card.model.pluspw;
            }
            if (attacker.model.pw + attacker.model.pluspw < defender.model.pw + defender.model.pluspw + add)//もしも合計値で相手を上回れるならガードする
            {
                Debug.Log("パターン３、まとめてガードできた");
                Debug.Log("addの値　=" + add);

                foreach (CardController card in canguardcards)//こっちでガードエリアまで運ぶ
                {
                    StartCoroutine(card.movement.MoveToField(gameManager.enemyguard));
                    yield return new WaitForSeconds(0.25f);
                    //if (card.model.state == true)
                    //{
                    //    StartCoroutine(card.movement.MoveToField(enemyguard));

                    //    StartCoroutine(card.movement.ReturnField());

                    //    Changestate(card, false);


                    //}
                    //else
                    //{
                    //    StartCoroutine(card.movement.MoveToField(enemyguard));
                    //    yield return new WaitForSeconds(0.25f);
                    //    Destroy(card.gameObject);

                    //}
                }
                gameManager.battle.CardsBattle(attacker, defender, add);//ガード値を足して戦闘
                foreach (CardController card in canguardcards)//こっちで元のばしょに戻すか壊す
                {
                    if (card.model.state == true)//ガードするカードが攻撃可能な場合
                    {
                        StartCoroutine(card.movement.ReturnField());
                        yield return new WaitForSeconds(0.25f);
                        card.Changestate(false);//攻撃した判定にする
                    }
                    else
                    {
                        if (card.place.placenumber == 3)//もしもリーダーをガードに出して、それが攻撃可能で無い場合
                        {
                            gameManager.enemyLife -= 2;//ライフを2減らす
                            StartCoroutine(card.movement.ReturnField());
                            yield return new WaitForSeconds(0.25f);
                        }
                        else
                        {
                            card.GotoDrop();//攻撃不可能で、リーダーでない場合は破壊する
                        }
                    }

                }
            }
            else//合計してもガードができない場合
            {
                Debug.Log("ガード不可");
                gameManager.battle.CardsBattle(attacker, defender, 0);

            }
        }
    }


    public IEnumerator effectactive(CardController[] EndEFcard)//敵AIによる効果処理のAI
    {
        Debug.Log("敵AIによる任意効果の選択処理に入りました");
        yield return new WaitForSeconds(1);

        foreach(CardController card in EndEFcard)//もしもエンド時に効果があるものがあれば、処理を続ける
        {
            if(card.model.selectable == true)//もしもカードが選択可能ならば
            {
                yield return StartCoroutine(CardEffects.instance.Aeffects(card));//ここでは発動するのみ、特殊効果は発動するか後に対応（コストが重すぎる場合は発動しない、ライフに見合ってないなど）
                yield return new WaitForSeconds(0.5f);

              //  card.Changestate(false);
            }
        }

        gameManager.effectpanel.SetActive(false);
        Debug.Log("敵AIによる任意効果の処理、終わり");
    }


    public IEnumerator selectDestroy(List<CardController> cards, int mount)//選択破壊効果の際に、破壊するカードを選択する
    {
        //ターゲットでエフェクティッドされているものを選ぶ
       // List<CardController> targets = new List<CardController>(Array.FindAll(cards, card => card.model.effected == true));
        //まずは最適を一つ選び（初期の設定、その後それらと比べる、グレード、次にパワー）
        for (int i = 0; i < mount; i++)
        {
            CardController target = cards[0];//とりあえず、最初のカードを選択しておく

            List<CardController> sames = new List<CardController> {target };//同程度の優先度で選択されたものを集めておく

            foreach(CardController card in cards)
            {
                if(target.model.lank < card.model.lank)//より大きいランクのものを狙うようにする
                {
                    target = card;//その大きいものがターゲットになる
                    while (sames.Count > 0)
                    {
                        sames.RemoveAt(0);//逸れ以外は外す
                    }
                    sames.Add(card);//その大きいものを加えておく
                  
                }
               else if (target.model.lank == card.model.lank)
                {
                    sames.Add(card);//ランクが同じならば入れておく
                }

            }

            if(sames.Count != 1)//もしも絞り込めなかった場合は、効果があるかどうかで選択する、それでもダメなら最初のものを選択
            {
                target = sames[0];
                foreach (CardController card in sames)
                {
                    if (card.model.effectA != 0)
                    {
                        target = card;
                    }
                }

               
            }
            yield return new WaitForSeconds(0.5f);
            //Destroy(target.gameObject);//カードを破壊する
            Debug.Log("効果3で破壊するカードが決まりました、");
            target.GotoDrop();

        }
    }
}
