#include "ConstraintValidation.h"

//given an point and another object, determines if the point has incidence with the object
bool isValidPointIncidence(Vertex &thePoint, Vertex &theOther, int part1, int part2)
{
    int type;
    float angle1a, angle1b, angle2a, angle2b, x1, y1, x2, y2, x3, y3;
    float pointAngle, length1, length2, length;

    if(part1!=1) return false;
    if(part2!=0) return true;

    type=theOther.returnType();

    x1=thePoint.returnDegValueByName(0);
    y1=thePoint.returnDegValueByName(1);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
        case 2:   angle1a=theOther.returnDegValueByName(4);
            angle1b=theOther.returnDegValueByName(5);
            return (getAngle(angle1a,angle1b)==getAngle(x2,y2,x1,y2));
            break;
        case 3:   x3=theOther.returnDegValueByName(2);
            y3=theOther.returnDegValueByName(3);
            length=getDistance(x2,y2,x3,y3);
            length1=getDistance(x1,y1,x2,y2);
            length2=getDistance(x1,y1,x3,y3);
            if(length1<length && length2<length) return true;
            else return false;
            break;
        case 5:   pointAngle=getAngle(x2,y2,x1,y2);
            angle1a=theOther.returnDegValueByName(4);
            angle1b=theOther.returnDegValueByName(5);
            angle2a=theOther.returnDegValueByName(6);
            angle2b=theOther.returnDegValueByName(7);
            if(pointAngle>=getAngle(angle1a,angle1b) && pointAngle<=getAngle(angle2a,angle2b)) return true;
            else return false;
        default:  return true;
            break;
    }
}

//given a line and another object, returns true if the object is incidence about the line
bool isValidLineIncidence(Vertex &theLine, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, m1, x2, y2, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    if(part1!=0) return false;
    if(part2!=0) return true;

    x1=theLine.returnDegValueByName(0);
    y1=theLine.returnDegValueByName(1);
    m1=theLine.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
        case 1:   m2=theOther.returnDegValueByName(4);
            return(m1!=m2);
            break;
        case 2:
        case 3:   if(type==2) m2=(theOther.returnDegValueByName(5)/theOther.returnDegValueByName(4));
            else m2=theOther.returnDegValueByName(4);
            getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
            return(isValidPointIncidence(theInter, theOther,1,0));
            break;
        case 4:   theInters.append(Vert1);
            theInters.append(Vert2);
            r2=theOther.returnDegValueByName(2);
            getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
            return(theInters.retrieve(1).returnType()!=-1);
            break;
        case 5:   theInters.append(Vert1);
            theInters.append(Vert2);
            r2=theOther.returnDegValueByName(2);
            getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
            if(theInters.retrieve(1).returnType()==-1)  return false;
            out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
            out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
            return(out1 || out2);
            break;
        default:  return true;
    }
}

//given a ray and another object, returns true if the object is incident upon the ray
bool isValidRayIncidence(Vertex &theRay, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, x2, y2, x3, y3, m1, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theRay.returnDegValueByName(0);
    y1=theRay.returnDegValueByName(1);
    m1=theRay.returnDegValueByName(5)/theRay.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
        case 2:   switch(part1*10+part2)
            {
                case 0:  m2=theOther.returnDegValueByName(5)/theOther.returnDegValueByName(4);
                    getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
                    out1=isValidPointIncidence(theInter, theOther,1,0);
                    out2=isValidPointIncidence(theInter, theRay,1,0);
                    return(out1 && out2);
                    break;
                case 1:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 10: Vert1.setType(0);
                    Vert1.setValues(0,x1);
                    Vert1.setValues(1,y1);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 11: return true;
                    break;
            }
            break;
        case 3:   switch(part1*10+part2)
            {
                case 0:  m2=theOther.returnDegValueByName(4);
                    getIntersectionLineLine(theInter, x1, y1, m1, x2, y2, m2);
                    out1=isValidPointIncidence(theInter, theOther,1,0);
                    out2=isValidPointIncidence(theInter, theRay,1,0);
                    return(out1 && out2);
                    break;
                case 1:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 2:  x3=theOther.returnDegValueByName(2);
                    y3=theOther.returnDegValueByName(3);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 10: Vert1.setType(0);
                    Vert1.setValues(0,x1);
                    Vert1.setValues(1,y1);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 11:
                case 12: return true;
                    break;
            }
            break;
        case 4:   switch(part1*10+part2)
            {
                case 0:  theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theRay,1,0);
                    out2=isValidPointIncidence(theInters.retrieve(2), theRay,1,0);
                    return(out1 || out2);
                case 3:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 10:
                case 13: return true;
                    break;
            }
            break;
        case 5:   switch(part1*10+part2)
            {
                case 0:  theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1, y1, m1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                    out1=(out1 && isValidPointIncidence(theInters.retrieve(1), theRay,1,0));
                    out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                    out2=(out2 && isValidPointIncidence(theInters.retrieve(2), theRay,1,0));
                    return(out1 || out2);
                    break;
                case 1:  r2=theOther.returnDegValueByName(2);
                    x3=x2+r2*theOther.returnDegValueByName(4);
                    y3=y2+r2*theOther.returnDegValueByName(5);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 2:  r2=theOther.returnDegValueByName(2);
                    x3=x2+r2*theOther.returnDegValueByName(6);
                    y3=y2+r2*theOther.returnDegValueByName(7);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 3:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theRay,1,0));
                    break;
                case 10: Vert1.setType(0);
                    Vert1.setValues(0,x1);
                    Vert1.setValues(1,y1);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 11:
                case 12:
                case 13: return true;
                    break;
            }
        default:  return true;
    }
}

//given a line segment and another object, returns true if the object is incident with the line segment
bool isValidLSIncidence(Vertex &theLS, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1a, y1a, x1b, y1b, x2, y2, x3, y3, m1, m2, r2;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1a=theLS.returnDegValueByName(0);
    y1a=theLS.returnDegValueByName(1);
    x1b=theLS.returnDegValueByName(2);
    y1b=theLS.returnDegValueByName(3);
    m1=theLS.returnDegValueByName(4);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);

    switch(type)
    {
        case 3:   switch(part1*10+part2)
            {
                case 0:  m2=theOther.returnDegValueByName(4);
                    getIntersectionLineLine(theInter, x1a, y1a, m1, x2, y2, m2);
                    out1=isValidPointIncidence(theInter, theOther,1,0);
                    out2=isValidPointIncidence(theInter, theLS,1,0);
                    return(out1 && out2);
                    break;
                case 1:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 2:  x3=theOther.returnDegValueByName(2);
                    y3=theOther.returnDegValueByName(3);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 10: Vert1.setType(0);
                    Vert1.setValues(0,x1a);
                    Vert1.setValues(1,y1a);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 20: Vert1.setType(0);
                    Vert1.setValues(0,x1b);
                    Vert1.setValues(1,y1b);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 21:
                case 11:
                case 12: return true;
                    break;
            }
            break;
        case 4:   switch(part1*10+part2)
            {
                case 0:  theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1a, y1a, m1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theLS,1,0);
                    out2=isValidPointIncidence(theInters.retrieve(2), theLS,1,0);
                    return(out1 || out2);
                case 3:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 10:
                case 20:
                case 13:
                case 23: return true;
                    break;
            }
            break;
        case 5:   switch(part1*10+part2)
            {
                case 0:  theInters.append(Vert1);
                    theInters.append(Vert2);
                    r2=theOther.returnDegValueByName(2);
                    getIntersectionLineCircle(theInters, x1a, y1a, m1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                    out1=(out1 && isValidPointIncidence(theInters.retrieve(1), theLS,1,0));
                    out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                    out2=(out2 && isValidPointIncidence(theInters.retrieve(2), theLS,1,0));
                    return(out1 || out2);
                    break;
                case 1:  r2=theOther.returnDegValueByName(2);
                    x3=x2+r2*theOther.returnDegValueByName(4);
                    y3=y2+r2*theOther.returnDegValueByName(5);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 2:  r2=theOther.returnDegValueByName(2);
                    x3=x2+r2*theOther.returnDegValueByName(6);
                    y3=y2+r2*theOther.returnDegValueByName(7);
                    Vert1.setType(0);
                    Vert1.setValues(0,x3);
                    Vert1.setValues(1,y3);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 3:  Vert1.setType(0);
                    Vert1.setValues(0,x2);
                    Vert1.setValues(1,y2);
                    return(isValidPointIncidence(Vert1,theLS,1,0));
                    break;
                case 10: Vert1.setType(0);
                    Vert1.setValues(0,x1a);
                    Vert1.setValues(1,y1a);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 20: Vert1.setType(0);
                    Vert1.setValues(0,x1b);
                    Vert1.setValues(1,y1b);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 11:
                case 12:
                case 13:
                case 21:
                case 22:
                case 23: return true;
                    break;
            }
        default:  return true;
    }
}

//given a circle and another object, returns true if the object is incident upon the circle
bool isValidCircleIncidence(Vertex &theCircle, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, r1, x2, y2, x3, y3, m2, r2, sep;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theCircle.returnDegValueByName(0);
    y1=theCircle.returnDegValueByName(1);
    r1=theCircle.returnDegValueByName(2);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);
    r2=theOther.returnDegValueByName(2);

    switch(type)
    {
        case 4:   switch(part1*10+part2)
            {
                case 0:  sep=getDistance(x1,y1,x2,y2);
                    if((sep+r1)<r2 || (sep+r2)<r1) return false;
                    else return(getDistance(x1,y1,x2,y2)<=(r1+r2));
                    break;
                case 3:
                case 10:
                case 30: return true;
                    break;
            }
            break;
        case 5:   switch(part1*10+part2)
            {
                case 0:  theInters.append(Vert1);
                    theInters.append(Vert2);
                    getIntersectionCircleCircle(theInters, x1, y1, r1, x2, y2, r2);
                    if(theInters.retrieve(1).returnType()==-1)  return false;
                    out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
                    out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
                    return(out1 || out2);
                    break;
                case 30: Vert1.setType(0);
                    Vert1.setValues(0,x1);
                    Vert1.setValues(1,y1);
                    return(isValidPointIncidence(Vert1,theOther,1,0));
                    break;
                case 1:
                case 2:
                case 3:
                case 31:
                case 32:
                case 33: return true;
                    break;
            }
        default:  return true;
    }
}

//given an arc and another object, returns true if the object is incident on the arc
bool isValidArcIncidence(Vertex &theArc, Vertex &theOther, int part1, int part2)
{
    int type;
    float x1, y1, r1, x2, y2, x3, y3, m2, r2, sep;
    Vertex theInter, Vert1, Vert2;
    List<Vertex> theInters;
    bool out1, out2;

    x1=theArc.returnDegValueByName(0);
    y1=theArc.returnDegValueByName(1);
    r1=theArc.returnDegValueByName(2);
    x2=theOther.returnDegValueByName(0);
    y2=theOther.returnDegValueByName(1);
    r2=theOther.returnDegValueByName(2);

    switch(part1*10+part2)
    {
        case 0:  sep=getDistance(x1,y1,x2,y2);
            if((sep+r1)<r2 || (sep+r2)<r1) return false;
            if(sep>(r1+r2)) return false;
            theInters.append(Vert1);
            theInters.append(Vert2);
            getIntersectionCircleCircle(theInters, x1, y1, r1, x2, y2, r2);
            if(theInters.retrieve(1).returnType()==-1)  return false;
            out1=isValidPointIncidence(theInters.retrieve(1), theOther,1,0);
            out1=out1 && isValidPointIncidence(theInters.retrieve(1), theArc,1,0);
            out2=isValidPointIncidence(theInters.retrieve(2), theOther,1,0);
            out2=out2 && isValidPointIncidence(theInters.retrieve(2), theArc,1,0);
            return(out1 || out2);
            break;
        case 1:  x3=x2+r2*theOther.returnDegValueByName(4);
            y3=y2+r2*theOther.returnDegValueByName(5);
            Vert1.setType(0);
            Vert1.setValues(0,x3);
            Vert1.setValues(1,y3);
            return(isValidPointIncidence(Vert1,theArc,1,0));
            break;
        case 2:  x3=x2+r2*theOther.returnDegValueByName(6);
            y3=y2+r2*theOther.returnDegValueByName(7);
            Vert1.setType(0);
            Vert1.setValues(0,x3);
            Vert1.setValues(1,y3);
            return(isValidPointIncidence(Vert1,theArc,1,0));
            break;
        case 3:  Vert1.setType(0);
            Vert1.setValues(0,x2);
            Vert1.setValues(1,y2);
            return(isValidPointIncidence(Vert1,theArc,1,0));
            break;
        case 10: x3=x1+r1*theOther.returnDegValueByName(4);
            y3=y1+r1*theOther.returnDegValueByName(5);
            Vert1.setType(0);
            Vert1.setValues(0,x3);
            Vert1.setValues(1,y3);
            return(isValidPointIncidence(Vert1,theOther,1,0));
            break;
        case 20: x3=x1+r1*theOther.returnDegValueByName(6);
            y3=y1+r1*theOther.returnDegValueByName(7);
            Vert1.setType(0);
            Vert1.setValues(0,x3);
            Vert1.setValues(1,y3);
            return(isValidPointIncidence(Vert1,theOther,1,0));
            break;
        case 30: Vert1.setType(0);
            Vert1.setValues(0,x1);
            Vert1.setValues(1,y1);
            return(isValidPointIncidence(Vert1,theOther,1,0));
            break;
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23:
        case 31:
        case 32:
        case 33: return true;
            break;
    }
}

// given the edge, if it is not imaginary returns true (if the constraint
// is not imaginary if must be satisfied after solution, otherwise the
// method calls the correct isValidIncidence method from above and returns
// true only if the incidence is met with the current values
bool isValidConstraint(Graph &graph0, Edge &theEdge, Cluster &theCluster)
{
    Vertex end1, end2;
    int v1, v2, type1, type2, part1, part2;

    v1=theEdge.returnEnd1();
    v2=theEdge.returnEnd2();

    part1=theEdge.returnPart1();
    part2=theEdge.returnPart2();

    end1=graph0.returnVertByName(v1);
    end2=graph0.returnVertByName(v2);

    type1=end1.returnType();
    type2=end2.returnType();

    if(!isImaginary(graph0, theEdge)) return true;

    switch(type1*10+type2)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:  return(isValidPointIncidence(end1,end2,part1,part2));
            break;
        case 10:
        case 20:
        case 30:
        case 40:
        case 50: return(isValidPointIncidence(end2,end1,part2,part1));
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15: return(isValidLineIncidence(end1,end2,part1,part2));
            break;
        case 21:
        case 31:
        case 41:
        case 51: return(isValidLineIncidence(end2,end1,part2,part1));
            break;
        case 22:
        case 23:
        case 24:
        case 25: return(isValidRayIncidence(end1,end2,part1,part2));
            break;
        case 32:
        case 42:
        case 52: return(isValidRayIncidence(end2,end1,part2,part1));
            break;
        case 33:
        case 34:
        case 35: return(isValidLSIncidence(end1,end2,part1,part2));
            break;
        case 43:
        case 53: return(isValidLSIncidence(end2,end1,part2,part1));
            break;
        case 44:
        case 45: return(isValidCircleIncidence(end1,end2,part1,part2));
            break;
        case 54: return(isValidCircleIncidence(end2,end1,part2,part1));
            break;
        case 55: return(isValidArcIncidence(end1,end2,part1,part2));
            break;
    }
}


//returns true if the constraint theEdge, is imaginary and must be checked to see if it is satisfied
bool isImaginary(Graph &graph0, Edge &theEdge)
{
    int end1Type, end2Type;
    int part1, part2;

    if(theEdge.returnType()!=1) return false;

    if(theEdge.returnName()==999)
    {
        end1Type=type1;
        end2Type=type2;
    }
    else
    {
        end1Type=graph0.returnVertByName(theEdge.returnEnd1()).returnType();
        end2Type=graph0.returnVertByName(theEdge.returnEnd2()).returnType();
    }
    part1=theEdge.returnPart1();
    part2=theEdge.returnPart2();

    if(part1!=0 && part2!=0) return false;

    switch(end1Type*10+end2Type)
    {
        case 0:
        case 2:
        case 3:
        case 5:
        case 20:
        case 22:
        case 23:
        case 25:
        case 30:
        case 32:
        case 33:
        case 35:
        case 50:
        case 52:
        case 53:
        case 55:  return(part1==0 || part2==0);
            break;
        case 15:
        case 42:
        case 43:
        case 45:  return(part2==0);
            break;
        case 51:
        case 24:
        case 34:
        case 54:  return(part1==0);
            break;
        default:  return(part1==0 && part2==0);
            break;
    }
    return false;
}
