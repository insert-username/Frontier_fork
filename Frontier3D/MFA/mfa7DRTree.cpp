//file: mfa7DRTree.cpp
/*Methods for distribution including distribute-forest, 
distribute-cluster etc., including all the defrost methods
for incorporating  input groups */

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


//recursively defrosts and entire forest, used with defrostTree

void defrostForest(List<Cluster> & DR_Trees, Graph &F);

void defrostTree(Cluster &DR_Tree, Graph &F)
{
   int i, numKids;
   List<Cluster> kids;

   kids = DR_Tree.children;
   numKids = kids.returnLen();

   F.defrostOneVert(DR_Tree.returnCore().returnName());

   if(numKids==0) return;

   defrostForest(kids, F);
}

void defrostForest(List<Cluster> &DR_Trees, Graph &F)
{
   int i, len;
   Cluster oneTree;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      oneTree = DR_Trees.retrieve(i);
      defrostTree(oneTree, F);
   }
   return;
}


//pops the minimal depth cluster in DR_Trees and returns a pointer to it
Cluster &popMinDepth(List<Cluster> &DR_Trees)
{
   int i,pos,len,depth,coreName,depthMin=nextVerName,nameOfMin=nextVerName;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      depth=DR_Trees.retrieve(i).returnCore().returnDepth();
      coreName=DR_Trees.retrieve(i).returnCore().returnName();
      if(depth==depthMin)
      {
         if(coreName<nameOfMin)
         {
            nameOfMin=coreName;
            pos=i;
         }
      }
      else if(depth<depthMin)
      {
         depthMin=depth;
         nameOfMin=coreName;
         pos=i;
      }
   }
   return DR_Trees.getByIndex(pos);
}

//pops the maximum depth cluster in DR_Trees and returns a pointer to it
Cluster &popMaxDepth(List<Cluster> &DR_Trees)
{
   int i, pos, len, depth, depthMax=0;

   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      depth=DR_Trees.retrieve(i).returnCore().returnDepth();
      if(depth>=depthMax)
      {
         depthMax=depth;
         pos=i;
      }
   }
   return DR_Trees.getByIndex(pos);
}

/*Distribute all the clsuters in DR_Trees assuring that the groups set by the user and found in the output
DR_Dag.  Both distributeForest and distributeCluster work recursively together to perform this process
*/

List<Cluster> &distributeForest(List<Cluster> &DRtrees, Graph &F,
  Graph &graph0, ostream &file1,ostream &file2, int groupID, int indentation);

Cluster &distributeTree(Cluster &aDRtree, Graph &F,
  Graph &graph0, ostream &file1, ostream &file2, int indentation)
{
   file2<<"---------------start---------------"<<endl;
   file2<<"DistributeTree"<<endl; 
   file2<<"Contents of F"<<endl;
   F.output(file2);
   file2<<"Contents of aDRTree"<<endl;
   printTree(aDRtree, file2, 0);

   int i, numChild, groupID;
   List<Cluster> Children, distriedChild;
   Cluster *newDRtree = new Cluster (aDRtree);

   if(newDRtree->returnCore().returnDepth()==0&&newDRtree->returnGroup()==0)
   {
	file2<<"Returning from depth 0"<<endl;
	file2<<"----------------end----------------"<<endl;
	return *newDRtree;
   }

   Children=newDRtree->children;


   distriedChild=distributeForest(Children,F,graph0,file1,file2,
                  aDRtree.returnGroup(),indentation);


   numChild=distriedChild.returnLen();

   if(numChild==1)
   {
      groupID=aDRtree.returnGroup();
      *newDRtree=Children.retrieve(1);
      newDRtree->setGroup(groupID);
      file1<<"Group="<<groupID<<endl;
      if(groupID!=0)
      {
         F.defrostGraph();
         graph0.defrostGraph();
      }
   }
   else
   {
      newDRtree->children=distriedChild;
   }

   file2<<"Returning from end"<<endl;
   file2<<"Ending value of newDRTree"<<endl;
   printTree(*newDRtree, file2, 0);
   file2<<"----------------end----------------"<<endl;
   return *newDRtree;
}

void explicitChild(Cluster &newTree, List<int> &nameList,List<Cluster> &forest)
{
   int i, j, numOfTree, childName, numOriginalV;
   Cluster aChild;
   Vertex childCore;
   List<int> childOrig;
   List<Cluster> tempTrees;

   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      childCore=aChild.returnCore();
      childName=childCore.returnName();
      if(nameList.hasElem(childName))
      {
          nameList.deleteElem(childName);
          newTree.deleteOrig(childName);
          childOrig=aChild.returnOrig();
          numOriginalV=childOrig.returnLen();
          for(j=1;j<=numOriginalV;j++)
          {
             newTree.addOrig(childOrig.retrieve(j)); //change original vertices
             nameList.deleteElem(childOrig.retrieve(j));
          }
          newTree.children.append(aChild); //add a child for new cluster
      }
      else
         tempTrees.append(aChild);
   }
   forest=tempTrees;
}


void implicitChild(Cluster &newTree, List<int> &nameList,
                      List<Cluster> &forest, Graph &F, Graph &graph0)
{
   int i, j, numOfTree, numOriginalV, originalVert,  numInner;
   Vertex childCore, parentCore;
   Edge innerEdge;
   Cluster aChild, *temp;
   List<int> tempOrig, childOrig;
   List<Cluster> tempTrees;

   numOfTree=forest.returnLen();
   for(i=1;i<=numOfTree;i++)
   {
      aChild=popMaxDepth(forest);
      if(aChild<=newTree)
      {
          childCore=aChild.returnCore();
          childCore.setLabel(1);
          F.appendVertex(childCore);
          numInner=aChild.returnInnerE().returnLen();
          for(j=1;j<=numInner;j++)
          {
             innerEdge=aChild.returnInnerE().retrieve(j);
             innerEdge.setLabel(1);
             F.appendEdge(innerEdge);
          }
          parentCore=newTree.returnCore();
          parentCore.setLabel(1);
          F.appendVertex(parentCore);
          numInner=newTree.returnInnerE().returnLen();
          for(j=1;j<=numInner;j++)
          {
             innerEdge=newTree.returnInnerE().retrieve(j);
             innerEdge.setLabel(1);
             F.appendEdge(innerEdge);
          }
          temp=getCluster(F, graph0);
          tempOrig=newTree.returnOrig();
          
          newTree.formCl(temp->returnCore(), temp->returnFrontiers(),
                 temp->returnInnerE(), temp->returnOuterE(), tempOrig);
          childOrig=aChild.returnOrig();
          numOriginalV=childOrig.returnLen();
          for(j=1;j<=numOriginalV;j++)
          {
             originalVert=childOrig.retrieve(j);
             nameList.deleteElem(originalVert);
             if(!tempOrig.hasElem(originalVert))
                newTree.addOrig(originalVert);
          }
          newTree.children.append(aChild); //add a child for new cluster
      }
      else
         tempTrees.append(aChild);
   }
   forest=tempTrees;
}


void addChild(Cluster &newTree, List<Cluster> &popedTrees,
               List<Cluster> &DR_Trees, Graph &F, Graph &graph0)
{
   int i, j, listLen, childName;
   List<int> nameList, originalV0, frontiers0;
   Cluster aChild;
   Vertex childCore;
   List<Edge> innerE, outerE;

   //out<<"add child for"<<endl;
   //printTree(newTree,out,0);

   nameList=newTree.returnOrig(); //vertex name list forms a new cluster

   //check each item (not poped) left in DR_Trees with the name list
   explicitChild(newTree, nameList, DR_Trees);

   // check each poped item with the name list
   explicitChild(newTree, nameList, popedTrees);

   // look implicit child in DR_Trees
   implicitChild(newTree, nameList, DR_Trees, F, graph0);

   // look implicit child in popedTrees
   implicitChild(newTree, nameList, popedTrees, F, graph0);

   //The left names in the list are frontier vertices shared by two or more
   //new clusters. We have to create one or more clusters for each of them.
   listLen=nameList.returnLen();
   for(i=1;i<=listLen;i++)
   {
       childName=nameList.retrieve(i);
       childCore=F.returnVertByName(childName);
       frontiers0.append(childName);
       originalV0.append(childName);
       aChild.formCl(childCore,frontiers0,innerE,outerE,originalV0);
       newTree.children.append(aChild); // add a single vertex child
   }
   return;
}


int checkClusterForBananas(Graph &graph0, Cluster &theCluster)
{
	if(theCluster.children.returnLen()!=2)
		return -1;

	int i, length=graph0.returnNumVer();

	List<int> overlappedVerts;

	cout<<"Getting list of overlapping vertices: ";

	for(i=1;i<=length;i++)
	{
		List<int> tempList;
		int vName;
			
		vName=graph0.returnVertByIndex(i).returnName();
		getContainedChildList(theCluster, vName, tempList);
		if(tempList.returnLen()==2)
		{
			overlappedVerts.append(vName);
			cout<<vName<<" ";
		}
	}

	cout<<endl;

	if(overlappedVerts.returnLen()<2)
		return -1;

	bool edgeBetween=false;
        length=graph0.returnNumEdg();
        for(i=1;i<=length;i++)
                edgeBetween=(getEdgeCode(graph0.returnEdgeByIndex(i),theCluster)==1 &&
				graph0.returnEdgeByIndex(i).returnType()!=1) || edgeBetween;
  
	bool partitionable;
	List<int> over1, over2;
	over1.append(overlappedVerts.retrieve(1));
	overlappedVerts.deleteIndex(1);

	for(i=1;i<=overlappedVerts.returnLen();i++)
		for(int j=1;j<=over1.returnLen();j++)
			if(graph0.returnEdgeByEnds(overlappedVerts.retrieve(i),over1.retrieve(j)).returnType()==1)
				if(!over1.hasElem(overlappedVerts.retrieve(i)))
					over1.append(overlappedVerts.retrieve(i));

	for(i=1;i<=over1.returnLen();i++)
		if(overlappedVerts.hasElem(over1.retrieve(i))) overlappedVerts.deleteElem(over1.retrieve(i));

	if(overlappedVerts.returnLen()==1)
		partitionable=true;
	else
	{
		if(overlappedVerts.returnLen()==0)
			partitionable=false;
		else
		{
			over2.append(overlappedVerts.retrieve(1));
			overlappedVerts.deleteIndex(1);

			for(i=1;i<=overlappedVerts.returnLen();i++)
		                for(int j=1;j<=over2.returnLen();j++)
					if(graph0.returnEdgeByEnds(overlappedVerts.retrieve(i),over2.retrieve(j)).returnType()==1)
                                		if(!over2.hasElem(overlappedVerts.retrieve(i)))
		                               		over2.append(overlappedVerts.retrieve(i));	

			for(i=1;i<=over2.returnLen();i++)
		                if(overlappedVerts.hasElem(over2.retrieve(i))) overlappedVerts.deleteElem(over2.retrieve(i));
	
			if(overlappedVerts.returnLen()!=0)
				partitionable==false;
			else    partitionable==true;
			
		}
	}

	if(partitionable && !edgeBetween)
		return 0;

	if(partitionable && edgeBetween)
		return 3;

/* Add Code here to check for a distance constraint between the two partitionable groups */

	return -4;
}


int getClusterType(Graph &graph0, Cluster &theCluster)
{
    int LSCount, distCount, pointCount, totalCount, i;
    
    totalCount=LSCount=distCount=pointCount=0;
    for(i=1;i<=theCluster.returnOrigLen();i++)
    {
       totalCount++;
       int temp=graph0.returnVertByName(theCluster.returnOrigV(i)).returnType();
       switch(temp)
       {
                case 6:         pointCount++;
                                break;
                case 7:         LSCount++;
                                break;
       }
    }

    int length=graph0.returnNumEdg();
    Edge tempEdge;
    int	notIncidCount=0;
    int totalEdgeCount=0;

    for(i=1;i<=length;i++)
    {
	tempEdge=graph0.returnEdgeByIndex(i);
	cout<<"Edge Name: "<<tempEdge.returnName()<<endl;
	cout<<"Edge Code: "<<getEdgeCode(tempEdge,theCluster)<<endl;
	cout<<"Edge Type: "<<tempEdge.returnType()<<endl;
	if(getEdgeCode(tempEdge,theCluster)==0 && tempEdge.returnType()==6)
		distCount++;
	if(getEdgeCode(tempEdge,theCluster)>=0 && tempEdge.returnType()!=1)
		notIncidCount++;
    }

    int childNotExtOver=0;

    for(i=1;i<=theCluster.children.returnLen();i++)
    {
	if(theCluster.children.retrieve(i).children.returnLen()==0)
		continue;
	if(theCluster.children.retrieve(i).returnType()!=2)
		childNotExtOver++;
    }
               
    cout<<"Getting Cluster "<<theCluster.returnName()<<" Type: "<<endl;
    cout<<"LSCount:     "<<LSCount<<endl;
    cout<<"distCount:   "<<distCount<<endl;
    cout<<"pointCount:  "<<pointCount<<endl;
    cout<<"totalCount:  "<<totalCount<<endl;

    if(notIncidCount==0 && childNotExtOver==0)
    {
        cout<<"Type set to 2 (2)"<<endl;
        return 2;
    }	

    if((distCount==1 && pointCount==2 && totalCount==2) ||
       (LSCount>0 && distCount==1))	
    {
	cout<<"Type set to 2 (1)"<<endl;
        return 2;
    }
 
    if(LSCount==1 && distCount==0)
    {
	cout<<"Type set to 1"<<endl;
        return 1;
    }

    cout<<"Type set to 0"<<endl;
    return 0;
}

void setAlias(Graph &graph0, Cluster &theCluster)
{
   if(theCluster.returnType()!=2) return;

   int i, length=theCluster.returnOrigLen();

   int alias=-1;

   for(i=1;i<=length;i++)
        if(graph0.returnVertByName(theCluster.returnOrigV(i)).returnType()==7)
	{
		alias=theCluster.currAlias=theCluster.returnOrigV(i);
		cout<<"Alias found: "<<alias<<endl;
		break;
	}

   if(alias==-1)
   {
	cout<<"Error: No Alias found."<<endl;
	return;
   }

   Edge currEdge;
   int currVertID, currVertType, aliasPart;

   for(i=1;i<=length;i++)
   {
	currVertID=theCluster.returnOrigV(i);
        currVertType=graph0.returnVertByName(currVertID).returnType();

        if(currVertType==7)
        {
                theCluster.lines.append(currVertID);
                cout<<"Line appended: "<<currVertID<<endl;
                continue;
        }
        
        if(currVertType!=6) continue;

	currEdge=graph0.returnEdgeByEnds(alias,currVertID);
	if(currEdge.returnName()==0) continue;
	if(currEdge.returnType()!=1) continue;

	cout<<"Checking Edge: "<<currEdge<<endl;

	if(currEdge.returnEnd1()==alias)
		aliasPart=currEdge.returnPart1();
	else
		aliasPart=currEdge.returnPart2();

	if(aliasPart==1)
	{
		theCluster.left.append(currVertID);
		cout<<"Left appended: "<<currVertID<<endl;
	}

	if(aliasPart==2)
	{
		theCluster.right.append(currVertID);
		cout<<"Right appended: "<<currVertID<<endl;
	}
   }
}


List<Cluster> &distributeForest(List<Cluster> &DR_Trees, Graph &F,
  Graph &graph0, ostream &file1, ostream &file2, int groupID, int indentation)
{
   int i, j, len;
   Cluster aTree, distried, newTree;
   List<Cluster> popedTrees;

   file2<<"---------------start---------------"<<endl;
   file2<<"DistributeForest"<<endl;
   file2<<"Contents of F"<<endl;
   F.output(file2);   
   file2<<"Contents of DRTrees"<<endl;
   printForest(DR_Trees, file2, 0);

   indentation++;
   
   len=DR_Trees.returnLen();
   for(i=1;i<=len;i++)
   {
      aTree = DR_Trees.pop();
      distried=distributeTree(aTree, F, graph0, file1,file2, indentation);
      if(distried.empty())
         delete &distried;
      else
         DR_Trees.append(distried);
   }
   
   if(len==1) {
      return DR_Trees;
   }

   if(groupID!=0)
   {
       F.freeze();
       graph0.freeze();
       defrostForest(DR_Trees, F);
       defrostForest(DR_Trees, graph0);
   }

   while(DR_Trees.returnLen()>0)
   {
      file1<<"---------------------------------------------"<<endl;
      if(extension(F, graph0, file1, file2))
      {
         file2<<"Found new Cluster by Extension:"<<endl;
         newTree=*getCluster(F, graph0);
         newTree.output(file2);
         file2<<"----------------end----------------"<<endl;
      }
      else
      {	
         aTree=popMinDepth(DR_Trees);  // item is deleted in DR_Trees
         popedTrees.append(aTree);
         file1<<"Pop Cluster, "<<aTree.returnCore().returnName();
         file1<<"  Group="<<aTree.returnGroup()<<endl;
         file2<<"Pop Cluster, "<<aTree.returnCore().returnName()<<endl;
         newTree=distributeCl(aTree, F, graph0, file1, file2);
      }

      if(!newTree.empty()) {

	  List<Cluster> tempDR_Trees;
	  if(graph0.returnDimen()==3)
	  {
	  	Graph tempGraph;
	  	for(int i=1;i<=DR_Trees.returnLen();i++)
			tempDR_Trees.append(DR_Trees.retrieve(i));
	  	copyG(F,tempGraph);
	  }
	  file2<<"Before addChild "<<endl;
          newTree.output(file2);
          file2<<"Found new Cluster: "<<endl;
	  cout<<"IN DISTRIBUTE FOREST"<<endl;	
          addChild(newTree, popedTrees, DR_Trees, F, graph0);
	  cout<<"AFTER ADDCHILD"<<endl;
          //constrain=F.constrainDegree();
          //file2<<"constrain="<<constrain<<endl;
          //newTree.setConst(constrain); //1--well constrained, 2,3...--over
          file2<<"In DistForest right before cluster output"<<endl;
          newTree.output(file2);
          file2<<"Starting Contents of F:"<<endl;
          file2<<"Right before deleting inner edges"<<endl;
	  F.output(file2);
          delInnerVer(F, graph0);
          file2<<"deleting inner edges"<<endl;
          F.output(file2);
          DR_Trees.append(newTree);
	  cout<<"AFTER APPEND"<<endl;
	  cout<<"Checking new cluster"<<endl;

	  if(graph0.returnDimen()==3)
	  {

	  switch(checkClusterForBananas(graph0, DR_Trees.retrieve(DR_Trees.returnLen())))
	  {
		case -1:	break;
		case 0:		DR_Trees.makeEmpty();
				for(int i=1;i<=tempDR_Trees.returnLen();i++)
			                DR_Trees.append(tempDR_Trees.retrieve(i));
				continue;
		case 3:		DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(3);
				cout<<"TYPE SET TO 3"<<endl;
				break;
		default:	cout<<"ERROR UNEXPECTED RETURN!"<<endl;
				break;
	  }
	  int cType=getClusterType(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
	  DR_Trees.addrByIndex(DR_Trees.returnLen())->setType(cType);
	  if(cType==2)
	  {
		DR_Trees.addrByIndex(DR_Trees.returnLen())->returnCore().incrsWt(-1);
		setAlias(graph0, DR_Trees.retrieve(DR_Trees.returnLen()));
		Cluster theCluster = DR_Trees.retrieve(DR_Trees.returnLen());
		cout<<"After aliasing for cluster: "<<theCluster.returnName()<<endl;
		cout<<"Alias: "<<theCluster.currAlias<<endl;
		cout<<"Left: ";
		for(int q=1; q<=theCluster.left.returnLen(); q++)
			cout<<theCluster.left.retrieve(q)<<" ";
		cout<<endl;
		cout<<"Right: ";
                for(int q=1; q<=theCluster.right.returnLen(); q++)
                        cout<<theCluster.right.retrieve(q)<<" ";
                cout<<endl;
		cout<<"Lines: ";
                for(int q=1; q<=theCluster.lines.returnLen(); q++)
                        cout<<theCluster.lines.retrieve(q)<<" ";
                cout<<endl;
	  }

	  }
          file1<<endl<<"Get new Cluster"<<endl;
          newTree.output(file1);
          //out<<"Get new Cluster"<<endl;
          //newTree.output(out);
          if(DR_Trees.returnLen()==1&&popedTrees.returnLen()==0)
          {
              file2<<"returning DR_Trees"<<endl;
              file2<<"----------------end----------------"<<endl;
              return DR_Trees;
          }
      }
   }

   DR_Trees=popedTrees; // the DR_Trees can form a cluster (not dense)
   file2<<"returning DR_Trees"<<endl;
   file2<<"----------------end----------------"<<endl;
   return DR_Trees;
}

// get trivial clusters from the single left vertices, after main distribution
void getTrivial(List<Cluster> &DRTrees, Graph &graph0)
{
   int i, j, numDRTree,numTempTree, vName1, vName2, gotOneCluster;
   int Eweight, Vweight=0;
   Cluster aTree1, aTree2;
   List<Cluster> tempTrees;
   Vertex v1, v2;
   Edge edge;

   numDRTree=DRTrees.returnLen();
   if(numDRTree<=1) return;
   for(i=1; i<=numDRTree; i++)
   {
      aTree1=DRTrees.pop();
      vName1=aTree1.returnCore().returnName();
      if(vName1>0&&vName1<=singleVertex)
            tempTrees.append(aTree1);
      else if(vName1>singleVertex)
            DRTrees.append(aTree1);
   }
   
   numTempTree=tempTrees.returnLen();
   for(i=1; i<=numTempTree; i++)
   { 
      gotOneCluster=0;
      aTree1=tempTrees.pop();
      v1=aTree1.returnCore();
      vName1=v1.returnName();
      for(j=1; j<numTempTree; j++)
      {
         aTree2=tempTrees.pop();
         v2=aTree2.returnCore();
         vName2=v2.returnName();
         edge=graph0.returnEdgeByEnds(vName1, vName2);
         Eweight=edge.returnWeight();
         Vweight=v1.returnWeight() + v1.returnWeight();
         if(Eweight>0 && Eweight - Vweight >= -3)//well,over
         {
            gotOneCluster=1;
            break;
         }
         else if(vName2>0)
            tempTrees.append(aTree2);
      }

      if(gotOneCluster==1)
      {
          Vertex *core = new Vertex;
          Cluster *newTree = new Cluster;

          core->setName(nextVerName++);
          core->setWeight(Vweight - Eweight);

          newTree->setCore(*core);
          newTree->addFrot(vName1);
          newTree->addFrot(vName2);
          newTree->addOrig(vName1);
          newTree->addOrig(vName2);
          //newTree->setConst(4 - Vweight + Eweight);
          newTree->children.append(aTree1);
          newTree->children.append(aTree2);
          tempTrees.append(*newTree);
      }
      else if(vName1>0)
         tempTrees.append(aTree1);
   }

   numTempTree=tempTrees.returnLen();
   for(i=1; i<=numTempTree; i++)
       DRTrees.append(tempTrees.pop());
}
