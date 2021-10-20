using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    



    public Transform playerHandTransform,
                                 playerFieldTransform,
                               enemyHandTransform ,
                                enemyFieldTransform;

   // [SerializeField] GameObject enemyFieldTransform;
    [SerializeField] CardController cardPrefab;
    public Transform playerLeaderarea;
    public Transform enemyLeaderarea;
    public Transform playersoularea;
    public Transform enemysoularea;
    public GameObject guardianerea;//ガーディアンエリアのもの
    public Transform playerdroparea;
    public Transform enemydroparea;
    public Transform playerDeckerea;
    public Transform enemyDeckerea;

    public GameObject effectpanel;//任意効果を発動する所
    public GameObject multipurposepanel;//任意効果を発動する所

    public Transform enemyguard;//敵がガードするところ
    public AI enemyai;//敵がガードするところ
    public UI ui;
    public Battle battle;
    public ShowExplain showexplain;


    public enum Phase
    {
        INIT,
        DRAW,
        MAIN,
        BATTLE,
        END
    };

    public bool PCOK;
    public Phase phase;
    bool isPlayerTurn;
    public static GameManager instance;
    private void Awake (){

        if (instance == null)
        {
            instance = this;
        }

        

    }

    Setgardian setgard;//ムリヤリガーディアンの設定を設けた、用改良

    public int playerLife  ;
    public int enemyLife  ;
    public int turncount ;


    List<int> playerDeck = new List<int>() { 1, 1, 2, 2, 3, 4, 5 };
  public  List<int> g_enemyDeck = new List<int>() { 1, 1, 2, 2, 3, 4, 5 };
  public  List<int> g_playerDeck = new List<int>() { 1, 1, 2, 2, 3, 4, 5 };


    List<int> LeadDeck = new List<int>() { 1, 3, 4, 5 };
    List<int> EMLeadDeck = new List<int>() { 1, 3, 4, 5 };
    List<CardController> souls = new List<CardController>() { };
   List<CardController> EMsouls = new List<CardController>() { };

    public List<CardController> drops = new List<CardController>() { };
    public List<CardController> enemydrops = new List<CardController>() { };

    public void ChangePhase(Phase phases){
        phase = phases;
       

        if (phase == Phase.DRAW)
        {
            //yield return でこの処理が完了するまで待つことができる
          StartCoroutine(DRAWroutine());
        }

        if(phase == Phase.MAIN)
        {
            StartCoroutine(MAINroutine());
        }

        if(phase == Phase.END)
        {

           
                StartCoroutine(ENDroutine());
        }
     }

    public IEnumerator DRAWroutine()
    {
        ui.Sphase(phase);   //変更点
        yield return StartCoroutine(effectroutine());
        ChangePhase(Phase.MAIN);
      

        PCOK = true;

        


        if (!isPlayerTurn) PCOK = false;

    }

    public IEnumerator MAINroutine()
    {
        ui.Sphase(phase);   //変更点

        yield return StartCoroutine(effectroutine());
        if(isPlayerTurn == true) PCOK = true;

    }

    public IEnumerator  ENDroutine()
        {
        ui.Sphase(phase);   //変更点

        yield return StartCoroutine(effectroutine());
                ChangePhase(Phase.DRAW);
            
                ChangeTurn();
             

       


    }

    public IEnumerator effectroutine()
    {
        yield return new WaitForSeconds(1);
        PCOK = false;
        CardController[] preEndEFcard;
        if (isPlayerTurn)
        {
            preEndEFcard = playerFieldTransform.GetComponentsInChildren<CardController>();
        }
        else
        {

            
            preEndEFcard = enemyFieldTransform.GetComponentsInChildren<CardController>();

        }
        CardController[] EndEFcard = Array.FindAll(preEndEFcard, card => card.model.effectC == phase);
        foreach (CardController card in EndEFcard)
        {
            card.model.selectable = true;//selectableでカードを選択できるようにする
            card.view.SetActiveSelectablePanel(true);
        }

        if(EndEFcard.Length != 0)
        {
            //自動カードの発動
            foreach (CardController Ecard in EndEFcard)
            {
                if (Ecard.model.effectB == 1)
                {
                    yield return new WaitForSeconds(0.5f);
                    yield return StartCoroutine(CardEffects.instance.Aeffects(Ecard));

                    yield return new WaitWhile(() => multipurposepanel.activeSelf);
                }
            }
           // yield return new WaitWhile(() => multipurposepanel.activeSelf);
            Debug.Log("任意効果発動前です");
           
            effectpanel.SetActive(true);//ここから任意カードの発動
            Debug.Log("任意効果の発動です phaseは　=" + phase　+ "カード群の長さ　=" + EndEFcard.Length + effectpanel.activeSelf);
            if (!isPlayerTurn)
            {
                yield return StartCoroutine(enemyai.effectactive(EndEFcard));
            }

            Debug.Log("敵がここならAI処理を終えました");
            
            while (EndEFcard.Length != 0 && effectpanel.activeSelf)
            {
                Debug.Log("任意効果発動！！！");
                yield return new WaitForSeconds(0.5f);
                

               
                EndEFcard = Array.FindAll(preEndEFcard, card => card.model.effectC == phase);

            }
            Debug.Log("エフェクトパネルが消え、whileるーぷから抜けました");
          

            
        }

    }




    public Transform Getfield(bool player)
    {
        if(player == true)
        {
            return playerFieldTransform;
        }
        else
        {
            return enemyFieldTransform;
        }
    }

    public CardController[] GetFieldcards(bool player)
    {
        if(player == true)
        {
            return playerFieldTransform.GetComponentsInChildren<CardController>();
        }
        else
        {
            return enemyFieldTransform.GetComponentsInChildren<CardController>();

        }
    }

    public void Phasebutton()
    {
        if (PCOK == false) return ;
        if(phase == Phase.END)
        {
            ChangePhase(Phase.DRAW);
            ChangeTurn();
            ui.Sphase(phase);   
        }
        else
        {
           ChangePhase( phase + 1);
            ui.Sphase(phase);   
        }
    }

    public IEnumerator GiveCardToHand(List<int> deck,Transform hand)
    {
        int cardID = deck[0];
        deck.RemoveAt(0);
        if(hand == playerHandTransform)//プレイヤーの手札にするなら
        {


            CardController card = Instantiate(cardPrefab,playerDeckerea, false);
            card.Init(cardID);
            card.movement.defaultParent = playerDeckerea.transform;
            yield return StartCoroutine( card.movement.MoveToField(hand));//ここでデッキを移動させる
           


        }
        else
        {
            CardController card = Instantiate(cardPrefab, enemyDeckerea, false);
            card.Init(cardID);
            card.movement.defaultParent = enemyDeckerea.transform;

            yield return StartCoroutine(card.movement.MoveToField(hand));//ここでデッキを移動させる



        }
        

    }

    void CreateCard(int cardID, Transform hand)
    {
        CardController card = Instantiate(cardPrefab, hand, false);
        card.Init(cardID);


    }


    void Start()
    {
        StartGame();
       


    }


    void StartGame()
    {

        ui.HiddonResultPanel();

        phase = Phase.INIT;
        PCOK = false;
        while (g_playerDeck.Count > 0)
        {
            g_playerDeck.RemoveAt(0);
        }

        while(g_enemyDeck.Count > 0)
        {
            g_enemyDeck.RemoveAt(0);
        }


        //  List<int> playerDeck = new List<int>() { 1, 1, 2, 2, 3, 4, 5 };
        //  List<int> enemyDeck = new List<int>() { 1, 1, 2, 2, 3, 4, 5 };

        //g_playerDeck.AddRange(playerDeck);
        //g_enemyDeck.AddRange(enemyDeck);
        
        ReadCard("Playercard.TXT",true);//主人公のデッキを読み込む
        g_playerDeck =  Cardshuffle(g_playerDeck);
        ReadCard(MoveProtagonist.GetEDname(), false);//敵のデッキを読み込む
        g_enemyDeck = Cardshuffle(g_enemyDeck);


        Debug.Log("プレイヤーデッキの長さ　=" + g_playerDeck.Count);

        while (LeadDeck.Count > 0)
        {
            LeadDeck.RemoveAt(0);
        }
        while(EMLeadDeck.Count > 0)
        {
            EMLeadDeck.RemoveAt(0);
        }

        List<int> coLeadDeck = new List<int>() { 1, 3, 5, 7 };
        List<int> coEMLeadDeck = new List<int>() { 1, 3, 6, 9 };
        LeadDeck.AddRange(coLeadDeck);
        EMLeadDeck.AddRange(coEMLeadDeck);

        while(drops.Count > 0)//ドロップファイルを消してしまう
        {
            drops.RemoveAt(0);
        }
        while (enemydrops.Count > 0)
        {
            drops.RemoveAt(0);
        }
        ui.HiddonResultPanel();
        SettingInitHand();
        isPlayerTurn = true;
        TurnCalc();
        UnitLeader();
        playerLife = 30;
        enemyLife = 30;
        turncount = 0;
        ui.Sturn(turncount);

     //   ui.ShowLife();

    }

    


    public void lifemove(int move)
    {
        playerLife += move;
    }
    public void Restart()
    {

        //handと fieldの削除
        CardController[] destroycardA = playerFieldTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in destroycardA)
        {
            Destroy(card.gameObject);
        }
        CardController[] destroycardB = playerHandTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in destroycardB)
        {
            Destroy(card.gameObject);
        }
        CardController[] destroycardC = enemyHandTransform.GetComponentsInChildren<CardController>();
        foreach (CardController card in destroycardC)
        {
            Destroy(card.gameObject);
        }
        CardController[] destroycardD = enemyFieldTransform.GetComponentsInChildren<CardController>();

        foreach (CardController card in destroycardD)
        {
            Destroy(card.gameObject);
        }

        //デッキを組みなおす

        SceneManager.LoadScene("TitleScene");

        StartGame();


    }


   

    void SettingInitHand()
    {
        //カードをそれぞれ3枚引く
        for (int i = 0; i < 3; i++)
        {
            StartCoroutine( GiveCardToHand(g_playerDeck, playerHandTransform));
            StartCoroutine( GiveCardToHand(g_enemyDeck,enemyHandTransform));
        }
    }

    void UnitLeader()
    {
        SettingLeader(LeadDeck, playerLeaderarea);
        SettingLeader(EMLeadDeck, enemyLeaderarea);
    }
    void SettingLeader(List<int>souldeck,Transform LeaderArea)
    {
        if (souldeck.Count <= 0) return;
        int cardID = souldeck[0];
        souldeck.RemoveAt(0);

        //あらかじめ元のリーダーをソウルに送っておく
        Transform soularea = playersoularea;
        if(LeaderArea == enemyLeaderarea)
        {
            soularea = enemysoularea;
        }

        CardController exLeader = LeaderArea.GetComponentInChildren<CardController>();
        if(exLeader != null)
        {
            exLeader.transform.parent = soularea;//元のリーダーカードをソウルに移す(親を変更)
            exLeader.transform.position = soularea.transform.position;//場所をソウルの場所に移す
           
            

            if (LeaderArea == playerLeaderarea)//リーダーエリアの場所により味方か敵かに分ける
            {
                souls.Add(exLeader.GetComponent<CardController>());
                exLeader.place.placearea = 10;//元リーダーカードのplaceareaを10(ソウル用)にする
                exLeader.place.placenumber = 0;
                souls.Add(exLeader);
            }
            else
            {
                EMsouls.Add(exLeader.GetComponent<CardController>());
                exLeader.place.placearea = 10;//元リーダーカードのenemyplaceareaを10(ソウル用)にする
                exLeader.place.placenumber = 0;
                EMsouls.Add(exLeader);
            }
        }
        
        //カード群を右にずらす
        //1　プレイヤーの
        
        for(int i = 0; i < souls.Count; i++)
        {
            Vector3 vector = playersoularea.transform.position;
            vector.x += 3 + 3 * i;
            vector.y += 3 + 3 * i;
            souls[i].transform.position = vector; 
        }
        
        //2　敵の
        for (int i = EMsouls.Count -1; i >= 0; i--)
        {
            Vector3 vector = enemysoularea.transform.position;
            vector.x -= 3 + 3 * i;
            vector.y -= 3 + 3 * i;
            EMsouls[i].transform.position = vector;
        }
        

        CreateCard(cardID, LeaderArea);
    }

    void TurnCalc()
    {
       
        turncount ++;
           ui.Sturn(turncount);  //変更点


        if (isPlayerTurn)
        {
             PlayerTurn();

        }
        else
        {
            StartCoroutine(enemyai.EnemyTurn());

        }
    }

     void PlayerTurn()
    {
        //  Debug.Log("Playerのターン");
        // CreateCard(playerHandTransform);もしも先行ドローさせたいならこっち
        ChangePhase(Phase.DRAW);

        

    }

  

    void waittime() {
        Debug.Log("一秒待ち時間です");
    }

    public void CardsGsetting(CardController attacker, CardController defender)
    {
        guardianerea.SetActive(true);
    }

  


   

    public void ChangeTurn()
    {
        isPlayerTurn = !isPlayerTurn;

        if (isPlayerTurn)
        {
            StartCoroutine( GiveCardToHand(g_playerDeck,playerHandTransform));
            fieldallstand(playerFieldTransform);
            FArepower(playerFieldTransform);
            FArepower(enemyFieldTransform);
            SettingLeader(LeadDeck, playerLeaderarea);
            Debug.Log("プレイヤーのターン");
           

        }
        else
        {
            StartCoroutine( GiveCardToHand(g_enemyDeck,enemyHandTransform));
            fieldallstand(enemyFieldTransform);
            FArepower(enemyFieldTransform);
            FArepower(playerFieldTransform);
            SettingLeader(EMLeadDeck, enemyLeaderarea);
            Debug.Log("敵のターン");
        }
        TurnCalc();
    }

    void fieldallstand(Transform field)//フィールドのカードを全てスタンドさせる
    {
        CardController[] FieldCardList = field.GetComponentsInChildren<CardController>();
        foreach (CardController Card in FieldCardList)
        {
            //カードを攻撃可能にする(実際は状態を1回復)
            Card.Changestate( true);
        }
    }
    void FArepower(Transform field)
    {
        CardController[] FieldCardList = field.GetComponentsInChildren<CardController>();
        foreach (CardController Card in FieldCardList)
        {
            
            Resetpower(Card);
        }
    }

    void Resetpower(CardController Card)
    {
        Card.model.pluspw = 0;
        Card.view.Show(Card.model);
    }

   

  


    void ReadCard(string Filename,bool isplayer)
    {
        Debug.Log("デッキケースの名前　=" + Filename);
        StreamReader sr = new StreamReader(@Filename);//読み込みたいところのバス
        for (int i = 0; i < 50; i++)
        {
            if (sr.EndOfStream)
            {
                break;
            }
            string line = sr.ReadLine();
            if(isplayer)
            {
                g_playerDeck.Add(int.Parse(line));
            }
            else
            {
                g_enemyDeck.Add(int.Parse(line));
            }

        }
    }
    public List<int> Cardshuffle(List<int> Deck)
    {
        List<int> cDeck = Deck.OrderBy(i => Guid.NewGuid()).ToList();
        Deck = cDeck;
        return cDeck;
    }

    public void GameEnd()
    {
        Application.Quit();
    }
}

