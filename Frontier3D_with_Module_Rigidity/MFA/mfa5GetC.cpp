//file: mfa5GetC.cpp
/*contains two methods: copying  graph and sequential extensions for
  the DR-planner*/
/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


// copy a graph and return K value
int copyG(Graph &g0, Graph &g1)
{
    int i, nVer, nEdg, dimen;

    g1.delAllVer();
    g1.delAllEdg();

    dimen=g0.returnDimen();
    g1.setDimen(dimen);

    nVer=g0.returnNumVer();
    nEdg=g0.returnNumEdg();

    for(i=1;i<=nVer;i++)
       g1.appendVertex(g0.returnVertByIndex(i));

    for(i=1;i<=nEdg;i++)
       g1.appendEdge(g0.returnEdgeByIndex(i));

    if(dimen==3) return 7;  //K=7 for 3-D problem
    else return 4;          //K=4 for 2-D problem
}

//checks for special cases that should be extended
bool checkSpecialExtensionCase(Graph &F, ostream &out)
{
    int v0Name;
    Vertex v0;
    int pointCount=0, lineSegCount=0;
    int nLabelE=0, nLabelV=0;

    if(F.returnDimen()!=3) return false;

    int i, length;
    Edge tempE1, e1;

    length=F.returnNumEdg();
    for(i=1;i<=length;i++)
    {
       tempE1=F.returnEdgeByIndex(i);
       if(tempE1.returnLabel()==0) continue;
       nLabelE++;
       e1=tempE1;
    }       

    if(nLabelE!=1) return false;

    for(v0Name=1;v0Name<nextVerName;v0Name++)
    {
       if(F.hasVert(v0Name))
       {
         v0=F.returnVertByName(v0Name);
	 if(v0.returnLabel()==0) continue;
     	 nLabelV++;
         int type;
         type=v0.returnType();
	 switch(type)
	 {
	       case 6: pointCount++;
		       break;
	       case 7: lineSegCount++;
		       break;
	 }
       } 
    }

    Vertex v1, v2;

    v1=F.returnVertByName(e1.returnEnd1());
    v2=F.returnVertByName(e1.returnEnd2());

    out<<"nLabelV="<<nLabelV<<" PointCount="<<pointCount<<" nLabelE="<<nLabelE<<" End1="<<e1.returnEnd1()
	<<" End2="<<e1.returnEnd2()<<endl;
    
    if(	     nLabelV==pointCount 
          && nLabelV==2 
          && nLabelE==1
          && v1.returnLabel()==1
          && v2.returnLabel()==1 ) return true;

    return false;
}

//check whether the current labeld vertices in F are able to be extended
bool canExtend(Graph &F, Graph &graph0, List<Cluster> &DR_Trees, int groupID, List<int> &groupV)
{
	if (groupID==0) return true;
	List<int> labeledV;

	List<Vertex> vlist=F.returnVlist();
 
	ofstream outf("canExtend.out", ios::app);
	outf<< "The beginning" << endl;
	int vname;
	for(int i=1; i<=vlist.returnLen();i++)
	{
		vname = vlist.retrieve(i).returnName();
		outf << "vname is: " << vname << endl;
		if(vlist.retrieve(i).returnLabel() && !groupV.hasElem(vname))
		{
			F.removeLabels();
			return false;
		}
		outf << "pass check" << endl;
	}
	return true;
}




//performs a sequential extension on the labeled section of F, sequential extensions are described in the
//attached paper documentation
int extension(Graph &F, Graph &graph0, ostream &file1, ostream &file2)
{
    Vertex v0, v1;
    Edge edge;
    List<Edge> newEdges, oldEdges;  // put new edges F when extend a vertex
    int j, nVertg0, nVertF, nLabelF, possible, extEdgeNum;
    int v0Name, v1Name;
    int eWeight, newEdgeLen, oldEdgeLen, newLabel=-2;
    int notIncidCount=0;

    nLabelF=F.numLabeled();
    file2<<"---------------start---------------"<<endl;
    file2<<"Extension, nLabeled="<<nLabelF<<endl;
    file2<<"Contents of F:"<<endl;
    F.output(file2);
    if(nLabelF==0)
    {
      file2<<"nLabeled=0"<<endl;
      file2<<"Extension returns 0"<<endl;
      file2<<"----------------end----------------"<<endl;
      return 0;
    }

  for(v0Name=1;v0Name<nextVerName;v0Name++) // v0--new vertex to be extended
  {
	file2 << "the v0name is: " << v0Name << endl;
     possible=0;
     if(F.hasVert(v0Name))
     {
         v0=F.returnVertByName(v0Name);
         if(v0.returnLabel()==0&&(!v0.isFrozen())) possible=1; //v0 in F, but NOT in new cluster
	 file2 << "v0 in F is: " << v0 << endl;
	 file2 << "possible: " << possible << endl;
     }
     else if(v0Name<=singleVertex&&graph0.hasVert(v0Name))
     {
         v0=graph0.returnVertByName(v0Name); //v0 not in F but in graph0
         if(F.returnDepth()==0&&v0.returnName()!=0&&(!v0.isFrozen())) possible=1;
	 file2 << "v0 in graph0 is: " << v0 << endl;
	 file2 << "possible: " << possible << endl;
     }

     if(possible==1)  // v0 is possible to be extended
     {
        eWeight=0;
        extEdgeNum=0;         // new extended edges for new cluster
        newEdges.makeEmpty(); // new edges for F
        oldEdges.makeEmpty();
	notIncidCount=0;

        file2<<"possible v0="<<v0<<endl;

        nVertF=F.returnNumVer();
        for(j=1;j<=nVertF;j++)           // for every vertex v1 in F
        {
           v1=F.returnVertByIndex(j);
           //v1 is frozen, skip it
           if (v1.isFrozen() != 0) continue;
           v1Name=v1.returnName();
           edge=F.returnEdgeByEnds(v0Name,v1Name);
           edge.setScan(0);
           if(edge.returnName()==0) // F dose not have this edge
           {
              edge=graph0.returnEdgeByEnds(v0Name,v1Name);
              if(v1.returnLabel()!=0) edge.setScan(1);//connected to new clust
              if(edge.returnName()!=0) newEdges.append(edge);
           }
           else
           {
              if(v1.returnLabel()!=0) edge.setScan(1);//connected to new clustr
              oldEdges.append(edge);
           }

           if(v1.returnLabel()!=0)       // v1 belongs to new cluster
           {
              if(edge.returnWeight()!=0)
              {
                 extEdgeNum++;
		 if(edge.returnType()!=1) notIncidCount++;
                 eWeight=eWeight+edge.returnWeight();
                 file2<<"Update:  extEdgeNum="<<extEdgeNum<<" eWeight="<<eWeight<<endl;
              }
           }
        }

	file2<<"Final:  extEdgeNum="<<extEdgeNum<<" eWeight="<<eWeight<<endl;

	bool test;

	ofstream output;

	output.open("../MFA/textSpecCase.txt", ofstream::app);

	F.output(output);

	test=checkSpecialExtensionCase(F, file2);

	output.close();

	file2<<"Check Special Case: "<<test<<endl;

        if(test) output<<"SPECIAL CASE = TRUE"<<endl;
        else output<<"SPECIAL CASE = FALSE"<<endl;

	file2<<"Dimension="<<F.returnDimen()<<endl;

        if((eWeight>=v0.returnWeight() && extEdgeNum>=F.returnDimen())
           || (test && (eWeight+1)>=v0.returnWeight())
	   || (notIncidCount==0 && eWeight>=v0.returnWeight())) //exists
        {
           if(eWeight>v0.returnWeight())
             v0.setLabel(3);
           else if(nLabelF>F.returnDimen())
             v0.setLabel(2);
           else
             v0.setLabel(1);

           if(F.hasVert(v0Name))
           {
              F.delVerByName(v0Name);
           }
	   F.appendVertex(v0);  // extended vertex is added to F

           newEdgeLen=newEdges.returnLen();
           for(j=1;j<=newEdgeLen;j++)
           {
              edge=newEdges.pop();
              if(edge.returnScan()==1) //edge is connected to new cluster
              {
                 edge.setLabel(1);
              }
              F.appendEdge(edge);
//	      F.distribute0(edge,file2);
           }
           oldEdgeLen=oldEdges.returnLen();
           for(j=1;j<=oldEdgeLen;j++)
           {
              edge=oldEdges.pop();
              if(edge.returnScan()==1) //edge is connected to new cluster
              {
                 F.delEdgeByName(edge.returnName());
                 edge.setLabel(1);
                 F.appendEdge(edge);
              }
           }
           file1<<"Extend Vertice "<<v0Name<<endl;
           file2<<"new extended v0="<<v0Name<<" current F is "<<endl;
           file2<<"Ending Contents of F: "<<endl;
           F.output(file2);
           file2<<"Extension returns 1"<<endl;
           file2<<"----------------end----------------"<<endl;
           return 1;
           //v0Name=0;   // try one more cycle
        }
     } //end of if(possible==1)
  }
  file2<<"Extension returns 0"<<endl;
  file2<<"----------------end----------------"<<endl;
  return 0;
}

int extension(Graph &F, Graph &graph0, bool firstTime, int groupID,
 	ostream &file1, ostream &file2)
{
	if(firstTime && groupID>0)
	{
		F.removeLabels();
		return 0;
	}
	return extension(F, graph0, file1, file2);
}

//After distribute has been run, Graph F contains vertices labeled for the creation of a new cluster
//this method isolates those vertices, performs any reductions to the interior vertices and edges 
//as necessary, and creates a new Cluster object, returning a pointer to it
//for more on the specifics of this reduction process, see attached paper documentation
Cluster * getCluster(Graph &F, Graph &graph0)
{
    Vertex vOld, vOther, vNew, v1, v2, v3, core;
    Edge e1Old, eOld, eNew, eTemp;
    List<Vertex> innerVers;
    List<Edge> innerE, outerE;
    List<int> frontiers;
    List<int> originalV;
    //List<int> originalE;
    int depth=0, oldDepth, constrain=0;
    Cluster *new_cluster=new Cluster;

    ofstream outf("getCluster.out", ios::app);
    outf<<"Getting Cluster "<<nextVerName<<endl;
    outf<<"Contents of F before new labeling"<<endl;
    F.output(outf);

    int i, j, k, l, nVerF, nFront=0, nIncid, nVerInner=0;
    int ItIsFront, e1Name, coreWeight, eWeight;
    int vOldName, v1Name, v2Name, v3Name, vOtherName;
    int v1OutF;

    frontiers.makeEmpty();
    //out<<"before getCluster"<<endl;
    //F.output(out);

    //labeled vertex in F is classified as frontier or not
    nVerF=F.returnNumVer();

    for(i=1;i<=nVerF;i++) // for every vertex in F
    {
	vOld=F.returnVertByIndex(i);
	if(vOld.returnLabel()!=0 && vOld.returnDepth()>0)
	{
		Vertex v2;		
		outf<<"Labeled core: "<<vOld.returnName()<<endl;		

		for(int j=1;j<=nVerF;j++)
		{
			v2=F.returnVertByIndex(j);
			eTemp=F.returnEdgeByEnds(vOld.returnName(),v2.returnName());			
			outf<<"Checking edge: "<<eTemp<<endl;
			if(eTemp.returnName()>0 && v2.returnLabel()==0)
			{
				F.VertAddr(v2.returnName())->setLabel(1);
				outf<<"Labeled vertex: "<<v2.returnName()<<endl;
			}
		}
	}
    }

    outf<<"Contents of F after new labeling"<<endl;
    F.output(outf);

    for(i=1;i<=nVerF;i++) // for every vertex in F
    {
        vOld=F.returnVertByIndex(i);
        if(vOld.returnLabel()!=0 && vOld.isFrozen()==0)  // make sure vOld in new cluster
        {
           if(vOld.returnLabel()-1>constrain) constrain=vOld.returnLabel()-1;
           oldDepth=vOld.returnDepth();
           if(depth<oldDepth) depth=oldDepth;
           vOldName=vOld.returnName();
           //prevClst.append(vOldName); //previous found clusters
           originalV.append(vOldName);
           outf<<"after append originalV"<<endl;
           F.output(outf);

           ItIsFront=0;
           nIncid=vOld.numIncid();
           for(j=1;j<=nIncid;j++)
           {
              e1Name=vOld.returnIncid(j);
              if(F.hasEdge(e1Name))
              {
                 e1Old=F.returnEdgeByName(e1Name);
                 vOtherName=e1Old.otherEnd(vOld.returnName());
                 vOther=F.returnVertByName(vOtherName);
                 if(vOther.returnLabel()==0) ItIsFront=1;//other not in cluster
              }
              else
                 ItIsFront=1;  // edge not in F
           }
           if(ItIsFront)
           {
               //prevClst.append(vOldName);
               frontiers.append(vOldName);
               outf<<"vOld="<<vOld<<endl;
               nFront++;
           }
           else
           {
               vOld.setScan(-1);
               innerVers.append(vOld);
               nVerInner++;
            }
        }
    }
    outf <<"after classify"<<endl;

    // set scan=-1 for all inner vertices
    for(i=1;i<=nVerInner;i++)
    {
        v2=innerVers.retrieve(i);
        F.delVer(v2);
        v2.setScan(-1);
        F.appendVertex(v2);
        //prevClst.append(v2.returnName());
    }

    /***
    outf<<"frontiers"<<endl;
    for(i=1;i<=frontiers.returnLen();i++)
       out<<frontiers.retrieve(i)<<endl;
    out<<"innerVers"<<endl;
    for(i=1;i<=innerVers.returnLen();i++)
       out<<innerVers.retrieve(i)<<endl;
    ***/

    //form inner and get outer edges
    coreWeight=0;
    for(i=1;i<=nFront;i++)
    {
       v1OutF=0; //part of v1's flow, absorbed from frontier & outer edges
       v1Name=frontiers.retrieve(i);
       v1=F.returnVertByName(v1Name);
       coreWeight=coreWeight-v1.returnWeight();
	outf << "coreWeight is" << coreWeight << endl;
       //get outer edges
       eWeight=0;
       for(j=1;j<=nVerF;j++)
       {
          v3=F.returnVertByIndex(j);
          v3Name=v3.returnName();
          eOld=F.returnEdgeByEnds(v1Name,v3Name);
          if(eOld.returnName()==0)
	  {
             eOld=graph0.returnEdgeByEnds(v1Name,v3Name);
	     if(eOld.returnName()!=0)
	     {
		F.appendEdge(eOld);
		eOld=F.returnEdgeByName(eOld.returnName());
	     }
	  }
          if(v3.returnScan()!=-1&&eOld.returnName())//v3 is not an inner vertex
          {
             if(v1Name==eOld.returnEnd1()) v1OutF=v1OutF+eOld.returnFlow1();
             if(v1Name==eOld.returnEnd2()) v1OutF=v1OutF+eOld.returnFlow2();

             if(v3.returnLabel()!=0&&v1Name!=v3Name&&!outerE.hasElem(eOld))
             {
                outerE.append(eOld);   // edge between frontiers
                eWeight=eWeight+eOld.returnWeight(); //claculate outer weight
                outf << "eWeight is: " << eWeight << endl;
             }
          }
       }

       coreWeight=coreWeight+eWeight;  // add outer edge weight
	outf << "coreWeight is" << coreWeight << endl;

       //form inner edges
       eWeight=0;
       outf<<"Calculating Inner Edges"<<endl;
       for(j=1;j<=nVerInner;j++)
       {
          v2=innerVers.retrieve(j);  // v2 is inner vertex
          v2Name=v2.returnName();
          eOld=F.returnEdgeByEnds(v1Name,v2Name);//v1,v2 form old innerEdge
	  outf << "eOld is: " << eOld << endl;
          eWeight=eWeight + eOld.returnWeight(); //claculate inner weight
                 outf << "eWeight is: " << eWeight << endl;
         v1.delIncid(eOld.returnName()); // delete an incident edge
       }
          //out<<"v1="<<v1<<endl;
          eNew.setName(nextEdgeName);
          if(eWeight!=0)
          {
             eNew.setWeight(eWeight);
             coreWeight=coreWeight+eWeight;
          }
          else
          {
             eNew.setWeight(1);         // dummy inner edge
             coreWeight=coreWeight+1;
          }
          eNew.setEnd(0, v1Name);
          //eNew.setFlow1(min(eWeight,v1.returnWeight()-v1OutF));
          //v1.setEstFlow(eNew.returnFlow1()+v1OutF);
          v1.appendIncid(nextEdgeName);//form an inner edge for each frontier

          eNew.setEnd(1, nextVerName); //nextVerName is core's name 
          core.appendIncid(nextEdgeName); // form an incident edge for core

          innerE.append(eNew);       // get a new inner edge for cluster
          eNew.makeEmpty();

          nextEdgeName++;

         v1.setEstFlow(v1OutF);
         F.delVerByName(v1Name);
         F.appendVertex(v1);
    }  
    core.setName(nextVerName++);
    //core.setEstFlow(coreWeight);
    if(F.returnDimen()==3) { coreWeight = coreWeight + 6; }
    else  { coreWeight =coreWeight+3; }   //core(w)+frontiers(w)-allEdge(w)=DOF
    core.setWeight(coreWeight);

    core.setDepth(depth+1);
    new_cluster->formCl(core,frontiers,innerE,outerE,originalV);

    if(constrain==0)
      new_cluster->setConst(F.constrainDegree());
    else
      new_cluster->setConst(constrain);
    //out<<"new Cluster"<<endl;
    //new_cluster->output(out);
    //out<<"after getCluster F is"<<endl;
    //F.output(out);

    outf << "The final cluster is: " << endl;
    (* new_cluster).output(outf);
    outf<<"END GETCLUSTER"<<endl;
 
    return new_cluster;
}
