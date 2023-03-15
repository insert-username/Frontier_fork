#include "solver/StringUtil.h"

#include <string>

//returns the substring of theString starting at pos with length length
std::string getSubString(std::string theString, int pos, int length)
{
    int i;
    std::string output;

    output="";

    if((pos+length)>theString.size()) return "";

    for(i=1;i<=length;i++)
    {
        output+=theString[pos];
        pos++;
    }

    return output;
}

//replaces all occurances of toReplace with replaceWith in theReplace
std::string replaceAll(std::string toReplace, std::string theReplace, std::string replaceWith)
{
    std::string output=toReplace;
    int theReplaceLen, replaceWithLen, pos=0;

    theReplaceLen=theReplace.size();
    replaceWithLen=replaceWith.size();

    while((pos+theReplaceLen)<=output.size())
    {
        if(getSubString(output,pos,theReplaceLen)==theReplace)
        {
            output.replace(pos,theReplaceLen,replaceWith);
            pos+=replaceWithLen-1;
        }
        pos++;
    }

    return output;
}

//returns a string in Maple format of the variable list
std::string getVarString()
{
    std::string output="{";
    int length, i;

    length=vars.returnLen();

    for(i=1;i<=length;i++)
    {
        if(i!=1) output+=",";
        output+=vars.retrieve(i);
    }

    output+="}";
    return output;
}

//converts a long to a string
std::string toString(long a)
{
    long temp;
    std::string theNumber, outString;

    if(a==0) return "0";

    temp = a;
    outString = "";

    while (temp>=1)
    {
        switch (temp%10)
        {
            case 1: theNumber = "1";
                break;
            case 2: theNumber = "2";
                break;
            case 3: theNumber = "3";
                break;
            case 4: theNumber = "4";
                break;
            case 5: theNumber = "5";
                break;
            case 6: theNumber = "6";
                break;
            case 7: theNumber = "7";
                break;
            case 8: theNumber = "8";
                break;
            case 9: theNumber = "9";
                break;
            case 0: theNumber = "0";
                break;
        }
        outString=theNumber+outString;
        temp/=10;
    }
    if(a>=0) return outString;
    else return("-"+outString);
}

//converts a int to a string
std::string toString(int a)
{
    return toString((long) a);
}

//converts a double to a string
std::string toString(double a)
{
    std::string output;
    char temp[100];

    sprintf(temp,"%f",a);

    output.assign(temp);

    return output;
}

//converts a float to a string
std::string toString(float a)
{
    return toString((double) a);
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


//given the letter in a variable name returns the index of the freedom value
//it corresponds to (a given type of value is always stored in the same index, see chart in
//mfa2Clas.h
int getFreeDegIndex(char keyChar, Vertex &theVertex)
{
    int out;

    switch(keyChar) {

        case 'p':
        case 'x': out=0;
            break;
        case 'q':
        case 'y': out=1;
            break;
        case 'c':
        case 't':
        case 'r': out=2;
            break;
        case 'd':
        case 'g':
        case 's': out=3;
            break;
        case 'v':
        case 'm': out=4;
            break;
        case 'w':
        case 'l': out=5;
            break;
        case 'a': out=6;
            break;
        case 'b': out=7;
            break;
    }
    return out;
}
