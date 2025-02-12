/**
 * Title:        SKItemInterface
 * Description:  Interface for SKBaseShape and SKBaseConstraint to implement common functionality
 */
package com.frontier.sketcher.ui.items;

   public interface SKItemInterface
   {
   //For hit test
      public boolean doHitTest(int x, int y);
      public String toString();
   
   //For clean-up
      public void severTies();
   
   //Misc
      public SKItemInterface getClonedItem();
   }