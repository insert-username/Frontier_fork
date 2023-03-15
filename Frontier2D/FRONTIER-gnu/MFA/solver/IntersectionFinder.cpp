#include "solver/IntersectionFinder.h"

#include "entities/Vertex.h"

//returns in Vertex, the point of intersection between line1 and line2
void getIntersectionLineLine(Vertex &theVertex, float x1, float y1, float m1, float x2, float y2, float m2)
{
    float Xout, Yout;
    float Xnew, Ynew;

    Xnew=x2-x1;
    Ynew=y2-y1;

    Xout=(m2*Xnew+Ynew)/(m1-m2);
    Yout=Xout*m1;

    Xout+=x1;
    Yout+=y1;

    theVertex.setValues(0,Xout);
    theVertex.setValues(1,Yout);
    theVertex.setType(0);
}

//returns in thePoints the two point of intersection between a line and a circle
void getIntersectionLineCircle(List<Vertex> &thePoints, float x1, float y1, float m1, float x2, float y2, float r)
{
    float Xout1, Yout1, Xout2, Yout2;
    float Xnew, Ynew;
    float deter;

    Xnew=x2-x1;
    Ynew=y2-y1;

    deter=-y2-pow(m1,2)*pow(x1,2)+2*y1*m1*x1+pow(r,2)+pow(m1,2)*pow(r,2);

    if(deter<0)
    {
        thePoints.retrieve(1).setType(-1);
        return;
    }

    Xout1=pow(m1,2)*x1+y1*m1+sqrt(deter)+x1;
    Xout2=pow(m1,2)*x1+y1*m1-sqrt(deter)+x2;
    Yout1=m1*Xout1;
    Yout2=m1*Xout2;

    thePoints.retrieve(1).setType(0);
    thePoints.retrieve(1).setValues(0,Xout1);
    thePoints.retrieve(1).setValues(1,Yout1);
    thePoints.retrieve(2).setType(0);
    thePoints.retrieve(2).setValues(0,Xout2);
    thePoints.retrieve(2).setValues(1,Yout2);
}

//returns a vertex list containing the two points of intersection between 2 circles
void getIntersectionCircleCircle(List<Vertex> &thePoints, float x1, float y1, float r1, float x2, float y2, float r2)
{
    float Xout1, Yout1, Xout2, Yout2;
    float Xnew, Ynew;
    float deter, d, e, f;

    Xnew=x2-x1;
    Ynew=y2-y1;

    d=pow(Xnew,2)+pow(Ynew,2);
    e=pow(r1,2)-pow(r2,2);
    f=pow(r1,2)+pow(r2,2);

    deter=-pow(d,2)+2*d*f-pow(e,2);

    if(deter<0)
    {
        thePoints.retrieve(1).setType(-1);
        return;
    }

    Xout1=(Xnew*(e+d)-Ynew*sqrt(deter))/(2*d)+x1;
    Xout2=(Xnew*(e+d)+Ynew*sqrt(deter))/(2*d)+x1;
    Yout1=(Ynew*(e+d)-Xnew*sqrt(deter))/(2*d)+y1;
    Yout2=(Ynew*(e+d)-Xnew*sqrt(deter))/(2*d)+x1;

    thePoints.retrieve(1).setType(0);
    thePoints.retrieve(1).setValues(0,Xout1);
    thePoints.retrieve(1).setValues(1,Yout1);
    thePoints.retrieve(2).setType(0);
    thePoints.retrieve(2).setValues(0,Xout2);
    thePoints.retrieve(2).setValues(1,Yout2);
}
