/**
 * Title:        SKNormalShape
 * Description:  Base for standard shape
 */
   package sketcher;
   import java.awt.Cursor;
   import java.awt.event.*;
   import java.awt.Point;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import javax.vecmath.*;

   public class SKNormalShape3d extends SKBaseShape3d
   {
      public float     posX,posY;
      boolean dragged = false;
      public SKNormalShape3d(SKMainFrame frame, int idnum, String newname, String shapeType, int shapeTypeID)
      {
         super(frame,idnum,newname,shapeType,shapeTypeID);
         //posX = x+(getWidth()>>1);
         //posY = y+(getHeight()>>1);
         frameMain = frame;
         //addMouseHandlers();
      }
   
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {super.updateMainShapeData(vPropList,vCurrentConstraints);
      
         if (vPropList != null)
         { //Update properties
            vPropList.add(frameMain.propFactory.getNewProp("X", Integer.toString( getX() ),1));
            vPropList.add(frameMain.propFactory.getNewProp("Y", Integer.toString( getY() ),1));
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
   
      public void setShapePoint(Point3d pt)
      {
         super.setShapePoint(pt);
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
      }
   
      public void updateFromUTU(char vr, char mod, double deltaX, double deltaY, double value)
      {
         switch (vr)
         {
            case 'x' :// setLocation((int)(value+deltaX)-(getWidth()>>1),getY());
               break;
            case 'y' :// setLocation(getX(),(int)(value+deltaY)-(getHeight()>>1));
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
      }
   
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         setShapePoint(new Point3d(data[startIdx],data[startIdx+1],data[startIdx+2]));
	 //System.out.println("111 bifur update "+getDX()+" "+getDY()+" "+getDZ());
         return startIdx+3;
      }
   
      public int bifurDataCount()
      {
         return 3;
      }
   
      public float distance(int x1, int y1, int x2, int y2)
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
   /*Adds mouse handlers to this shape (for dragging, selecting, etc)
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
                                  if (e.isPopupTrigger())
                                  {
                                     if ( frameMain.SelectedShapes.size()==0 ) frameMain.addSelectedShape(sh);
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
                                           
                                              for (int i=0; i<frameMain.SelectedShapes.size(); i++)
                                                 frameMain.SelectedShapes.get(i).doMove(0,0,true);
                                           }
                                           else sh.doMove(0,0,true);
                                        }
                                        sh.DragState = -1;
                                        sh.getSelectable().updateMainShapeData(frameMain.vCurrentProps,null);
                                        frameMain.tableObjectProp.repaint();
                                     
                                     
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
                                    {/*
                                       SKBaseShape sh = (SKBaseShape)e.getComponent();
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
                                                      for (int i=0; i<frameMain.SelectedShapes.size(); i++)
                                                         frameMain.SelectedShapes.get(i).doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                      frameMain.allSelNeedsResolved = !doSimpleSolve;
                                                   }
                                                   else
                                                   { //Just move sh
                                                      boolean subshape = false;
                                                      for(int i=0; i<frameMain.SelectedShapes.size(); i++)
                                                      { //for(int j=1; j<frameMain.SelectedShapes.get(i).getNumSubShapes()+1; j++)
                                                         dragged = true;
                                                         if ((frameMain.SelectedShapes.get(i) instanceof SKArcShape))
                                                         { 
                                                            subshape=true;
                                                            if (sh==frameMain.SelectedShapes.get(i).getSubShape(1) || 
                                                                  (((SKArcShape)frameMain.SelectedShapes.get(i)).fixradius==false &&((SKArcShape)frameMain.SelectedShapes.get(i)).fixangle==false) )
                                                            { frameMain.SelectedShapes.get(i).doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                               dragged = true;
                                                            }
                                                            else 
                                                            {sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                               frameMain.SelectedShapes.get(i).repaint();
                                                            }
                                                         }
                                                         else if((frameMain.SelectedShapes.get(i) instanceof SKCircleShape))
                                                         {//subshape = true;
                                                         ((SKCircleShape)sh).increaseRadius(e.getX(),e.getY());
                                                         System.out.println("reached here");
                                                         double dist = java.lang.Math.sqrt(java.lang.Math.pow((double)(((SKCircleShape)sh).center.getX()-sh.DragX),2)+java.lang.Math.pow((double)(((SKCircleShape)sh).center.getY()-sh.DragY),2));
                                                           if(((SKCircleShape)sh).radius == dist)
                                                          ((SKCircleShape)sh).radius=java.lang.Math.sqrt(java.lang.Math.pow((double)(((SKCircleShape)sh).center.getX()-e.getX()),2)+java.lang.Math.pow((double)(((SKCircleShape)sh).center.getY()-e.getY()),2));
                                                          }
                                                      }
                                                      if(!subshape)
                                                         sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                   }
                                             }
                                             frameMain.RefreshShapeArea();
                                          }
                                          if (frameMain.SelectedShapes.size() == 2)
                                          {
                                             Point pt1 = frameMain.SelectedShapes.get(0).getPointForDistance(frameMain.SelectedShapes.get(1)),
                                             pt2 = frameMain.SelectedShapes.get(1).getPointForDistance(frameMain.SelectedShapes.get(0));
                                             frameMain.sbStatus.updatePanelText(Float.toString((float)pt1.distance(pt2)),2);
                                          }
                                                  
                                                  if (sh.DragState==-1)
                                                  { //Dragging -- show what X,Y for shape would be
                                                    frameMain.sbStatus.updatePanelText((e.getX()+sh.getX()-sh.DragX)+" , "+(e.getY()+sh.getY()-sh.DragY),2);
                                                  }
                                                  else
                                                  { //Other - show real X,Y
                                                    frameMain.sbStatus.updatePanelText((sh.getX()+(sh.getWidth()>>1))+" , "+(sh.getY()+(sh.getHeight()>>1)),2);
                                                  }
                                                  
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
      }*/
   }
