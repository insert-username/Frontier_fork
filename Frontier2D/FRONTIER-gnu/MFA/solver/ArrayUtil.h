#ifndef MFA_SOLVER_ARRAY_UTIL_H
#define MFA_SOLVER_ARRAY_UTIL_H


//Array IO Functions
void stringToArray(std::string theString, int*& theInts);
std::string readStringFromArray(int& start, int* theInts);
void vertexToArray(Vertex &theVertex, int*& theInts, float*& theFloats);
void readVertexFromArray(Vertex &theVertex, int &startI, int* theInts, int& startF, float* theFloats);
void edgeToArray(Edge &theEdge, int*& theInts, float*& theFloats);
void readEdgeFromArray(Edge &theEdge, int &startI, int* theInts, int &startF, float* theFloats);
void graphToArray(Graph &graph0, int*& theInts, float*& theFloats);
void readGraphFromArray(Graph &graph0, int &startI, int* theInts, int &startF, float* theFloats);
void clusterToArray(Cluster &theCluster, int*& theInts, float*& theFloats);
void readClusterFromArray(Cluster &theCluster, int &startI, int* theInts, int &startF, float* theFloats);
void outputArrays(int* theInts, float* theFloats);
void inputArrays(int*& theInts, float*& theFloats);
void saveState(Graph &graph0);
void saveState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int*& thejInts, double*& thejDoubles);
void loadState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int &startI, int* thejInts, int &startF, double* thejDoubles);
void setArraysForOutput(int*& theJInts, double*& theJDoubles);
void setArraysForInput(int*& theJInts, double*& theJDoubles);
void outputDRDAGToArray(List<Cluster> &theCluster, int& index, int*& theInts, bool first);


#endif