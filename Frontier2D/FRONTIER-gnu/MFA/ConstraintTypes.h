#ifndef CONSTRAINT_TYPES_H
#define CONSTRAINT_TYPES_H

#include <stdexcept>
#include <sstream>

class ConstraintType {
public:
    static void checkValidValue(const int& value) {
        if (value < 0 || value > 5) {
            std::stringstream ss;
            ss << "Illegal constraint value: " << value;

            throw std::runtime_error(ss.str());
        }
    }
};

class ConstraintTypeIDs {

public:
    static const int DISTANCE = 0;
    static const int INCIDENCE = 1;
    static const int PERPENDICULARITY = 2;
    static const int PARALLELISM = 3;
    static const int ANGLE = 4;
    static const int TANGENT = 5;
};

#endif