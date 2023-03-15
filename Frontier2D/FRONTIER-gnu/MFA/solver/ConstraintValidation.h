#ifndef MFA_SOLVER_CONSTRAINT_VALIDATION_H
#define MFA_SOLVER_CONSTRAINT_VALIDATION_H

#include "entities/Vertex.h"
#include "entities/Graph.h"

//Imaginary Constraint recognition functions
bool isValidPointIncidence(Vertex &thePoint, Vertex &theOther, int part1, int part2);
bool isValidLineIncidence(Vertex &theLine, Vertex &theOther, int part1, int part2);
bool isValidRayIncidence(Vertex &theRay, Vertex &theOther, int part1, int part2);
bool isValidLSIncidence(Vertex &theLS, Vertex &theOther, int part1, int part2);
bool isValidCircleIncidence(Vertex &theCircle, Vertex &theOther, int part1, int part2);
bool isValidArcIncidence(Vertex &theArc, Vertex &theOther, int part1, int part2);
bool isImaginary(Graph &graph0, Edge &theEdge);


#endif