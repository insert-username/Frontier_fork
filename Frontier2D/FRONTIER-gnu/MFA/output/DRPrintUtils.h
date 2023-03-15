#ifndef MFA_DR_PRINT_UTILS_H
#define MFA_DR_PRINT_UTILS_H

#include <iostream>
#include "entities/Cluster.h"

class DRPrintUtils {
public:

    static void printTree(Cluster &DR_Tree, std::ostream &os, int indent);

    static void printForest(List<Cluster> &DR_Trees, std::ostream &os, int indent);
};

#endif