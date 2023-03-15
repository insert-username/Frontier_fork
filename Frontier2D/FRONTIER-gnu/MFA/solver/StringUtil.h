#ifndef MFA_SOLVER_STRING_UTIL_H
#define MFA_SOLVER_STRING_UTIL_H

#include "entities/List.h"

#include <string>

//String Functions
std::string getSubString(std::string theString, int pos, int length);
std::string replaceAll(std::string toReplace, std::string theReplace, std::string replaceWith);
std::string getVarString(const List<std::string>& vars);
std::string toString(long a);
std::string toString(int a);
std::string toString(double a);
std::string toString(float a);


#endif