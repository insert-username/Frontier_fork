#ifndef MFA_SOLVER_EQUATION_GENERATOR_H
#define MFA_SOLVER_EQUATION_GENERATOR_H

#include "entities/Vertex.h"
#include "entities/Edge.h"
#include "entities/Graph.h"
#include "entities/Cluster.h"

#include <string>

//Equation Generators
std::string getDistanceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, std::string theDist);
std::string getTangencyEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getIncidenceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getAngleEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getParallelEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
std::string getPerpendicularEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge);
int getEdgeCode(Edge &theEdge, Cluster &theCluster);
std::string switchString(Vertex &theVertex, int type, int shapeName, int clusterName, std::string toReplace, bool solveOrValue);
std::string getEquationBetweenClusters(Graph& graph0, Edge &theEdge, Cluster &theCluster);
std::string getEquationInCluster(Graph& graph0, Edge &theEdge, Cluster &theCluster);
std::string getEquation(Graph &graph0, Edge &theEdge, Cluster &theCluster);
std::string getSinCosConstraint();
std::string getOverlapConstraint(Graph &graph0, Cluster &theCluster);
std::string getLineConstraint(Graph &graph0, List<Cluster> &theChildren);
std::string getOriginEquations(Edge &theEdge, Graph &graph0, int clusterName);
std::string getRotationEquations(Cluster &theCluster);


#endif