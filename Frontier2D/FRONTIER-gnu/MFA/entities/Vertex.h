#ifndef MFA_VERTEX_H
#define MFA_VERTEX_H

#include "List.h"
#include <iostream>

class Vertex
{
private:
    int name;           // unique object ID
    int type;           // point=0,line=1,ray=2,LSeg=3,circle=4,arc=5,cluster=-1
    int weight;         // degree of freedom
    float degValues[8]; // solved values of this object
    int freeDegs[8];    //if freeDegs[i]=0, solver can change degValues[i]
    //if freeDegs[i]=1, degValues[i] is changed by solver
    //if freeDegs[i]=-1, degValues[i] is fixed by user
    //if freeDegs[i]=-2, degValues[i] is NA(Not Applicable) or not defined


    //Table shows the different types of degrees of freedom used for each object type and
    //the degvalue location where it is stored
    /******************************************************************\
    *              point line   Ray   LineSeg circle  arc      cluster *
    *  degValue[0]   x    x1     x1     x1      x      x        x-sft  *
    *  degValue[1]   y    y1     y1     y1      y      y        y-sft  *
    *  degValue[2]   -    x2     x2     x2      r      r       rot-cos *
    *  degValue[3]   -    y2     y2     y2      -    angle     rot-sin *
    *  degValue[4]   -  slope   cos   slope     -  start_cos      -    *
    *  degValue[5]   -          sin   length    -  start_sin      -    *
    *  degValue[6]   -    -      -      -       -   end_cos       -    *
    *  degValue[7]   -    -      -      -       -   end_sin       -    *
    \******************************************************************/

    List<int> incid;  // List of the ID's of the inicident edges
    int nIncid;       // number of incident edges
    int label;        // 0 or 1 used for augmenting path
    int scan;         // 0 or 1 used for augmenting path
    int frozen;       // If frozen=1, we don't distribute; If frozen=2, frozen forever
    int posFlow;      // possible flow that could be delivered to this vertex
    int existFlow;    // flow already absorbed by this vertex
    int predEdge;     // predecessor edge in the augmenting path
    int depth;        // depth==0 for original vertex

public:

    //Constructors

    //default initializes all values to zero
    Vertex();

    //sets the ID of vertex to theName, and theType to theType
    Vertex(int theName, int theType);

    //copy constructor
    Vertex(const Vertex &);

    void setName(int nm) { name=nm; }
    void setType(int tp) { type=tp; }
    void setWeight(int wt) { weight=wt; }
    void setPosFlow(int fl) { posFlow=fl; }
    void setEstFlow(int fl) { existFlow=fl; }
    void setPredEdg(int e) { predEdge=e; }
    void setDepth(int d) { depth=d; }
    void notDefinedValue(int ix) { freeDegs[ix]=-2; }
    void initialValue(int ix, float value, int free) {
        degValues[ix]=value;
        freeDegs[ix]=free;
    }
    bool setValues(int ix, float value) {
        if(ix>=8 || freeDegs[ix] < 0 )  // solver cannot change degValues[ix]
            return false;
        freeDegs[ix]=1;
        degValues[ix]=value;
    }
    void appendIncid(int edge_name) { incid.append(edge_name); nIncid++; }
    void setLabel(int lb) { label=lb; }
    void setScan(int sc) { scan=sc; }
    void freeze() { if(frozen==0) frozen=1; } // can be defrosted
    void freezeForever() { frozen=2; }        // can't be defrosted
    void incrsWt(int wt) { weight=weight+wt; }
    void incrsEstFlow(int fl) { existFlow=existFlow+fl; }
    void incrsPosFlow(int fl) { posFlow=posFlow+fl; }
    void delIncid(int e) { if(incid.hasElem(e)) {incid.deleteElem(e);nIncid--;}}
    void defrost() { if(frozen==1) frozen=0; }  // if frozen=2, it can't be defrosted
    int returnName() { return name; }
    int returnType() { return type; }
    int returnWeight() { return weight; }
    bool hasFreeDeg() {
        for(int i=0; i<8; i++) if(freeDegs[i]>=0) return true;
        return false;
    }
    int returnFreeDegs(int index) { return freeDegs[index]; }
    float returnDegValueByName(int index) { return degValues[index]; }
    int returnIncid(int index) { return incid.retrieve(index); }
    int numIncid() { return nIncid; }
    bool hasIncid(int e) { return incid.hasElem(e); }
    int returnLabel() { return label; }
    int returnScan() { return scan; }
    int isFrozen() { return frozen; }
    int returnEstFlow() { return existFlow; }
    int returnPosFlow() { return posFlow; }
    int returnPredEdg() { return predEdge; }
    int returnDepth() { return depth; }

    //overridden = operator
    Vertex & operator = (const Vertex &ver);

    //two vertices are equal if they have the same name and weight
    inline bool operator == (const Vertex &v) {
        if(name==v.name&&weight==v.weight) return true;
        else return false;
    }

    //stream output
    friend std::ostream& operator <<(std::ostream&, const Vertex&);
};



#endif