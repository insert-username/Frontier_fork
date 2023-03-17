#ifndef UTU_UTU_H
#define UTU_UTU_H

#include <vector>

class UTU {
public:

    /**
     * Previously the UTU accepted an int/double array as input. for c++ const vectors by ref can be used.
     * Keeping the name to make searching easier.
     */
    static void Java_utuJava_utuC(std::vector<int> &arrInt, std::vector<double> &arrDbl);

};

#endif