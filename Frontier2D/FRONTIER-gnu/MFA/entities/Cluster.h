#ifndef MFA_CLUSTER_H
#define MFA_CLUSTER_H

#include "List.h"
#include "Vertex.h"
#include "Edge.h"
#include "DegValues.h"

#include <string>
#include <iostream>

class Cluster
{
    using string = std::string;

private:
    Vertex core;           // core vertex formed by the reduction of all the interior vertices in the cluster
    int group;             // group ID is specified by user
    List<int> frontiers;   // list of the frontier edges in the cluster by ID
    List<Edge> innerE;     // list of edges between core and frontiers
    List<Edge> outerE;     // list of edges between frontiers
    List<int> originalV;   // list of vertices corresponding to original graph
    List<string> bifurs;   // list of the strings representing the bifurcations of this cluster
    int constrain;         // 1--well constrained, 2,3,...--over constrained
    int numBifurs;         // lenght of the list bifurs
    int currBifur;         // the currently selected bifurcation, -1 means no selection
    bool solved;           // true if the cluster has been solved before
    int currDeg;           // the index of the currect set of DegValues;
    List<DegValues> values;// same format and use as in Vertex

public:
    List<Cluster> children;    // list of children

    //Default constructor sets all values to default, note that the default value of the degValues[2] is 1
    //since a zero rotation has a cosine of 1
    Cluster() {
        int i,j;
        DegValues first;

        first.setValue(2,1.0);
        group=0;
        constrain=0;
        currBifur=-1;
        numBifurs=0;
        solved=false;
        currDeg=1;
        values.append(first);

    }

    //intializes a cluster in group g
    Cluster(int g) { group=g; }

    //copy constructor
    Cluster(const Cluster &);

    void setName(int n) { core.setName(n); }
    void setGroup(int g) { group=g; }
    void setBifurs(List<string>& theBifurs) { bifurs=theBifurs; numBifurs=theBifurs.returnLen(); }
    void setCurrBifur(int theNum) { currBifur=theNum; }
    void setCore(Vertex &c) { core=c; }
    void setConst(int c) { constrain=c; }
    void setValue(int index, float value) { values.retrieve(currDeg).setValue(index,value); }
    void setSolved(bool sol) { solved=sol; }
    void setCurrDeg(int i) { currDeg=i; }
    void setDegValues(List<DegValues> theValues) { values=theValues; }

    //adds a new set of degvalues to this cluster in the case that this cluster has multiple parents
    void setToCluster(Cluster &theCluster)
    {
        int i, length, name;
        bool found;
        DegValues theNew;

        length=values.returnLen();
        name=theCluster.returnName();

        if(length==1 && values.retrieve(1).returnValue(7)==0)
        {
            currDeg=1;
            values.retrieve(currDeg).setValue(7,name);
        }
        else
        {
            found=false;
            for(i=1;i<=length;i++)
                if(values.retrieve(i).returnValue(7)==name)
                {
                    found=true;
                    currDeg=1;
                }
            if(!found)
            {
                theNew.setValue(2,1.0);
                values.append(theNew);
                currDeg=length+1;
                values.retrieve(currDeg).setValue(7,name);
            }
        }
    }

    //creates a new cluster
    void formCl (Vertex &,List<int> &,List<Edge> &,List<Edge> &,List<int>&);

    void addOrig(int v) { if(!originalV.hasElem(v)) originalV.append(v); }
    void addFrot(int v) { if(!frontiers.hasElem(v)) frontiers.append(v); }
    void deleteOrig(int ver) { originalV.deleteElem(ver); }
    void deleteFrot(int ver) { frontiers.deleteElem(ver); }
    void makeEmpty()
    {
        int i,j;
        DegValues first;

        core.setName(0);
        group=0;
        frontiers.makeEmpty();
        innerE.makeEmpty();
        outerE.makeEmpty();
        originalV.makeEmpty();
        children.makeEmpty();
        bifurs.makeEmpty();
        currBifur=-1;
        numBifurs=0;
        constrain=0;
        solved=false;
        values.makeEmpty();
        currDeg=1;
        first.setValue(2,1.0);
        values.append(first);
    }

    Vertex & returnCore() {  return core; }
    int returnName() {return core.returnName(); }
    int returnGroup() { return group; }
    int returnConst() { return constrain; }
    int returnCurrDeg() { return currDeg; }
    int returnNumDeg() { return values.returnLen(); }
    float returnValue(int index) { return values.retrieve(currDeg).returnValue(index); }
    int returnNumBifurs() { return numBifurs; }
    int returnCurrBifur() { return currBifur; }
    string        returnCurrBifurString() { return bifurs.retrieve(currBifur); }
    List<string>& returnBifurs() { return bifurs; }
    List<int>&    returnFrontiers() { return frontiers; }
    int           returnFronLen() { return frontiers.returnLen(); }
    int 	         returnFrontier(int index) { return frontiers.retrieve(index); }
    List<Edge>&   returnInnerE() { return innerE; }
    int 	         returnInnerELen() { return innerE.returnLen(); }
    Edge& 	 returnInnerE(int index) { return innerE.retrieve(index); }
    List<Edge>&   returnOuterE() { return outerE; }
    int           returnOuterELen() { return outerE.returnLen(); }
    Edge&	 returnOuterE(int index) { return outerE.retrieve(index); }
    List<int>&    returnOrig() { return originalV; }
    int           returnOrigV(int index) { return originalV.retrieve(index); }
    int           returnOrigLen() { return originalV.returnLen(); }
    bool          isSolved() { return solved; }


    bool empty() {
        if(core.returnName()==0&&group==0&&children.empty())
            return true;
        else return false;
    }

    //clusters are equal if they have the same name and are in the same group
    inline bool operator == (const Cluster &clu)
    {
        if(core==clu.core && group==clu.group) return true;
        else return false;
    }

    // = operator
    Cluster & operator = (const Cluster &clu);

    // a cluster is a sub cluster of another if all of its original vertices are original vertices in the
    // larger cluster
    bool operator <= (Cluster &cl0)  // subset operator
    {
        int i, nFront, nOrig0;
        List<int> origV0;

        nFront=frontiers.returnLen();
        origV0=cl0.returnOrig();
        nOrig0=origV0.returnLen();
        if(nFront>nOrig0||nFront<2) return false;

        for(i=1;i<=nFront;i++)
        {
            if(! origV0.hasElem(frontiers.retrieve(i)) )  return false;
        }
        return true;
    }

    // stream output
    void output(std::ostream&);
};



#endif