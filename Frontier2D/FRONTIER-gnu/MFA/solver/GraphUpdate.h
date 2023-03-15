#ifndef MFA_SOLVER_GRAPH_UPDATE
#define MFA_SOLVER_GRAPH_UPDATE

#include "entities/Vertex.h"
#include "entities/Graph.h"
#include "entities/Cluster.h"

//Graph Update Functions
void rotate(Cluster &theCluster, Vertex &v0);
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex, int location);
void setValueInCluster(Graph &graph0, Cluster &theCluster, Vertex &theVertex);
void populateCluster(Graph &graph0, Cluster &theCluster);
void populateForest(Graph &graph0, List<Cluster> &SolverTrees);
void populateGraph(Graph &graph0, List<Cluster> &SolverTrees);
void updateGraph(Graph &graph0, Cluster &theCluster);


#endif