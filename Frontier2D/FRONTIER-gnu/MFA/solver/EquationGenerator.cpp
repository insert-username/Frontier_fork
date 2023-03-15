#include "solver/EquationGenerator.h"


//for every edge in graph0 in theCluster, gets a corresponding equation string
std::string getEquation(Graph &graph0, Edge &theEdge, Cluster &theCluster)
{
    int temp;
    std::string output;

    temp = getEdgeCode(theEdge, theCluster);

    if(temp==1) output+=getEquationBetweenClusters(graph0, theEdge, theCluster);
    if(temp==0) output+=getEquationInCluster(graph0, theEdge, theCluster);
    return output;
}


//Ray objects use the sine and cosine of the angle of the ray compared to the positive x
//axis as degrees of freedom in there equation sets.  This method generates additional equations
//that insure that the squares of these sine and cosine values alway sum to 1
std::string getSinCosConstraint()
{
    int i, length;
    std::string temp, temp2, clust;
    std::string output="";

    length=vars.returnLen();

    for(i=1;i<=length;i++)
    {
        temp=vars.retrieve(i);
        temp2=temp;
        clust=temp2.erase(0,1);
        if(temp[0]=='w') output+=",1=(w"+clust+"^2+v"+clust+"^2)";
        if(temp[0]=='a') output+=",1=(a"+clust+"^2+b"+clust+"^2)";
    }
    return output;
}

//Within the DRTree used by this solver, original vertices in more than one cluster have
//duplicate datastructures in each of the clusters that contain them.  At the time of solution,
//the implied constaint that each of these duplicated objects, must actually be the same, must be
//resolved.  To assure this, additional equations are added, stating that for each of these
//overlaps, some rotation and translation of one cluster must bring all these duplicate points
//into the correct positions.
std::string getOverlapConstraint(Graph &graph0, Cluster &theCluster)
{
    int i, j, length, type, currOrig;
    int clustint1, clustint2;
    std::string cluster1, cluster2, origStr, nameStr;
    std::string newStr1a, newStr2a, newStr3a, newStr1b, newStr2b, newStr3b;
    std::string clusterV1, clusterV2, clusterV3, clusterV4;
    List<int> theOrig, in, theClust;
    Cluster tempCluster1, tempCluster2;
    Vertex tempVertex1, tempVertex2;
    std::string output="";
    std::string temp="";

    if(theCluster.returnOrig().returnLen()==theCluster.children.returnLen()) return "";

    theOrig=theCluster.returnOrig();

    length=theOrig.returnLen();

    for(i=1; i<=length; i++)
    {
        currOrig=theOrig.retrieve(i);
        getContainedChildList(theCluster,currOrig,in);
        for(j=1; j<in.returnLen();j++)
        {
            type=graph0.returnVertByName(currOrig).returnType();
            clustint1=in.retrieve(j);
            clustint2=in.retrieve(j+1);
            if(clustint1!=withHeldCluster) if(!theClust.hasElem(clustint1)) theClust.append(clustint1);
            if(clustint2!=withHeldCluster) if(!theClust.hasElem(clustint2)) theClust.append(clustint2);
            tempCluster1=theCluster.children.retrieve(getChildIndexByName(theCluster, clustint1));
            tempCluster2=theCluster.children.retrieve(getChildIndexByName(theCluster, clustint2));
            tempVertex1.setName(currOrig);
            tempVertex2.setName(currOrig);
            setValueInCluster(graph0, tempCluster1, tempVertex1);
            setValueInCluster(graph0, tempCluster2, tempVertex2);
            if(aOverlap==0 && clustint1!=withHeldCluster) aOverlap=clustint1;
            else if(aOverlap==0) aOverlap=clustint2;
            cluster1=toString(clustint1);
            cluster2=toString(clustint2);
            origStr=toString(currOrig);
            clusterV1="p"+cluster1;
            clusterV2="q"+cluster1;
            clusterV3="t"+cluster1;
            clusterV4="s"+cluster1;
            if(clustint1==withHeldCluster)
            {
                newStr1a="("+toString(tempVertex1.returnDegValueByName(0))+")";
                newStr2a="("+toString(tempVertex1.returnDegValueByName(1))+")";
                newStr3a="("+toString(tempVertex1.returnDegValueByName(2))+")";
            }
            else
            {
                if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
                if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
                if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
                if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
                newStr1a="(("+toString(tempVertex1.returnDegValueByName(0))+")*"+clusterV3+"-("+toString(tempVertex1.returnDegValueByName(1))+")*"+clusterV4+"+"+clusterV1+")";
                newStr2a="(("+toString(tempVertex1.returnDegValueByName(1))+")*"+clusterV3+"+("+toString(tempVertex1.returnDegValueByName(0))+")*"+clusterV4+"+"+clusterV2+")";
                newStr3a="((("+toString(tempVertex1.returnDegValueByName(2))+")*"+clusterV3+"+"+clusterV4+")/(("+toString(tempVertex1.returnDegValueByName(2))+")*"+clusterV4+"+"+clusterV3+"))";
            }
            clusterV1="p"+cluster2;
            clusterV2="q"+cluster2;
            clusterV3="t"+cluster2;
            clusterV4="s"+cluster2;
            if(clustint2==withHeldCluster)
            {
                newStr1b="("+toString(tempVertex2.returnDegValueByName(0))+")";
                newStr2b="("+toString(tempVertex2.returnDegValueByName(1))+")";
                newStr3b="("+toString(tempVertex2.returnDegValueByName(2))+")";
            }
            else
            {
                if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
                if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
                if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
                if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
                newStr1b="(("+toString(tempVertex2.returnDegValueByName(0))+")*"+clusterV3+"-("+toString(tempVertex2.returnDegValueByName(1))+")*"+clusterV4+"+"+clusterV1+")";
                newStr2b="(("+toString(tempVertex2.returnDegValueByName(1))+")*"+clusterV3+"+("+toString(tempVertex2.returnDegValueByName(0))+")*"+clusterV4+"+"+clusterV2+")";
                newStr3b="((("+toString(tempVertex2.returnDegValueByName(2))+")*"+clusterV3+"+"+clusterV4+")/(("+toString(tempVertex2.returnDegValueByName(2))+")*"+clusterV4+"+"+clusterV3+"))";
            }
            switch(type)
            {
                case 4:
                case 0:  temp+=newStr1a+"="+newStr1b+",";
                    temp+=newStr2a+"="+newStr2b;
                    equationCount+=2;
                    break;
                case 1:  temp+=newStr1a+"="+newStr1b+",";
                    temp+=newStr2a+"="+newStr2b+",";
                    temp+=newStr3a+"="+newStr3b;
                    equationCount+=3;
                    break;
                default: temp="";
                    break;
            }
            output+=","+temp;
            temp="";
        }
        in.makeEmpty();
    }
    if(equationCount==0) output.erase(0,1);
/*    for(i=1;i<=theClust.returnLen();i++)
    {
       temp=toString(theClust.retrieve(i));
       output+=",s"+temp+"^2+t"+temp+"^2=1";
       equationCount++;
    } */
    return output;
}

//the line object, has only two degrees of freedom, its y axis intercept and its slope; however,
//the equations generated for the line object use three degrees of freedom a point and a slope.
//in order to eliminate this additional added degree of freedom, this method generates simple equations
//setting the x coordinate of any line to zero
std::string getLineConstraint(Graph &graph0, List<Cluster> &theChildren)
{
    std::string outString;
    Cluster theCluster;
    Vertex theVert;
    int i, length;
    std::string name;
    std::string intName;
    std::string slopeVar;

    outString="";

    length=theChildren.returnLen();

    for(i=1;i<=length;i++)
    {
        theCluster=theChildren.retrieve(i);
        name="x"+toString(theCluster.returnName());
        if(graph0.hasVert(theCluster.returnName()))
        {
            theVert=graph0.returnVertByName(theCluster.returnName());
            if(theVert.returnType()>0 && theVert.returnType()<3 && vars.hasElem(name))
            {
                outString+=",x"+toString(theCluster.returnName())+"=0";
                equationCount++;
            }
            if(theVert.returnType()==3)
            {
                intName=toString(theCluster.returnName());
                outString+=",(y"+intName+"-d"+intName+")=m"+intName+"*(x"+intName+"-c"+intName+")";
                slopeVar="m"+intName;
                if(!vars.hasElem(slopeVar)) vars.append(slopeVar);
            }
        }
    }
    return outString;
}


//Each solved cluster has three remaining degrees of freedom, two translational degrees and one
//rotation, this program expresses these degrees of freedom in four variables, x translation,
//y translation, sine of rotation angle, and cosine of rotation angle.  To insure that when the
//equations are solved, the relationship between the sine and cosine is maintained, this method
//generated additional equations stating that the sum of the squares of the sine and cosine must
//be 1
std::string getRotationEquations(Cluster &theCluster)
{
    std::string outString, temp;
    int i, length, childName;

    outString="";

    if(theCluster.children.returnLen()==0) return outString;
    if(theCluster.children.returnLen()==theCluster.returnOrigLen()) return outString;

    length=theCluster.children.returnLen();

    for(i=1;i<=length;i++)
    {
        childName=theCluster.children.retrieve(i).returnName();
        if(theCluster.children.retrieve(i).children.returnLen()!=0 && childName!=withHeldCluster)
        {
            temp=toString(childName);
            outString+=",s"+temp+"^2+t"+temp+"^2=1";
            equationCount++;
        }
    }

    return outString;
}

//after all of the equations for a cluster have been generated, if the cluster is rigid, exactly
//three degrees of freedom will still remain.  This method generates simple equations which fix those
//degrees of freedom, so Maple will generate exact answers
std::string getOriginEquations(Edge &theEdge, Graph &graph0, int clusterName)
{
    std::string outString, tempString, secondName;
    Vertex vEnd1, vEnd2;
    int v1Name, v2Name;
    int type1, type2;

    outString="";

    if(clusterName>0)
    {
        tempString=toString(clusterName);
        outString="p"+tempString+"=0,q"+tempString+"=0,t"+tempString+"=0";
        return outString;
    }

    vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
    vEnd2=graph0.returnVertByName(theEdge.returnEnd2());

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    if(type2==2 || type2==1)
        secondName="y";
    else secondName="x";

    switch(type1)
    {
        case 5:
        case 4:
        case 0: if(type2==2 || type2==1)
                secondName="m";
            else secondName="x";
            outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
            break;
        case 3: outString="x"+toString(v1Name)+"=0,y"+toString(v1Name)+"=0,d"+toString(v1Name)+"=0.0001";
            break;
        case 1: outString="y"+toString(v1Name)+"=0,m"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
            break;
        case 2: outString="y"+toString(v1Name)+"=0,w"+toString(v1Name)+"=0,"+secondName+toString(v2Name)+"=0";
            break;
    }
    return(outString);
}


//gets a equation for a edge where both endpoints fall in the same cluster, it simply calls the
//corresponding getEquation method above for each particular edge type.
std::string getEquationInCluster(Graph& graph0, Edge &theEdge, Cluster &theCluster)
{
    std::string outString;
    Vertex vEnd1, vEnd2;

    outString="";

    if(isImaginary(graph0, theEdge)) theImags.append(theEdge);

    if(theEdge.returnName()==999)
    {
        vEnd1=testVertex1;
        vEnd2=testVertex2;
    }
    else
    {
        vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
        vEnd2=graph0.returnVertByName(theEdge.returnEnd2());
    }
    switch(theEdge.returnType()) {

        case 0: outString=getDistanceEQ(vEnd1,vEnd2,theEdge, "");
            break;
        case 1: outString=getIncidenceEQ(vEnd1,vEnd2,theEdge);
            break;
        case 4: outString=getAngleEQ(vEnd1,vEnd2,theEdge);
            break;
        case 3: outString=getParallelEQ(vEnd1,vEnd2,theEdge);
            break;
        case 2: outString=getPerpendicularEQ(vEnd1,vEnd2,theEdge);
            break;
        case 5: outString=getTangencyEQ(vEnd1,vEnd2,theEdge);
            break;
    }

    return(outString);
}


/*  All of the following methods generate the equation strings used to solve the equations in
    Maple.  For each constraint type, distance, incidence, tangent, parallel, angle and
    perpendicular, there is a corresponding method of the form getDistanceEQ etc.  Each method
    simply a long case statement that considers the type of object at each endpoint of the edge
    and generates the appropriate equation.  In addition, whenever a constraint is generated
    any variables used are added to the global list of variables, vars, and are used later to
    generate a list of variables required as input for Maple.
*/
std::string getDistanceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge, std::string theDist)
{
    std::string x1,x2,y1,y2,coeff,m,v,w,output, firstVal;
    int v1Name, v2Name;
    int type1, type2;
    float value;

    value=theEdge.returnValue();
    if(value<0) value=-value;
    if (theDist=="") firstVal=toString(sqrt(value));
    else firstVal=theDist;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    x1="x"+toString(v1Name);
    y1="y"+toString(v1Name);
    x2="x"+toString(v2Name);
    y2="y"+toString(v2Name);
    switch(type1*10+type2)
    {
        case 2:
        case 12:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 32:
        case 42:
        case 52: if(type1==2)
            {
                v="v"+toString(v1Name);
                w="w"+toString(v1Name);
                m="("+w+"/"+v+")";
                output="("+firstVal+")^2=(("+y1+"+"+y2+"-"+m+"*("+x1+"-"+x2+"))/(1+"+m+"^2))";
            }
            else
            {
                v="v"+toString(v2Name);
                w="w"+toString(v2Name);
                m="("+w+"/"+v+")";
                output="("+firstVal+")^2=(("+y2+"+"+y1+"-"+m+"*("+x2+"-"+x1+"))/(1+"+m+"^2))";
            }
            if(!vars.hasElem(v)) vars.append(v);
            if(!vars.hasElem(w)) vars.append(w);
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;

        case 1:
        case 3:
        case 10:
        case 11:
        case 13:
        case 14:
        case 15:
        case 30:
        case 31:
        case 33:
        case 34:
        case 35:
        case 41:
        case 43:
        case 51:
        case 53: if(type1==1 || type1==3)
            {
                m="m"+toString(v1Name);
                output="("+firstVal+")^2=(("+y1+"+"+y2+"-"+m+"*("+x1+"-"+x2+"))/(1+"+m+"^2))";
            }
            else
            {
                m="m"+toString(v2Name);
                output="("+firstVal+")^2=(("+y2+"+"+y1+"-"+m+"*("+x2+"-"+x1+"))/(1+"+m+"^2))";
            }
            if(!vars.hasElem(m)) vars.append(m);
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;
        default: if(theDist=="") firstVal=toString(value);
            else firstVal=theDist;
            output="("+firstVal+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;
    }
    return output;
}

//returns an equation for a tangency constraint
std::string getTangencyEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    std::string output="", dist="", r1, r2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    switch(type1*10+type2)
    {
        case 35:
        case 25:
        case 15:
        case 34:
        case 24:
        case 14:  dist="r"+toString(v2Name);
            if(!vars.hasElem(dist)) vars.append(dist);
            break;
        case 53:
        case 52:
        case 51:
        case 43:
        case 42:
        case 41:  dist="r"+toString(v1Name);
            if(!vars.hasElem(dist)) vars.append(dist);
            break;
        case 55:
        case 44:  dist="r"+toString(v1Name)+"+r"+toString(v2Name);
            r1="r"+toString(v1Name);
            r2="r"+toString(v2Name);
            if(!vars.hasElem(r1)) vars.append(r1);
            if(!vars.hasElem(r2)) vars.append(r2);
            break;
    }
    if(dist!="") output=getDistanceEQ(vEnd1, vEnd2, theEdge, dist);
    return output;
}

//returns an equation corresponding to an incidence constraint
std::string getIncidenceEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name, part1, part2;;
    std::string output="", dist;
    std::string x1, y1, a1, b1, c1, d1, g1, l1, m1, r1, v1, w1;
    std::string x2, y2, a2, b2, c2, d2, g2, l2, m2, r2, v2, w2;
    std::string z1, z2;


    type1=vEnd1.returnType();
    type2=vEnd2.returnType();
    part1=theEdge.returnPart1();
    part2=theEdge.returnPart2();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    z1=toString(v1Name);
    z2=toString(v2Name);
    x1="x"+z1;
    y1="y"+z1;
    a1="a"+z1;
    b1="b"+z1;
    c1="c"+z1;
    d1="d"+z1;
    g1="g"+z1;
    l1="l"+z1;
    m1="m"+z1;
    r1="r"+z1;
    v1="v"+z1;
    w1="w"+z1;
    x2="x"+z2;
    y2="y"+z2;
    a2="a"+z2;
    b2="b"+z2;
    c2="c"+z2;
    d2="d"+z2;
    g2="g"+z2;
    l2="l"+z2;
    m2="m"+z2;
    r2="r"+z2;
    v2="v"+z2;
    w2="w"+z2;
    /*  Key to above lettering:

        (x,y) - base point for all shapes
        (c,d) - second point
        (p,q) - translation point for clusters
        (v,w) - first angle
        (a,b) - second angle
        (t,s) - translation angle for clusters
          r   - radius
          g   - separtion angle
          l   - length
    */

    switch(type1*10+type2)
    {
        case 0:  output=x1+"="+x2+","+y1+"="+y2;
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;
        case 1:  output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;
        case 10: output="("+y2+"-"+y1+")="+m1+"*("+x2+"-"+x1+")";
            if(!vars.hasElem(m1)) vars.append(m1);
            if(!vars.hasElem(x1)) vars.append(x1);
            if(!vars.hasElem(y1)) vars.append(y1);
            if(!vars.hasElem(x2)) vars.append(x2);
            if(!vars.hasElem(y2)) vars.append(y2);
            break;
        case 2:  if(part2==0)
            {
                output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                if(!vars.hasElem(m2)) vars.append(m2);
                if(!vars.hasElem(x1)) vars.append(x1);
                if(!vars.hasElem(y1)) vars.append(y1);
                if(!vars.hasElem(x2)) vars.append(x2);
                if(!vars.hasElem(y2)) vars.append(y2);
            }
            else
            {
                output=x1+"="+x2+","+y1+"="+y2;
                if(!vars.hasElem(x1)) vars.append(x1);
                if(!vars.hasElem(y1)) vars.append(y1);
                if(!vars.hasElem(x2)) vars.append(x2);
                if(!vars.hasElem(y2)) vars.append(y2);
            }
            break;
        case 20: if(part1==0)
            {
                output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                if(!vars.hasElem(v1)) vars.append(v1);
                if(!vars.hasElem(w1)) vars.append(w1);
                if(!vars.hasElem(x1)) vars.append(x1);
                if(!vars.hasElem(y1)) vars.append(y1);
                if(!vars.hasElem(x2)) vars.append(x2);
                if(!vars.hasElem(y2)) vars.append(y2);
            }
            else
            {
                output=x1+"="+x2+","+y1+"="+y2;
                if(!vars.hasElem(x1)) vars.append(x1);
                if(!vars.hasElem(y1)) vars.append(y1);
                if(!vars.hasElem(x2)) vars.append(x2);
                if(!vars.hasElem(y2)) vars.append(y2);
            }
            break;
        case 3:  switch (part1*10+part2)
            {
                case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 12: output=x1+"="+c2+","+y1+"="+d2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
            }
            break;
        case 30: switch (part1*10+part2)
            {
                case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 21: output=c1+"="+x2+","+d1+"="+y2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 4:  switch (part1*10+part2)
            {
                case 10: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 40: switch (part1*10+part2)
            {
                case 1:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 5:  switch (part1*10+part2)
            {
                case 10: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
        case 50: switch (part1*10+part2)
            {
                case 1: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(x2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(x2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 11: output="";
            break;
        case 12: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 21: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 10: output="("+y2+"-"+y1+")="+m2+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 13: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 2:  output="("+y1+"-"+d2+")="+m1+"*("+x1+"-"+c2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
            }
            break;
        case 31: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 20: output="("+d1+"-"+y2+")="+m2+"*("+c1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 14: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 41: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 15: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
                case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    break;
                case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 51: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
                case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    break;
                case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 22: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 23: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 2:  output="("+d2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+c2+"-"+x1+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
                case 10: output="("+y2+"-"+y1+")="+m2+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 12: output=x1+"="+c2+","+y1+"="+d2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
            }
            break;
        case 32: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y2+"-"+y1+")="+m1+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 20: output="("+d1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+c1+"-"+x2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 21: output=x2+"="+c1+","+y2+"="+d1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    break;
            }
            break;
        case 24: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 3:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 42: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 30: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 25: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+"("+w1+"/"+v1+")"+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
                case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+"("+w1+"/"+v1+")"+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    break;
                case 3:  output="("+y2+"-"+y1+")="+"("+w1+"/"+v1+")"+"*("+x2+"-"+x1+")";
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 52: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+"("+w2+"/"+v2+")"+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
                case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+"("+w2+"/"+v2+")"+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    break;
                case 30: output="("+y1+"-"+y2+")="+"("+w2+"/"+v2+")"+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(y1)) vars.append(r1);
                    break;
                case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 33: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 2:  output="("+y1+"-"+d2+")="+m1+"*("+x1+"-"+c2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
                case 10: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 20: output="("+y1+"-"+d2+")="+m2+"*("+x1+"-"+c2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 12: output=x1+"="+c2+","+y1+"="+d2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
                case 21: output=c1+"="+x2+","+d1+"="+y2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 22: output=c1+"="+c2+","+d1+"="+d2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    break;
            }
            break;
        case 34: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 20: output="("+r2+")^2=(("+c1+"-"+x2+")^2+("+d1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 23: output=c1+"="+x2+","+d1+"="+y2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 43: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 2:  output="("+r1+")^2=(("+c2+"-"+x1+")^2+("+d2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 32: output=c2+"="+x1+","+d2+"="+y1;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 35: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+w2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+v2+")"+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    break;
                case 2:  output="("+y1+"-"+"("+y2+"+"+r2+"*"+b2+")"+")="+m1+"*("+x1+"-"+"("+x2+"+"+r2+"*"+a2+")"+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    break;
                case 3:  output="("+y1+"-"+y2+")="+m1+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m1)) vars.append(m1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 20: output="("+r2+")^2=(("+c1+"-"+x2+")^2+("+d1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 12: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 21: output=c1+"="+x2+"+"+v2+"*"+r2+","+d1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 22: output=c1+"="+x2+"+"+a2+"*"+r2+","+d1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 23: output=c1+"="+x2+","+d1+"="+y2;
                    if(!vars.hasElem(c1)) vars.append(c1);
                    if(!vars.hasElem(d1)) vars.append(d1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 53: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+r1+")^2=(("+x1+"-"+x2+")^2+("+y1+"-"+y2+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 2:  output="("+r1+")^2=(("+c2+"-"+x1+")^2+("+d2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 10: output="("+y2+"-"+"("+y1+"+"+r1+"*"+w1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+v1+")"+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    break;
                case 20: output="("+y2+"-"+"("+y1+"+"+r1+"*"+b1+")"+")="+m2+"*("+x2+"-"+"("+x1+"+"+r1+"*"+a1+")"+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    break;
                case 30: output="("+y1+"-"+y2+")="+m2+"*("+x1+"-"+x2+")";
                    if(!vars.hasElem(m2)) vars.append(m2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 11: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(x2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 21: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(x2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 31: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 12: output=c2+"="+x1+"+"+v1+"*"+r1+","+d2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(y1)) vars.append(r1);
                    break;
                case 22: output=c2+"="+x1+"+"+a1+"*"+r1+","+d2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 32: output=c2+"="+x1+","+d2+"="+y1;
                    if(!vars.hasElem(c2)) vars.append(c2);
                    if(!vars.hasElem(d2)) vars.append(d2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
            }
            break;
        case 44: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 33: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 45: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 1:  output="("+r1+")^2=(("+x2+"+"+v2+"*"+r2+"-"+x1+")^2+("+y2+"+"+w2+"*"+r2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 2:  output="("+r1+")^2=(("+x2+"+"+a2+"*"+r2+"-"+x1+")^2+("+y2+"+"+b2+"*"+r2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 31: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 32: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 33: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 54: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"+"+v1+"*"+r1+"-"+x2+")^2+("+y1+"+"+w1+"*"+r1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 20: output="("+r2+")^2=(("+x1+"+"+a1+"*"+r1+"-"+x2+")^2+("+y1+"+"+b1+"*"+r1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(y1)) vars.append(r1);
                    break;
                case 23: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 33: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
        case 55: switch (part1*10+part2)
            {
                case 0:  output="";
                    break;
                case 1:  output="("+r1+")^2=(("+x2+"+"+v2+"*"+r2+"-"+x1+")^2+("+y2+"+"+w2+"*"+r2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 2:  output="("+r1+")^2=(("+x2+"+"+a2+"*"+r2+"-"+x1+")^2+("+y2+"+"+b2+"*"+r2+"-"+y1+")^2)";
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    break;
                case 3:  output=getDistanceEQ(vEnd1,vEnd2,theEdge,r1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 10: output="("+r2+")^2=(("+x1+"+"+v1+"*"+r1+"-"+x2+")^2+("+y1+"+"+w1+"*"+r1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 20: output="("+r2+")^2=(("+x1+"+"+a1+"*"+r1+"-"+x2+")^2+("+y1+"+"+b1+"*"+r1+"-"+y2+")^2)";
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
                case 30: output=getDistanceEQ(vEnd1,vEnd2,theEdge,r2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 11: output=x2+"+"+v2+"*"+r2+"="+x1+"+"+v1+"*"+r1+","+y2+"+"+w2+"*"+r2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 21: output=x2+"+"+v2+"*"+r2+"="+x1+"+"+a1+"*"+r1+","+y2+"+"+w2+"*"+r2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 31: output=x1+"="+x2+"+"+v2+"*"+r2+","+y1+"="+y2+"+"+w2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(v2)) vars.append(v2);
                    if(!vars.hasElem(w2)) vars.append(w2);
                    if(!vars.hasElem(y2)) vars.append(r2);
                    break;
                case 12: output=x1+"+"+v1+"*"+r1+"="+x2+"+"+a2+"*"+r2+","+y1+"+"+w1+"*"+r1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 22: output=x1+"+"+a1+"*"+r1+"="+x2+"+"+a2+"*"+r2+","+y1+"+"+b1+"*"+r1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 32: output=x1+"="+x2+"+"+a2+"*"+r2+","+y1+"="+y2+"+"+b2+"*"+r2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(a2)) vars.append(a2);
                    if(!vars.hasElem(b2)) vars.append(b2);
                    if(!vars.hasElem(r2)) vars.append(r2);
                    break;
                case 13: output=x2+"="+x1+"+"+v1+"*"+r1+","+y2+"="+y1+"+"+w1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(v1)) vars.append(v1);
                    if(!vars.hasElem(w1)) vars.append(w1);
                    if(!vars.hasElem(y1)) vars.append(r1);
                    break;
                case 23: output=x2+"="+x1+"+"+a1+"*"+r1+","+y2+"="+y1+"+"+b1+"*"+r1;
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(a1)) vars.append(a1);
                    if(!vars.hasElem(b1)) vars.append(b1);
                    if(!vars.hasElem(r1)) vars.append(r1);
                    break;
                case 33: output=x1+"="+x2+","+y1+"="+y2;
                    if(!vars.hasElem(x1)) vars.append(x1);
                    if(!vars.hasElem(y1)) vars.append(y1);
                    if(!vars.hasElem(x2)) vars.append(x2);
                    if(!vars.hasElem(y2)) vars.append(y2);
                    break;
            }
            break;
    }
    return output;
}

//returns an equation corresponding to a angle constraint
std::string getAngleEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    std::string output="", tanAngleStr, m1, m2, mA, mB, v1, v2, w1, w2, a1, a2, b1, b2;
    std::string n1, n2;
    float tanAngle;


    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    a1="a"+n1;
    b1="b"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;
    a2="a"+n2;
    b2="b"+n2;

    tanAngle=tan(theEdge.returnValue()*PI/180.0);
    tanAngle=tanAngle*tanAngle*10000;
    tanAngle=floor(tanAngle);
    tanAngle=tanAngle/10000;
    tanAngleStr=toString(tanAngle);

    switch(type1*10+type2)
    {
        case 11:
        case 13:
        case 31:
        case 33:  mA=m1;
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(m1)) vars.append(m1);
            break;
        case 21:
        case 23:  mA="("+w1+"/"+v1+")";
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            break;
        case 22:  mA="("+w1+"/"+v1+")";
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
        case 12:
        case 32:  mA=m1;
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(m1)) vars.append(m1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
        case 51:
        case 53:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
            mB=m2;
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(a1)) vars.append(a1);
            if(!vars.hasElem(b1)) vars.append(b1);
            if(!vars.hasElem(m2)) vars.append(m2);
            break;
        case 35:
        case 15:  mA=m1;
            mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            if(!vars.hasElem(a2)) vars.append(a2);
            if(!vars.hasElem(b2)) vars.append(b2);
            if(!vars.hasElem(m1)) vars.append(m1);
            break;
        case 52:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(a1)) vars.append(a1);
            if(!vars.hasElem(b1)) vars.append(b1);
            break;
        case 25:  mA="("+w1+"/"+v1+")";
            mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            if(!vars.hasElem(a2)) vars.append(a2);
            if(!vars.hasElem(b2)) vars.append(b2);
            break;
        case 55:  mA="((1-"+v1+"*"+a1+"+"+w1+"*"+b1+")/("+w1+"*"+a1+"+"+v1+"*"+b1+"))";
            mB="((1-"+v2+"*"+a2+"+"+w2+"*"+b2+")/("+w2+"*"+a2+"+"+v2+"*"+b2+"))";
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(a1)) vars.append(a1);
            if(!vars.hasElem(b1)) vars.append(b1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            if(!vars.hasElem(a2)) vars.append(a2);
            if(!vars.hasElem(b2)) vars.append(b2);
            break;
    }
    if(mA!="" || mB!="") output="("+tanAngleStr+")=(("+mA+"-"+mB+")/(1+"+mA+"*"+mB+"))^2";
    else output="";
    return output;
}

//returns an equation for a parallel constraint
std::string getParallelEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    std::string output="",mB,mA,m1,v1,w1,m2,v2,w2,n1,n2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;

    switch(type1*10+type2)
    {
        case 11:
        case 13:
        case 31:
        case 33:  mA=m1;
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(m1)) vars.append(m1);
            break;
        case 21:
        case 23:  mA="("+w1+"/"+v1+")";
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            break;
        case 22:  mA="("+w1+"/"+v1+")";
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
        case 12:
        case 32:  mA=m1;
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(m1)) vars.append(m1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
    }
    output="";
    if(mA!="" || mB!="") output=mA+"="+mB;
    return output;
}


//returns an equation corresponding to a perpendicularity constraint
std::string getPerpendicularEQ(Vertex &vEnd1, Vertex &vEnd2, Edge &theEdge)
{
    int type1, type2, v1Name, v2Name;
    std::string output="",mB,mA,m1,v1,w1,m2,v2,w2,n1,n2;

    type1=vEnd1.returnType();
    type2=vEnd2.returnType();

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    n1=toString(v1Name);
    n2=toString(v2Name);

    m1="m"+n1;
    v1="v"+n1;
    w1="w"+n1;
    m2="m"+n2;
    v2="v"+n2;
    w2="w"+n2;

    switch(type1*10+type2)
    {
        case 11:
        case 13:
        case 31:
        case 33:  mA=m1;
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(m1)) vars.append(m1);
            break;
        case 21:
        case 23:  mA="("+w1+"/"+v1+")";
            mB=m2;
            if(!vars.hasElem(m2)) vars.append(m2);
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            break;
        case 22:  mA="("+w1+"/"+v1+")";
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(v1)) vars.append(v1);
            if(!vars.hasElem(w1)) vars.append(w1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
        case 12:
        case 32:  mA=m1;
            mB="("+w2+"/"+v2+")";
            if(!vars.hasElem(m1)) vars.append(m1);
            if(!vars.hasElem(v2)) vars.append(v2);
            if(!vars.hasElem(w2)) vars.append(w2);
            break;
    }
    output="";
    if(mA!="" || mB!="") output="(-1)="+mA+"*"+mB;
    return output;
}


/* Generates an equation for a constraint between two vertices in different clusters.
   It gets an equation as if the vertices were in one cluster, then replaces
   all of the variables for each cluste; either with the 2D arbitrary rotation
   or with the previously solved values of that vertex if its parent cluster
   will not be rotated. */
std::string getEquationBetweenClusters(Graph& graph0, Edge &theEdge, Cluster &theCluster)
{
    std::string outString;
    Vertex vEnd1, vEnd2;
    int v1Name, v2Name;
    int end1C, end2C;

    outString="";

    if(isImaginary(graph0, theEdge)) theImags.append(theEdge);

    vEnd1=graph0.returnVertByName(theEdge.returnEnd1());
    vEnd2=graph0.returnVertByName(theEdge.returnEnd2());

    v1Name=vEnd1.returnName();
    v2Name=vEnd2.returnName();

    switch(theEdge.returnType()) {

        case 0: outString=getDistanceEQ(vEnd1,vEnd2,theEdge, "");
            break;
        case 1: outString=getIncidenceEQ(vEnd1,vEnd2,theEdge);
            break;
        case 4: outString=getAngleEQ(vEnd1,vEnd2,theEdge);
            break;
        case 3: outString=getParallelEQ(vEnd1,vEnd2,theEdge);
            break;
        case 2: outString=getPerpendicularEQ(vEnd1,vEnd2,theEdge);
            break;
        case 5: outString=getTangencyEQ(vEnd1,vEnd2,theEdge);
            break;
    }
    end1C=getChildNameWithVertex(theCluster, v1Name);
    end2C=getChildNameWithVertex(theCluster, v2Name);
    setValueInCluster(graph0,theCluster.children.retrieve(getChildIndexByName(theCluster, end1C)), vEnd1);
    setValueInCluster(graph0,theCluster.children.retrieve(getChildIndexByName(theCluster, end2C)), vEnd2);
    if (end1C==-1 || end2C==-1) return "";
    if (end1C!=v1Name)
    {
        outString=switchString(vEnd1, vEnd1.returnType(), v1Name, end1C, outString, end1C!=withHeldCluster);
        if(end1C!=withHeldCluster)
            outString=switchString(vEnd1, vEnd1.returnType(), v1Name, end1C, outString, false);
    }
    if (end2C!=v2Name)
    {
        outString=switchString(vEnd2, vEnd2.returnType(), v2Name, end2C, outString, end2C!=withHeldCluster);
        if(end2C!=withHeldCluster)
            outString=switchString(vEnd2, vEnd2.returnType(), v2Name, end2C, outString, false);
    }

    return(outString);
}


//returns -1 if the edge is not in the cluster
//returns  0 if the edge is in the cluster, but doesn't contain a subcluster
//returns  1 if the edge is between two clusters or between a cluster and original V
int getEdgeCode(Edge &theEdge, Cluster &theCluster)
{
    int i, length, childLength, origLength;
    bool isEnd;
    List<int> childrenWithEnd1, childrenWithEnd2;

    childrenWithEnd1.makeEmpty();
    childrenWithEnd2.makeEmpty();

    childLength=(theCluster.children).returnLen();
    origLength=theCluster.returnOrigLen();

    isEnd=(childLength==origLength);

    length=(theCluster.children).returnLen();

    if(length==0) return -1;

    if(!inOriginalV(theEdge.returnEnd1(), theCluster)||!inOriginalV(theEdge.returnEnd2(),theCluster))
        return -1;

    if(isEnd) return 0;

    for(i=1;i<=length;i++)
    {
        if(inOriginalV(theEdge.returnEnd1(), (theCluster.children).retrieve(i)))
            childrenWithEnd1.append(i);
        if(inOriginalV(theEdge.returnEnd2(), (theCluster.children).retrieve(i)))
            childrenWithEnd2.append(i);
    }
    for(i=1;i<=childrenWithEnd1.returnLen();i++)
        if(childrenWithEnd2.hasElem(childrenWithEnd1.retrieve(i))) return -1;

    if(childrenWithEnd1.returnLen()==0 && childrenWithEnd2.returnLen()==0) return 0;
    else return 1;

}

/* switchString takes an string as input then replaces all of the variables from a given cluster
   with an expression for that variable times the 2D rotation matrix.
   The boolean allows the method to also replace the variables with strings representing there real
   solved positions. */
std::string switchString(Vertex &theVertex, int type, int shapeName, int clusterName, std::string toReplace, bool solveOrValue)
{
    std::string repStr1, repStr2, repStr3, repStr4, repStr5, repStr6;
    std::string repStrb1, repStrb2, repStrb3, repStrb4, repStrb5, repStrb6;
    std::string newStr1, newStr2, newStr3, newStr4, newStr5, newStr6;
    std::string clusterV1, clusterV2, clusterV3, clusterV4;
    std::string output;

    if(solveOrValue)
    {
        clusterV1="p"+toString(clusterName);
        clusterV2="q"+toString(clusterName);
        clusterV3="t"+toString(clusterName);
        clusterV4="s"+toString(clusterName);

        if(!vars.hasElem(clusterV1)) vars.append(clusterV1);
        if(!vars.hasElem(clusterV2)) vars.append(clusterV2);
        if(!vars.hasElem(clusterV3)) vars.append(clusterV3);
        if(!vars.hasElem(clusterV4)) vars.append(clusterV4);
    }

    output=toReplace;


    switch(type)
    {
        case 4:
        case 0:  repStr1="x"+toString(shapeName);
            repStr2="y"+toString(shapeName);
            if(solveOrValue)
            {
                repStrb1=repStr1;
                repStrb2=repStr2;
                repStrb1.insert(1,"?");
                repStrb2.insert(1,"?");
                newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
            }
            else
            {
                newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                vars.deleteElem(repStr1);
                vars.deleteElem(repStr2);
            }
            output=replaceAll(output, repStr1, newStr1);
            output=replaceAll(output, repStr2, newStr2);
            break;
        case 1:  repStr1="x"+toString(shapeName);
            repStr2="y"+toString(shapeName);
            repStr3="m"+toString(shapeName);
            if(solveOrValue)
            {
                repStrb1=repStr1;
                repStrb2=repStr2;
                repStrb3=repStr3;
                repStrb1.insert(1,"?");
                repStrb2.insert(1,"?");
                repStrb3.insert(1,"?");
                newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                newStr3="(("+repStrb3+"*"+clusterV3+"+"+clusterV4+")/("+repStrb3+"*"+clusterV4+"+"+clusterV3+"))";
            }
            else
            {
                newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                vars.deleteElem(repStr1);
                vars.deleteElem(repStr2);
                vars.deleteElem(repStr3);
            }
            output=replaceAll(output, repStr1, newStr1);
            output=replaceAll(output, repStr2, newStr2);
            output=replaceAll(output, repStr3, newStr3);
            break;
        case 2:  repStr1="x"+toString(shapeName);
            repStr2="y"+toString(shapeName);
            repStr3="v"+toString(shapeName);
            repStr4="w"+toString(shapeName);
            if(solveOrValue)
            {
                repStrb1=repStr1;
                repStrb2=repStr2;
                repStrb3=repStr3;
                repStrb4=repStr4;
                repStrb1.insert(1,"?");
                repStrb2.insert(1,"?");
                repStrb3.insert(1,"?");
                repStrb4.insert(1,"?");
                newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+")";
                newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+")";
            }
            else
            {
                newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                newStr4="("+toString(theVertex.returnDegValueByName(5))+")";
                vars.deleteElem(repStr1);
                vars.deleteElem(repStr2);
                vars.deleteElem(repStr3);
                vars.deleteElem(repStr4);
            }
            output=replaceAll(output, repStr1, newStr1);
            output=replaceAll(output, repStr2, newStr2);
            output=replaceAll(output, repStr3, newStr3);
            output=replaceAll(output, repStr4, newStr4);
            break;
        case 3:  repStr1="x"+toString(shapeName);
            repStr2="y"+toString(shapeName);
            repStr3="c"+toString(shapeName);
            repStr4="d"+toString(shapeName);
            repStr5="m"+toString(shapeName);
            if(solveOrValue)
            {
                repStrb1=repStr1;
                repStrb2=repStr2;
                repStrb3=repStr3;
                repStrb4=repStr4;
                repStrb5=repStr5;
                repStrb1.insert(1,"?");
                repStrb2.insert(1,"?");
                repStrb3.insert(1,"?");
                repStrb4.insert(1,"?");
                repStrb5.insert(1,"?");
                newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+"+"+clusterV1+")";
                newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+"+"+clusterV2+")";
                newStr5="(("+repStrb5+"*"+clusterV3+"+"+clusterV4+")/("+repStrb5+"*"+clusterV4+"+"+clusterV3+"))";
            }
            else
            {
                newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                newStr3="("+toString(theVertex.returnDegValueByName(2))+")";
                newStr4="("+toString(theVertex.returnDegValueByName(3))+")";
                newStr5="("+toString(theVertex.returnDegValueByName(4))+")";
                vars.deleteElem(repStr1);
                vars.deleteElem(repStr2);
                vars.deleteElem(repStr3);
                vars.deleteElem(repStr4);
                vars.deleteElem(repStr5);
            }
            output=replaceAll(output, repStr1, newStr1);
            output=replaceAll(output, repStr2, newStr2);
            output=replaceAll(output, repStr3, newStr3);
            output=replaceAll(output, repStr4, newStr4);
            output=replaceAll(output, repStr5, newStr5);
            break;
        case 5:  repStr1="x"+toString(shapeName);
            repStr2="y"+toString(shapeName);
            repStr3="v"+toString(shapeName);
            repStr4="w"+toString(shapeName);
            repStr5="a"+toString(shapeName);
            repStr6="b"+toString(shapeName);
            if(solveOrValue)
            {
                repStrb1=repStr1;
                repStrb2=repStr2;
                repStrb3=repStr3;
                repStrb4=repStr4;
                repStrb5=repStr5;
                repStrb6=repStr6;
                repStrb1.insert(1,"?");
                repStrb2.insert(1,"?");
                repStrb3.insert(1,"?");
                repStrb4.insert(1,"?");
                repStrb5.insert(1,"?");
                repStrb6.insert(1,"?");
                newStr1="("+repStrb1+"*"+clusterV3+"-"+repStrb2+"*"+clusterV4+"+"+clusterV1+")";
                newStr2="("+repStrb2+"*"+clusterV3+"+"+repStrb1+"*"+clusterV4+"+"+clusterV2+")";
                newStr3="("+repStrb3+"*"+clusterV3+"-"+repStrb4+"*"+clusterV4+")";
                newStr4="("+repStrb4+"*"+clusterV3+"+"+repStrb3+"*"+clusterV4+")";
                newStr5="("+repStrb5+"*"+clusterV3+"-"+repStrb6+"*"+clusterV4+")";
                newStr6="("+repStrb6+"*"+clusterV3+"+"+repStrb5+"*"+clusterV4+")";
            }
            else
            {
                newStr1="("+toString(theVertex.returnDegValueByName(0))+")";
                newStr2="("+toString(theVertex.returnDegValueByName(1))+")";
                newStr3="("+toString(theVertex.returnDegValueByName(4))+")";
                newStr4="("+toString(theVertex.returnDegValueByName(5))+")";
                newStr5="("+toString(theVertex.returnDegValueByName(6))+")";
                newStr6="("+toString(theVertex.returnDegValueByName(7))+")";
                vars.deleteElem(repStr1);
                vars.deleteElem(repStr2);
                vars.deleteElem(repStr3);
                vars.deleteElem(repStr4);
                vars.deleteElem(repStr5);
                vars.deleteElem(repStr6);
            }
            output=replaceAll(output, repStr1, newStr1);
            output=replaceAll(output, repStr2, newStr2);
            output=replaceAll(output, repStr3, newStr3);
            output=replaceAll(output, repStr4, newStr4);
            output=replaceAll(output, repStr5, newStr5);
            output=replaceAll(output, repStr6, newStr6);
            break;
    }
    output=replaceAll(output, "?", "");

    return output;
}
