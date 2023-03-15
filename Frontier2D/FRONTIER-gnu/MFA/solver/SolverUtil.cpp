#include "solver/SolverUtil.h"

#include "entities/List.h"
#include "entities/Cluster.h"

//returns the ID of the first immediate child of theCluster to contain the given original vertex ID
int getChildNameWithVertex(Cluster &theCluster, int theV)
{

    int length, i;
    List<int> theOrig;

    length=theCluster.children.returnLen();

    for(i=1;i<=length;i++)
    {
        if(inOriginalV(theV,theCluster.children.retrieve(i)))
        {
            return theCluster.children.retrieve(i).returnName();
        }
    }
    return -1;

}

//returns true if Name is a clild of theCluster
bool inOriginalV(int Name, Cluster &theCluster)
{
    List<int> theOrig;

    theOrig=theCluster.returnOrig();

    if (theOrig.hasElem(Name)) return true;

    return false;
}


//returns the distance between (x1,y1) and (x2,y2)
float getDistance(float x1, float y1, float x2, float y2)
{
    return(sqrt(pow((x1-x2),2)+pow((y1-y2),2)));
}

//returns the angle from (x1,y1) to (x2,y2)
float getAngle(float x1, float y1, float x2, float y2)
{
    float cos1, sin1, PI=3.141592;

    sin1=(y2-y1)/getDistance(x1,y1,x2,y2);
    cos1=(x2-x1)/getDistance(x1,y1,x2,y2);

    if((sin1>=0 && cos1>=0) || (sin1>=0 && cos1<0)) return acos(cos1);
    if(sin1<0 && cos1<=0) return 2*PI-acos(cos1);
    return 2*PI+asin(sin1);
}

//returns the angle that correspones to a given sine and cosine value
float getAngle(float theCos, float theSin)
{
    float PI=3.141592;

    if((theSin>=0 && theCos>=0) || (theSin>=0 && theCos<0)) return acos(theCos);
    if(theSin<0 && theCos<=0) return 2*PI-acos(theCos);
    return 2*PI+asin(theSin);
}


//returns in theList, the names of all the children in theCluster that contain
//vName as an original Vertex
void getContainedChildList(Cluster &theCluster, int vName, List<int> &theList)
{
    List<int> output;
    int i, length;

    length=theCluster.children.returnLen();
    for(i=1; i<=length; i++)
        if(inOriginalV(vName, theCluster.children.retrieve(i)))
            theList.append(theCluster.children.retrieve(i).returnName());
}


//within the child of theCluster named ID, sets the indexth solved value to setValue
void setValue(Cluster &theCluster, int ID, float theValue, int index)
{
    int i=1;

    while(theCluster.children.retrieve(i).returnName()!=ID) i++;

    theCluster.children.retrieve(i).setValue(index, theValue);

}


//sets the longest distance constraint in the cluster to .000001 less than before, this tricks
//maple to returning some numerical solutions that it otherwise would not with exact values
void setValueReduction(Graph &graph0, Cluster &theCluster)
{
    int i, length=graph0.returnNumEdg();
    int end1, end2, type;
    Edge* tempEdge;
    int longestDistIndex=-1;
    float longestDist=-1.0, value;
    bool in1, in2;

    for(i=1;i<=length;i++)
    {
        if(!graph0.hasEdge(i)) continue;
        tempEdge=graph0.EdgeAddr(i);
        end1=tempEdge->returnEnd1();
        end2=tempEdge->returnEnd2();
        type=tempEdge->returnType();
        value=tempEdge->returnValue();
        in1=inOriginalV(end1, theCluster);
        in2=inOriginalV(end2, theCluster);
        std::cout<<"end1: "<<end1<<" end2: "<<end2<<" type: "<<type<<" in1: "
                 <<in1<<" in2: "<<in2<<" value: "<<value<<std::endl;
        if(in1 && in2 && type==0 && value>longestDist)
        {
            longestDistIndex=i;
            longestDist=value;
        }
    }
    std::cout<<"longestIndex: "<<longestDistIndex<<std::endl;
    if(longestDistIndex>0)
    {
        tempEdge=graph0.EdgeAddr(longestDistIndex);
        tempEdge->setValue(longestDist-0.000001);
    }
}


//resets the fin state of all of the cluster in the DRTree above theCluster
void resetFinByClusterName(Cluster &theCluster, int name)
{
    int i, length;

    if(inOriginalV(name, theCluster))
    {
        std::cout<<"Reset Cluster: "<<theCluster.returnName()<<std::endl;
        theCluster.setSolved(false);
    }
    else std::cout<<"In Cluster: "<<theCluster.returnName()<<std::endl;

    length=theCluster.children.returnLen();

    for(i=1;i<=length;i++)
        resetFinByClusterName(theCluster.children.retrieve(i), name);
}


//resets the DRTrees datastructure to contain only the original vertices in the graph
//this method is used right before calling the DRPlanner to force it to recalculate the
//entire tree
void resetTrees(GlobalState &globalState, Graph &graph0, List<Cluster> &SolverTrees)
{
    int i, length;
    Cluster newCluster;
    List<Edge> innerE, outerE;
    List<int> frontiers, originalV;
    Vertex core;
    int coreName;

    SolverTrees.makeEmpty();

    length=graph0.returnNumVer();

    for(i=1;i<=length;i++)
    {
        core=graph0.returnVertByIndex(i);
        coreName=core.returnName();
        core.defrost();
        graph0.defrostOneVert(coreName);
        frontiers.append(coreName);
        originalV.append(coreName);
        newCluster.formCl(core, frontiers, innerE, outerE, originalV);
        SolverTrees.append(newCluster);
        frontiers.makeEmpty();
        originalV.makeEmpty();
    }

    globalState.setSingleVertex(length+4);
}


//eliminates extra vertex in frontiers latent after DRPlanner
void fixTree(List<Cluster> &SolverTrees)
{
    int i, length;

    length=SolverTrees.returnLen();

    for(i=1;i<=length;i++)
        fixCluster(SolverTrees.retrieve(i));
}

//as fixTrees but for clusters
void fixCluster(Cluster &theCluster)
{
    Vertex theCore;
    int i, length, coreName;

    theCore=theCluster.returnCore();
    coreName=theCore.returnName();

    if(coreName==0) return;

    if(theCore.returnDepth()==0)
    {
        length=theCluster.returnOrigLen();
        for(i=1;i<=length;i++)
            if(theCluster.returnOrigV(i)!=coreName)
                theCluster.deleteOrig(theCluster.returnOrigV(i));
    }
    else fixTree(theCluster.children);
}


//for the given cluster, returns the index of its first child with the name vName,
//it no such child exists returns -1
int getChildIndexByName(Cluster &theCluster, int vName)
{
    int i, length;

    length=theCluster.children.returnLen();
    for(i=1;i<=length;i++)
        if(theCluster.children.retrieve(i).returnName()==vName) return i;

    return -1;
}


//assigns a given value to a freedeg in Vertex
void setValue(Vertex &theVertex, int index, float theValue)
{
    theVertex.setValues(index, theValue);
}
