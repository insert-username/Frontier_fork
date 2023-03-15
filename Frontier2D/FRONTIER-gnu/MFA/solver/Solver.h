#ifndef MFA_SOLVER_SOLVER_H
#define MFA_SOLVER_SOLVER_H

//file: mfa9Solver.h

/* header file for the equation solution manager (ESM) */
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
as a file both in the FRONTIER-gnu directory, and 
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "entities/Vertex.h"
#include "entities/Edge.h"
#include "entities/Cluster.h"
#include "entities/Graph.h"

#include <iostream>
#include <string>



//Global variable declarations
List<int> keys;                          //records the keys pressed by the user
List<Edge> theImags;                     //theImaginary Constraints
// List<std::string> vars;                       //holds the list of variable strings
List<Cluster> roots;                     //roots of the DRDAG
List<Cluster>* toSolverTrees;		 //points to SolverTrees

Vertex testVertex1, testVertex2;         //sample vertices used in testing

std::string outFile="./Sketcher/testout.txt"; //path to output file

int aOverlap;                            //an example constraint between two clusters
int equationCount;                       //the number of equations generated for a cluster
int withHeldCluster;                     //theCluster whose values are carried over in solving
int type1, type2;                        //used in the testEQ section
int startI;                              //starting point for int array read  
int startF;                              //starting point for float array read
int currPosI;				 //current location in inputInts
int currPosF;				 //current location in inputFloats
int* inputInts;				 //global pointer to the int array
int* bifurAnswers;			 //answers to the current bifurs

float* inputFloats;                      //global pointer to the float array

bool toSolve;                            //stores a command to solve or to halt
bool first;                              //true if this is the first message to the sketcher
bool autoSolve;                          //automatically searches solution set if true
bool usingArrays;                        //true if the current data came from arrays
bool usedBifurs;			 //true if the bifurs have been placed

Graph gGraph1;				 //globalized graph1



//Function Headers
//For definitions and function documentation see the inidividual function definitions in mfa9Solver.cpp

//Solver Functions
void shellMaple();
void setWithHeldCluster(Cluster &theCluster);
int solveCluster(Graph &graph0, Cluster &theCluster, bool resolve);
int solveForest(Graph &graph0, List<Cluster> &theForest);

#endif
