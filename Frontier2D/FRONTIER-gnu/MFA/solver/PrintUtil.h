#ifndef MFA_SOLVER_PRINT_UTIL_H
#define MFA_SOLVER_PRINT_UTIL_H

#include "entities/Cluster.h"
#include "entities/Graph.h"

class PrintUtil {
public:
    //Printing Functions
    static void print(Graph &graph0, List<Cluster> &SolverTrees);
    static void print(Graph &graph0, Cluster &theCluster);
    static void printBifurcations(Cluster &theCluster);
    static void generateOutput(Graph &graph0, Cluster &theCluster, std::ostream &outfile, bool printNum);
//void updateGraph(Graph &graph0, Cluster &theCluster);
    static void outputDRDAG(List<Cluster> &theCluster, std::ostream &output, bool first);
    static void outputFinState(Cluster &theCluster, int &startI, int*& theInts, int &count);
};



#endif