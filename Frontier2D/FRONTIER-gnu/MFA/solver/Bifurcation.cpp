#include "solver/Bifurcation.h"
#include "SolverUtil.h"

//sets the degrees of freedom in theCluster to the values stored in the given
//bifurcation string
void parseBifurString(Graph &graph0, Cluster &theCluster)
{
    int i, length, inputLength;
    int pos=0, ID, childLen;
    float value;
    char varID, temp;
    std::string currParse, IDString, input;

    childLen=theCluster.children.returnLen();
    for(i=1;i<=childLen;i++)
        if(theCluster.children.retrieve(i).children.returnLen()!=0)
            theCluster.children.retrieve(i).setToCluster(theCluster);

    input=theCluster.returnCurrBifurString();
    length=0;
    inputLength=input.size();
    for(i=0; i<inputLength; i++)
        if(input[i]=='=') length++;
    temp=input[pos];
    for(i=0; i<length; i++)
    {
        currParse="";
        while(temp!=',' && pos<inputLength)
        {
            currParse=currParse+temp;
            pos++;
            temp=input[pos];
        }
        IDString="";
        if(currParse[currParse.size()-1]=='.') currParse.erase(currParse.size()-1);
        varID=currParse[0];
        currParse.erase(0,1);
        while(currParse[0]!='=')
        {
            IDString+=currParse[0];
            currParse.erase(0,1);
        }
        currParse.erase(0,1);
        value=atof(currParse.c_str());
        if(fabs(value)<1e-5) value=0;
        ID=atoi(IDString.c_str());
        setValue(theCluster,ID,value,getFreeDegIndex(varID,graph0.returnVertByName(ID)));
        pos++;
        if(pos<inputLength) temp=input[pos];
    }
}


//sets the children of theCluster to the next distinct set of bifurcations
bool getNextBifurcation(Graph &graph0, Cluster &theCluster)
{
    int i, childLength=theCluster.children.returnLen();

    if(theCluster.children.retrieve(1).returnCurrBifur()==-1)
    {
        for(i=1;i<=childLength;i++)
        {
            theCluster.children.retrieve(i).setCurrBifur(1);
            parseBifurString(graph0, theCluster.children.retrieve(i));
            updateGraph(graph0, theCluster.children.retrieve(i));
        }
        return(true);
    }

    int numBifur, currBifur;

    for(i=1;i<=childLength;i++)
    {
        numBifur=theCluster.children.retrieve(i).returnNumBifurs();
        currBifur=theCluster.children.retrieve(i).returnCurrBifur();
        if(numBifur==currBifur)
        {
            theCluster.children.retrieve(i).setCurrBifur(1);
            parseBifurString(graph0, theCluster.children.retrieve(i));
            updateGraph(graph0, theCluster.children.retrieve(i));
            continue;
        }
        theCluster.children.retrieve(i).setCurrBifur(currBifur+1);
        parseBifurString(graph0, theCluster.children.retrieve(i));
        updateGraph(graph0, theCluster.children.retrieve(i));
        return(true);
    }
    return(false);
}

/*  Chooses the bifurcation for theCluster.  Two automatic methods are used first:

    if there is only one bifurcation it is selected.
    if the cluster contains only original vertices and the bifurcation only adjusts reflections
      in space, the first bifurcation is chosen.

    In any other case, the method outputs the information about the clusters to the sketcher
    which queries the user for the correct bifurcation.  The format for this output is:

    <ID of the cluster whose bifurcations are being chosen>
    <number of bifurcations>

    for each bifurcation:

    <output of the generateOutput method above called with theCluster set to the correct bifur.>

*/
int selectBifurcation(Graph &graph0, Cluster &theCluster, bool &useFile)
{
    int i, answer=-1;
    int numBifurs=theCluster.returnNumBifurs();
    int tag=0;
    std::string answerString, bifurString;
    std::ofstream outfile;
    std::ifstream infile;
    bool validToSkip=true;

    useFile=false;

    if(numBifurs==1) return 1;
    if(numBifurs==0) return 0;

    int childLength=0, totalSingle=0;

    childLength=theCluster.children.returnLen();

    bifurString=theCluster.returnCurrBifurString();

    if(strchr(bifurString.c_str(),'r')!=NULL) validToSkip=false;
//    if(strchr(bifurString.c_str(),'m')!=NULL) validToSkip=false;

    for(i=1;i<=childLength;i++)
        if(theCluster.children.retrieve(i).returnOrigLen()>1) totalSingle++;

    if(totalSingle==0 && validToSkip) return 1;

    useFile=true;

    inputInts[currPosI++]=theCluster.returnName();
    inputInts[currPosI++]=numBifurs;
    for(i=1;i<=numBifurs;i++)
    {
        theCluster.setCurrBifur(i);
        parseBifurString(graph0, theCluster);
        updateGraph(graph0, theCluster);
        generateOutputToArray(graph0, theCluster, i==1);
    }

    theCluster.setCurrBifur(-1);

/*  REMOVE OLD SELECTION METHODS

    outfile.open("./Sketcher/testin.txt");
    outfile<<-1;
    outfile.close();

    outfile.open(outFile.c_str());

    if(first)
    {
       first=false;
       outfile<<-2<<endl;
       outfile<<roots.returnLen()<<endl;
       outputDRDAG(roots,outfile);
       outfile<<-1<<endl;
    }
    outfile<<theCluster.returnName()<<endl<<numBifurs<<endl;
    for(i=1;i<=numBifurs;i++)
    {
       theCluster.setCurrBifur(i);
       parseBifurString(graph0, theCluster);
       updateGraph(graph0, theCluster);
       generateOutput(graph0, theCluster, outfile, i==1);
    }
    outfile.close();

    while(answer<0)
    {
       infile.open("./Sketcher/testin.txt");
       infile>>answer;
       infile.close();
    }

    return answer+1;

    answerString="";

    printBifurcations(theCluster);
    cout<<endl;

    while(answer<1 || answer>numBifurs)
    {
        if(tag==1) cout<<"   Incorrect selection, Please make another: ";
        if(tag==0) {cout<<"   Select a bifurcation to continue: "; tag=1; }
        cin>>answerString;
        answer=atoi(answerString.c_str());
    }
    cout<<endl;
    keys.append(answer);
    return (answer);

END REMOVE OLD SELECTION METHODS */

}


//eliminates any clusters that do not satisfy all of the imaginary constraints
void checkBifurs(Graph &graph0, Cluster &theCluster)
{
    int i,j,num,length,part1,part2;
    Edge temp;
    Vertex v1,v2;
    bool out;
    List<std::string> theList;

    num=theCluster.returnNumBifurs();
    length=theImags.returnLen();

    for(i=1;i<=num;i++)
    {
        out=true;
        theCluster.setCurrBifur(i);
        parseBifurString(graph0, theCluster);
        updateGraph(graph0, theCluster);
        for(j=1;j<=length;j++)
        {
            temp=theImags.retrieve(i);
            out=(out && isValidConstraint(graph0, temp, theCluster));
        }
        if(out) theList.append(theCluster.returnCurrBifurString());
    }
    theCluster.setBifurs(theList);
    theCluster.setCurrBifur(-1);
}


//checks that a given bifurcation string is valid by checking that it contains
//only the valid characters below
bool validBifur(std::string theString)
{
    int i, length;
    bool output;

    output=true;

    length=theString.size();

    for(i=0;i<length;i++)
    {
        switch(theString[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'q':
            case 'p':
            case 's':
            case 't':
            case 'x':
            case 'y':
            case 'r':
            case 'm':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'w':
            case 'v':
            case '.':
            case '=':
            case ',':
            case 'e':
            case 'l':
            case 'g':
            case '-':  break;
            default:   output=false;
                break;
        }
    }
    return output;
}

//reads the bifurcations from filename and sets them in theCluster, fsolve is used to
//differentiate the two different cases.  When true the data is read as if the output comes
//Maple's numeric solver, otherwise as if the symbolic solver was used.
void setBifurs(std::string filename, Cluster& theCluster, bool usingfSolve)
{
    std::ifstream infile;
    int caretCount=0;
    std::string output="";
    char in;
    int tag=0;
    std::string temp;
    List<std::string> theList;

    theList.makeEmpty();

    infile.open(filename.c_str());

    if(usingfSolve)
    {
        while(caretCount!=3)
        {
            if(infile.get()=='>') caretCount++;
            if(infile.eof()) return;
        }
        while(infile.get()!=';') if(infile.eof()) return;

        infile>>in;
        while(in!='{')
        {
            if(infile.eof()) return;
            if(in=='f') return;
            infile>>in;
        }

        infile>>in;
        while(tag==0)
        {
            while(in!='}')
            {
                temp="";
                temp+=in;
                temp+=" ";
                if(temp!="\\ ") output+=in;
                infile>>in;
                if(infile.eof()) return;
            }
            if(validBifur(output)) theList.append(output);
            output="";
            while(in!='{' && in!='q')
            {
                infile>>in;
                if(infile.eof()) return;
            }
            if(in=='q') tag=1;
            if(in=='{') infile>>in;
        }
        infile.close();
        theCluster.setBifurs(theList);
        return;
    }

    while(caretCount!=5)
    {
        if(infile.get()=='>') caretCount++;
        if(infile.eof()) return;
    }

    while(infile.get()!='{') if(infile.eof()) return;

    infile>>in;
    while(tag==0)
    {
        while(in!='}')
        {
            temp="";
            temp+=in;
            temp+=" ";
            if(temp!="\\ ") output+=in;
            infile>>in;
            if(infile.eof()) return;
        }
        if(validBifur(output)) theList.append(output);
        if(theList.returnLen()==5) break;
        output="";
        while(in!='{' && in!='q')
        {
            infile>>in;
            if(infile.eof()) return;
        }
        if(in=='q') tag=1;
        if(in=='{') infile>>in;
    }

    infile.close();
    theCluster.setBifurs(theList);
}
