/**
 * Title:        SKBifurFrame
 * Description:  Frame used to display bifurcations
 */
package com.frontier.sketcher.ui.dialogs;

import com.frontier.sketcher.ui.utils.SKGTNArray;
import com.frontier.sketcher.ui.groups.SKGroups;
import com.frontier.sketcher.ui.app.SKMainFrame;
import com.frontier.sketcher.ui.items.constraints.SKAngleConstraint;
import com.frontier.sketcher.ui.items.constraints.SKDistanceConstraint;
import com.frontier.sketcher.ui.items.shapes.SKShapeArray;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.Graphics;

public class TreeDialog extends JDialog
{
   public static SKMainFrame frame1;
   public static SKGTNArray Groups = new SKGTNArray(100);
   public static int ID,w,h;
   public static trycanvas try1 = null;
   public static JScrollPane pnlShapes = null;
   public static int[] flag;

   public float  lastScale = 1;
   public boolean approved = true;
   public JButton btnContinue = new JButton();
   public JButton btnCancel = new JButton();

   public JPanel pnlBifurButtons = new JPanel();
   public JPanel pnlMain = new JPanel();
   public JPanel pnlButtons = new JPanel();

   public JScrollBar scrVert = new JScrollBar();
   public JScrollBar scrHoriz = new JScrollBar();

   public BorderLayout borderLayout1 = new BorderLayout();
   public BorderLayout borderLayout2 = new BorderLayout();

   public TreeDialog(Frame owner, SKGTNArray allGroups, int actID, int[] fin, boolean modal)
   {
      super(owner,modal);
      frame1 = (SKMainFrame)owner;
      allGroups.copyArrayTo(Groups);
      ID = actID;
      try1 = new trycanvas();
      pnlShapes = new JScrollPane(try1);
      flag = new int[fin.length];
      for(int i=0; i<fin.length; i++)
         flag[i] = fin[i];
      pack();
      //pnlShapes = new JScrollPane(try1);
      try
      {
         jbInit();
         setSize(500,350);
         pnlShapes.setPreferredSize(new Dimension(300,250));
         //Center on desktop
         Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
         Dimension d = getSize();
         int x = (screen.width - d.width) >> 1;
         int y = (screen.height - d.height) >> 1;
        // setLocation(x, y);
      }
         catch(Exception e)
         {
            e.printStackTrace();
         }

   }


   private void jbInit() throws Exception
   {
      pnlMain.setLayout(borderLayout1);

      btnContinue.setText("Continue");
      btnContinue.addActionListener(

                              new java.awt.event.ActionListener()

                              {


                                 public void actionPerformed(ActionEvent e)

                                 {
                                    btnContinue_actionPerformed(e);
                                 }
                              });

      btnCancel.setText("Cancel");
      btnCancel.addActionListener(

                              new java.awt.event.ActionListener()

                              {


                                 public void actionPerformed(ActionEvent e)

                                 {
                                    btnCancel_actionPerformed(e);
                                 }
                              });

      pnlMain.setBorder(BorderFactory.createLoweredBevelBorder());
      pnlButtons.setLayout(borderLayout2);

      this.addWindowListener(
                              new java.awt.event.WindowAdapter()
                              {

                                 public void windowOpened(WindowEvent e)
                                 {
                                    this_windowOpened(e);
                                 }
                              });
      pnlBifurButtons.add(btnContinue, null);
      pnlBifurButtons.add(btnCancel, null);
      this.getContentPane().add(pnlMain, BorderLayout.CENTER);
      pnlMain.add(pnlShapes, BorderLayout.CENTER);
      pnlButtons.add(pnlBifurButtons, BorderLayout.CENTER);
      this.getContentPane().add(pnlButtons, BorderLayout.SOUTH);
   }

   public void btnContinue_actionPerformed(ActionEvent e)
   {
      frame1.IntArray[0]=frame1.getModeStateMachine().getModeFlag().getCode();
      frame1.treeCont = true;
      System.out.println("treedia "+frame1.getModeStateMachine().getModeFlag().getCode());
      hide();
      frame1.RefreshShapeArea();
   }
   public void btnCancel_actionPerformed(ActionEvent e)
   {
      approved = false;
      hide();
      frame1.allshapes.copyArrayTo(frame1.oldShapes);
      frame1.allConstraints.copyArrayTo(frame1.oldConstraints);
      frame1.oldShapeID = frame1.IDCnt - 1;
      frame1.oldConstraintID = frame1.ConstrIDCnt;
      frame1.oldGroupID = frame1.GroupIDCnt;

      for(int i=0; i<frame1.oldConstraints.size(); i++)
      {
         if(frame1.oldConstraints.get(i) instanceof SKDistanceConstraint)
            frame1.consArray[frame1.oldConstraints.get(i).ID] = ((SKDistanceConstraint)frame1.oldConstraints.get(i)).distance;

         else if(frame1.oldConstraints.get(i) instanceof SKAngleConstraint)
            frame1.consArray[frame1.oldConstraints.get(i).ID] = ((SKAngleConstraint)frame1.oldConstraints.get(i)).angle;
      }
      //frame1.mode = 0;

   }
   public void this_windowOpened(WindowEvent e)
   {this.show();
   }

   public static class trycanvas extends Canvas


   {
      int center = 0;
      SKGTNArray temp = new SKGTNArray(100);
      SKGTNArray final1 = new SKGTNArray(100);
      SKGTNArray root = new SKGTNArray(100);
      SKGTNArray children = new SKGTNArray(100);
      SKShapeArray exshapes = new SKShapeArray(100);
      String title = "god id the greatest";
      int x1,y1,x2,y2,total;
      private Color black = Color.black;
      private Color magenta = Color.magenta;
      private Color red = Color.red;
      private Color blue = Color.blue;


      public void paint(Graphics g)
      {
         root = new SKGTNArray(2);
         Groups.copyArrayTo(temp);
         for(int i=0; i< temp.size();i++)
         {
            if(temp.get(i).parents.size()==0)
               root.add(temp.get(i));
         }
         SKGTNArray tempgroups = new SKGTNArray(root.size());
         SKGTNArray temproots = new SKGTNArray(root.size());
         root.copyArrayTo(temproots);
         for(int i=0; i<temproots.size(); i++)
         {
            SKGroups.separateGroups(temproots.get(i),temp,tempgroups);
            int rootsize= temproots.get(i).shapes.size();
            for(int a=0; a<tempgroups.size(); a++)
            {
               int diff=rootsize-tempgroups.get(a).shapes.size();
               tempgroups.get(a).width=((temproots.get(i).width)-50*diff);

            }

            tempgroups.clear();
         }

         for(int i=0; i< root.size();i++)
         {root.get(i).x= (this.getWidth()/(root.size()+1))*(i+1);
            root.get(i).y=50;

         }
         final1.clear();
         if(root.size()>0)
            for(int i=0;i<root.size();i++)
               final1.add(root.get(i));

         while(root.size()>0)
         {SKGTNArray temproot = new SKGTNArray(100);
            for (int a=0; a<root.size(); a++)
               for (int b=0; b<root.get(a).children.size(); b++)
               {
                  children.add(root.get(a).children.get(b));
                  temproot.add(root.get(a).children.get(b));
               }
            if(temproot.size()>0)temproot.copyArrayTo(root);
            else root.clear();
         }



         if(children.size()>0)
            for(int i=0;i<children.size();i++)
               //if(final1.indexOf(children.get(i))==-1)
               final1.add(children.get(i));

         for(int i=0; i<final1.size();i++)
         {
            exshapes.clear();
            if(final1.get(i).children.size()>0)
            {
               SKShapeArray childshapes = new SKShapeArray(100);
               for(int c = 0; c <final1.get(i).children.size();c++)
                  for(int b=0; b<final1.get(i).children.get(c).shapes.size();b++)
                     if(   childshapes.indexOf( final1.get(i).children.get(c).shapes.get(b) ) == -1 )
                        childshapes.add(final1.get(i).children.get(c).shapes.get(b));

               if(final1.get(i).shapes.size()>childshapes.size())
               {//SKShapeArray exshapes = new SKShapeArray(100);
                  for(int c=0; c<final1.get(i).shapes.size(); c++)
                     if(childshapes.indexOf(final1.get(i).shapes.get(c))==-1)
                        exshapes.add(final1.get(i).shapes.get(c));
                  total = final1.get(i).children.size()+exshapes.size();
               }

               for(int a=0; a<final1.get(i).children.size(); a++)
               {
                  final1.get(i).children.get(a).y= final1.get(i).y + 45;
                  if(exshapes.size()==0)
                     final1.get(i).children.get(a).x= (final1.get(i).x-(final1.get(i).width/2))+(  (final1.get(i).width/ (final1.get(i).children.size()+1) )*(a+1));
                  else final1.get(i).children.get(a).x= (final1.get(i).x-(final1.get(i).width/2))+((final1.get(i).width/(total+1))*(a+1));
               }


               if(exshapes.size()>0)
                  for(int b=0;b<exshapes.size();b++)
                  {
                     x1=final1.get(i).x;
                     y1=final1.get(i).y;
                     y2=final1.get(i).y+45;
                     int exsx=(final1.get(i).x-(final1.get(i).width/2))+((final1.get(i).width/(total+1))*(final1.get(i).children.size()+b+1));
                     g.setColor(black);
                     g.drawLine(x1,y1,exsx,y2);
                     g.setColor(red);
                     g.drawString(exshapes.get(b).Name.charAt(0)+""+exshapes.get(b).ID,exsx,y2);
                  }

            }
         }
         for(int i=0; i<final1.size();i++)
            for(int a = 0; a <final1.get(i).children.size();a++)
            {
               x1=final1.get(i).x;
               y1=final1.get(i).y;
               x2=final1.get(i).children.get(a).x;
               y2=final1.get(i).children.get(a).y;
               g.setColor(black);
               g.drawLine(x1,y1,x2,y2);
            }
         for(int i=0; i<final1.size(); i++)
         {
            if(final1.get(i).ID==ID)g.setColor(magenta);
            else if (flag[Groups.indexOf(final1.get(i))]==1) g.setColor(blue);
            else g.setColor(black);
            if(final1.get(i).parents.size()==0)g.drawString(final1.get(i).name,final1.get(i).x,final1.get(i).y);
            else if(ID==0) g.drawString("G"+final1.get(i).ID,final1.get(i).x,final1.get(i).y);
            else g.drawString("C"+final1.get(i).ID,final1.get(i).x,final1.get(i).y);
            if(final1.get(i).children.size()==0)
            {int sx, sy;
               for  (int a=0; a<final1.get(i).shapes.size(); a++)
               { sy= final1.get(i).y +45;
                  if(final1.get(i).shapes.size()==1)  sx=final1.get(i).x;
                  else  sx= (final1.get(i).x-(final1.get(i).width/4))+(  ((final1.get(i).width/2)/ (final1.get(i).shapes.size()+1) )*(a+1));
                  g.setColor(black);
                  g.drawLine(final1.get(i).x,final1.get(i).y,sx,sy);
                  g.setColor(red);
                  g.drawString(final1.get(i).shapes.get(a).Name.charAt(0)+""+final1.get(i).shapes.get(a).ID,sx,sy);

               }
            }

         }
         final1.clear();
      }
   }
}