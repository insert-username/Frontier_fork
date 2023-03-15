//file: mfa9Solver.cpp

/*The entire equation and solution manager ESM*/

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


#include "solver/Solver.h"
#include "entities/Graph.h"

#include "algo/Graph.h"
#include "output/DRPrintUtils.h"

#include <iostream>
#include <fstream>
#include <cstring>


/*  Same as above but places the output values in the transfer arrays, inputInts and 
    inputFloats.  The format is as follows, if printnum, the ID of theCluster is output,
    otherwise nothing.  Then for every original vertex in theCluster, the ID is printed,
    then the following information, for every point the x then y coordinate, followed by
    information depending on the type of the object:

       points - nothing
       lines - second point on line
       rays - second point on ray
       line segment - second endpoint
       circle - radius
       arc - points of the start and end of the arc, x coordinates first
*/
void generateOutputToArray(Graph &graph0, Cluster &theCluster, bool printNum)
{   
   int i, length;
   Vertex currVert;   
   float slope, angle1, angle2;
   float x,y,tempValue, tempValue2;
   
   if(printNum) inputInts[currPosI++]=theCluster.returnOrigLen();
   length=graph0.returnNumVer();
   for(i=1;i<=length;i++)
   {
    
      currVert=graph0.returnVertByIndex(i);
      if(inOriginalV(currVert.returnName(),theCluster))
      {
        inputInts[currPosI++]=currVert.returnName();
        x=currVert.returnDegValueByName(0);
        y=currVert.returnDegValueByName(1);
        inputFloats[currPosF++]=x;
        inputFloats[currPosF++]=y;
        switch(currVert.returnType())
        {
              case 0:   break;
              case 1:   tempValue=currVert.returnDegValueByName(4);
                        inputFloats[currPosF++]=x+1;
                        inputFloats[currPosF++]=y+tempValue;
                        break;
              case 2:   tempValue=currVert.returnDegValueByName(4);
                        tempValue2=currVert.returnDegValueByName(5);
                        inputFloats[currPosF++]=x+tempValue;
                        inputFloats[currPosF++]=y+tempValue2;
                        break;
              case 3:   inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(3);
                        break;  
              case 4:   tempValue=currVert.returnDegValueByName(2);
                        if(tempValue<0) tempValue=-tempValue;
                        inputFloats[currPosF++]=tempValue;
                        break;
              case 5:   angle1=getAngle(currVert.returnDegValueByName(4),currVert.returnDegValueByName(5));
                        angle2=getAngle(currVert.returnDegValueByName(6),currVert.returnDegValueByName(7));
                        inputFloats[currPosF++]=currVert.returnDegValueByName(2);
                        inputFloats[currPosF++]=currVert.returnDegValueByName(3);
                        inputFloats[currPosF++]=angle1;
                        inputFloats[currPosF++]=angle2;
                        break;
        }
      }  
   }
}

//shells to maple and calls the solver
void shellMaple()
{
    std::string outputString;

    outputString="maple <input.txt> output.txt";
    remove("output.txt");
    system(outputString.c_str());
}

//chooses the largest child of theCluster and sets it to be withheld, meaning
//when theCluster is reconciled, all of the other clusters will be rotated into
//the coordinate system of this withheld cluster
void setWithHeldCluster(Cluster &theCluster)
{
    int i, length=theCluster.children.returnLen();
    int temp=0;
    int currLen=0;
    int origLen;

    for(i=1;i<=length;i++)
    {
       origLen=theCluster.children.retrieve(i).returnOrigLen();
       if(origLen>1 && origLen>currLen)
       {
         temp=theCluster.children.retrieve(i).returnName();
         currLen=origLen;
       }
    }
    withHeldCluster=temp;
}

//solves a given cluster
int solveCluster(Graph &graph0, Cluster &theCluster, bool resolve)
{
    int i, j, q, length, childLength, origLength;
    std::string temp, equation, answerString, childString;
    int answer=0, child, numChild;
    int tag;
    char in;
    int totalBifur,currChild=1;
    bool useFileToBifur, useFileTotal;
    Edge forOriginSet;    //stores an Edge so that it can be used for setting the origin

    childLength=(theCluster.children).returnLen();
    origLength=theCluster.returnOrigLen();

    //if the cluster has no children or no original vertices it must be an original vertex
    //end the recursion
    if(childLength==0) return 0;
    if(theCluster.isSolved()) return 0;

    //solver the children of this cluster and store the return value in tag
    tag=solveForest(graph0, theCluster.children);

    std::cout<<"Children Solved"<<std::endl;
 
    //if the return from the children is -2, then no solution is found, pass this up the calling stack
    if(tag==-2) return(-2);

    //if resolve is false, no additional bifurcation options remain, a -1 tag for no solution by additional
    //solutions remain is invalid here pass value up calling stack
    if(!resolve && tag==-1) return(-1);

    //answer is set to three when a valid solution is found, otherwise this loop tries to find a
    //solution with each different set of bifurcations of its children
    while(answer!=3)
    {
         useFileToBifur=true;
         useFileTotal=false;

	   //empty the lists of variables and imaginary constaints
         vars.makeEmpty();
         theImags.makeEmpty();

        std::cout<<"Solving Cluster "<<theCluster.returnName()<<"..."<<std::endl;
         answer=0;
         totalBifur=0;
         
	   //output the current cluster to the screen
         print(graph0, theCluster.children);

	   //count the total number of bifurcations availible to the children
         for(i=1;i<=childLength;i++)
            if(theCluster.children.retrieve(i).returnOrigLen()==0)
              totalBifur++;

         std::cout<<std::endl;

         std::cout<<"Bifurcations counted."<<std::endl;

         bool getNextB;

	   //in the autosolve mode, select the next bifurcation combination with the getNextBifur
         //method, if no solution exists when immediate children are resolved, resolve their children
         if(autoSolve)
         {
           getNextB=getNextBifurcation(graph0,theCluster);
           print(graph0, theCluster.children);
           if(getNextB)
             while(currChild<=childLength && solveCluster(graph0,theCluster.children.retrieve(currChild),true)==-1)
                  currChild++;
           if(currChild>childLength) return(-1); 
         }        
	   //if not set in autosolve, and the bifurcation answers returned from the sketcher have not been placed
         else if(usedBifurs)
              {
                  std::cout<<"Solver usedBifurs"<<std::endl;

		    //save the state of the DRDag and the graph
                saveState(graph0);

                //output all of the bifurcation options of all of the children of this cluster
                //into the transfer array
                std::cout<<"Solver state saved"<<std::endl;
                inputInts[0]++;
                currPosI=inputInts[0]+2;
                currPosF=(int) inputFloats[0];
                inputInts[inputInts[0]-1]=0;               

		    int count=0, currPosBackUp=currPosI++;
		    int theNumTrees=toSolverTrees->returnLen();
                for(i=1;i<=theNumTrees;i++)
                   outputFinState(toSolverTrees->retrieve(i), currPosI, inputInts, count);
		    
                inputInts[currPosBackUp]=count;

                for(i=1;i<=childLength;i++)
                {
                   std::cout<<"Child processed: "<<i<<std::endl;
                   selectBifurcation(graph0,theCluster.children.retrieve(i),useFileToBifur);
                   useFileTotal=(useFileTotal || useFileToBifur);
                   if(useFileToBifur) 
                   {
                     std::cout<<"useFileToBifur is true"<<std::endl;
                     inputInts[inputInts[0]-1]++;
                     continue;
                   }
                   std::cout<<"useFileToBifur is false"<<std::endl;

                   parseBifurString(graph0, theCluster.children.retrieve(i));

                   std::cout<<"Bifur parsed"<<std::endl;

                   updateGraph(graph0, theCluster.children.retrieve(i));

                   std::cout<<"Graph updated"<<std::endl;
                }
                inputInts[inputInts[0]]=0;
                inputInts[inputInts[0]+1]=inputInts[inputInts[0]-1];

                std::cout<<"Ending Solver usedBifurs"<<std::endl;
              }

              //if the bifurcations have not been placed yet set the children of this cluster
              //to have those bifurcation values
              else 
              {
                std::cout<<"Solver not usedBifur"<<std::endl;

                tag=0;
                for(i=1;i<=childLength;i++)
                {
                   if(theCluster.children.retrieve(i).returnCurrBifur()>=0) continue;
                   if(theCluster.children.retrieve(i).children.returnLen()==0) continue;
                   std::cout<<"Bifur Used: "<<bifurAnswers[tag]<<std::endl;
                   theCluster.children.retrieve(i).setCurrBifur(bifurAnswers[tag++]);
                   parseBifurString(graph0, theCluster.children.retrieve(i));
                   updateGraph(graph0, theCluster.children.retrieve(i));
                   usedBifurs=true;
                }
              }                   
         
         //useFileTotal is set if any of the children have bifurcations, meaning that output to
         //the sketcher has been generated, and the method must be closed and control returned to
         //the sketcher so that an answer to the bifurcations can be chosen
         if(useFileTotal) std::cout<<"Bifurs written, returning"<<std::endl;
         if(useFileTotal) return -2;

	   //name of input file generated for Maple
         temp="input.txt";
         aOverlap=0;
         equationCount=0;

	   //sets the global cluster pointer, withHeldCluster
         setWithHeldCluster(theCluster);
         std::cout<<"ABOUT TO SET REDUCTION"<<std::endl;

	   //reduction is only used when the cluste contains distance constraints
         bool useReduction=true;
         int theVName;
	   length=graph0.returnNumVer();
	   for(i=1;i<=length;i++)
         {
             std::cout<<useReduction<<" ";
             std::cout<<graph0.returnVertByIndex(i).returnType()<<std::endl;
            theVName=graph0.returnVertByIndex(i).returnName();
            if(getChildNameWithVertex(theCluster, theVName)!=theVName) continue;
            if(graph0.returnVertByIndex(i).returnType()!=0) useReduction=false;
         }
	   if(useReduction) setValueReduction(graph0, theCluster);
         
         //equation generation begins
         tag=0;
        std::ofstream inputFile;

	   //delete the old input file
         remove(temp.c_str());

        std::cout<<"Input File opened"<<std::endl;
         inputFile.open(temp.c_str());

	   //interface set to return text output
         inputFile<<"interface(prettyprint=0);"<<std::endl;
  
         //ten second solution time limit set
         inputFile<<"timelimit(10,solve({";

         length=graph0.returnNumEdg();
	   bool overWrite=true;
         tag=0;
         equationCount=0;

	   //equations are generated for every edge within this cluster
         for(i=1; i<=length; i++)
         {
            equation=getEquation(graph0, graph0.returnEdgeByIndex(i), theCluster);
            if(equation!="") equationCount++;
            if(tag==1 && equation!="") inputFile<<",";
            if(tag==0 && equation!="")
	      tag=1;

		//one edge must be stored to use to set the origin as in the getOriginEquations method
            //above, this edge should not be a parallel or perpendicular constraints, this loop insures this         
	      if(overWrite && equation!="")
            {
              forOriginSet=graph0.returnEdgeByIndex(i);
              if(forOriginSet.returnType()!=2 && forOriginSet.returnType()!=3) overWrite=false;
            }
            inputFile<<equation;
         }

	   //additional constraints are generated, see the appropriate methods for more details
         equation=getOverlapConstraint(graph0, theCluster);
         if(tag==0) equation.erase(0,1);
         inputFile<<equation;
         inputFile<<getRotationEquations(theCluster);
         inputFile<<getLineConstraint(graph0,theCluster.children);
         if(vars.returnLen()>equationCount)
           inputFile<<","<<getOriginEquations(forOriginSet, graph0, aOverlap);
         inputFile<<getSinCosConstraint();
         inputFile<<"},";
         inputFile<<getVarString();
         inputFile<<"));"<<std::endl;
         inputFile<<"allvalues(%);"<<std::endl;
         inputFile<<"evalf(%);"<<std::endl;
         inputFile.close();

        std::cout<<"OUTPUT FILE WRITTEN"<<std::endl;


	   //the output information restored and altered in case the numeric solver must be used
	   int semiCount=0;
         int commaCount=0;
        std::ifstream outTest;
        std::string stringInputData="";
         outTest.open(temp.c_str());
	   outTest>>in;
	   while(!outTest.eof() && in!=',') outTest>>in;
	   while(!outTest.eof() && semiCount<1)
         {
	        outTest>>in;
              stringInputData+=in;
              if(in==';')
              {
                 stringInputData+='\n';
                 semiCount++;
	        }
         }
	   outTest.close();

	   //generated equations output to the screen         
         outTest.open(temp.c_str());
         outTest>>in;
         while(in!='{') outTest>>in;
         outTest>>in;
         while(in!='}')  
         { 
           if(in==',' || in==';') std::cout<<std::endl;
           else std::cout<<in;
           outTest>>in;
         }
         std::cout<<std::endl;
         std::cout<<getVarString()<<std::endl;
         outTest.close();

	   //maple shelled
         shellMaple();

         //bifurcation strings are read from output.txt, the maple output file
         setBifurs("output.txt",theCluster, false);
         
 	   //if no solution is found with the symbolic solver, the number solver is tried
	   if(theCluster.returnNumBifurs()==0)
         {
           std::cout<<"Using Numeric Solver"<<std::endl;
           inputFile.open(temp.c_str());
           inputFile<<"interface(prettyprint=0);"<<std::endl;
           inputFile<<"timelimit(60,f";
           inputFile<<stringInputData;
           inputFile.close();
           shellMaple();
           setBifurs("output.txt",theCluster, true);
         }

	   //found bifurcations are printed
         printBifurcations(theCluster);

         //bifurcations are checked to see if they meet the imaginary constraints
         checkBifurs(graph0, theCluster);

	   //bifurcation choice is set to -1 -- none chosen yet
         theCluster.setCurrBifur(-1);

	   //if when autosolving no valid bifurcations are found, terminate
         if(theCluster.returnNumBifurs()==0 && !useFileTotal && !autoSolve) 
         {
             std::cout<<"No Solution exists for this set of constraints.  Terminating Solver."<<std::endl;
             exit(-2);
         } 
    
	   //if there are no bifurcations and in autosolve, resolve this cluster with different bifurcations
         if(theCluster.returnNumBifurs()==0 && autoSolve) answer=1;

         //if this cluster has bifurcations, either in autosolve or not, continue
         if(theCluster.returnNumBifurs()!=0 && autoSolve) answer=3;
         if(!autoSolve) answer=3;
    }
  
    //set the fin flag of this cluster to true
    theCluster.setSolved(true);
    return 0;
}

//calls solve cluster for every cluster in theForest
int solveForest(Graph &graph0, List<Cluster> &theForest)
{
    int length, i, tag;

    length=theForest.returnLen();
    for(i=1;i<=length;i++)
    {
       tag=solveCluster(graph0, theForest.retrieve(i), false);
       if(tag<0) return tag;
    }
    return 0;
}

//checks to see if theEdge is valid using the already solved values in SolverTrees
bool checkEdge(Edge &theEdge, Graph &graph0, List<Cluster> &SolverTrees)
{
   int i, length=SolverTrees.returnLen();
   int edgeCode;
   std::string input;

   std::cout<<"Edge: "<<theEdge<<std::endl;

   if(length==0) return false;

   for(i=1;i<=length;i++)
   {
      edgeCode=getEdgeCode(theEdge, SolverTrees.retrieve(i));
      std::cout<<"Checking Cluster: "<<SolverTrees.retrieve(i).returnName()<<" edgeCode: "<<edgeCode<<std::endl;
      if(edgeCode==0)
      {
        vars.makeEmpty();
        input=getEquation(graph0, theEdge, SolverTrees.retrieve(i));
        std::cout<<input<<std::endl;
        std::cout<<getVarString()<<std::endl;
        return true;
        
      }
   }
   return checkEdge(theEdge, graph0, SolverTrees.retrieve(i).children);
}
      
void Solver(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int* inputTheInts, double* inputDouble)
{
   //Variables
   int i, numTrees=SolverTrees.returnLen();
   std::ofstream outfile;
   std::string answer;
   bool useless;
   int count, tag;
   bool useFileTotal, useFileToBifur;

   AlgoGraph::copyG(graph1, gGraph1);

   std::cout << "Solver Start" << std::endl;
   
   setArraysForInput(inputTheInts, inputDouble);

   //Global Initializations
   startI=1;
   startF=1;
   usingArrays=(inputInts[0]==2 || inputInts[0]==1);
   autoSolve=(inputInts[0]==1);
   first=true;
   roots=SolverTrees;
   vars.makeEmpty();
   usedBifurs=true;
   toSolverTrees=&SolverTrees;

   std::cout<<"AutoSolve: "<<(autoSolve ? "true" : "false")<<std::endl;
   std::cout<<"Variable Initialized usingArrays: "<<(usingArrays ? "true" : "false")<<std::endl;

   //if the DRDag has just been generated, outputs it to the sketcher
   if(!usingArrays)
   {
     saveState(graph0);
     std::cout << "State Saved" << std::endl;
     inputInts[inputInts[0]]=-1;
     inputInts[0]++;
     
     int backupIndex=inputInts[0];

     outputDRDAGToArray(SolverTrees, backupIndex, inputInts, true);
     std::cout<<"DAG Output"<<std::endl;
     outputArrays(inputInts, inputFloats);
     setArraysForOutput(inputTheInts, inputDouble);
     std::cout<<"Arrays output, Solver returns"<<std::endl;
     return;
   }          

   //runs the solver on the DRDag and if necessary sets the bifurcation of the root nodes in the DRDag
   if(usingArrays)
   {
     graph0.makeEmpty();
     SolverTrees.makeEmpty();
     gGraph1.makeEmpty();
     readGraphFromArray(graph1, startI, inputInts, startF, inputFloats);
     readGraphFromArray(graph0, startI, inputInts, startF, inputFloats);
     
     int length=inputInts[startI++]; 

     for(i=0;i<length;i++)
     {
          Cluster newClust;
          readClusterFromArray(newClust, startI, inputInts, startF, inputFloats);
          SolverTrees.append(newClust);
     }

     std::cout << "Graph and Cluster read:" << std::endl;

     numTrees=SolverTrees.returnLen();
     graph0.output(std::cout);
     print(graph0, SolverTrees);
     std::cout << "value at startI=" << inputInts[startI] << std::endl;
     count=inputInts[startI];
     if(count>0) bifurAnswers=new int[count];
     for(i=1;i<=count;i++)
     {
        usedBifurs=false;
        bifurAnswers[i-1]=inputInts[startI+i]+1;
        std::cout << bifurAnswers[i-1] << std::endl;
     }

     std::cout << "Bifucations read, number=" << count << std::endl;

     tag=0;
     for(i=1;i<=numTrees;i++)
     {
        tag=solveForest(graph0, SolverTrees);
        if(tag==-2)
        {
          std::cout << "Return by tag" << std::endl;
          setArraysForOutput(inputTheInts, inputDouble);
          return;
        }
     }

     std::cout<<"Solver ran, return value="<<tag<<std::endl;

     useFileToBifur=true;
     useFileTotal=false;

     std::cout<<"usedBifurs="<<(usedBifurs ? "true" : "false")<<std::endl;

     if(usedBifurs && !autoSolve)
     { 
       saveState(graph0);
       inputInts[0]++;
       currPosI=inputInts[0]+2;
       currPosF=(int) inputFloats[0];
       inputInts[inputInts[0]-1]=0;

       int count=0, currPosBackUp=currPosI++;
       for(i=1;i<=numTrees;i++)
          outputFinState(SolverTrees.retrieve(i), currPosI, inputInts, count);
       inputInts[currPosBackUp]=count;

       for(i=1;i<=numTrees;i++)
       {
          selectBifurcation(graph0,SolverTrees.retrieve(i),useFileToBifur);
          useFileTotal=(useFileTotal || useFileToBifur);
          if(useFileToBifur)
          {
            inputInts[inputInts[0]-1]++;
            continue;
          } 
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));  
       }
       inputInts[inputInts[0]]=0;
       inputInts[inputInts[0]+1]=inputInts[inputInts[0]-1];
     }
     else if(!autoSolve)
     {
       tag=0;
       for(i=1;i<=numTrees;i++)
       {
          if(SolverTrees.retrieve(i).returnCurrBifur()>=0) continue;
          SolverTrees.retrieve(i).setCurrBifur(bifurAnswers[tag++]);
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));
       }
     }

     if(useFileTotal && !autoSolve) 
     {
       std::cout << "Return by useFileTotal" << std::endl;
       setArraysForOutput(inputTheInts, inputDouble);
       return;
     }

     if(autoSolve)
     {
       saveState(graph0);
       for(i=1;i<=numTrees;i++)
       {
          SolverTrees.retrieve(i).setCurrBifur(1);
          parseBifurString(graph0, SolverTrees.retrieve(i));
          updateGraph(graph0, SolverTrees.retrieve(i));
       }
     }

     currPosI=inputInts[0];
     currPosF=(int) inputFloats[0];
     inputInts[currPosI++]=1;
     inputInts[currPosI++]=numTrees;
     for(i=1;i<=numTrees;i++)
     {
	updateGraph(graph0, SolverTrees.retrieve(i));	
        inputInts[currPosI++]=SolverTrees.retrieve(i).returnName();
        generateOutputToArray(graph0, SolverTrees.retrieve(i), true);
     }
   }

   setArraysForOutput(inputTheInts, inputDouble);

   std::cout << "Final Bifurcation Return" << std::endl;
   
   return;
}

