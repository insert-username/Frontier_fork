/**
 * Title:        SKUserObject
 * Description:  Used to keep track of user-defined objects in the current library (repository)
 */
package com.frontier.sketcher.ui.widgets;

import javax.swing.JLabel;
import java.awt.Graphics;

public class SKUserObject extends JLabel
{
   public String Name;

   public SKUserObject(String name)
   {
      Name = name;
   }

   public void paintComponent(Graphics g)
   {
      g.drawString(Name,0,0);
   }

   public String toString()
   {
      return Name;
   }
}