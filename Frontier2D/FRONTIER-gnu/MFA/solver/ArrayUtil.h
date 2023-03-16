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
void saveState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, const std::vector<int> &thejInts, const std::vector<double> &thejDoubles);
void loadState(Graph &graph1, Graph &graph0, List<Cluster> &SolverTrees, int &startI, const std::vector<int> &thejInts, int &startF, const std::vector<double> &thejDoubles);
void setArraysForOutput(const std::vector<int> &theJInts, const std::vector<double> &theJDoubles);
void setArraysForInput(const std::vector<int> &theJInts, const std::vector<double> &theJDoubles);
void outputDRDAGToArray(List<Cluster> &theCluster, int& index, const std::vector<int> &theInts, bool first);


#endif