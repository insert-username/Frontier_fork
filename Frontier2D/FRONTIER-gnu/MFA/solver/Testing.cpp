#include "solver/Testing.h"

//test method which generates an equation for every type of constraint/vertex combination
void testEquations(Graph &graph0)
{
    int i,j,k,part1,part2;
    Edge testEdge;
    std::ofstream dataOut;
    Cluster dummy;
    std::string outData;
    std::string one, two, three;

    dataOut.open("c:/utu/outFile.txt");

    testVertex1.setName(1);
    testVertex2.setName(2);
    testEdge.setName(999);

    for(i=0;i<=6;i++)
    {
        switch(i)
        {
            case 0: one="Distance";
                break;
            case 1:
            case 2: one="Incidence";
                break;
            case 3: one="Angle";
                break;
            case 4: one="Parallel";
                break;
            case 5: one="Perpendicular";
                break;
            case 6: one="Tangency";
                break;
        }
        dataOut<<one+" Equations:"<<std::endl;
        for(j=0;j<=12;j++)
        {
            switch(j)
            {
                case 0: two="Point";
                    type1=0;
                    part1=1;
                    break;
                case 1: two="Line";
                    type1=1;
                    part1=0;
                    break;
                case 2: two="Ray (arb)";
                    type1=2;
                    part1=0;
                    break;
                case 3: two="Ray (Pt1)";
                    type1=2;
                    part1=1;
                    break;
                case 4: two="Line Seg (arb)";
                    type1=3;
                    part1=0;
                    break;
                case 5: two="Line Seg (Pt1)";
                    type1=3;
                    part1=1;
                    break;
                case 6: two="Line Seg (Pt2)";
                    type1=3;
                    part1=2;
                    break;
                case 7: two="Circle (arb)";
                    type1=4;
                    part1=0;
                    break;
                case 8: two="Circle (center)";
                    type1=4;
                    part1=3;
                    break;
                case 9: two="Arc (arb)";
                    type1=5;
                    part1=0;
                    break;
                case 10: two="Arc (Pt1)";
                    type1=5;
                    part1=1;
                    break;
                case 11: two="Arc (Pt2)";
                    type1=5;
                    part1=2;
                    break;
                case 12: two="Arc (Center)";
                    type1=5;
                    part1=3;
                    break;
            }
            dataOut<<" End 1 is a/an "+two<<std::endl;
            for(k=0;k<=12;k++)
            {
                switch(k)
                {
                    case 0:  three="Point:          ";
                        type2=0;
                        part2=1;
                        break;
                    case 1:  three="Line:           ";
                        type2=1;
                        part2=0;
                        break;
                    case 2:  three="Ray (arb):      ";
                        type2=2;
                        part2=0;
                        break;
                    case 3:  three="Ray (Pt1):      ";
                        type2=2;
                        part2=1;
                        break;
                    case 4:  three="Line Seg (arb): ";
                        type2=3;
                        part2=0;
                        break;
                    case 5:  three="Line Seg (Pt1): ";
                        type2=3;
                        part2=1;
                        break;
                    case 6:  three="Line Seg (Pt2): ";
                        type2=3;
                        part2=2;
                        break;
                    case 7:  three="Circle (arb):   ";
                        type2=4;
                        part2=0;
                        break;
                    case 8:  three="Circle (center):";
                        type2=4;
                        part2=3;
                        break;
                    case 9:  three="Arc (arb):      ";
                        type2=5;
                        part2=0;
                        break;
                    case 10: three="Arc (Pt1):      ";
                        type2=5;
                        part2=1;
                        break;
                    case 11: three="Arc (Pt2):      ";
                        type2=5;
                        part2=2;
                        break;
                    case 12: three="Arc (center):   ";
                        type2=5;
                        part2=3;
                        break;
                }
                outData="";
                if(i<2) testEdge.setType(i);
                else testEdge.setType(i-1);
                testEdge.setPart(0,part1);
                testEdge.setPart(1,part2);
                testVertex1.setType(type1);
                testVertex2.setType(type2);
                testEdge.setValue(5.0);
                outData=getEquationInCluster(graph0, testEdge, dummy);
                if(isImaginary(graph0,testEdge)) outData+=" *** IMAGINARY ***";
                if(outData!="")
                {
                    dataOut<<"  End 2 is a/an "+three<<" ";
                    dataOut<<outData<<std::endl;
                }
            }
        }
    }
    std::cout<<getSinCosConstraint();
    dataOut<<getVarString()<<std::endl;
    dataOut.close();
}


//used for testing only
//reads a graph struction from text input in a file
void getGraphFromFile(Graph &graph0, List<Cluster> &SolverTrees)
{
    std::ifstream dataIn;
    std::string filename;
    int numShapes;
    int numEdges;
    int i, ID, type, end1, end2;
    char s;
    float value;
    List<int> originalV, nullIntList, originalV2;
    List<Vertex> vList, nullVList;
    List<Edge> eList, nullEList;
    List<Cluster> cList, nullCList;
    Graph newGraph;

    nullIntList.makeEmpty();
    nullVList.makeEmpty();
    nullEList.makeEmpty();
    nullCList.makeEmpty();

    std::cout<<"Enter a filename: ";
    std::cin>>filename;

    dataIn.open(filename.c_str());

    dataIn>>s;

    if(s=='Y' || s=='y') toSolve=true;
    else toSolve =false;

    dataIn>>numShapes;

    for(i=0;i<numShapes;i++)
    {
        originalV.makeEmpty();
        dataIn>>ID;
        originalV2.append(ID);
        originalV.append(ID);
        dataIn>>type;
        Vertex currVert(ID, type);
        vList.append(currVert);
        Cluster currClust;
        currClust.formCl(currVert,nullIntList,nullEList,nullEList,originalV);
        currClust.children=nullCList;
        cList.append(currClust);
    }

    dataIn>>numEdges;

    for(i=0;i<numEdges;i++)
    {
        dataIn>>ID;
        dataIn>>type;
        value=0;
        if(type==0 || type==2) dataIn>>value;
        dataIn>>end1;
        dataIn>>end2;
        Edge currEdge;
        currEdge.setName(ID);
        currEdge.setType(type);
        currEdge.setValue(value);
        currEdge.setEnd(0,end1);
        currEdge.setEnd(1,end2);
        eList.append(currEdge);
    }

    Vertex currVert;

    currVert.setName(99);
    currVert.setType(-1);
    currVert.setDepth(1);

    Cluster newClust;

    newClust.formCl(currVert,nullIntList,nullEList,nullEList,originalV2);
    newClust.children=cList;

    SolverTrees.makeEmpty();
    SolverTrees.append(newClust);

    newGraph.setVlist(vList);
    newGraph.setElist(eList);

    graph0=newGraph;

    dataIn.close();
}
