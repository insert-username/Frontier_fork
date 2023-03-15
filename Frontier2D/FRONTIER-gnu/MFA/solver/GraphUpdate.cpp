#include "solver/GraphUpdate.h"
#include "solver/SolverUtil.h"

#include "entities/Vertex.h"
#include "entities/Cluster.h"

//rotates the object stored in v0, and rotates using the rotation matrix stored in theCluster
void rotate(Cluster &theCluster, Vertex &v0)
{
    int type;
    float x, y, m, x2, y2, x2New, y2New, xNew, yNew, mNew;
    float sin, cos, p, q;


    type=v0.returnType();

    switch(type)
    {
        case 0:
        case 4:   x=v0.returnDegValueByName(0);
            y=v0.returnDegValueByName(1);
            p=theCluster.returnValue(0);
            q=theCluster.returnValue(1);
            cos=theCluster.returnValue(2);
            sin=theCluster.returnValue(3);
            xNew=(x*cos)-(y*sin)+p;
            yNew=(y*cos)+(x*sin)+q;
            v0.setValues(0,xNew);
            v0.setValues(1,yNew);
            break;
        case 1:   x=v0.returnDegValueByName(0);
            y=v0.returnDegValueByName(1);
            m=v0.returnDegValueByName(4);
            x2=x+1.0;
            y2=y+m;
            p=theCluster.returnValue(0);
            q=theCluster.returnValue(1);
            cos=theCluster.returnValue(2);
            sin=theCluster.returnValue(3);
            xNew=(x*cos)-(y*sin)+p;
            yNew=(y*cos)+(x*sin)+q;
            x2New=(x2*cos)+(y2*sin)+p;
            y2New=(y2*cos)-(x2*sin)+q;
            mNew=(y2New-yNew)/(x2New-xNew);
            v0.setValues(0,xNew);
            v0.setValues(1,yNew);
            v0.setValues(4,mNew);
            break;
    }
}

//recursively determines the position of any object in the given cluster,
//by rotating it through all the past rotations up from the children
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex, int location)
{
    int i;
    int childIndex;

    if(theCluster.returnName()==theVertex.returnName())
    {
        theVertex=graph0.returnVertByName(theVertex.returnName());
        for(i=0;i<6;i++)
            theVertex.setValues(i,theCluster.returnValue(i));

    }
    else
    {
        childIndex=getChildIndexByName(theCluster, getChildNameWithVertex(theCluster, theVertex.returnName()));
        theCluster.children.retrieve(childIndex).setToCluster(theCluster);
        setValueInCluster(graph0, theCluster.children.retrieve(childIndex), theVertex, location+1);
        if(location!=1) rotate(theCluster, theVertex);
    }
}

//the setValueInCluster method is interpreted to set the value given vertex in the currnet cluster
//this second method prevents the recursive method from rotating the vertex through the cluster
//in which its called
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex)
{
    setValueInCluster(graph0, theCluster, theVertex, 1);
}


//if the cluster is singleton, sets the values in the corresponding vertex in graph0
//to the values stored in theCluster, otherwise recursively calls itself on every one
//of its children with populateForest
void populateCluster(Graph &graph0, Cluster &theCluster)
{
    int length=theCluster.children.returnLen();
    int name;
    int i;

    if(theCluster.returnOrig().returnLen()==1)
    {
        name=theCluster.returnName();
        for(i=0;i<6;i++)
            setValue(graph0.returnVertByName(name),i,theCluster.returnValue(i));
    }
    if(length>0) populateForest(graph0, theCluster.children);
}

//calls populate cluster for every cluster in SolverTrees
void populateForest(Graph &graph0, List<Cluster> &SolverTrees)
{
    int i, numTrees;

    numTrees=SolverTrees.returnLen();
    for(i=1; i<=numTrees; i++)
        populateCluster(graph0, SolverTrees.retrieve(i));
}

//begins the recursion completed in populateForest and PopulateCluster
void populateGraph(Graph &graph0, List<Cluster> &SolverTrees)
{
    populateForest(graph0, SolverTrees);
}