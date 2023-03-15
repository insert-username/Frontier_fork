#ifndef MFA_SOLVER_PRINT_UTIL_H
#define MFA_SOLVER_PRINT_UTIL_H


//Printing Functions
void print(Graph &graph0, List<Cluster> &SolverTrees);
void print(Graph &graph0, Cluster &theCluster);
void printBifurcations(Cluster &theCluster);
void generateOutput(Graph &graph0, Cluster &theCluster, std::ostream &outfile, bool printNum);
void updateGraph(Graph &graph0, Cluster &theCluster);
void outputDRDAG(List<Cluster> &theCluster, std::ostream &output, bool first);
void outputFinState(Cluster &theCluster, int &startI, int*& theInts, int &count);


#endif