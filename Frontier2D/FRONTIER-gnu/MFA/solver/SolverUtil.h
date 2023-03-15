#ifndef SOLVER_UTIL_H
#define SOLVER_UTIL_H


//Utility Functions
float getDistance(float x1, float y1, float x2, float y2);
float getAngle(float x1, float y1, float x2, float y2);
float getAngle(float theCos, float theSin);
int getChildIndexByName(Cluster &theCluster, int vName);
int getChildNameWithVertex(Cluster &theCluster, int theV);
bool inOriginalV(int Name, Cluster &theCluster);
void setValue(Vertex &theVertex, int index, float theValue);
void getContainedChildList(Cluster &theCluster, int vName, List<int> &theList);
void setValue(Cluster &theCluster, int ID, float theValue, int index);
int getFreeDegIndex(char keyChar, Vertex &theVertex);
void resetFinByClusterName(Cluster &theCluster, int name);
void resetTrees(Graph &graph0, List<Cluster> &SolverTrees);
void setValueReduction(Graph &graph0, Cluster &theCluster);
void fixCluster(Cluster &theCluster);
void fixTree(List<Cluster> &SolverTrees);


#endif