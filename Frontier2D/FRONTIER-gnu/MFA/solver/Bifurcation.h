#ifndef MFA_SOLVER_BIFURCATION_H
#define MFA_SOLVER_BIFURCATION_H


//Bifurcation functions
bool isValidConstraint(Graph &graph0, Edge &theEdge, Cluster &theCluster);
bool validBifur(std::string theString);
void setBifurs(std::string filename, Cluster& theCluster, bool usingfSolve);
void parseBifurString(Graph &graph0, Cluster &theCluster);
bool getNextBifurcation(Graph &graph0, Cluster &theCluster);
int selectBifurcation(Graph &graph0, Cluster &theCluster, bool &useFile);
void checkBifurs(Graph &graph0, Cluster &theCluster);


#endif