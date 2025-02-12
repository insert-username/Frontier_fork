/**
 * Title:        SKNormalShape
 * Description:  Base for standard shape
 */
package com.frontier.sketcher.ui.items.shapes;

import com.frontier.sketcher.ui.*;
import com.frontier.sketcher.ui.app.SKMainFrame;
import com.frontier.sketcher.ui.items.constraints.SKConstraintArray;
import com.frontier.sketcher.ui.properties.SKPropArray;

import java.awt.Cursor;
import java.awt.event.*;
import java.awt.Point;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.IOException;

public class SKNormalShape extends SKBaseShape
{
   public float     posX,posY;
   boolean dragged = false;
   public SKNormalShape(SKMainFrame frame, int idnum, String newname, String shapeType, int shapeTypeID, int x, int y)
   {
      super(frame,idnum,newname,shapeType,shapeTypeID);
      posX = x+(getWidth()>>1);
      posY = y+(getHeight()>>1);
      frameMain = frame;
      addMouseHandlers();
   }

   public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
   {
     // super.updateMainShapeData(vPropList,vCurrentConstraints);

      if (vPropList != null)
      { //Update properties
         vPropList.add(frameMain.propFactory.getNewProp("X", Integer.toString( getShapeX() ),1));
         vPropList.add(frameMain.propFactory.getNewProp("Y", Integer.toString( getShapeY() ),1));
      }
   }

   public boolean setShapeData(String PropName, Object PropData)
   {
      if (super.setShapeData(PropName,PropData))
         return true;

      if (PropName=="Y")
      {
      //setLocation(getX(),new Integer(PropData.toString()).intValue()-(getHeight()>>1));
         posY = Integer.parseInt(PropData.toString());
      }
      else if (PropName=="X")
      {
       //setLocation(new Integer(PropData.toString()).intValue()-(getWidth()>>1),getY());
         posX = Integer.parseInt(PropData.toString());
      }
      else
         return false;

      return true;
   }

   public int getShapeX()
   {
      return getX()+(getWidth()>>1);
   }

   public int getShapeY()
   {
      return getY()+(getHeight()>>1);
   }

   public void setShapePoint(Point pt)
   {
      setLocation(pt.x-(getWidth()>>1),pt.y-(getHeight()>>1));
   }

   public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
   {
      this.setLocation(getX()+deltaX, getY()+deltaY);

      if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && resolveConstraints)
      {
         if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
            SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable(),new SKConstraintArray(3));
         else
            SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable());
      }
   }
   public void doMove(int deltaX, int deltaY, boolean resolveConstraints, boolean image)
   {
      if(image)
      {
         this.setLocation(getX()+deltaX, getY()+deltaY);

         if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && resolveConstraints)
         {
            if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable(),new SKConstraintArray(3));
            else
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable());
         }
      }
   }
   public void updateFromUTU(char vr, char mod, double deltaX, double deltaY, double value)
   {
      switch (vr)
      {
         case 'x' : setLocation((int)(value+deltaX)-(getWidth()>>1),getY());
            break;
         case 'y' : setLocation(getX(),(int)(value+deltaY)-(getHeight()>>1));
      }
   }

   public void writeAdditionalProps(DataOutputStream p) throws IOException
   {
      super.writeAdditionalProps(p);

      p.writeInt( getX() );
      p.writeInt( getY() );
   }

   public void readAdditionalProps(DataInputStream p) throws IOException
   {
      super.readAdditionalProps(p);

      reshape( p.readInt(), p.readInt(), getWidth(), getHeight() );

   }
   public void readAdditionalProps(DataInputStream p, int x, int y) throws IOException

   {
      super.readAdditionalProps(p);
      if(frameMain.repFirst)
      {
         frameMain.repx1= p.readInt();
         frameMain.repy1= p.readInt();
         reshape( x, y, getWidth(), getHeight() );
      }
      else
      {
         frameMain.repx2= p.readInt();
         frameMain.repy2= p.readInt();
         x= frameMain.repx - frameMain.repx1 +frameMain.repx2;
         y= frameMain.repy - frameMain.repy1 +frameMain.repy2;
         reshape( x, y, getWidth(), getHeight() );}

   }

   public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
   {
      setShapePoint(new Point((int)(data[startIdx]*scale)+addX,(int)(data[startIdx+1]*scale)+addY));
      return startIdx+2;
   }

   public int bifurDataCount()
   {
      return 2;
   }

   public float distance(int x1, int y1, int x2, int y2)

   {
      return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
   }

//Adds mouse handlers to this shape (for dragging, selecting, etc)
   public void addMouseHandlers()
   {
      addMouseListener(
                         new MouseListener()
                         {
                            public void mouseClicked(MouseEvent e)
                            {
                               SKBaseShape sh = (SKBaseShape)e.getComponent();
                               if (e.isControlDown())
                               { //Multi select
                                  frameMain.toggleSelectedShape(sh);
                               }
                               else
                               {
                                  if (!sh.isSelected()) frameMain.addOnlySelectedShape(sh);
                               }
                            }
                            public void mouseReleased(MouseEvent e)
                            {
                               SKBaseShape sh = (SKBaseShape)e.getComponent();
                               final var selectionModel = frameMain.getItemSelectionModel();
                               if(dragged)
                                  if(selectionModel.selectedShapesCount()==1)
                                     if(selectionModel.selectedShapeAt(0) instanceof SKArcShape )
                                     {
                                        if(((SKArcShape) selectionModel.selectedShapeAt(0)).fixradius==true )
                                        {
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).Adjust=true;
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).fixRadiusAdjust();
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).repaint();
                                        }
                                        else if (((SKArcShape) selectionModel.selectedShapeAt(0)).fixangle==true)
                                        {
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).Adjust=true;
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).fixAngleAdjust();
                                           ((SKArcShape) selectionModel.selectedShapeAt(0)).repaint();
                                        }
                                     }
                               dragged = false;
                               if (e.isPopupTrigger())
                               {
                                  if ( selectionModel.selectedShapesCount()==0 ) frameMain.addSelectedShape(sh);
                                  frameMain.popupShape.show(sh,e.getX(),e.getY());
                               }
                               else
                               {
                                  if ( !sh.isSelected() )
                                     return;
                                  if (sh.DragState != -1)
                                  {
                                           //Do SimpleSolver
                                     if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==1)
                                     {
                                        if (frameMain.allSelNeedsResolved)
                                        {
                                           frameMain.allSelNeedsResolved = false;

                                           for (int i=0; i<selectionModel.selectedShapesCount(); i++)
                                              selectionModel.selectedShapeAt(i).doMove(0,0,true);
                                        }
                                        else sh.doMove(0,0,true);
                                     }
                                     sh.DragState = -1;
                                     sh.getSelectable().updateMainShapeData(frameMain.vCurrentProps,null);
                                     frameMain.tableObjectProp.repaint();

                                     if (sh.getSelectable().highestGroup != null)
                                        sh.getSelectable().highestGroup.groupRect.w = 0;
                                     sh.repaint();
                                  }
                               }
                            }
                            public void mousePressed(MouseEvent e) {
                            }
                            public void mouseExited(MouseEvent e) {
                            }
                            public void mouseEntered(MouseEvent e) {
                            }
                         } );
      addMouseMotionListener(
                              new MouseMotionListener()
                              {
                                 public void mouseDragged(MouseEvent e)
                                 {
                                    SKBaseShape sh = (SKBaseShape)e.getComponent();
                                    final var selectionModel = frameMain.getItemSelectionModel();

                                    if (sh.isSelected())
                                    {
                                       if (sh.DragState==-1)
                                       {
                                          sh.DragState = 0;
                                          sh.DragX = e.getX();
                                          sh.DragY = e.getY();
                                          sh.setCursor( Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR) );
                                       }
                                       else
                                       { //Draw as user is dragging
                                          boolean doSimpleSolve = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==2);
                                          switch (sh.DragState)
                                          {
                                             case 0 :
                                                if (e.isAltDown())
                                                { //Move all selected shapes relative to sh
                                                   for (int i=0; i<selectionModel.selectedShapesCount(); i++)
                                                      selectionModel.selectedShapeAt(i).doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                   frameMain.allSelNeedsResolved = !doSimpleSolve;
                                                }
                                                else
                                                { //Just move sh
                                                   boolean subshape = false;
                                                   for(int i=0; i<selectionModel.selectedShapesCount(); i++)
                                                   { //for(int j=1; j<selectionModel.selectedShapeAt(i).getNumSubShapes()+1; j++)
                                                      dragged = true;
                                                      if ((selectionModel.selectedShapeAt(i) instanceof SKArcShape))
                                                      {
                                                         subshape=true;
                                                         if (sh==selectionModel.selectedShapeAt(i).getSubShape(1) ||
                                                               (((SKArcShape)selectionModel.selectedShapeAt(i)).fixradius==false &&((SKArcShape)selectionModel.selectedShapeAt(i)).fixangle==false) )
                                                         { selectionModel.selectedShapeAt(i).doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                            dragged = true;
                                                         }
                                                         else
                                                         {sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                            selectionModel.selectedShapeAt(i).repaint();
                                                         }
                                                      }
                                                      else if((selectionModel.selectedShapeAt(i) instanceof SKCircleShape))
                                                      {//subshape = true;
                                                      /*((SKCircleShape)sh).increaseRadius(e.getX(),e.getY());
                                                      System.out.println("reached here");
                                                      double dist = java.lang.Math.sqrt(java.lang.Math.pow((double)(((SKCircleShape)sh).center.getX()-sh.DragX),2)+java.lang.Math.pow((double)(((SKCircleShape)sh).center.getY()-sh.DragY),2));
                                                        if(((SKCircleShape)sh).radius == dist)
                                                       ((SKCircleShape)sh).radius=java.lang.Math.sqrt(java.lang.Math.pow((double)(((SKCircleShape)sh).center.getX()-e.getX()),2)+java.lang.Math.pow((double)(((SKCircleShape)sh).center.getY()-e.getY()),2));
                                                      */ }
                                                   }
                                                   if(!subshape)
                                                      sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                }
                                          }
                                          frameMain.RefreshShapeArea();
                                       }
                                       if (selectionModel.selectedShapesCount() == 2)
                                       {
                                          Point pt1 = selectionModel.selectedShapeAt(0).getPointForDistance(selectionModel.selectedShapeAt(1)),
                                          pt2 = selectionModel.selectedShapeAt(1).getPointForDistance(selectionModel.selectedShapeAt(0));
                                          frameMain.sbStatus.updatePanelText(Float.toString((float)pt1.distance(pt2)),2);
                                       }
                                               /*
                                               if (sh.DragState==-1)
                                               { //Dragging -- show what X,Y for shape would be
                                                 frameMain.sbStatus.updatePanelText((e.getX()+sh.getX()-sh.DragX)+" , "+(e.getY()+sh.getY()-sh.DragY),2);
                                               }
                                               else
                                               { //Other - show real X,Y
                                                 frameMain.sbStatus.updatePanelText((sh.getX()+(sh.getWidth()>>1))+" , "+(sh.getY()+(sh.getHeight()>>1)),2);
                                               }
                                               */
                                    }
                                 }
                                 public void mouseMoved(MouseEvent e)
                                 { //Set correct mouse cursor
                                    SKBaseShape sh = (SKBaseShape)e.getComponent();
                                    frameMain.sbStatus.updatePanelText(e.getX()+sh.getX()+" , "+(e.getY()+sh.getY()),2);
                                    frameMain.sbStatus.updatePanelText(sh.getSelectable().Name,3);
                                    if (sh.getSelectable() == sh && frameMain.mouseOverItem != sh)
                                    {
                                       frameMain.mouseOverItem = sh;
                                       frameMain.RefreshShapeArea();
                                    }
                                    sh.setCursor( Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR) );
                                 }
                              } );
   }
}
