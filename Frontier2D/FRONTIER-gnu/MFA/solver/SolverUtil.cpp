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