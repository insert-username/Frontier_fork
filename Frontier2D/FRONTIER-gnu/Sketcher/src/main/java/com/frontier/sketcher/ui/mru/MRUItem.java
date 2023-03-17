/**
 * Title:        MRUItem
 * Description:  MRU item
 */
package com.frontier.sketcher.ui.mru;

   public class MRUItem
   {
      public String displayText;
      public String data;
   
      public MRUItem(String displayTxt, String MRUdata)
      {
         displayText = displayTxt;
         data = MRUdata;
      }
   }