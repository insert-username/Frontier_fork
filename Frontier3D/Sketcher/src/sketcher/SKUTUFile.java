/**
 * Title:        SKUTUFile
 * Description:  Methods to create file for UTU
 */
   package sketcher;

   import java.awt.Container;
   import java.awt.Point;
   import java.io.*;
   import java.awt.geom.*;
   import javax.vecmath.*;
   import javax.swing.*;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.Graphics;
   import utuJava;

   public class SKUTUFile
   {
      static int dataInt[];
      static int actCluster=0, indexInt=0,indexDbl=0,errResolveID=-1;
      static double dataDouble[];
      static char dataChar[];
      static SKGTNArray dag;
      static SKMainFrame frameMain=null;
     
   /**
   *Method that reads the arrays of solutions sent by the UTU and redraws the solved sketch using the new coordinates. 
   *Also displays the DR-Dag on a separate window
   */
      public static boolean waitForUpdate(int[] IntArray, double[] DblArray, char[] CharArray, SKShape3dArray shapes3d, SKShapeArray shapes,SKMainFrame frame, int width, int height, SKGTNArray dagMain, boolean threeD )
      {  
         frameMain = frame;
         JOptionPane dialog = null;
         SKBaseShape sh;
         SKBaseShape3d sh3d=null;
         TreeDialog clusters=null;
         TreeFrame actClusters = null;
         utuJava utuDriver = new utuJava();
         SKGroupTreeNode root = null, enode = null;
         SKGTNArray roots = new SKGTNArray(10);
         SKGTNArray copyDag = new SKGTNArray(10);
         SKShapeArray allShapes = new SKShapeArray(10);
         SKShape3dArray allShapes3d = new SKShape3dArray(10);
         shapes.copyArrayTo(allShapes);
         shapes3d.copyArrayTo(allShapes3d);
         int wid = width, hgt = height, w = width >> 1, h = height >> 1,numSolutions;
         int g,count=shapes.size(),number;
         double num1=0,num2=0,num3=0, 
         x, y, z=0,  x2=0, y2=0, z2=0, x3=0, y3=0, z3=0,
         minX, minY, maxX, maxY, minZ, maxZ,
         delta=0;
         boolean done=false, doscale = true;
         maxX = -999;
         minX = 999;
         maxY = -999;
         minY = 999;
         minZ = 999;
         maxZ = -999;
         dataInt = IntArray;
         dataDouble = DblArray;
         dataChar = CharArray;

         System.out.println("SKUTU index"+dataInt[0]);

         indexInt = dataInt[0];
         indexDbl = (int)dataDouble[0];
         numSolutions = dataInt[indexInt++];

         System.out.println("first "+dataInt[0]);
         for ( h= (int)dataInt[0]; h<dataInt[0]+50 ; h++)
            System.out.println("Received "+dataInt[h]);
         System.out.println("first "+dataDouble[0]);
         for ( h= (int)dataDouble[0]; h<dataDouble[0]+10 ; h++)
            System.out.println("Received "+dataDouble[h]);
      
      //received the DR DAG
         if(numSolutions == -2)
         {
            dag = new SKGTNArray(100);
            int numRoots= dataInt[indexInt++];
            int ClusterID = dataInt[indexInt++];
	    int overConst;
            while(ClusterID != -1)
            {
	       overConst = dataInt[indexInt++];
               boolean isPresent=false;
               for(int n=0; n<dag.size(); n++)
                  isPresent= isPresent || (dag.get(n).ID==ClusterID);
               if(!isPresent)
               {
                  SKGroupTreeNode node = new SKGroupTreeNode(null,ClusterID,"Group"+ClusterID);
		  node.overConst = overConst;
                  dag.add(node);
                  enode = (SKGroupTreeNode)node;
               }
               else                            
                  for(int n=0; n<dag.size(); n++) 
                     if(dag.get(n).ID==ClusterID)
                        {
			  enode = (SKGroupTreeNode)dag.get(n);
			  enode.overConst = overConst;
			}
               int ssize= dataInt[indexInt++];
            
               SKShapeArray Shapes = new SKShapeArray(ssize);
            
               for(int m=0; m<ssize; m++)
               {
                  int SID = dataInt[indexInt++];
                  SKBaseShape shape = (SKBaseShape)frame.allshapes.findByID(SID);
                  Shapes.add(shape);
               }
            
               Shapes.copyArrayTo(enode.shapes);
               int numChildren = dataInt[indexInt++];
            
               for(int m=0; m<numChildren; m++)
               {
                  ClusterID = dataInt[indexInt++];
                  isPresent=false;
                  for(int n=0; n<dag.size(); n++)
                     isPresent= isPresent || (dag.get(n).ID==ClusterID);
                  if(!isPresent)
                  {
                     SKGroupTreeNode node1 = new SKGroupTreeNode(enode,ClusterID,"Cluster"+ClusterID);
		     node1.overConst = overConst;
                     enode.children.add(node1);
                     dag.add(node1);
                  }
                  else 
                     for(int n=0; n<dag.size(); n++) 
                        if(dag.get(n).ID==ClusterID)
                           enode.children.add(dag.get(n));
               }
            
               ClusterID = dataInt[indexInt++];
            }
         
            for(int l=0; l<dag.size(); l++)
               for(int m=0; m<dag.get(l).children.size(); m++)
                  for( int n =0 ; n<dag.size(); n++)
                     if(dag.get(n)==dag.get(l).children.get(m))
                        dag.get(n).parents.add(dag.get(l));
         
            for(int l=0; l<dag.size(); l++)
               if (dag.get(l).parents.size()==0)
                  roots.add(dag.get(l));
            dag.copyArrayTo(dagMain);
            int[] fin = new int[dag.size()];
            for(int i=0; i<fin.length; i++)
               fin[i]=1;
         
            clusters = new TreeDialog(frame,dag,actCluster,fin,true);
            clusters.setLocation(691,54);
            clusters.setVisible(true);

	  /*  String overConstClusters = "";
	    for(int l=0; l<dag.size(); l++)
		if(dag.get(l).overConst>0)
		  overConstClusters= overConstClusters.concat(dag.get(l).name+" ");

	    if(!overConstClusters.equals(""))	    
		JOptionPane.showMessageDialog(clusters,"The system is over-constrained at "+overConstClusters);*/

            return clusters.approved;
         }
         else if(numSolutions == -3)
         {
	    int savedIndex = indexInt-1;
            int flag = dataInt[indexInt++];
	    int[] fin = new int[dag.size()];
            for(int i=0; i<fin.length; i++)
               fin[i]=1;
	    actClusters = new TreeFrame(frame,dag,0,fin);
            actClusters.setLocation(691,54);
            actClusters.setVisible(true);
	    int reply =-1;
            if(flag ==0)
               reply=dialog.showConfirmDialog(frameMain.panelShapeArea,"No Solution found. Do you want to Re-do?","Error",JOptionPane.YES_NO_OPTION);
            else if(flag ==1)
               reply=dialog.showConfirmDialog(frameMain.panelShapeArea,"Infinite Solutions found. Do you want to Re-do?","Error",JOptionPane.YES_NO_OPTION);
            else if(flag ==2)
	       reply=dialog.showConfirmDialog(frameMain.panelShapeArea,"Error! Do you want to Re-do?","Error",JOptionPane.YES_NO_OPTION);
	    if(reply == JOptionPane.YES_OPTION)
	    {
		final JDialog reSolveDialog = new JDialog(frameMain,"Re-solve",true);
         	final JTextField cluster = new JTextField();
         	JTextArea text = new JTextArea("Enter the cluster ID to re-Do");
         	text.setBackground(Color.lightGray);
         	JPanel okPanel = new JPanel();
         	JButton okBtn = new JButton();
         	okBtn.setText("OK");
         	okBtn.addActionListener(
                	              new java.awt.event.ActionListener()
                        	         {
          	
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       errResolveID = java.lang.Integer.parseInt(cluster.getText());
                                       reSolveDialog.hide();
                                    }
                                 });
	        reSolveDialog.setSize(250,100);
        	reSolveDialog.setLocation(691,420);
	        reSolveDialog.getContentPane().add(text,BorderLayout.NORTH);
        	reSolveDialog.getContentPane().add(cluster,BorderLayout.CENTER);
         	reSolveDialog.getContentPane().add(okPanel,BorderLayout.SOUTH);
         	okPanel.add(okBtn);
	        reSolveDialog.show();       
		dataInt[0]=2;
		dataInt[savedIndex++]=-1;
		dataInt[savedIndex++]=errResolveID;
		SKGroupTreeNode reSolveGrp = dag.findByID(errResolveID);
                unFixShapes(reSolveGrp);
                frameMain.mode=2;
		frame.solved=true;
		actClusters.hide();
		return true;
	    }
	    else
	      actClusters.hide();
         }         
         
         //received final output
         else if(numSolutions == 1)
         {   	//Solved, parse and update shapes
	    int savedIndex = indexInt-1;
            boolean first=false,frontierV=false;
            SKShapeArray doneShapes = new SKShapeArray(2);
            int i;
            double fixx=0, fixy=0, fixz=0;
            //frame.update = false;
            dagMain.copyArrayTo(dag);
            int noRoots = dataInt[indexInt++];
	    int[] fin = new int[dag.size()];
	    int actCluster = 0;
	    actClusters = new TreeFrame(frame,dag,actCluster,fin);
	    actClusters.setLocation(691,54);
            actClusters.setVisible(true);
            System.out.println("SKUTUF "+dataInt[0]);
            for( int a =0; a<noRoots;a++)
            {
               int rootID = dataInt[indexInt++];
               number = dataInt[indexInt++];
            
               for(i=indexInt; i<indexInt+number; i++)
                  frontierV = frontierV ||(doneShapes.findByID(dataInt[i])!=null);
            
               if(frontierV)
               {
                  int j=indexDbl;               
                  for(i=indexInt; i<indexInt+number; i++)
                  {
                     sh = shapes.findByID(dataInt[i]);
                     if((doneShapes.indexOf(sh)==-1)&&(sh instanceof SKPointShape))
                        j=j+2;
                     else if((doneShapes.indexOf(sh)==-1)&& (sh instanceof SKLineShape))
                        j=j+5;
                     else if((doneShapes.indexOf(sh)==-1)&& (sh instanceof SKImageShape))
                        j=j+5;
                     else if((doneShapes.indexOf(sh)==-1)&& (sh instanceof SKCircleShape))
                        j=j+3;	
                     else if (doneShapes.indexOf(sh)!=-1)
                     {  
                        if(sh instanceof SKCircleShape) ((SKCircleShape)sh).bifur=true;
                        num1 = dataDouble[j++];
                        num2 = dataDouble[j++];
                        if(threeD)
                           num3 = dataDouble[j++];
                        x=num1;
                        y=num2;
                        if(threeD)
                           z=num3;
                        if (x > maxX)  maxX = x;
                        if (x < minX)  minX = x;
                        if (y > maxY)  maxY = y;
                        if (y < minY)  minY = y;
                        x3=x;
                        y3=y;
                        x2=(int)sh.getShapeX();
                        y2=(int)sh.getShapeY();
                     
                        first=false;
                        fixx = x2-x3;
                        fixy = y2-y3;
                     }
                  }
               }
               else first = true;
            
               for(i=1;i<=number;i++)
               {
                  g = dataInt[indexInt++];
                  sh = shapes.findByID(g);
                  if(threeD)
                     sh3d = shapes3d.findByID(g);
                  if(sh instanceof SKCircleShape) ((SKCircleShape)sh).bifur=true;
                  if(doneShapes.indexOf(sh) == -1)
                  {
                     doneShapes.add(sh);
                     num1 = dataDouble[indexDbl++];
                     num2 = dataDouble[indexDbl++];
                     if(threeD)
                        num3 = dataDouble[indexDbl++];
                     if(threeD)
                     {
                        x = num1;
                        y = num2;
                        z = num3;
                     }
                     else
                     {
                        x = java.lang.Math.round( num1*40 ) + w;  
                        y = java.lang.Math.round( num2*40 ) + h;
                     }
                     if (x > maxX)  maxX = x;
                     if (x < minX)  minX = x;
                     if (y > maxY)  maxY = y;
                     if (y < minY)  minY = y;
                  
                     if (sh instanceof SKPointShape)
                     {
                        if(first)
                        {
                           x3=x;
                           y3=y;
                           if(threeD)
                           {  z3=z;
                              x2=sh3d.getDX();
                              y2=sh3d.getDY();
                              z2=sh3d.getDZ();
                           }
                           else
                           {
                              x2=sh.getShapeX();
                              y2=sh.getShapeY();
                              //System.out.println("111 "+x2+" "+y2);
                           }
                           first=false;
                           fixx = x2-x3;
                           fixy = y2-y3;
                           if(threeD)
                              fixz = z2-z3;
                        
                        }  
                        else 
                        {
                           x=x2-(x3-x);
                           y=y2-(y3-y);
                           if(threeD)
                              z=z2-(z3-z);
                           sh.setShapePoint(new Point((int)x,(int)y));
                           if(threeD)
                              ((SKPointShape3d)sh3d).doMove(new Vector3f((float)x,(float)y,(float)z));
                        }
                     }
                     else if (sh instanceof SKLineShape)
                     {
                        if(first)
                        {
                           x3=x;
                           y3=y;
                           if(threeD)
                           {
                              z3=z;
                              x2=((SKLineShape3d)sh3d).pt1.getDX();
                              y2=((SKLineShape3d)sh3d).pt1.getDY();
                              z2=((SKLineShape3d)sh3d).pt1.getDZ();
                           }
                           else
                           {
                              x2=((SKLineShape)sh).pt1.getShapeX();
                              y2=((SKLineShape)sh).pt1.getShapeY();		
                           }	
                           first=false;
                           fixx = x2-x3;
                           fixy = y2-y3;
                           if(threeD)
                              fixz = z2-z3;
                        }
                        else 
                        {
                           x=x2-(x3-x);
                           y=y2-(y3-y);
                           if(threeD)
                              z=z2-(z3-z);
                           ((SKLineShape)sh).pt1.setShapePoint(new Point((int)x,(int)y));
                           if(threeD)
                              ((SKLineShape3d)sh3d).pt1.doMove(new Vector3f((float)x,(float)y,(float)z));
                        }
                        System.out.println("line "+sh+" "+x+" "+y);
                        num1 = dataDouble[indexDbl++];
                        num2 = dataDouble[indexDbl++];
                        if(threeD)
                           num3 = dataDouble[indexDbl++];
                        num3=0;
                     
                        if(threeD)
                        {x = num1;
                           y = num2;
                           z = num3;}
                        else
                        {x = java.lang.Math.round( num1*40 ) + w;
                           y = java.lang.Math.round( num2*40 ) + h;}
                     
                        System.out.println("read "+sh+" "+x+" "+y);
                        x=x2-(x3-x);
                        y=y2-(y3-y);
                        if(threeD)
                           z=z2-(z3-z);
                        ((SKLineShape)sh).pt2.setShapePoint(new Point((int)x,(int)y));
                        if(threeD)
                           ((SKLineShape3d)sh3d).pt2.doMove(new Vector3f((float)x,(float)y,(float)z));
                        System.out.println("line "+sh+" "+x+" "+y);
                        if (x > maxX)  maxX = x;
                        if (x < minX)  minX = x;
                        if (y > maxY)  maxY = y;
                        if (y < minY)  minY = y;
                     }
                     else if (sh instanceof SKCircleShape)
                     {
                        ((SKCircleShape)sh).bifur=true;
                     
                        if(first)
                        {
                           x3=x;
                           y3=y;
                           x2=(int)((SKCircleShape)sh).center.getShapeX();
                           y2=(int)((SKCircleShape)sh).center.getShapeY();
                           ((SKCircleShape)sh).radius =dataDouble[indexDbl++];
                           first=false;
                           System.out.println("circle "+sh+" "+x2+" "+y2);
                        }
                        else
                        {
                           x=x2-(x3-x);   
                           y=y2-(y3-y);
                           ((SKCircleShape)sh).center.setShapePoint(new Point((int)x,(int)y));
                           ((SKCircleShape)sh).radius = dataDouble[indexDbl++];
                           System.out.println("circle "+sh+" "+x+" "+y);
                        }
                     }
                     else if (sh instanceof SKImageShape)
                     {  int new1x,new1y,new2x,new2y,centrex=(int)(sh.getX()+sh.getWidth()/2),centrey=(int)(sh.getY()+sh.getHeight()/2),adjusted1y, adjusted2y;
                        doscale = false;
                        new1x=(int)x;
                        new1y=(int)y;
                        num1 = dataDouble[indexDbl++];
                        num2 = dataDouble[indexDbl++];
                        x = (int)java.lang.Math.round( num1*40 ) + w;
                        y = (int)java.lang.Math.round( num2*40 ) + h;
                        System.out.println("read "+sh+" "+x+" "+y);
                        new2x=(int)x;
                        new2y=(int)y;
                        adjusted1y=frame.getHeight()-new1y;
                        adjusted2y=frame.getHeight()-new2y;
                     
                        double hei = distance(new1x,new1y,new2x,new2y);
                        double slope = (double)(new2y-new1y)/(double)(new2x-new1x);
                     
                        double inc;
                        if(new2y<new1y)
                        {
                           if(new2x<new1x)
                              inc = (java.lang.Math.atan(slope)+4.712);
                           else
                              inc = (java.lang.Math.atan(slope)+1.57);
                        }
                        else
                        {
                           if(new2x<new1x)
                              inc = (java.lang.Math.atan(slope)+4.712);
                           else 
                              inc = (java.lang.Math.atan(slope)+1.57);
                        }
                        double diaslope1;
                        if(new2y<new1y)
                        {
                           if(new2x<new1x)
                              diaslope1 = java.lang.Math.tan (java.lang.Math.atan(slope) - 0.785);
                           else
                              diaslope1 = java.lang.Math.tan (2.35 + java.lang.Math.atan(slope));
                        }
                        else
                        {
                           if(new2x<new1x)
                              diaslope1 = java.lang.Math.tan(java.lang.Math.atan(slope)-0.785);
                           else
                              diaslope1 = java.lang.Math.tan(java.lang.Math.atan(slope)-0.785);
                        }
                     
                        double diaslope2 = -1/diaslope1;
                        double c1 = new1y - (diaslope1*new1x);
                        double c2 = new2y - (diaslope2*new2x);
                        double[] array = new double[2];
                     
                        SKSimpleSolver.lineintersection(array,diaslope1,diaslope2,c1,c2);
                        if(first)
                        {
                           x3=(int)array[0];
                           y3=(int)array[1];
                           x2=(int)(sh.getX()+sh.getWidth()/2);
                           y2=(int)(sh.getY()+sh.getHeight()/2);
                           centrex=(int)x2;
                           centrey=(int)y2;
                           first=false;
                        }
                        else
                        {
                           centrex=(int)(x2-(x3-(int)array[0]));
                           centrey=(int)(y2-(y3-(int)array[1]));
                        }
                        System.out.println("new center "+(int)array[0]+" "+(int)array[1]);
                     
                        ((SKImageShape)sh).scale= (float)(hei/(((SKImageShape)sh).getHeight()*0.7));
                        ((SKImageShape)sh).rotation=(float)inc;
                        System.out.println("rot "+((SKImageShape)sh).rotation);
                        ((SKImageShape)sh).reshape((int)(centrex-(sh.getWidth()/2*((SKImageShape)sh).scale)),(int)(centrey-(sh.getHeight()/2*((SKImageShape)sh).scale)),(int)(sh.getWidth()*((SKImageShape)sh).scale),(int)(sh.getHeight()*((SKImageShape)sh).scale));
                        ((SKImageShape)sh).pt1.setShapePoint(new Point((int)(sh.getX()+(sh.getWidth()*0.85)),(int)(sh.getY()+(sh.getHeight()*0.85))));
                        ((SKImageShape)sh).pt2.setShapePoint(new Point((int)(sh.getX()+(sh.getWidth()*0.85)), (int)(sh.getY()+(sh.getHeight()*0.15))));
                        if (x > maxX)  maxX = x;
                        if (x < minX)  minX = x;
                        if (y > maxY)  maxY = y;
                        if (y < minY)  minY = y;
                     }
                  
                  }
                  else 
                  {
                     if(sh instanceof SKPointShape)
                     {
                        if(threeD)
                           indexDbl=indexDbl+3;
                        else
                           indexDbl+=2;}
                     else if (sh instanceof SKLineShape)
                     {
                        if(threeD)
                           indexDbl=indexDbl+7;
                        else
                           indexDbl+=5;} 
                     else if(sh instanceof SKCircleShape)
                        indexDbl+=3;
                  }
               }
            }
         
         
            delta = maxX-minX;
            SKBaseShape fixSh= (SKBaseShape) frame.panelShapeArea.getComponent(0);
            minX = fixx + minX;
            maxX = fixx + maxX;
            minY = fixy + minY;
            maxY = fixy + maxY;
         
         /**********************for partially solved sketch***************************/
         
            for(int l=0; l<roots.size(); l++)
               for(int m=l+1; m<roots.size(); m++)
               {
                  boolean noCommon = true;
                  int cnt=0;
                  SKBaseShape common=null;
               
                  for(int n=0; n<roots.get(l).shapes.size(); n++)
                  {
                     noCommon = noCommon && (roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1);
                     if(!(roots.get(m).shapes.indexOf(roots.get(l).shapes.get(n))==-1))
                     {
                        common = roots.get(l).shapes.get(n);
                        cnt++;
                     }
                  }
                  if(!noCommon &&(cnt==1))
                  {
                     int min = 999;
                     SKBaseShape tip =null;
                     for( i=0; i<roots.get(m).shapes.size(); i++)
                     {
                        if(common.ID != roots.get(m).shapes.get(i).ID)
                        { 
                           SKBaseShape t = roots.get(m).shapes.get(i);
                           if(min>t.getX())
                           {
                              tip = t;
                              min = t.getX();
                           }
                        }
                     }
                     if(tip == null) tip = common;
                     int p1x=common.getShapeX(), p1y=common.getShapeY(), p2x=tip.getShapeX(), p2y=tip.getShapeY(), px=0, py=0;
                     boolean outside=false;
                     double rad=0;
                     double pSlope , c1;
                     if(common.ID == tip.ID) 
                     {p1y=0;
                        p2y=300;}
                     if(p1x==p2x)
                        pSlope = Double.MAX_VALUE;
                     else pSlope = (p1y-p2y)/(p1x-p2x);
                     c1 = p1y - (pSlope*p1x);
                     p1x=0;
                     p1y=(int)((p1x*pSlope) + c1);
                     p2y=999;
                     p2x=(int)((p2y-c1)/pSlope);
                  
                     while(!outside && (rad<=java.lang.Math.PI*2))
                     {
                        outside = true;
                        for( i=0; i<roots.get(l).shapes.size(); i++)
                        {
                           px = roots.get(l).shapes.get(i).getShapeX();
                           py = roots.get(l).shapes.get(i).getShapeY();
                           outside = outside && (((py-p1y)*(p2x-p1x))-((px-p1x)*(p2y-p1y)) > 0);
                           //System.out.println(roots.get(l).shapes.get(i)+" "+outside);
                        }
                        if(!outside)
                        {
                           float r = distance(p1x,p1y,p2x,p2y);
                           rad = rad+(java.lang.Math.PI/36);
                           AffineTransform T = new AffineTransform();
                           T.setToRotation(rad,common.getShapeX(),common.getShapeY());
                        
                           float[] src,dst;
                        
                           src = new float[2];
                           dst = new float[2];
                           src[0]=p2x;
                           src[1]=p2y;
                           T.transform(src,0,dst,0,1);
                           p2x=(int)dst[0];
                           p2y=(int)dst[1];
                           //System.out.println("second "+p2x+" "+p2y+" "+rad);
                           for( i=0; i<roots.get(m).shapes.size(); i++)
                           {
                              px = roots.get(m).shapes.get(i).getShapeX();
                              py = roots.get(m).shapes.get(i).getShapeY();
                              src[0]=px;
                              src[1]=py;
                              T.transform(src,0,dst,0,1);
                              SKBaseShape shape;
                              for(int k=0; k<frame.panelShapeArea.getComponentCount(); k++)
                                 for(int j=0; j<((SKBaseShape)frame.panelShapeArea.getComponent(k)).getNumSubShapes(); j++)
                                    if(roots.get(m).shapes.get(i).ID == ((SKBaseShape)frame.panelShapeArea.getComponent(k)).getSubShape(j).ID) 
                                    {
                                       shape=((SKBaseShape)frame.panelShapeArea.getComponent(k)).getSubShape(j);
                                       shape.setShapePoint(new Point((int)dst[0],(int)dst[1]));
                                    }
                           
                           }
                        }
                     }
                  }
               }
         /*************************************************************************/
            done = true;
            frame.update=true;
            frame.solved = true;
         
         /************************for scaling and centralizing ********************/
         
            if(threeD)
            {frame.panelShapeArea.setVisible(false);
               frame.getContentPane().add(frame.panelShapeArea3d);
               for(i=0; i<frame.allshapes3d.size(); i++)
                  for(int j=1; j<frame.allshapes3d.get(i).getNumSubShapes()+1;j++)
                  {
                     SKBaseShape3d shape = frame.allshapes3d.get(i).getSubShape(j);
                     if(minX>shape.getDX()) minX = shape.getDX();
                     if(maxX<shape.getDX()) maxX = shape.getDX();
                     if(minY>shape.getDY()) minY = shape.getDY();
                     if(maxY<shape.getDY()) maxY = shape.getDY();
                     if(minZ>shape.getDZ()) minZ = shape.getDZ();
                     if(maxZ<shape.getDZ()) maxZ = shape.getDZ();
                     //System.out.println("111 shape "+shape+" minX "+minX+" minY "+minY+" maxX "+maxX+" maxY "+maxY+" minZ "+minZ+" maxZ "+maxZ); 
                  }
               double[] minMax = new double[6];
               minMax[0] = 999;
               minMax[1] = 999;
               minMax[2] = 999;
               minMax[3] = -999;
               minMax[4] = -999;
               minMax[5] = -999;
               centralize(minX, minY, minZ, maxX, maxY, maxZ, frame.allshapes3d, minMax);
               minX = minMax[0];
               minY = minMax[1];
               minZ = minMax[2];
               maxX = minMax[3];
               maxY = minMax[4];
               maxZ = minMax[5];
               doScale(frame.allshapes3d);
               for(i=0; i<frame.allshapes3d.size(); i++) 
                  for(int j=0; j<frame.allshapes3d.get(i).ConstraintList.size(); j++)
                  {	frame.allshapes3d.get(i).ConstraintList.get(j).doPaint();
                  //System.out.println("111> > draw constraints "+frame.allshapes3d.get(i)+" "+frame.allshapes3d.get(i).ConstraintList.get(j));
                  }
               frame.panelShapeArea3d.setVisible(true);
            }	 
            else
            {
               int scaleFixx = fixSh.getX(), scaleFixy = fixSh.getY();
               if(doscale)
               {
                  if ((maxY-minY) > delta)
                  {     
                     delta = maxY-minY + 20;
                     if (delta > height)
                     {
                        frame.doScale((float)(height/delta));  
                        delta = height/delta;  
                        maxX = (int)((maxX-scaleFixx)*delta)+scaleFixx;
                        minX = (int)((minX-scaleFixx)*delta)+scaleFixx;
                        maxY = (int)((maxY-scaleFixy)*delta)+scaleFixy;
                        minY = (int)((minY-scaleFixy)*delta)+scaleFixy;
                     }        
                     if(maxY-minY <250)
                     {
                        frame.doScale((float)(frame.lastScale*2));
                        maxX = (int)((maxX-scaleFixx)*2)+scaleFixx;
                        minX = (int)((minX-scaleFixx)*2)+scaleFixx;
                        maxY = (int)((maxY-scaleFixy)*2)+scaleFixy;
                        minY = (int)((minY-scaleFixy)*2)+scaleFixy;
                     }
                  }
                  else
                  {
                     delta += 20; 
                     if (delta > width)
                     {
                        frame.doScale((float)(width/delta));
                        delta = height/delta;
                        maxX = (int)((maxX-scaleFixx)*delta)+scaleFixx;
                        minX = (int)((minX-scaleFixx)*delta)+scaleFixx;
                        maxY = (int)((maxY-scaleFixy)*delta)+scaleFixy;
                        minY = (int)((minY-scaleFixy)*delta)+scaleFixy;
                     }
                     if(maxX-minX <250)
                     {
                        maxX = (int)((maxX-scaleFixx)*2)+scaleFixx;
                        minX = (int)((minX-scaleFixx)*2)+scaleFixx;
                        maxY = (int)((maxY-scaleFixy)*2)+scaleFixy;
                        minY = (int)((minY-scaleFixy)*2)+scaleFixy;
                     }
                  }
               }
               if (minX < 0)
                  frame.doTranslate((int)(-minX),0);
               else if (maxX > width)
                  frame.doTranslate((int)(width-maxX),0);
            
               if (minY < 0)
                  frame.doTranslate((int)(-minY),0);
               else if (maxY > height)
                  frame.doTranslate((int)(height-maxY),0);	    
            }
         /**************************************************************************/
	    int ret = dialog.showConfirmDialog(frameMain.panelShapeArea, "Do you want to re-do any cluster?","re-do?", JOptionPane.YES_NO_OPTION);
            if (ret == JOptionPane.YES_OPTION)
		{
		final JDialog reSolveDialog = new JDialog(frameMain,"Re-solve",true);
                final JTextField cluster = new JTextField();
                JTextArea text = new JTextArea("Enter the cluster ID to re-Do");
                text.setBackground(Color.lightGray);
                JPanel okPanel = new JPanel();
                JButton okBtn = new JButton();
                okBtn.setText("OK");
                okBtn.addActionListener(
                                      new java.awt.event.ActionListener()
                                         {
            
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       errResolveID = java.lang.Integer.parseInt(cluster.getText());
                                       reSolveDialog.hide();
                                    }
                                 });
                reSolveDialog.setSize(250,100);
                reSolveDialog.setLocation(691,420);
                reSolveDialog.getContentPane().add(text,BorderLayout.NORTH);
                reSolveDialog.getContentPane().add(cluster,BorderLayout.CENTER);
                reSolveDialog.getContentPane().add(okPanel,BorderLayout.SOUTH);
                okPanel.add(okBtn);
                reSolveDialog.show();
                dataInt[0]=2;
                dataInt[savedIndex++]=-1;
                dataInt[savedIndex++]=errResolveID;
                SKGroupTreeNode reSolveGrp = dag.findByID(errResolveID);
                unFixShapes(reSolveGrp);
                frameMain.mode=2;
                frame.solved=true;
                actClusters.hide();
		return true;
		}
	    else 
		{
	         frameMain.oldShapeID = frameMain.IDCnt - 1;
        	 frameMain.oldConstraintID = frameMain.ConstrIDCnt;
         	 frameMain.oldGroupID = frameMain.GroupIDCnt;
         	 for( i=0; i<frameMain.oldConstraints.size(); i++) 
                 {
            	 if(frameMain.oldConstraints.get(i) instanceof SKDistanceConstraint)
	            frameMain.consArray[frameMain.oldConstraints.get(i).ID] = ((SKDistanceConstraint)frameMain.oldConstraints.get(i)).distance;
        	 else if(frameMain.oldConstraints.get(i) instanceof SKAngleConstraint)
               	    frameMain.consArray[frameMain.oldConstraints.get(i).ID] = ((SKAngleConstraint)frameMain.oldConstraints.get(i)).angle;
         	 }
                 actClusters.hide();
		 return false;
		}
         }
         
         //received bifurcation information
         else if(numSolutions == 0)
         {  
            dagMain.copyArrayTo(dag);
            for(int l=0; l<dag.size(); l++)
               if (dag.get(l).parents.size()==0)
                  roots.add(dag.get(l));
         
            boolean reSol = false;
            int startBifurChoice = dataInt[0];//the flag for solver to re-solve the parent of the indicated cluster.  
            int c = startBifurChoice;
            int numClusterBifur = dataInt[indexInt++];//Number of clusters whose bifurcations have been sent.
            int[] index= new int[2];
            int[] fin = new int[dag.size()];
            int elements = dataInt[indexInt++];
            int numCluster = 1;//Number of the cluster whose bifucation is being showed to the user.
            int savedActClusterID = -1;
            SKClusterObjArray clusterArray = new SKClusterObjArray(2);
         
            for(int i=0; i<fin.length; i++)
               fin[i]=1;
         
            for(int i=0; i<elements; i++)
            {
               int ind =dataInt[indexInt++];
               ind=dag.indexOf(dag.findByID(ind));
               fin[ind] = dataInt[indexInt++];
            }
            while(numCluster<=numClusterBifur)
            {  
               if(clusterArray.findByID(dataInt[indexInt])==null)
                  clusterArray.add(new SKClusterObj(dataInt[indexInt],indexInt,indexDbl,numCluster,c)); 
            
               SKBifurFrame dlg = null;
               SKBifurFrame2d dlg2d = null;
               actCluster = dataInt[indexInt++]; 
               actClusters = new TreeFrame(frame,dag,actCluster,fin);
               actClusters.setLocation(691,54);
               actClusters.setVisible(true);
	       int tempReSolveID = -1;
               int numbifur = dataInt[indexInt++];                  
               index[0]=indexInt;
               index[1]=indexDbl;    
            
               //Show the dialog	
               if(threeD)
               {
                  dlg = new SKBifurFrame(frame,"Bifurcations",true);
                  dlg.bifurcations = new SKBifurcationArray(5);
		  dlg.bifurcations.readFromStream(dataInt,dataDouble,index,numbifur,allShapes3d,frame,dlg.pnlShapes3d,dlg);
                  dlg.setLocation(691,420);
                  dlg.setVisible(true);
                  if (!dlg.approved )
                     return false;
               }
               else
               {
                  dlg2d = new SKBifurFrame2d(frame,"Bifurcations",true);
                  dlg2d.bifurcations = new SKBifurcationArray2d(5);
		  dlg2d.bifurcations.readFromStream(dataInt,dataDouble,index,numbifur,allShapes,frame,dlg2d.pnlShapes,dlg2d);
                  dlg2d.setLocation(691,420);
                  dlg2d.setVisible(true);
                  if (!dlg2d.approved )
                     return false;
               } 
               indexInt=index[0];
               indexDbl=index[1];
               boolean canFix=true;
               for(int a=0 ; a<roots.size(); a++)
                  for(int i=0; i<roots.get(a).children.size(); i++)
                     if(actCluster==roots.get(a).children.get(i).ID) canFix=false;

	       if(threeD)
                  tempReSolveID = dlg.reSolveID;
               else
                  tempReSolveID = dlg2d.reSolveID;
            
                  if(tempReSolveID != -1)
                  {
                     reSol=true;
                     if(clusterArray.findByID(tempReSolveID)== null)
                     {
                        dataInt[startBifurChoice] = -1;
                        dataInt[startBifurChoice+1] = tempReSolveID;
                        SKGroupTreeNode reSolveGrp = dag.findByID(tempReSolveID);
                        unFixShapes(reSolveGrp);
                        numCluster=numClusterBifur;
                        frameMain.IntArray[0]=2;
                        frameMain.mode=2;
			reSol=false;
                     }
                     else
                     {
                        SKClusterObj newCluster = clusterArray.findByID(tempReSolveID);
                        indexInt = newCluster.Int;
                        indexDbl = newCluster.Dbl;
                        numCluster = newCluster.index-1;
                        c = newCluster.bifurChoice;
                        savedActClusterID=actCluster;
                        SKGroupTreeNode group = dag.findByID(tempReSolveID);
                        for(int i=0; i<group.shapes.size();i++)
                           frameMain.allshapes.findByID(group.shapes.get(i).ID).fixed = false;
                     }
                  }
                  else
                  {
                       if(threeD)
                         dataInt[c++] = dlg.currBifurcation;
		       else
			 dataInt[c++] = dlg2d.currBifurcation;
//	  	       System.out.println("111 Choice"+dlg2d.currBifurcation);

                     if(reSol)
                     {
                        reSol=false;
                        SKClusterObj savedCluster = clusterArray.findByID(savedActClusterID);
                        indexInt=savedCluster.Int;
                        indexDbl=savedCluster.Dbl;
                        numCluster=savedCluster.index-1;
                        c = savedCluster.bifurChoice;
                     }
                  }
               numCluster++;
               actClusters.setVisible(false);
            }
            frame.solved = true;
            if(threeD)
            {
               frame.panelShapeArea.setVisible(false);
               frame.panelShapeArea3d.setVisible(true);
            }
            return true;
            //utuDriver.utuC(dataInt, dataDouble);
         }
         return false;
         //}
      }
   	/**
   *Writes all the data like coordinate of the shapes, their constraints and the groups in the sketch to two arrays viz., dataInt and dataDouble and sends them to 
   *UTU for further processing before solving.
   */
      public static void writeUTUFile(int[] IntArray, double[] DblArray, char[] CharArray, SKShapeArray shapes, SKGroupTreeNode groupRoot, boolean threeD)
      {  
         dataInt = IntArray;
         dataDouble = DblArray;
         dataChar = CharArray;
         indexInt = 0;
         indexDbl = 0;
         System.out.println("size = "+shapes.size());
      
         for(int j=0; j<dataInt.length; j++)
            dataInt[j] = 0;
         for(int j=0; j<dataDouble.length; j++)
            dataDouble[j]=0;
         dataInt[indexInt++]=0; //flag for UTU
         dataInt[indexInt++]=SKOptions.byteOptions[ SKOptions.ESMInteraction ]; //flag for ESM
      //System.out.println("111 lets check !!! "+SKOptions.byteOptions[SKOptions.ESMInteraction ]);
      	//Write shape list
         SKBaseShape sh,tmp;
         int i,g,h;
         for (i=0; i<shapes.size(); i++)
         {
            sh = (SKBaseShape)shapes.get(i);
            if(threeD)
            {
               switch(sh.ShapeTypeID)
               {
                  case 3:
                     SKLineShape line = (SKLineShape)sh;
                        //Write TypeID based if Segment,Ray,Line
                     switch (line.pt1.pointType+line.pt2.pointType)
                     {
                        case 0:dataInt[indexInt++]=7; //Segment
                           //System.out.println("typeID"+3);
                           break;
                        case 1: dataInt[indexInt++]=8; //Ray
                           break;
                        case 2: dataInt[indexInt++]=9; //Line (infinite)
                     }
                     break;
                  case 0:
                     dataInt[indexInt++]=6;
                     break;
                  default:
                     dataInt[indexInt++]=sh.ShapeTypeID;
                     break;
               }
            }
            else
            {
               switch(sh.ShapeTypeID)
               {
                  case 3:
                     SKLineShape line = (SKLineShape)sh;
                        //Write TypeID based if Segment,Ray,Line
                     switch (line.pt1.pointType+line.pt2.pointType)
                     {
                        case 0:dataInt[indexInt++]=3; //Segment
                           break;
                        case 1: dataInt[indexInt++]=2; //Ray
                           break;
                        case 2: dataInt[indexInt++]=1; //Line (infinite)
                     }
                     break;
                  case 6:
                     if(sh instanceof SKImageShape)
                        dataInt[indexInt++]=3;	
                     else 
                        dataInt[indexInt++]=6;
                     break;
                  default:
                     dataInt[indexInt++]=sh.ShapeTypeID;
               }
            }
            dataInt[indexInt++]=sh.ID;	
            switch (sh.ShapeTypeID)
            {
            
               case 0:
                  {
                     dataDouble[indexDbl++]=(double)sh.getShapeX();
                     dataDouble[indexDbl++]=(double)sh.getShapeY();
                     if(threeD)
                        dataDouble[indexDbl++]=(double)sh.getShapeZ(); 
                  }
                  break;
               case 3 : 
                  { //SKLineShape
                     SKLineShape line = (SKLineShape)sh;
                     dataDouble[indexDbl++]=(double)line.pt1.getShapeX();
                     dataDouble[indexDbl++]=(double)line.pt1.getShapeY();
                     if(threeD)
                        dataDouble[indexDbl++]=(double)line.pt1.getShapeZ();
                     dataDouble[indexDbl++]=(double)line.pt2.getShapeX();
                     dataDouble[indexDbl++]=(double)line.pt2.getShapeY();
                     if(threeD)
                        dataDouble[indexDbl++]=(double)line.pt2.getShapeZ();
                     dataDouble[indexDbl++]=line.length;
                  }
                  break;
               case 4 : 
                  { //SKCircleShape
                     dataDouble[indexDbl++]=(double)((SKCircleShape)sh).center.getShapeX();
                     dataDouble[indexDbl++]=(double)((SKCircleShape)sh).center.getShapeY() ;
                     dataDouble[indexDbl++]=((SKCircleShape)sh).radius ;
                  }
                  break;
               case 5:
                  {
                     dataDouble[indexDbl++]=(double)((SKArcShape)sh).center.getShapeX();
                     dataDouble[indexDbl++]=(double)((SKArcShape)sh).center.getShapeY();
                     dataDouble[indexDbl++]=((SKArcShape)sh).radius;
                     dataDouble[indexDbl++]=((SKArcShape)sh).angle;
                  }
                  break;
               case 6:
                  {  
                     if(sh instanceof SKImageShape)
                     {
                        dataDouble[indexDbl++]=(double)((SKImageShape)sh).pt1.getShapeX();
                        dataDouble[indexDbl++]=(double)((SKImageShape)sh).pt1.getShapeY();
                        dataDouble[indexDbl++]=(double)((SKImageShape)sh).pt2.getShapeX();
                        dataDouble[indexDbl++]=(double)((SKImageShape)sh).pt2.getShapeY();
                        dataDouble[indexDbl++]=-1;
                     }
                  }
                  break;
               default: 
                  break;
            
            }
         
         }
      
      //Signify end of shape list
         dataInt[indexInt++]=-1;
      //p.writeInt(-1);
      
      //Write constraint list
         SKBaseConstraint con;
         SKConstraintArray Cons= new SKConstraintArray(100);
         for (i=0; i<shapes.size(); i++)
         {
            sh = (SKBaseShape)shapes.get(i);
            for (h=0; h<sh.ConstraintList.size(); h++)
            {
               con = sh.ConstraintList.get(h);

               if (con.isPrimaryShape(sh) && Cons.indexOf(con)==-1)
               {
                  Cons.add(con);
                  if((con.typeID == 0) && threeD)
                     dataInt[indexInt++]=6;
                  else if ((con.typeID == 4) && threeD)
                     dataInt[indexInt++]=7;
                  else
                     dataInt[indexInt++]=con.typeID;
                  dataInt[indexInt++]=con.ID;
                  dataInt[indexInt++]=con.ShapeList.size();
                  if (con instanceof SKNormalConstraint)
                  { //CAN consist of specific subshapes
                     for (g=0; g<con.ShapeList.size(); g++)
                     {
                        tmp = con.ShapeList.get(g);
                        dataInt[indexInt++]=tmp.ID;
                        if(tmp instanceof SKLineShape)
                        {
                           if((((SKLineShape)tmp).pt1.pointType+((SKLineShape)tmp).pt2.pointType)==2)
                              dataInt[indexInt++]=0;
                           else
                              dataInt[indexInt++]=con.getConInfo(tmp,0);
                        }
                        else
                           dataInt[indexInt++]=con.getConInfo(tmp,0);
                     }
                  
                     switch (con.typeID)
                     {
                        case 0: dataDouble[indexDbl++]=((SKDistanceConstraint)con).distance;
                           break;
                        case 4: dataDouble[indexDbl++]=((SKAngleConstraint)con).angle;
                           break;
                     }
                  }
                  else
                  { //Cannot consist of specific subshapes
                     for (g=0; g<con.ShapeList.size(); g++)
                        dataInt[indexInt++]=con.ShapeList.get(g).ID;
                  }
               }
            }
         
         }
      
      //Signify end of constraint list
         dataInt[indexInt++]=-1;
      
      //Write group list
         indexInt=groupRoot.saveToUTUFile(indexInt, dataInt);
      
      //Signify end of group list (and file)
         dataInt[indexInt++]=-1;
      
      
         for (int x= 0; x<indexInt ; x++)
            System.out.println("Sent "+dataInt[x]);
         for (int x= 0; x<indexDbl ; x++)
            System.out.println("Sent "+dataDouble[x]);
      
         utuJava utuDriver = new utuJava();
         utuDriver.utuC(dataInt, dataDouble, dataChar);
      
      }
   
   
      public static float distance(int x1, int y1, int x2, int y2)
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
      public static void doScale(SKShape3dArray shapes)
      {
         int indicator = 0;
         int numSubShapes=0;
         for(int i=0; i<shapes.size(); i++)
            for(int j=0; j<shapes.get(i).getNumSubShapes(); j++)
               numSubShapes++;
         double xcoords[] = new double[numSubShapes];
         double ycoords[] = new double[numSubShapes];
         double zcoords[] = new double[numSubShapes];
         int index=0;
         for(int i=0; i<shapes.size(); i++)
            for(int j=0; j<shapes.get(i).getNumSubShapes(); j++)
            {
               xcoords[index] = shapes.get(i).getSubShape(j).getDX();
               ycoords[index] = shapes.get(i).getSubShape(j).getDY();
               zcoords[index++] = shapes.get(i).getSubShape(j).getDZ();
            System.out.println("111 utu"+xcoords[index-1]+" "+ycoords[index-1]+" "+zcoords[index-1]);
            }
      
         indicator =  checkFit(xcoords,ycoords,zcoords);
         if(indicator<0)
         {
            while (indicator<0)
            {
               changeScale(0.9,xcoords,ycoords,zcoords);
               indicator = checkFit(xcoords,ycoords,zcoords);
            }
            changeScale(1.1,xcoords,ycoords,zcoords);
         }
         else
         {
            while (indicator>0)
            {
               changeScale(1.1,xcoords,ycoords,zcoords);
               indicator = checkFit(xcoords,ycoords,zcoords);
            }   
            changeScale(0.9,xcoords,ycoords,zcoords);
         }
         scaleSketch(xcoords, ycoords,zcoords,shapes);
      }
      public static int checkFit(double[] xcoords, double[] ycoords, double[] zcoords)
      {
         double maxX=-999,minX=999,maxY=-999,minY=999, maxZ=-999, minZ=999;
         for(int i=0; i<xcoords.length; i++)
         {     
            if(xcoords[i]<minX) minX=xcoords[i];
            if(xcoords[i]>maxX) maxX=xcoords[i];
            if(ycoords[i]<minY) minY=ycoords[i];
            if(ycoords[i]>maxY) maxY=ycoords[i];
            if(zcoords[i]<minZ) minZ=zcoords[i];
            if(zcoords[i]>maxZ) maxZ=zcoords[i];
         }
         if((maxX<7) && (minX>-5) && (maxY<7) && (minY>-7) && (maxZ<7) && (minZ>-7))
            return 1;
         else
            return -1;
      }
      public static void changeScale(double scale, double[] x, double[] y, double[] z)
      {
         for(int i=0; i<x.length; i++)
         {
            x[i] *= scale;
            y[i] *= scale;
            z[i] *= scale;
            //System.out.println(" 111 scaling : "+x[i]+" "+y[i]);
         }
      }
      public static void scaleSketch(double[] x, double[] y, double[] z, SKShape3dArray shapes)
      {
         int index=0;
	 double scale=-1;
	 for(int i=0; i<shapes.size(); i++)
            for(int j=0; j<shapes.get(i).getNumSubShapes(); j++)
            {
		if(shapes.get(i).getSubShape(j).getDX()!=0.0)
		 {scale = x[index++]/shapes.get(i).getSubShape(j).getDX();
		  break;}
		else index++;
	    }
	 index=0;
         for(int i=0; i<shapes.size(); i++)
            for(int j=0; j<shapes.get(i).getNumSubShapes(); j++)
            {	
               SKBaseShape3d sh = shapes.get(i).getSubShape(j);
            //System.out.println("111 scale pic "+sh+" "+x[index]+" "+y[index]+" "+sh.getDZ());
	       if(sh instanceof SKPointShape3d && !(sh instanceof SKPointSubShape3d))
	       {
 	         System.out.println("111 before = "+((SKPointShape3d)sh).radius);
		 ((SKPointShape3d)sh).changeRadius(((SKPointShape3d)sh).radius*scale);
	         System.out.println("111 after = "+((SKPointShape3d)sh).radius);
	       }
               sh.doMove(new Vector3f((float)x[index],(float)y[index],(float)z[index]));
               index++;
            }
      }
      public static void centralize(double minX, double minY, double minZ, double maxX, double maxY, double maxZ, SKShape3dArray shapes, double[] minMax)
      {
         double centerX = 0;
         double centerY = 0;
         double centerZ = 0;
         double centerSketchX = ((maxX-minX)/2) + minX;
         double centerSketchY = ((maxY-minY)/2) + minY;
         double centerSketchZ = ((maxZ-minZ)/2) + minZ;
         double translateX = centerX-centerSketchX;
         double translateY = centerY-centerSketchY;
         double translateZ = centerZ-centerSketchZ;
         for(int i=0; i<shapes.size(); i++)
            for(int j=1; j<shapes.get(i).getNumSubShapes()+1; j++)
            {
               SKBaseShape3d sh = shapes.get(i).getSubShape(j);
               double x = sh.getDX()+translateX;
               double y = sh.getDY()+translateY;
               double z = sh.getDZ()+translateZ;
               ((SKPointShape3d)sh).doMove(new Vector3f((float)x,(float)y,(float)z));
               //System.out.println("111 after "+sh+" x "+x+" y "+y+" z "+z);
               if(x<minMax[0]) minMax[0]=x;
               if(y<minMax[1]) minMax[1]=y;
               if(z<minMax[2]) minMax[2]=z;
               if(x>minMax[3]) minMax[3]=x;
               if(y>minMax[4]) minMax[4]=y;
               if(z>minMax[5]) minMax[5]=z;
            
            }
      }
      static void unFixShapes(SKGroupTreeNode group)
      {
         for(int i=0; i<group.shapes.size(); i++)
            frameMain.allshapes.findByID(group.shapes.get(i).ID).fixed=false;
         if(group.parent!=null)
            unFixShapes(group.parent);
      }
   }

