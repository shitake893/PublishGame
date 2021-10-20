using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class UI : MonoBehaviour
{
    [SerializeField] Text playerLifeTx;
    [SerializeField] Text enemyLifeTx;
    [SerializeField] Text TurnTx;


    [SerializeField] GameObject resultPanel;
    [SerializeField] Text resultText;
    [SerializeField] Text PHASEText;
    [SerializeField] Transform cardexplainpanel;


    GameManager gameManager;

    private void Start()
    {
        gameManager = GameManager.instance;
    }

    public void Sphase(GameManager.Phase phase )
    {
        PHASEText.text = phase.ToString();
    }

    public void Sturn(int turncount)
    {
        TurnTx.text = "ターン　" + turncount.ToString(); //変更点2
     
    }

    public void ShowLife()
    {
        playerLifeTx.text = gameManager.playerLife.ToString();
        enemyLifeTx.text = gameManager.enemyLife.ToString();
    }

    public void CheckLife()
    {
        if (gameManager.playerLife <= 0 || gameManager.enemyLife <= 0)
        {
            resultPanel.SetActive(true);
            playerLifeTx.text = 30.ToString();
            enemyLifeTx.text = 30.ToString();
            if (gameManager.playerLife <= 0)
            {
                resultText.text = "まけ";
            }
            else
            {
                resultText.text = "かち";
            }
            
        }
    }

    public void HiddonResultPanel()
    {
        resultPanel.SetActive(false);
    }
    public void ResultPanel()
    {
        resultPanel.SetActive(true);
    }

    public IEnumerator ViewEffectedcard(CardController card)
    {
        float interval = 30;

     
        GameObject explaincard = Instantiate(card.gameObject);
        explaincard.transform.parent = cardexplainpanel.transform;

        explaincard.transform.position = cardexplainpanel.position;
        explaincard.transform.localScale = Vector3.one * 3;
        explaincard.GetComponent<CardController>().place.placearea = -100;

        //透明度を変更する

            yield return new WaitForSeconds(0.25f);

     //   explaincard.GetComponent<Renderer>().material.color = explaincard.GetComponent<Renderer>().material.color - new Color(0, 0, 0, 255);
     //メッシュレンダラーで変えたがだめだった
     for(int i = 0; i < interval; i++)
        {
            foreach (Image col in explaincard.GetComponentsInChildren<Image>())
            {
                col.color = col.color - new Color(0, 0, 0, 1/ interval);
            }//とりあえず文字が残った
            foreach (Text tx in explaincard.GetComponentsInChildren<Text>())
            {
                tx.color = tx.color - new Color(0, 0, 0, 1 / interval);
            }
            yield return new WaitForSeconds(0.75f / interval);
        }

       

        //yield return new WaitForSeconds(1);
        Destroy(explaincard.gameObject);
    }
}
