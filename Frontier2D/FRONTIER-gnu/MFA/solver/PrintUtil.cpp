#include "solver/PrintUtil.h"
#include "solver/SolverUtil.h"

#include "entities/Graph.h"
#include "entities/Cluster.h"

#include "output/DRPrintUtils.h"

//prints the a cluster and all its children starting with a single cluster
void print(Graph &graph0, Cluster &theCluster)
{
    theCluster.output(std::cout);

    int i, length;

    length=theCluster.children.returnLen();

    if(theCluster.returnName()==0) return;

    for(i=1;i<=length;i++)
        print(graph0, theCluster.children.retrieve(i));
}

//for every cluster in SolverTrees, this method prints contents of the cluster and all its children
void print(Graph &graph0, List<Cluster> &SolverTrees)
{
    int i, numTrees;
    numTrees=SolverTrees.returnLen();

    std::cout<<std::endl<<numTrees<<" Solver Input Trees"<<std::endl;
    for(i=1;i<=numTrees;i++)
    {
        std::cout<<"---------"<<std::endl;
        std::cout<<"Tree "<<i<<std::endl;
        DRPrintUtils::printTree(SolverTrees.retrieve(i), std::cout, 1);
    }
}

//prints the string corresponding to the bifurcations of theCluster
void printBifurcations(Cluster &theCluster)
{
    int i, length;
    List<std::string> theList;

    theList=theCluster.returnBifurs();

    length=theCluster.returnNumBifurs();

    for(i=1;i<=length;i++)
        std::cout<<"       "<<theList.retrieve(i)<<std::endl;
}


//outputs the fin state of every cluster.  Fin flags are used to tell the solver which
//clusters have and have not been solved.  This method visits each node in the tree, and
//at each visit outputs the clusters name, then fin start 0 for solved, 1 for not solved,
//before recursively continuing for each of its children
void outputFinState(Cluster &theCluster, int &startI, int*& theInts, int &count)
{
    int i, length;

    count++;
    theInts[startI++]=theCluster.returnName();
    if(theCluster.isSolved()) theInts[startI++]=0;
    else theInts[startI++]=1;

    length=theCluster.children.returnLen();
    for(i=1;i<=length;i++)
        outputFinState(theCluster.children.retrieve(i), startI, theInts, count);
}


//THIS METHOD HAS BEEN OUTDATED BY THE USE OF THE JAVA NATIVE INTERFACE
//prints the graph data into a text file in a format that can be read by the sketcher
//when printNum is true the ID of theCluster is printed first
void generateOutput(Graph &graph0, Cluster &theCluster, std::ostream &outfile, bool printNum)
{
    int i, length;
    Vertex currVert;
    float slope, angle1, angle2;
    float x,y,tempValue, tempValue2;

    if(printNum) outfile<<theCluster.returnOrigLen()<<std::endl;
    length=graph0.returnNumVer();
    for(i=1;i<=length;i++)
    {

        currVert=graph0.returnVertByIndex(i);
        if(inOriginalV(currVert.returnName(),theCluster))
        {
            outfile<<currVert.returnName()<<std::endl;
            x=currVert.returnDegValueByName(0);
            y=currVert.returnDegValueByName(1);
            outfile<<x<<std::endl;
            outfile<<y<<std::endl;
            switch(currVert.returnType())
            {
                case 0:   break;
                case 1:   tempValue=currVert.returnDegValueByName(4);
                    outfile<<x+1<<std::endl;
                    outfile<<y+tempValue<<std::endl;
                    break;
                case 2:   tempValue=currVert.returnDegValueByName(4);
                    tempValue2=currVert.returnDegValueByName(5);
                    outfile<<x+tempValue<<std::endl;
                    outfile<<y+tempValue2<<std::endl;
                    break;
                case 3:   outfile<<currVert.returnDegValueByName(2)<<std::endl;
                    outfile<<currVert.returnDegValueByName(3)<<std::endl;
                    break;
                case 4:   tempValue=currVert.returnDegValueByName(2);
                    if(tempValue<0) tempValue=-tempValue;
                    outfile<<tempValue<<std::endl;
                    break;
                case 5:   angle1=getAngle(currVert.returnDegValueByName(4),currVert.returnDegValueByName(5));
                    angle1=getAngle(currVert.returnDegValueByName(6),currVert.returnDegValueByName(7));
                    outfile<<currVert.returnDegValueByName(2)<<std::endl;
                    outfile<<currVert.returnDegValueByName(3)<<std::endl;
                    outfile<<angle1<<std::endl;
                    outfile<<angle2<<std::endl;
                    break;
            }
        }
    }
}


//NO LONGER USED SINCE THE IMPLEMENTATION OF THE JAVA NATIVE INTERFACE
//outputs the DRDAG to a file
void outputDRDAG(List<Cluster> &theCluster, std::ostream &output, bool first)
{
    int i, j, length, lengthOrig, lengthChild;

    length=theCluster.returnLen();

    if(first)
    {
        output<<-2<<std::endl;
        output<<length<<std::endl;
    }

    for(int i=1;i<=length;i++)
    {
        output<<theCluster.retrieve(i).returnName()<<std::endl;
        lengthOrig=theCluster.retrieve(i).returnOrigLen();
        output<<lengthOrig<<std::endl;
        for(j=1;j<=lengthOrig;j++)
            output<<theCluster.retrieve(i).returnOrigV(j)<<std::endl;
        lengthChild=theCluster.retrieve(i).children.returnLen();
        output<<lengthChild<<std::endl;
        for(j=1;j<=lengthChild;j++)
            output<<theCluster.retrieve(i).children.retrieve(j).returnName()<<std::endl;
        outputDRDAG(theCluster.retrieve(i).children,output,false);
    }
    if(first) output<<-1<<std::endl;
}

/*  Ouputs the DRDag to an array, the following format is used:

	-2			-- signal to sketcher that the DRDag is coming
	<number of trees in the DAG>

	then recursively, each node before its children the following is output:

      <cluster ID>
      <number of original vertices>
      <list of original vertices>
      <number of children>
      <list of children>

      (Note that all values are always printed even if they are 0);
*/
void outputDRDAGToArray(List<Cluster> &theCluster, int &index, int*& theInts, bool first)
{
    int i, j, length, lengthOrig, lengthChild;

    length=theCluster.returnLen();

    if(first)
    {
        theInts[index++]=-2;
        theInts[index++]=length;
    }

    for(int i=1;i<=length;i++)
    {
        theInts[index++]=theCluster.retrieve(i).returnName();
        lengthOrig=theCluster.retrieve(i).returnOrigLen();
        theInts[index++]=lengthOrig;
        for(j=1;j<=lengthOrig;j++)
            theInts[index++]=theCluster.retrieve(i).returnOrigV(j);
        lengthChild=theCluster.retrieve(i).children.returnLen();
        theInts[index++]=lengthChild;
        for(j=1;j<=lengthChild;j++)
            theInts[index++]=theCluster.retrieve(i).children.retrieve(j).returnName();
        outputDRDAGToArray(theCluster.retrieve(i).children, index, theInts, false);
    }
    if(first) theInts[index]=-1;
}
