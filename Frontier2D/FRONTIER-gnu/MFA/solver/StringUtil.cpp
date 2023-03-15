#include "solver/StringUtil.h"
#include "entities/Vertex.h"

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
std::string getVarString(const List<std::string> &vars)
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
