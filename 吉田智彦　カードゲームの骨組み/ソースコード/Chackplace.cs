using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Chackplace :MonoBehaviour
{
    //カードが何番目にあるか(横)
    public int placenumber = 0;
    //カードがどの場所にあるか(縦)
    public int placearea = 0;


    public void Setnumber(int number)
    {
        placenumber = number;
    }

    //public void SetPlace(int place)
    //{
    //    placearea = place;   
    //}
    //public int Shownumber()
    //{
    //    return placenumber;
    //}
    //public int Showarea()
    //{
    //    return placearea;
    //}
}
