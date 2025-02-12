/**
 * Title:        MRUMenuItem
 * Description:  JMenuItem with MRU info
 */
package com.frontier.sketcher.ui.mru;

   import javax.swing.JMenuItem;

   public class MRUMenuItem extends JMenuItem
   {
      MRUManager theMRUManager;
      MRUItem    MRUitem;
   
      public MRUMenuItem(MRUManager manager, MRUItem item)
      {
         super(item.displayText);
         theMRUManager = manager;
         MRUitem = item;
      }
   }