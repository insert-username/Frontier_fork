#ifndef MFA_SOLVER_INTERSECTION_FINDER_H
#define MFA_SOLVER_INTERSECTION_FINDER_H

#include "entities/Vertex.h"

//Intersection finders
void getIntersectionLineLine(Vertex &theVertex, float x1, float y1, float m1, float x2, float y2, float m2);
void getIntersectionLineCircle(List<Vertex> &thePoints, float x1, float y1, float m1, float x2, float y2, float r);
void getIntersectionCircleCircle(List<Vertex> &thePoints, float x1, float y1, float r1, float x2, float y2, float r2);


#endif