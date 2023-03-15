#include "solver/ArrayUtil.h"
#include "solver/constants.h"

#include "entities/Vertex.h"
#include "entities/Edge.h"
#include "entities/Graph.h"
#include "entities/Cluster.h"
#include <string>

//stores a string in the output arrays by storing the ascii value of each of its chars in
//consequtive integers
void stringToArray(std::string theString, int*& theInts)
{
    int indexI=theInts[0];
    int i, length=theString.size();

    for(i=0;i<length;i++) theInts[indexI++]=theString[i];
    theInts[indexI++]=-1;
    theInts[0]=indexI;
}

//reads a string in the above format
std::string readStringFromArray(int& start, int* theInts)
{
    std::string output="";

    while(theInts[start]!=-1)
        output+=(char) theInts[start++];

    start++;
    return output;
}

//stores a vertex in the output arrays
void vertexToArray(Vertex &theVertex, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    int i, nIncid;

    theInts[indexI++]=theVertex.returnName();
    theInts[indexI++]=theVertex.returnType();
    theInts[indexI++]=theVertex.returnWeight();
    theInts[indexI++]=theVertex.returnDepth();
    theInts[indexI++]=theVertex.returnLabel();
    theInts[indexI++]=theVertex.returnScan();
    theInts[indexI++]=theVertex.isFrozen();
    theInts[indexI++]=theVertex.returnPosFlow();
    theInts[indexI++]=theVertex.returnEstFlow();
    nIncid=theInts[indexI++]=theVertex.numIncid();
    for(i=1;i<=nIncid;i++) theInts[indexI++]=theVertex.returnIncid(i);
    theInts[indexI++]=theVertex.returnPredEdg();
    for(i=0;i<8;i++) theInts[indexI++]=theVertex.returnFreeDegs(i);
    theInts[0]=indexI;

    for(i=0;i<8;i++) theFloats[indexF++]=theVertex.returnDegValueByName(i);
    theFloats[0]=indexF;
}

//reads a vertex from the output arrays
void readVertexFromArray(Vertex &theVertex, int &startI, int* theInts, int& startF, float* theFloats)
{
    int i, count;

    theVertex.setName(theInts[startI++]);
    theVertex.setType(theInts[startI++]);
    theVertex.setWeight(theInts[startI++]);
    theVertex.setDepth(theInts[startI++]);
    theVertex.setLabel(theInts[startI++]);
    theVertex.setScan(theInts[startI++]);
    count=theInts[startI++];
    if(count==2) theVertex.freezeForever();
    if(count==1) theVertex.freeze();
    theVertex.setPosFlow(theInts[startI++]);
    theVertex.setEstFlow(theInts[startI++]);
    count=theInts[startI++];
    for(i=0;i<count;i++)
        theVertex.appendIncid(theInts[startI++]);
    theVertex.setPredEdg(theInts[startI++]);
    for(i=0;i<8;i++)
    {
        theVertex.initialValue(i,theFloats[startF++],theInts[startI++]);
    }
}

//outputs an edge to the output arrays
void edgeToArray(Edge &theEdge, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];

    theInts[indexI++]=theEdge.returnName();
    theInts[indexI++]=theEdge.returnType();
    theInts[indexI++]=theEdge.returnWeight();
    theInts[indexI++]=theEdge.returnLabel();
    theInts[indexI++]=theEdge.returnScan();
    theInts[indexI++]=theEdge.returnPosFlow();
    theInts[indexI++]=theEdge.returnFlow1();
    theInts[indexI++]=theEdge.returnFlow2();
    theInts[indexI++]=theEdge.returnEnd1();
    theInts[indexI++]=theEdge.returnPart1();
    theInts[indexI++]=theEdge.returnEnd2();
    theInts[indexI++]=theEdge.returnPart2();
    theInts[indexI++]=theEdge.returnPredVer();
    theInts[0]=indexI;

    theFloats[indexF++]=theEdge.returnValue();
    theFloats[0]=indexF;
}

//reads an edge from the output arrays
void readEdgeFromArray(Edge &theEdge, int &startI, int* theInts, int &startF, float* theFloats)
{
    theEdge.setName(theInts[startI++]);
    theEdge.setType(theInts[startI++]);
    theEdge.setWeight(theInts[startI++]);
    theEdge.setLabel(theInts[startI++]);
    theEdge.setScan(theInts[startI++]);
    theEdge.setPosFlow(theInts[startI++]);
    theEdge.setFlow1(theInts[startI++]);
    theEdge.setFlow2(theInts[startI++]);
    theEdge.setEnd(0,theInts[startI++]);
    theEdge.setPart(0,theInts[startI++]);
    theEdge.setEnd(1,theInts[startI++]);
    theEdge.setPart(1,theInts[startI++]);
    theEdge.setPredVer(theInts[startI++]);
    theEdge.setValue(theFloats[startF++]);
}

//outputs a graph to an array
void graphToArray(Graph &graph0, int*& theInts, float*& theFloats)
{
    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    int i, count;

    theInts[indexI++]=graph0.returnDimen();
    theInts[indexI++]=graph0.returnDepth();
    count=theInts[indexI++]=graph0.returnNumVer();
    theInts[0]=indexI;
    for(i=1;i<=count;i++) vertexToArray(graph0.returnVertByIndex(i), theInts, theFloats);
    indexI=theInts[0];

    count=theInts[indexI++]=graph0.returnNumEdg();
    theInts[0]=indexI;
    for(i=1;i<=count;i++) edgeToArray(graph0.returnEdgeByIndex(i), theInts, theFloats);
}

//reads a graph from the arrays
void readGraphFromArray(Graph &graph0, int &startI, int* theInts, int &startF, float* theFloats)
{
    int i, count;

    graph0.setDimen(theInts[startI++]);
    graph0.setDepth(theInts[startI++]);
    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
        Vertex newVert;
        readVertexFromArray(newVert, startI, theInts, startF, theFloats);
        graph0.appendVertex(newVert);
    }
    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
        Edge newEdge;
        readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
        graph0.appendEdge(newEdge);
    }
}

//outputs a cluster to the arrays
void clusterToArray(Cluster &theCluster, int*& theInts, float*& theFloats)
{
    vertexToArray(theCluster.returnCore(), theInts, theFloats);

    int indexI=theInts[0];
    int indexF=(int) theFloats[0];
    int i, j, count, mem;

    theInts[indexI++]=theCluster.returnGroup();
    count=theInts[indexI++]=theCluster.returnFronLen();
    for(i=1;i<=count;i++) theInts[indexI++]=theCluster.returnFrontier(i);
    count=theInts[indexI++]=theCluster.returnInnerELen();
    theInts[0]=indexI;
    for(i=1;i<=count;i++) edgeToArray(theCluster.returnInnerE(i), theInts, theFloats);
    indexI=theInts[0];
    count=theInts[indexI++]=theCluster.returnOuterELen();
    theInts[0]=indexI++;
    for(i=1;i<=count;i++) edgeToArray(theCluster.returnOuterE(i), theInts, theFloats);
    indexI=theInts[0];
    indexF=(int) theFloats[0];
    count=theInts[indexI++]=theCluster.returnOrigLen();
    for(i=1;i<=count;i++) theInts[indexI++]=theCluster.returnOrigV(i);
    count=theInts[indexI++]=theCluster.returnNumBifurs();
    mem=theInts[indexI++]=theCluster.returnCurrBifur();
    theInts[0]=indexI;
    for(i=1;i<=count;i++)
    {
        theCluster.setCurrBifur(i);
        stringToArray(theCluster.returnCurrBifurString(), theInts);
    }
    theCluster.setCurrBifur(mem);
    indexI=theInts[0];
    if(theCluster.isSolved()) theInts[indexI++]=0;
    else theInts[indexI++]=-1;
    theInts[indexI++]=theCluster.returnConst();
    mem=theInts[indexI++]=theCluster.returnCurrDeg();
    count=theInts[indexI++]=theCluster.returnNumDeg();

    for(i=1;i<=count;i++)
    {
        theCluster.setCurrDeg(i);
        for(j=0;j<8;j++) theFloats[indexF++]=theCluster.returnValue(j);
    }
    theCluster.setCurrDeg(mem);
    theFloats[0]=indexF;

    count=theInts[indexI++]=theCluster.children.returnLen();
    theInts[0]=indexI;
    for(i=1;i<=count;i++)
        clusterToArray(theCluster.children.retrieve(i), theInts, theFloats);
}

//reads a cluster from the arrays
void readClusterFromArray(Cluster &theCluster, int &startI, int* theInts, int &startF, float* theFloats)
{
    Vertex core;

    readVertexFromArray(core, startI, theInts, startF, theFloats);

    int i, j, count;

    theCluster.setGroup(theInts[startI++]);

    List<int> fronts, origs;
    List<Edge> inner, outer;

    count=theInts[startI++];
    for(i=0;i<count;i++) fronts.append(theInts[startI++]);

    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
        Edge newEdge;
        readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
        inner.append(newEdge);
    }

    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
        Edge newEdge;
        readEdgeFromArray(newEdge, startI, theInts, startF, theFloats);
        outer.append(newEdge);
    }

    count=theInts[startI++];
    for(i=0;i<count;i++) origs.append(theInts[startI++]);

    theCluster.formCl(core, fronts, inner, outer, origs);

    List<std::string> strings;
    int currBifur;

    count=theInts[startI++];
    currBifur=theInts[startI++];
    for(i=0;i<count;i++)
    {
        std::string test=readStringFromArray(startI, theInts);
        strings.append(test);
    }
    theCluster.setBifurs(strings);
    theCluster.setCurrBifur(currBifur);
    theCluster.setSolved(theInts[startI++]==0);
    theCluster.setConst(theInts[startI++]);

    List<DegValues> values;
    int currDeg;

    currDeg=theInts[startI++];
    count=theInts[startI++];

    for(i=0;i<count;i++)
    {
        DegValues newDeg;
        for(j=0;j<8;j++) newDeg.setValue(j,theFloats[startF++]);
        values.append(newDeg);
    }
    theCluster.setDegValues(values);
    theCluster.setCurrDeg(currDeg);

    count=theInts[startI++];
    for(i=0;i<count;i++)
    {
        Cluster newClust;
        readClusterFromArray(newClust, startI, theInts, startF, theFloats);
        theCluster.children.append(newClust);
    }
}

//prints the arrays to a file
void outputArrays(int* theInts, float* theFloats)
{
    int i;
    std::ofstream arrayOut;

    arrayOut.open("arrayOut.txt");

    for(i=0;i<INTSIZE;i++)
        arrayOut<<theInts[i]<<std::endl;

    for(i=0;i<FLOATSIZE;i++)
        arrayOut<<theFloats[i]<<std::endl;

    arrayOut.close();
}

//reads the arrays from a file
void inputArrays(int*& theInts, float*& theFloats)
{
    int i;

    std::ifstream arrayIn;

    arrayIn.open("arrayOut.txt");

    for(i=0;i<INTSIZE;i++)
    {
        arrayIn>>theInts[i];
        if(theInts[0]==-1) return;
    }

    for(i=0;i<FLOATSIZE;i++) arrayIn>>theFloats[i];

    arrayIn.close();
}

//saves the DRTree and the graph into the output arrays, used when called within this file
void saveState(Graph &graph0)
{
    int i, length;

    length=toSolverTrees->returnLen();

    inputInts[0]=1;
    inputFloats[0]=1.0f;
    graphToArray(gGraph1, inputInts, inputFloats);
    graphToArray(graph0, inputInts, inputFloats);
    inputInts[inputInts[0]++]=length;
    for(i=1;i<=length;i++)
        clusterToArray(toSolverTrees->retrieve(i), inputInts, inputFloats);
}

//saves the DRTree and the graph into the output arrays, used when caled outside this file
void saveState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int*& thejInts, double*& thejDoubles)
{
    int i, length;
    int *inputInts=new int[INTSIZE];
    float *inputFloats=new float[FLOATSIZE];

    length=SolverTrees.returnLen();

    inputInts[0]=1;
    inputFloats[0]=1.0f;
    graphToArray(graph1, inputInts, inputFloats);
    graphToArray(graph0, inputInts, inputFloats);
    inputInts[inputInts[0]++]=length;
    for(i=1;i<=length;i++)
        clusterToArray(SolverTrees.retrieve(i), inputInts, inputFloats);
    for(i=0;i<INTSIZE;i++)
        thejInts[i]=inputInts[i];
    for(i=0;i<FLOATSIZE;i++)
        thejDoubles[i]=inputFloats[i];
}

//loads the DRTree and graph from the arrays
void loadState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int &startI, int* thejInts, int &startF, double* thejDoubles)
{
    int i, length;
    int *inputInts=new int[INTSIZE];
    float *inputFloats=new float[FLOATSIZE];

    for(i=0;i<INTSIZE;i++)
        inputInts[i]=thejInts[i];

    for(i=0;i<FLOATSIZE;i++)
        inputFloats[i]=(float) thejDoubles[i];

    graph0.makeEmpty();
    graph1.makeEmpty();
    SolverTrees.makeEmpty();
    readGraphFromArray(graph1, startI, inputInts, startF, inputFloats);
    readGraphFromArray(graph0, startI, inputInts, startF, inputFloats);

    length=inputInts[startI++];

    for(i=0;i<length;i++)
    {
        Cluster newClust;
        readClusterFromArray(newClust, startI, inputInts, startF, inputFloats);
        SolverTrees.append(newClust);
    }

    startI++;
}

//Sets the contents of the globals inputInts and inputFloats
//to output arrays inputTheInts and inputDouble
void setArraysForOutput(int*& theJInts, double*& theJDoubles)
{
    int i;

    for(i=0;i<FLOATSIZE;i++)
        theJDoubles[i]=inputFloats[i];

    outputArrays(inputInts, inputFloats);
}

//Sets the contents of the globals inputInts and inputFloats
//to input arrays inputTheInts and inputDouble
void setArraysForInput(int*& theJInts, double*& theJDoubles)
{
    int i;

    inputInts=new int[INTSIZE];

    inputInts=(int*) theJInts;

    inputFloats=new float[FLOATSIZE];

    for(i=0;i<FLOATSIZE;i++)
        inputFloats[i]=(float) theJDoubles[i];
}
