/*The UTU (universal transfer unit)*/

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

#include "UTU.h"

#include<string>
#include <fstream>
#include<math.h>
#include<values.h>

#include "algo/Graph.h"

#include "entities/List.h"
#include "entities/Vertex.h"
#include "entities/Edge.h"
#include "entities/Graph.h"
#include "entities/Cluster.h"

#include "solver/Solver.h"
#include "solver/SolverUtil.h"
#include "solver/ArrayUtil.h"
#include "solver/PrintUtil.h"

#include "algo/Groups.h"
#include "algo/MinimalDense.h"

void UTU::Java_utuJava_utuC(std::vector<int> &dataInt, std::vector<double> &dataDouble) {
    std::cout<<"OPEN UTU"<<std::endl;

    // define the global state values (nextEdgeName, nextVerName, singleVertex).
    GlobalState globalState;

   // Get int and double arrays from Java
   //jsize intLen = env->GetArrayLength(arrInt);
   //jint *dataInt = env->GetIntArrayElements(arrInt, 0);
//
   //jsize dblLen = env->GetArrayLength(arrDbl);
   //jdouble *dataDouble = env->GetDoubleArrayElements(arrDbl, 0);

   Graph graph0;
   List<Cluster> SolverTrees;
   List<Cluster> DRTrees;

   std::ofstream outfile1; // output file
   std::ofstream outfile2; // detail file

   outfile1.open("mfa.out");
   outfile1.open("mfa.dtl");
                        
   int startGroup, startI, startF;
   int type, name, numEnds, theEnd;
   double value;


   int i, j, length;

   Edge newEdge, *theEdge;

   Graph graph1;
  
   int flag=dataInt[0];
   bool autoS;

   autoS=false;

   std::cout<<"FLAG: "<<flag<<std::endl;

   if(flag>10)
   {
      flag=flag-10;
      autoS=true;
   }

   switch(flag)
   {
/*	case 1:		dataInt[0]=2;
			break; */

	case 0:         // Initialize graph0 and DR_Trees based on input file (utu.bin or utu.txt)
                        startI=1;
			startF=0;
      			startGroup=graph0.sketchInput(globalState, startI, dataInt, startF, dataDouble);

			std::cout<<"original graph:"<<std::endl;
      			//graph0.output(outfile1);

      			// Initialize the DRTrees structure based on input data
      			getGroups(globalState, dataInt, graph0, startGroup, DRTrees);
			
			std::cout<<"Groups read"<<std::endl;

      			AlgoGraph::copyG(graph0, graph1);  // copy Graph from graph0 to graph1
      			graph1.simplify();      // merge multi-edges and remove zero-weight edges

			std::cout<<"Graph Copied"<<std::endl;

			graph0.output(std::cout);

			std::cout<<"SolverTrees called"<<std::endl;

      			// MFA Algorithm: input are graph0, DR_Trees; output is SolverTrees
      			SolverTrees = MinimalDense::mfaAlgo(globalState, graph1, DRTrees, outfile1, outfile2);

			fixTree(SolverTrees);

    			std::cout<<"SolverTrees returned"<<std::endl;

			break;

	case 3:		startI=startF=1;	
		
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
			
			theEdge=graph0.EdgeAddr(dataInt[startI++]);
		
			value=dataDouble[startF++];

			std::cout<<"Value"<<value<<std::endl;
			
			for(i=startF;i<startF+10;i++)
			   std::cout<<i<<" "<<dataDouble[i]<<std::endl;

			theEdge->setValue((float) value);
			
			length=SolverTrees.returnLen();

			for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd1());
                        for(i=1;i<=length;i++)
			   resetFinByClusterName(SolverTrees.retrieve(i),theEdge->returnEnd2());
			
                        dataInt[0] = 0;
			break;

	case 4: 	std::cout<<"Start Add"<<std::endl;
			startI=startF=1;   
			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);

			std::cout<<"State Loaded"<<std::endl;

			type=dataInt[startI++];
			name=dataInt[startI++];
			numEnds=dataInt[startI++];

			newEdge.setName(name);
			newEdge.setType(type);
			newEdge.setWeight(1);			

			for(i=0;i<numEnds;i++)
			{
			   theEnd=dataInt[startI++];
			   newEdge.setEnd(i,theEnd);
			   graph0.VertAddr(theEnd)->appendIncid(name);
			   if(type<=1 || type==4)
			     newEdge.setPart(i, dataInt[startI++]);
			   else newEdge.setPart(i, 0);
			}
		
			if(type==1 && newEdge.returnPart1()==0 && newEdge.returnPart2()==0)
			  newEdge.setWeight(0);
                        if(type==1 && newEdge.returnPart1()!=0 && newEdge.returnPart2()!=0)
                          newEdge.setWeight(2);
			
			if(type==0 || type==4)
			  newEdge.setValue(dataDouble[startF++]);
			else newEdge.setValue(-2.0);

			graph0.appendEdge(newEdge);

			AlgoGraph::copyG(graph0, graph1);
                        graph1.simplify();

                        resetTrees(graph0, DRTrees);

            globalState.setSingleVertex(10);
			
                        SolverTrees = MinimalDense::mfaAlgo(globalState, graph1, DRTrees, outfile1, outfile2);

			std::cout<<"AFTER ADDING CONSTRAINT"<<std::endl;

			graph0.output(std::cout);

			PrintUtil::print(graph0, SolverTrees);
			
			dataInt[0] = 0;

                        break;	
		
	case 5:		startI=startF=1;
                        loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
	
			graph0.output(std::cout);

			PrintUtil::print(graph0, DRTrees);
		
			graph0.delEdgeByName(dataInt[startI++]);

                        AlgoGraph::copyG(graph0, graph1);
                        graph1.simplify();

                        resetTrees(graph0, DRTrees);

                        SolverTrees = MinimalDense::mfaAlgo(globalState, graph1, DRTrees, outfile1, outfile2);

			std::cout<<"AFTER REMOVING CONSTRAINT"<<std::endl;

                        graph0.output(std::cout);

                        PrintUtil::print(graph0, SolverTrees);

			dataInt[0]=0;		

                        break;	

	case 7:		startI=startF=1;
 			loadState(graph1, graph0, SolverTrees, startI, dataInt, startF, dataDouble);
			
			graph0.sketchInput(globalState, startI, dataInt, startF, dataDouble);

			AlgoGraph::copyG(graph0, graph1);
                        graph1.simplify();
                          
                        resetTrees(graph0, DRTrees);
                        
                        SolverTrees = MinimalDense::mfaAlgo(globalState, graph1, DRTrees, outfile1, outfile2);

			std::cout<<"AFTER ADDING OBJECT & CONSTRAINT"<<std::endl;

                        graph0.output(std::cout);

                        PrintUtil::print(graph0, SolverTrees);

                        dataInt[0] = 0;
                        
                        break;
   }			

   if(autoS) {
       dataInt[0] = 1;
   }

   // Solver subroutine
   Solver::solve(graph1, graph0, SolverTrees, dataInt, dataDouble);

   Solver::checkEdge(graph0.returnEdgeByName(1), graph0, SolverTrees);

   //env->SetIntArrayRegion(arrInt, 0, intLen, dataInt);
   //env->SetDoubleArrayRegion(arrDbl, 0, dblLen, dataDouble);
   //env->ReleaseIntArrayElements(arrInt, dataInt, 0);
   //env->ReleaseDoubleArrayElements(arrDbl, dataDouble, 0);
}
