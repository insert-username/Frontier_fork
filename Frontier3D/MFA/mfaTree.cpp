#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <vector>
#include <fstream.h>
#include <iomanip.h>
//#include <sys/ddi.h>
//#include <thread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#include "mfa10Solver.h"

//Objects of this class contains the end entities' type, the part of the entities involved
//and the type of constraint between the entities  
class Ord_Set
{
protected:
  int type1, type2;           // entity type
  int part1, part2;           // part involved
  int const_type;             // constraint type
  
public:
  
  // Constructor
  Ord_Set()
  {
    type1=type2=part1=part2=const_type=0;
  }

  
  // Initializes values
  void Set_Val(int a, int b, int c, int d, int e)
  {
    type1 = a;
    part1 = b;
    type2 = c;
    part2 = d;
    const_type = e;
  }

  
  // Displays values as an ordered set
  void Display_Val()
  {
    cout << "(" << type1 << "," << part1 << "," << type2 ;
    cout << "," << part2 << "," << const_type << ")\n";
  }


  // Returns values as a concatenated string
  char* Disp_Val()
  {
    //cout << type1 << "," << part1 << "," << type2 ;
    //cout << "," << part2 << "," << const_type << "\n";
    char* str = new char[50];
    char* temp = new char[10];
    strcpy(str,"");
    strcat(str,itoa(type1,temp));
    //strcat(str,lltostr(long(type1),temp));
    strcat(str,",");
    strcat(str,itoa(part1,temp));
    //strcat(str,lltostr(long(part1),temp));
    strcat(str,",");
    strcat(str,itoa(type2,temp));
    //strcat(str,lltostr(long(type2),temp));
    strcat(str,",");
    strcat(str,itoa(part2,temp));
    //strcat(str,lltostr(long(part2),temp));
    strcat(str,",");
    strcat(str,itoa(const_type,temp));
    //strcat(str,lltostr(long(const_type),temp));
    strcat(str,"\n");
    delete temp;
    return str;
  }


  //converts a valid integer into a string
  char* itoa(int n, char s[])
  {
    int i, sign;
    if ((sign = n ) < 0)
      n = -n;
    i=0;
    do 
      {
	s[i++] = n%10 + '0';
      } while ((n/=10)>0);
    if (sign < 0)
      s[i++]= '-';
    s[i]='\0';
    reverse(s);
    return s;
  }
  
  
  // reverses a string
  void reverse(char s[])
  {
    int c, j;
    for (int i=0, j=strlen(s)-1; i<j; i++, j--)
      {
	c=s[i];
	s[i]=s[j];
	s[j]=c;
      }
  }

  // returns the id of first entity
  int ret_1()
  {
    return type1;
  }

  
  // returns the part id of first entity
  int ret_2()
  {
    return part1;
  }
 

  // returns the id of second entity
  int ret_3()
  {
    return type2;
  }
  

  // returns the part id of second entity  
  int ret_4()
  {
    return part2;
  }


  // returns the id of the constraint between the two end entities
  int ret_5()
  {
    return const_type;
  }


  // Useful during FIND operation, checks if entities, parts and constraints match
  int Check_List(int a, int b, int c, int d, int e);


  //Stores information of conditions into Array
  void copyOrdSetIntoArray(int* array); //, int *index);

  //Stores information of conditions from Array
  void copyArrayIntoOrdSet(int &startI, int* array);
};


//An object of this class contains a list of the conditions that should be satisfied to enter 
// a particular subtree, it is a dynamic array of pointers to objects of class Ord_Set 
class Condition
{
  protected:
  //public:
  vector <Ord_Set *> list;  //the dynamic list of conditions
  int cond_count;           //no. of conditions
  
public:
 
  //Constructor
  Condition()
  {
    cond_count=0;          
  }
  
  //Add a condition to the existing list of conditions 
  void add_Cond(int a, int b, int c, int d, int e)
  { 
    cond_count++;
    list.resize(cond_count);
    list[cond_count-1]= new Ord_Set;
    list[cond_count-1]->Set_Val(a,b,c,d,e);    
  }

  //Display the list of conditions 
  void disp_Cond()
  {
    if (cond_count!=0)
      for (int x=0; x<cond_count; x++)
	list[x]->Display_Val();
  }

  // Returns the list of conditions for writing in file
  char* write_Cond()
  {
    //cout << "Am i here??"; 
    char* str = new char[500];
    strcpy(str,"");
    if (cond_count!=0)
      for (int x=0; x<cond_count; x++)
	strcat(str,list[x]->Disp_Val());
    return str;
  }

  // Returns the no. of conditions
  int ret_Cond()
  {
    return cond_count;
  }


  //Useful during FIND operation, checks if any of the conditions are present
  int Check_Cond(int a,int b, int c, int d, int e);
  
  //Stores information of conditions into Array
  void copyCondIntoArray(int* array); // , int *index);

  //Stores information of conditions from Array
  void Condition::copyArrayIntoCond(int &startI, int* array);
};


//An object of this class is a node in the intended tree. It contains the equation that 
//needs to be output.  
class Node
{
  //protected: 
  public:
  
  char* label;               //Label for the node
  char* eqn;                 //Equation at that node
  int child_count;           //no. of children nodes for this node
  vector <Node *> ptr;       //pointers to children
  vector <Condition *> cond; //conditions for the children nodes 
  Node *parent;
  
public:
  
  //Constructor
  Node()
  {
    child_count=0;
    // label = new char[100];
    //eqn  = new char[100];
    parent = NULL;
    eqn = new char[strlen("No equation")];
    strcpy(eqn,"No equation");
  }
  
  
  //Sets the label for the node
  void Set_label(char* s)
  {
    label = new char[strlen(s)+1];
    strcpy(label,s);
  }

 
  //Prints the label of a node
  void Get_label()
  {
    cout << label << "\n";
  }


  //Returns the label of a node
  char* Ret_label()
  {
    return(label);
  }


  // Sets the parent for a node
  void Set_parent(Node *i)
  {
    parent=i;
  }

  // Returns the label of the parent of any given node
  char* Get_Parent()
  {
    return parent->Ret_label();
  }


  //Sets the children nodes after taking in the conditions for that node
  // Called during Input mode only
  void Set_child(int i, Node* j, Condition* k, char* str)
  { 
    child_count++;
    ptr.resize(child_count);
    cond.resize(child_count);
    ptr[child_count-1]=j;
    cond[child_count-1]=k;
    char x='y';
    while ((x=='y') || (x=='Y'))
      {
	int a,b,c,d,e;
	a=b=c=d=e=0;
	x='n';
	cout << "Give conditions :";
	cin >> a >> b >> c >> d >> e ; // >> f; 
	cond[child_count-1]->add_Cond(a,b,c,d,e);
	// cond[child_count-1]->disp_Cond();
	cout << "More conditions? : (Y/N)";
	cin >> x; 
      }
  }
  
  

  //Sets the children nodes by taking the conditions for that node from a file f_name
  // Called during Find mode only
  void Set_child(int i, Node* j, Condition* k, char* path_name, char* f_name, int a)
  { 
     if (a==1)
      {
	child_count++;
	ptr.resize(child_count);
	cond.resize(child_count);
	ptr[child_count-1]=j;
	cond[child_count-1]=k;
	
	/*
	char* path_file = new char[200];
	strcpy(path_file, "/cise/research/constraints/ramesh/2dto3d/datafiles/");
	strcat(path_file, f_name);
	*/
	
	path_name="";

	char* link = new char[200];
	strcpy(link, path_name);
	//cout<<"IN SET_CHILD path name = "<<path_name<<" file name= "<<f_name<<endl;
	strcat(link, f_name);
	
	ifstream infile(link);
	//ifstream infile(path_file);
	char buffer[200];
	while (infile)
	  {
	    int buf[6];
	    char* temp=new char[500];
	    infile.getline(buffer, 500);
	    
	    if (strcmp(buffer,"")!=0)
	      {  
		unsigned int e=0;
		int start=0;
		int cnt=0;
		while (cnt<=5)
		  //while (e<(strlen(buffer)-1))
		  {
		    strcpy(temp,"");
		    while ((buffer[e]!=',') && (e!=(strlen(buffer)-1)))
		      e++;
		    if (e!=(strlen(buffer)-1))
		      {
			for (unsigned int z=start; z<e; z++)
			  strcat(temp,&buffer[z]);
		      }
		    else
		      {
			strcat(temp,&buffer[start]);
		      }
		    strcat(temp,"\0");
		    if (e!=strlen(buffer)-1)
		      { 
			e++;
			start=e;
		      }
		    if (cnt<=5)
		      {
			buf[cnt]=atoi(temp);
			cnt++;
		      }
		  }
		cond[child_count-1]->add_Cond(buf[0],buf[1],buf[2],buf[3],buf[4]);
	      }
	  }

	/* Do not delete..
	   displays the equations
	
	cout << ptr[child_count-1]->label;
	cout << ptr[child_count-1]->Get_Eqn() << endl;
	*/
	
	/* Do not delete..
	   displays the conditions
	
	cond[child_count-1]->disp_Cond();
	*/
      }
    else
      cout << "Error in mode\n";
  }
    
  
  //Sets the equation in the node
  void Set_Eqn(char* s)
  {
    eqn = new char[strlen(s)+1];
    strcpy(eqn,s);
  }


  // Returns the equation in the node
  char* Get_Eqn()
  {
    return eqn;
  }
  

  // Displays the equation in the node
  void disp_Eqn()
  {
    cout << eqn << "." << endl;
  }

  
  // Returns the no. of children a node has in the tree
  int Get_child_count()
  {
    return(child_count);
  }  


  /*
  void display()
  {
    cout << "Label= " << label << "\nChildren= " << child_count << "\nSubs_count= ";
    if (child_count!=0)
      for (int j=1; j<=child_count; j++)
        {
          //cout << ptr[j-1]->id << ":";
          cout << ptr[j-1]->label << "\n";
	}
  }
  */
 

  // Displays the tree in Pre-order form
  void showTree()
  {
    if (child_count!=0)
      for (int j=1; j<=child_count; j++)
	{  
	  //cout << space << ptr[j-1]->label << "\n";
	  cout << ptr[j-1]->label << "\n";
	  //cout << "Equation:" << ptr[j-1]->eqn << "\n";
	  //cout << space; 
	  cond[j-1]->disp_Cond();
	  //char* newspace = new char[100];
	  //strcpy(newspace,space);
	  //strcat(newspace,"  ");
    	  //ptr[j-1]->showTree(newspace);
	  ptr[j-1]->showTree();
	}
    //cout << "Child count " << child_count << endl; 
  }


  // Writes the tree into a file
  void write_Tree(char* file_String)
  {
    if (child_count!=0)
      for (int j=1; j<=child_count; j++)
	{  
	  strcat(file_String,ptr[j-1]->label);
	  strcat(file_String,"\n");
	  strcat(file_String,ptr[j-1]->eqn);
	  strcat(file_String,"\n");
	  strcat(file_String,ptr[j-1]->Get_Parent());
	  strcat(file_String,"\n\n");
	  //cout << "Writable string : " << file_String << " !!! Ends" << endl;

	  char* create_file = new char[100];
	  char* file_name = new char[100];
	  strcpy(create_file,"touch ");
	  strcat(create_file,ptr[j-1]->label);
	  system(create_file);
	  strcpy(file_name, ptr[j-1]->label);
	  ofstream outfile(file_name);
	  outfile << cond[j-1]->write_Cond();
	  ptr[j-1]->write_Tree(file_String);
	  
	  delete create_file, file_name;
	}
  }
  

  //Useful in FIND operation. Finds the equation given the end entities and the constraints 
  char* Find_Eqn(int a,int b, int c, int d, int e);
  char* Find_Eqn_def_mode(int a,int b, int c, int d, int e);


};


// Global variables
int glo_flag=0;                            // Flag used for indicating end of search
char* file_String = new char[10000];       // Global string used for writing into files
//char* condition_String = new char[50000];
int search_flag=0;                         // Global flag to know which type of search
Node *rootOfTree; 
//int array[10000];                          // Global array for passing information to JNI
//char strArray[10000]; 

// An object of this class is the Tree for conversion from constraints to mathematical equations
class the_Tree
{
  //public :
private:

  int node_count;             // No. of nodes in the tree
  vector <Node *> Block;      // The dynamic list of nodes of the tree to be created   
  vector <Condition *> Cases; // The dynamic list of condition for nodes of the tree to be created


public:
  
  // Constructor
  the_Tree(int select)  // "select" informs if the Tree has to be intialized all over again - value 0 indicates usage from the old Tree, value 1 indicates reconstuction with new constraints  
  {
    node_count=0;
    glo_flag=0;
    strcpy(file_String,"");
    //if (select==1)
    //  build_Tree(select);
  }

  
  // Constructor
  the_Tree()
  {
    /*
    node_count=0;
    glo_flag=0;
    strcpy(file_String,"");
    build_Tree(0);
    */
  }
  


  // This gets the choice of the user as to whether he/she would like to use a previously 
  // constructed tree OR create a new one and save it for the future
  void build_Tree(int select)
  {
    int mode=0;
    /* 
    if (select==1)
      {
	// User selects mode
    
	cout << "1. En-Bloc Search\n" <<  "2. Non-interactive search\n" << "Enter selection:";
	cin >> search_flag;
	cout << endl;
	
    */

    if (select==1)
      search_flag=1;
    else
      search_flag=2;
    
	//int mode=0;
	cout << "1. Find mode\n" << "2. Input Mode\n"  << "Enter selection:";
	mode=1;
	//cin >> mode;
	cout << endl;

	/*  }
    else
      {
	search_flag=1;
	mode=1;
      }
	*/

    // Create Root node irrespective of mode of operation
    // Root node is a dummy node with no equation attached to it
    node_count++;  
    Block.resize(node_count);
    Cases.resize(node_count);
    Block[node_count-1]=new Node; 
    Cases[node_count-1]=new Condition;
    Block[node_count-1]->Set_parent(Block[0]);
    char *rootname = new char[5];
    strcpy(rootname,"Root");
    Block[0]->Set_label(rootname);
    

    /*
    //pass the Tree root to JNI
    rootOfTree = Block[0];            // store address of Root as a global variable
    treeToArray(rootOfTree, theInts); // input rootOfTree into the integer array for JNI
    */

    
    if (mode!=2)     // If user inputs a number other than 1 or 2, default mode is 1
      if (mode!=1)
	{
	  cout << "Entered an invalid mode. So, default mode is Find mode\n";
	  mode=1;
	}
    
    if (mode==1)     // Find mode, i.e., User can use already existing Tree structures
                     // to design his Tree
      {
	const int maxSize = 200;
	int count=node_count;
	//node_count++;
	
	char* buffer0 = new char[maxSize];
	char* buffer1 = new char[maxSize];  
	char* buffer2 = new char[maxSize];  
        char* buffer3 = new char[maxSize];  
	char* buffer4 = new char[maxSize];  
	
	
	// create a temperory file called orig_path for storing the current directory
	system("pwd > orig_path");
	ifstream inp_file("orig_path");
	inp_file.getline(buffer0,maxSize);
	//cout << "String :" << buffer0 << endl; 
	
	
	char* dir_name = new char[maxSize];
	chdir("../datafiles/");
	
        strcpy(dir_name,"");

	/*
	if (select==1)
	  {
	*/  
	    cout << "The available data files directories are:\n";
	    system("ls");
	    
	    cout << "Give name of the required data files directory : ";
	    //cin >> dir_name;
	    strcpy(dir_name,"original");
	    /*      }
	    
	else 
	  strcpy(dir_name,"");
	    */
	
	// assign default 
	if (strcmp(dir_name,"")==0)
	  strcpy(dir_name,"original");
	
	
	
	//chdir("/cise/research/constraints/ramesh/2dto3d/datafiles/");
	chdir(dir_name);
	system("pwd");
	
	
	//strcpy(buffer4, "/cise/research/constraints/ramesh/2dto3d/datafiles/");
	//strcat(direc, file_name);
	
	/*
	  Groups.txt is a file that contains information regrading the Tree
	  such as Parent-child relationships and also contains the information
	  of the equation associated with each node
	*/
	ifstream gpFile("Groups.txt");
	
	while (gpFile)
	  {
	    gpFile.getline(buffer1, maxSize);  // Gets the label of each node
	    gpFile.getline(buffer2, maxSize);  // Gets the equation to be put in each node
	    gpFile.getline(buffer3, maxSize);  // Gets the label of the parent node of current node
	
	    int index = 0, flag = 0, parent=0;
	    // Search for the Parent node in the list of nodes
	    while ((flag==0) && (index<node_count))            
	      {
		if (strcmp(Block[index]->Ret_label(),buffer3)==0)   
		  {
		    flag=1;
		    parent=index; //identifies the parent id
		    break;
		  }
		index++;
	      }
	
	    node_count++;
	    Block.resize(node_count);  
	    Cases.resize(node_count);
	
	    // creation of a new Node for the current node in Tree
	    Block[count]=new Node;
	    Cases[count]=new Condition;
	    Block[count]->Set_parent(Block[parent]);  // Sets the parent
	
	    Block[count]->Set_label(buffer1); // Sets the label of the current node
	    Block[count]->Set_Eqn(buffer2);   // Sets the equation of the current node
	    Block[parent]->Set_child(Block[parent]->Get_child_count(),Block[count],Cases[count],buffer4, buffer1,mode); // Sets the node as a child of its parent
	    count++;
	    gpFile.getline(buffer3, maxSize);
	
	  }
    
	cout << "In req. dir";
	system("pwd");
    
	chdir(buffer0);
	system("rm orig_path");
    
    
	delete buffer0, buffer1;
	delete buffer2, buffer3;	
	delete dir_name, buffer4;
      }
  
  
    if (mode==2)    // Input mode, i.e., user would have to input the Tree and save it if necessary
      //  for later use
      {
	char more='y';
	while ((more=='y') || (more=='Y'))
	  {
	    cout << "More nodes? (Y/N): ";
	    cin >> more;  
	    cout << endl;
	
	    if ((more=='y') || (more=='Y'))
	      {
		char* str1 = new char[400];
		char* str2 = new char[400];
	    
		// Get Equation for new node
		cout << "Give Equation: ";   
		cin >> str1;

		// Attach to which parent node?
		cout << "\nWhat is the Parent of new Node :";
		cin >> str2;
		cout << endl;
	      	
		int i = 0; 
		int flag = 0;
		int par=0;
	
		// Search for the Parent node in the list of nodes
		while ((flag==0) && (i<node_count))            
		  {
		    if (strcmp(Block[i]->Ret_label(),str2)==0)   
		      {
			flag=1;
			par=i;
		      }
		    i++;
		  }
		
		if (flag==0)
		  {
		    cout << "Sorry! No such node found to be parent.\n";
		    more='y';
		  }
		else
		  {
		    char* str3 = new char[200];	
		    node_count++; 
		    Block.resize(node_count);
		    Cases.resize(node_count);
		    Block[node_count-1]=new Node;
		    Cases[node_count-1]=new Condition;
		    Block[node_count-1]->Set_Eqn(str1);
		    cout << "Give label for node :";
		    cin >> str3; 
		    cout << endl;
		    Block[node_count-1]->Set_label(str3);
		    Block[node_count-1]->Set_parent(Block[par]);
		    Block[par]->Set_child(Block[par]->Get_child_count(),Block[node_count-1],Cases[node_count-1],str1);
		    delete str3;
		  }
		delete str1, str2;
	      }
	    else
	      more='n';
	  }
	
	// Save the Tree structure and data into files so that they can be read abd used later
	cout << "The Tree structure and constraints can be saved in a directory." << endl;
	cout << "Save (Y/N)? ";
	char save_info;
	cin >> save_info;
	if ((save_info=='y') || (save_info=='Y'))
	  {
	    // Get the name of the directory from the user
	    char* dir_name = new char[100];
	    cout << "\nGive name of new directory :";
	    cin >> dir_name;
	    
	    // make a sub-directory under the current directory and give permission 755
	    char* cmd2 = new char[200];
	    chdir("datafiles");
	    system("pwd");
	    mkdir(dir_name, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	    strcpy(cmd2,"chmod ");
	    strcat(cmd2,"755 ");	    
	    strcat(cmd2, dir_name);
	    system(cmd2); 
	    
	    // go into the newly created directory
	    chdir(dir_name);
	    system("pwd");

	    // make new files in the directory
	    ofstream out_file("Groups.txt");
	    Block[0]->write_Tree(file_String);
	    out_file << file_String;
	    
	    delete dir_name, cmd2;
	  }
      }

    

    //copies the Tree into an array
    //copyTreeIntoArray(array, strArray);
    
    //int f=0;
    //copies the array and builds the Tree
    //copyArrayIntoTree(&f, array, strArray);
    //copyArrayIntoTree(array, strArray);

    //copyTreeIntoArray(array, strArray);
    
    
  }
  

  void copyTreeIntoArray(int *array, char *strArray)
  {
    char* name[10000]; 

    for (int i=0; i<10000; i++)
      name[i]=0;

    vector <Node *> Q;
    vector <int> suppQ;


    /*
    for (int i=0; i<10000; i++)
      array[i]=-2;
    for (int i=0; i<10000; i++)
      strArray[i]='\n';
    */      
  
    Node *nd = NULL;

    cout << "Starting location for data: " << array[0] << endl; 

    int index=array[0];

    //int strStore = index+1;  //Old
    int strStore = index++; //New
    int strIndex = 0;
    array[strStore]=0;

    int parIndex=0, nodeNo=0;
    int count=1; 
    
    /*  Old  */  
    array[index++]=nodeNo;
    array[index++]=nodeNo;
    array[index++]=Block[0]->Get_child_count();

    /* New 
    array[index+1]=nodeNo;
    array[index+2]=nodeNo;
    array[index+3]=Block[0]->Get_child_count();
    index+=4;
    */    

    array[0]=index;

    for (int i=0; i<Block[0]->Get_child_count(); i++)
      {
	Block[0]->cond[i]->copyCondIntoArray(array);
	index = array[0];
	//cout << "SPSPSPSIndex & Array[0]= " << index << " " << array[0] << endl;
      }
    nodeNo++;
 
    nd=Block[0];
    suppQ.push_back(nd->Get_child_count());
    while (count <= nd->Get_child_count())
      {
	Q.push_back(nd->ptr[count-1]);
	count++;
      }
    
    while (!Q.empty())
      {
	nd = popQ(Q);
	array[index++]=nodeNo;
	array[index++]=parIndex;
	
      	strIndex = array[strStore];
	cout << "index Here : " << index << " " << strIndex << endl;
	array[index++]=strIndex;

	strcpy((strArray+strIndex),nd->Get_Eqn());
	strIndex = strIndex + strlen(nd->Get_Eqn())+1;
	array[strStore] = strIndex;
	
	name[index-3]= new char[strlen(nd->Ret_label())+1];
	strcpy(name[index-3],nd->Ret_label());
	
	if (nd->parent !=NULL)
	  {
	    name[index-2]= new char[strlen(nd->parent->Ret_label())+1];
	    strcpy(name[index-2],nd->parent->Ret_label());
	  }
	
	array[index++]=nd->Get_child_count();
	array[0]=index;
	for (int i=0; i<(nd->Get_child_count()); i++)
	  {
	    nd->cond[i]->copyCondIntoArray(array);
	    index = array[0];
	    //cout << "HHHHHHHHHIndex & Array[0]= " << index << " " <<array[0] << endl;
	  }
	
	count=1;
	suppQ.push_back(nd->Get_child_count());
	while (count <= nd->Get_child_count())
	  {
	    Q.push_back(nd->ptr[count-1]);
	    count++;
	  }
	
	if (suppQ[0]==0)
	  {
	    if (suppQ.size()!=0)
	    {
	      popfunc(suppQ);
	      parIndex++;
	    }
	  }
	else
	  {
	    suppQ[0]-=1;
	    if (suppQ[0]==0)
	      {
		if (suppQ.size()!=0)
		  {
		    popfunc(suppQ);
		    parIndex++;
		  }
		while (suppQ[0]==0)
		  {
		    if (suppQ.size()!=0)
		      {
			popfunc(suppQ);
			parIndex++;
		      }
		    else
		      break;
		  }
	      }
	  }
	nodeNo++;
      }
    array[index++]=-1;
    cout << "Max for Tree : " << index-1 <<endl;
    cout << "Value at max index = " << array[index-1] << endl; 
    array[0]=index;

    cout << "\nThe arrays\n";
    for (int i=0; i<100; i++)
      {
	/*
	int a;
	if (i%1000==0)
	  cin >> a;
	*/
	if (array[i]!=-2)
	  {
	    cout << "index=" << i << " " << array[i] << endl; //<< " " << name[i] << endl;
	  } 
	else
	  break;
      }
    
    for (int i=0; i<100; i++)
      {
	if (strArray[i]!='\n')
	  {
	    cout << strArray[i] ;
	    if (strArray[i]=='\0')
	      cout << endl;
	  }
      }
    cout << "The arrays\n";
  }
  

  //pops the first element out of the vector
  void popfunc(vector <int> &A)
  {
    int check=0;
    int size=A.size();
    while (check<size-1)
      {
	A[check]=A[check+1];
	check++;
      }
    A.pop_back();
  }

  //pops the first element out of the vector of Nodes
  Node *popQ(vector <Node *> &A)
  {
    Node *temp = A[0];
    int check=0;
    int size=A.size();
    while (check<size-1)
      {
	A[check]=A[check+1];
	check++;
      }
    A.pop_back();
    return temp;
  }


  /*
void copyArrayIntoTree(int &startI, int *&array, char *&strArray)
  {}
  */

  // Changes the data in array to Tree form 
    void copyArrayIntoTree(int &startI, int *&array, char *&strArray)
      //void copyArrayIntoTree(int *array, char *strArray)
  {
    int index=startI;
    
    //cout << "INDEX = " << index;
    //int ll;
    //cin >> ll;

    
    int strStore = index;
    int strIndex = 0;

    //clears the old Tree
    Block.clear();
    node_count=0;
    
    node_count++;  
    Block.resize(node_count);
    Cases.resize(node_count);
    Block[node_count-1]=new Node; 
    Cases[node_count-1]=new Condition;
    Block[node_count-1]->Set_parent(Block[0]);
    char *rootname = new char[5];
    strcpy(rootname,"Root");
    Block[0]->Set_label(rootname);   
    

    index+=3;
    startI=index;

    int nodeIndex = 0;
    int parIndex = 0;
    int no_of_children = array[index++];
    //cout << "No. of children = " << no_of_children << endl;

    startI=index;
    
    while (array[index]!=-1)   //until all nodes are retrieved 
      {
	for (int i=0; i<no_of_children; i++)  // set all conditions for child nodes
	  {
	    Block[nodeIndex]->cond.resize(i+1);
	    Block[nodeIndex]->cond[i] = new Condition;
	    Block[nodeIndex]->cond[i]->copyArrayIntoCond(startI,array);
	    index = startI;
	  }

	if (array[index]!=-1)
	  {
	    //cout << "index = " << index << " " << ;
	    
	    nodeIndex = array[index++];  //get Node no.
	    parIndex = array[index++];   //get node no. of parent
	    
	    node_count++;  
	    Block.resize(node_count);
	    Cases.resize(node_count);
	    Block[node_count-1]=new Node; 
	    Cases[node_count-1]=new Condition;
	    Block[node_count-1]->child_count=0;

	    Block[node_count-1]->Set_parent(Block[parIndex]); //set parent node of current node

	    char *nodeName = new char[15];
	    char *temp = new char[15];
	    strcpy(nodeName,"Node:");
	    strcat(nodeName,itoa(nodeIndex, temp));
 	    Block[nodeIndex]->Set_label(nodeName);   //set label to current node
	    
	    Block[parIndex]->child_count++; //increase no. of children of its parent
	    Block[parIndex]->ptr.resize(Block[parIndex]->child_count);
	    Block[parIndex]->ptr[Block[parIndex]->child_count-1]=Block[nodeIndex]; //set current node as child for its parent
	
	    strIndex = array[index++];
	    Block[nodeIndex]->Set_Eqn(strArray+strIndex);  //sets equation of the current node
	        
	    no_of_children = array[index++];
	    startI=index;
	    //startI = index;
	  }
      }

      startI++;
  }
  

  
  //converts a valid integer into a string
  char* itoa(int n, char s[])
  {
    int i, sign;
    if ((sign = n ) < 0)
      n = -n;
    i=0;
    do 
      {
	s[i++] = n%10 + '0';
      } while ((n/=10)>0);
    if (sign < 0)
      s[i++]= '-';
    s[i]='\0';
    reverse(s);
    return s;
  }
  
  
  // reverses a string
  void reverse(char s[])
  {
    int c, j;
    for (int i=0, j=strlen(s)-1; i<j; i++, j--)
      {
	c=s[i];
	s[i]=s[j];
	s[j]=c;
      }
  }



  // Display the Tree so far constructed
  void display_Tree()
  {
    cout << "\nThe Tree:\n\n"; 
    cout << "Root" << endl;
    Block[0]->showTree();
    cout << endl;
  }

  // Searches the Tree for a single constraint in automated search mode
  char* search_Tree(int a, int b, int c, int d, int e)
  {  
    return(Block[0]->Find_Eqn_def_mode(a,b,c,d,e));    
  }
  


  // Searches the Tree for a set of constraints but in a one-by-one basis rather than en-bloc
  string search_Tree(Graph &graph0, Cluster &theCluster, ostream &inputFile, int flag)
  {
    ClustData test;
    
    test=summCluster(graph0, theCluster);
    
    int length=test.length;
    string newString, outputString;	 
    Vertex vEnd1, vEnd2;
    int end1C, end2C;
    
    //set correct output style in maple
    outputString+="interface(prettyprint=0);\n";
    
    //ten second solution time limit set
    outputString+="timelimit(10,solve({";
    
    // Tree.search_Tree();
    
  
    //outputString+=search_Tree(test, graph0, theCluster, inputFile);
    
    
    for(int i=0; i<length; i++)
      {
	int a,b,c,d,e;
	float value;
	
	a=test.e1Type[i];
	b=test.e1Part[i];
	c=test.e2Type[i];
	d=test.e2Part[i];
	e=test.edgeType[i];
	value=test.values[i];
	
	int end1ID=test.e1ID[i];
	int end2ID=test.e2ID[i];
	
	string str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13;
	string rep_1, rep_2, rep_3, rep_4, rep_5, rep_6, rep_7, rep_8, rep_9, rep_10, rep_11, rep_12, rep_13;
	
	cout << "Search :" << a << b << c << d << e << ": "; 
	newString=search_Tree(a,b,c,d,e);
	cout << newString << endl;
	
	
	//search_Tree(a,b,c,d,e)      - Searches tree for a single constraint
	//search_Tree()               - Searches tree in User-input-Search mode

	if (e==4)
	  value = (tan((value/180)*PI))*(tan((value/180)*PI)); 	
	//value = (tan((value/180)*PI));
	if (e==8)
	  value = cos((value/180)*PI);

	//newString=replaceAll(newString,"1",toString(end1ID));
	//newString=replaceAll(newString,"2",toString(end2ID));
	newString=replaceAll(newString,"value","("+toString(value)+")");
	//      newString=replaceAll(newString,",","\n");
	
	int j=0, k=0;
	for (int round=1;round<=2; round++) 
	  {		
	    if (round==1) 
	      {  
		j=end1ID;
		k=a;     
	      } 
	    else
	      {
		j=end2ID;
		k=c;    
	      }
	    //	  cout << "j=" << j << ",k=" << k << endl;
	    switch(k)
	      {
	      case 0:		
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j); 
		str2="y"+str2;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		str1="x"+toString(j);
		str2="y"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		break;
	      case 1:
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j);
		str2="y"+str2;
		str3="?"+toString(j);
		str3="m"+str3;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_3="m"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_3,str3);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str3="m"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str3)) vars.append(str3);
		break;
	      case 2:
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j);
		str2="y"+str2;
		str4="?"+toString(j);
		str4="w"+str4;
		str5="?"+toString(j);
		str5="v"+str5;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_4="w"+toString(round);
		rep_5="v"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_4,str4);
		newString=replaceAll(newString,rep_5,str5);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str4="w"+toString(j);
		str5="v"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str4)) vars.append(str4);
		if(!vars.hasElem(str5)) vars.append(str5);
		break;
	      case 3:
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j);
		str2="y"+str2;
		str6="?"+toString(j);
		str6="c"+str6;
		str7="?"+toString(j);
		str7="d"+str7;
		str3="?"+toString(j);
		str3="m"+str3;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_6="c"+toString(round);
		rep_7="d"+toString(round);
		rep_3="m"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_6,str6);
		newString=replaceAll(newString,rep_7,str7);
		newString=replaceAll(newString,rep_3,str3);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str6="c"+toString(j);
		str7="d"+toString(j);
		str3="m"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str6)) vars.append(str6);
		if(!vars.hasElem(str7)) vars.append(str7);
		if(!vars.hasElem(str3)) vars.append(str3);
		break;
	      case 4:
		str1="?"+toString(j); 
		str1="x"+str1;
		str2="?"+toString(j);
		str2="y"+str2;
		str8="?"+toString(j);     
		str8="r"+str8;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_8="r"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_8,str8);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str8="r"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str8)) vars.append(str8);
		break;
	      case 5:
		str1="?"+toString(j);      
		str1="x"+str1;
		str2="?"+toString(j); 
		str2="y"+str2;
		str8="?"+toString(j); 
		str8="r"+str8;
		str11="?"+toString(j);
		str11="a"+str11;
		str12="?"+toString(j);
		str12="b"+str12;
		str5="?"+toString(j);
		str5="v"+str5;
		str4="?"+toString(j);
		str4="w"+str4;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_8="r"+toString(round);
		rep_11="a"+toString(round);
		rep_12="b"+toString(round);      
		rep_4="w"+toString(round);
		rep_5="v"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_8,str8);
		newString=replaceAll(newString,rep_11,str11);
		newString=replaceAll(newString,rep_12,str12);
		newString=replaceAll(newString,rep_4,str4);
		newString=replaceAll(newString,rep_5,str5);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str8="r"+toString(j);
		str11="a"+toString(j);
		str12="b"+toString(j);
		str4="w"+toString(j);
		str5="v"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str8)) vars.append(str8);
		if(!vars.hasElem(str11)) vars.append(str11);	
		if(!vars.hasElem(str12)) vars.append(str12);
		if(!vars.hasElem(str4)) vars.append(str4);
		if(!vars.hasElem(str5)) vars.append(str5);
		break;
	      case 6:		
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j); 
		str2="y"+str2;
		str13="?"+toString(j);
		str13="z"+str13;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_13="z"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_13,str13);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str13="z"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str13)) vars.append(str13);
		break;
	      case 7:
		str1="?"+toString(j);
		str1="x"+str1;
		str2="?"+toString(j);
		str2="y"+str2;
		str3="?"+toString(j);
		str3="z"+str3;
		str6="?"+toString(j);
		str6="c"+str6;
		str7="?"+toString(j);
		str7="d"+str7;
		str8="?"+toString(j);
		str8="e"+str8;
		rep_1="x"+toString(round);
		rep_2="y"+toString(round);
		rep_3="z"+toString(round);
		rep_6="c"+toString(round);
		rep_7="d"+toString(round);
		rep_8="e"+toString(round);
		newString=replaceAll(newString,rep_1,str1);
		newString=replaceAll(newString,rep_2,str2);
		newString=replaceAll(newString,rep_3,str3);
		newString=replaceAll(newString,rep_6,str6);
		newString=replaceAll(newString,rep_7,str7);
		newString=replaceAll(newString,rep_8,str8);
		str1="x"+toString(j);
		str2="y"+toString(j);
		str3="z"+toString(j);
		str6="c"+toString(j);
		str7="d"+toString(j);
		str8="e"+toString(j);
		if(!vars.hasElem(str1)) vars.append(str1);
		if(!vars.hasElem(str2)) vars.append(str2);
		if(!vars.hasElem(str6)) vars.append(str6);
		if(!vars.hasElem(str7)) vars.append(str7);
		if(!vars.hasElem(str3)) vars.append(str3);
		if(!vars.hasElem(str8)) vars.append(str8);
		break;
	      }
	  }
	
	newString=replaceAll(newString,"?","");
	
	vEnd1=graph0.returnVertByName(end1ID);
	vEnd2=graph0.returnVertByName(end2ID);
      
	end1C=getChildNameWithVertex(theCluster, end1ID);
	end2C=getChildNameWithVertex(theCluster, end2ID);

	int ind1=getChildIndexByName(theCluster, end1C);
	int ind2=getChildIndexByName(theCluster, end2C);

/*	if(theCluster.children.retrieve(ind1).returnType()==2 ||
	   theCluster.children.retrieve(ind2).returnType()==2)
	{	
		outputString+=newString;
		if(i<(length-1)) outputString+=",";
		continue;
	}
*/	

	setValueInCluster(graph0,theCluster.children.retrieve(ind1), vEnd1);
	setValueInCluster(graph0,theCluster.children.retrieve(ind2), vEnd2);

	if (end1C==-1 || end2C==-1) return "";
	if (end1C!=end1ID) 
	  {
	    newString=switchString(vEnd1, a, end1ID, end1C, newString, end1C!=withHeldCluster);
	    if(end1C!=withHeldCluster)
	      newString=switchString(vEnd1, a, end1ID, end1C, newString, false);
	  }
	if (end2C!=end2ID)
	  {
	    newString=switchString(vEnd2, c, end2ID, end2C, newString, end2C!=withHeldCluster);
	    if(end2C!=withHeldCluster)
	      newString=switchString(vEnd2, c, end2ID, end2C, newString, false);
	  }
	outputString+=newString;
	if(i<(length-1)) outputString+=",";
      
      }
    return outputString;
  }
  

  // Searches the Tree in En-Bloc fashion for the constraints
  string search_Tree(Graph &graph0, Cluster &theCluster, ostream &inputFile)
  {
    ClustData test;
    test=summCluster(graph0, theCluster);
    
    int no_of_searches=0;        // No. of searches to be done 
    vector <Ord_Set *> Searches; // The dynamic list for Search operation 
    vector <char *> t_str;
    
    int t_par[500];
    int flag_arr[500];
    char contin = 'y';
  
    int length=test.length;
    cout << "Length : " << length << endl;

    string tempString, newString, outputString;
    string stringArray[500];
    Vertex vEnd1, vEnd2;
    int end1C, end2C;    


    //set correct output style in maple
    outputString+="interface(prettyprint=0);\n";
              
    //ten second solution time limit set
    outputString+="timelimit(10,solve({";

    for (int i=0; i<length; i++)
      {
	int a=test.e1Type[i];
	int b=test.e1Part[i];
	int c=test.e2Type[i];
	int d=test.e2Part[i];
	int e=test.edgeType[i];
      	
	no_of_searches++;
	Searches.resize(no_of_searches);
	t_str.resize(no_of_searches);
	Searches[no_of_searches-1]=new Ord_Set;
	Searches[no_of_searches-1]->Set_Val(a,b,c,d,e);
	t_str[no_of_searches-1] = new char[200];
	t_par[no_of_searches-1] = 0;
	flag_arr[no_of_searches-1] = 0;
      }
    
    cout << "No_of_Searches : " << no_of_searches << endl;

    while (contin=='y')
      {	
	cout<<"While Loop"<<endl;
	for (int j=0; j<no_of_searches; j++) // Search the Tree
	  {
	    cout<<"For Loop j="<<j<<endl;
	    int a,b,c,d,e;
	    int end1ID=test.e1ID[j];
	    int end2ID=test.e2ID[j];
	    float value=test.values[j];

	    cout<<"Before searches"<<endl;
	    
	    a=Searches[j]->ret_1();
	    b=Searches[j]->ret_2();
	    c=Searches[j]->ret_3();
	    d=Searches[j]->ret_4();
	    e=Searches[j]->ret_5();
	    cout << "Search No." << j+1 << ": (" <<a<< "," <<b<< "," <<c<< "," <<d<< "," <<e<<  "): ";
	    //strcpy(t_str[j],Block[t_par[j]]->Find_Eqn(a,b,c,d,e)) ;
	    strcpy(t_str[j],Block[t_par[j]]->Find_Eqn(a,b,c,d,e)) ;

	    if (e==4)
	      value = (tan((value/180)*PI))*(tan((value/180)*PI));
	    //value = (tan((value/180)*PI));
	    if (e==8)
	      value = cos((value/180)*PI);
	    
	    tempString=t_str[j];
	    tempString=replaceAll(tempString,"value","("+toString(value)+")");

	    string str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13;
	    string rep_1, rep_2, rep_3, rep_4, rep_5, rep_6, rep_7, rep_8, rep_9, rep_10, rep_11, rep_12, rep_13;

	    
	    int ind=0, k=0;

	    
	    for (int round=1;round<=2; round++) 
	      {		
		if (round==1) 
		  {  
		    ind=end1ID;
		    k=a;     
		  } 
		else
		  {
		    ind=end2ID;
		    k=c;    
		  }
	    


		switch(k)
		  {
		  case 0:		
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind); 
		    str2="y"+str2;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    break;
		  case 1:
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind);
		    str2="y"+str2;
		    str3="?"+toString(ind);
		    str3="m"+str3;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_3="m"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_3,str3);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str3="m"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str3)) vars.append(str3);
		    break;
		  case 2:
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind);
		    str2="y"+str2;
		    str4="?"+toString(ind);
		    str4="w"+str4;
		    str5="?"+toString(ind);
		    str5="v"+str5;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_4="w"+toString(round);
		    rep_5="v"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_4,str4);
		    tempString=replaceAll(tempString,rep_5,str5);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str4="w"+toString(ind);
		    str5="v"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str4)) vars.append(str4);
		    if(!vars.hasElem(str5)) vars.append(str5);
		    break;
		  case 3:
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind);
		    str2="y"+str2;
		    str6="?"+toString(ind);
		    str6="c"+str6;
		    str7="?"+toString(ind);
		    str7="d"+str7;
		    str3="?"+toString(ind);
		    str3="m"+str3;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_6="c"+toString(round);
		    rep_7="d"+toString(round);
		    rep_3="m"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_6,str6);
		    tempString=replaceAll(tempString,rep_7,str7);
		    tempString=replaceAll(tempString,rep_3,str3);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str6="c"+toString(ind);
		    str7="d"+toString(ind);
		    str3="m"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str6)) vars.append(str6);
		    if(!vars.hasElem(str7)) vars.append(str7);
		    if(!vars.hasElem(str3)) vars.append(str3);
		    break;
		  case 4:
		    str1="?"+toString(ind); 
		    str1="x"+str1;
		    str2="?"+toString(ind);
		    str2="y"+str2;
		    str8="?"+toString(ind);     
		    str8="r"+str8;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_8="r"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_8,str8);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str8="r"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str8)) vars.append(str8);
		    break;
		  case 5:
		    str1="?"+toString(ind);      
		    str1="x"+str1;
		    str2="?"+toString(ind); 
		    str2="y"+str2;
		    str8="?"+toString(ind); 
		    str8="r"+str8;
		    str11="?"+toString(ind);
		    str11="a"+str11;
		    str12="?"+toString(ind);
		    str12="b"+str12;
		    str5="?"+toString(ind);
		    str5="v"+str5;
		    str4="?"+toString(ind);
		    str4="w"+str4;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_8="r"+toString(round);
		    rep_11="a"+toString(round);
		    rep_12="b"+toString(round);      
		    rep_4="w"+toString(round);
		    rep_5="v"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_8,str8);
		    tempString=replaceAll(tempString,rep_11,str11);
		    tempString=replaceAll(tempString,rep_12,str12);
		    tempString=replaceAll(tempString,rep_4,str4);
		    tempString=replaceAll(tempString,rep_5,str5);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str8="r"+toString(ind);
		    str11="a"+toString(ind);
		    str12="b"+toString(ind);
		    str4="w"+toString(ind);
		    str5="v"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str8)) vars.append(str8);
		    if(!vars.hasElem(str11)) vars.append(str11);	
		    if(!vars.hasElem(str12)) vars.append(str12);
		    if(!vars.hasElem(str4)) vars.append(str4);
		    if(!vars.hasElem(str5)) vars.append(str5);
		    break;
		  case 6:		
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind); 
		    str2="y"+str2;
		    str13="?"+toString(ind);
		    str13="z"+str13;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_13="z"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_13,str13);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str13="z"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str13)) vars.append(str13);
		    break;
		  case 7:
		    str1="?"+toString(ind);
		    str1="x"+str1;
		    str2="?"+toString(ind);
		    str2="y"+str2;
		    str3="?"+toString(ind);
		    str3="z"+str3;
		    str6="?"+toString(ind);
		    str6="c"+str6;
		    str7="?"+toString(ind);
		    str7="d"+str7;
		    str8="?"+toString(ind);
		    str8="e"+str8;
		    rep_1="x"+toString(round);
		    rep_2="y"+toString(round);
		    rep_3="z"+toString(round);
		    rep_6="c"+toString(round);
		    rep_7="d"+toString(round);
		    rep_8="e"+toString(round);
		    tempString=replaceAll(tempString,rep_1,str1);
		    tempString=replaceAll(tempString,rep_2,str2);
		    tempString=replaceAll(tempString,rep_3,str3);
		    tempString=replaceAll(tempString,rep_6,str6);
		    tempString=replaceAll(tempString,rep_7,str7);
		    tempString=replaceAll(tempString,rep_8,str8);
		    str1="x"+toString(ind);
		    str2="y"+toString(ind);
		    str3="z"+toString(ind);
		    str6="c"+toString(ind);
		    str7="d"+toString(ind);
		    str8="e"+toString(ind);
		    if(!vars.hasElem(str1)) vars.append(str1);
		    if(!vars.hasElem(str2)) vars.append(str2);
		    if(!vars.hasElem(str6)) vars.append(str6);
		    if(!vars.hasElem(str7)) vars.append(str7);
		    if(!vars.hasElem(str3)) vars.append(str3);
		    if(!vars.hasElem(str8)) vars.append(str8);
		    break;
		  }
	      }
	    
	    tempString=replaceAll(tempString,"?","");
	    
	    stringArray[j]=tempString;
	    
	    //cout << "tempString: " << tempString << endl;
	    cout << stringArray[j] << endl;
	    	    
	    int u = 0; 
	    int flag = 0;
	    while ((flag==0) && (u<node_count))            
	      {
		if (strcmp(Block[u]->Get_Eqn(),t_str[j])==0)   
		  {
		    flag=1;
		    t_par[j]=u;
		  }
		u++;
	      }
	  
	    if (glo_flag==1)
	      {
		flag_arr[j]=0;
		cout << "  Search ends\n";
	      }
	    else
	      {
		flag_arr[j]=1;
		cout << "  Search can continue\n";
	      }
	  
	  }

      	
	int sig_flag=0; 
	for (int ind=0; ind<no_of_searches; ind++)
	  {
	    if (flag_arr[ind]==1)
	      { 
		sig_flag=1; 
		continue;
	      }
	  }
      

	if (sig_flag==1)
	  {
	    cout << "Continue? (y/n)" ;
	    contin='y';
	    //cin >> contin; cout << endl;
	    if ((contin=='y') || (contin=='Y'))
	      contin = 'y';
	    else
	      contin = 'n';
	  }
	else contin = 'n';
      }	  
  
  
    for (int i=0; i<no_of_searches; i++)
      {
	int a=Searches[i]->ret_1();
	int c=Searches[i]->ret_3();

	Edge theEdge=graph0.returnEdgeByName(test.edgeID[i]);
	    
	int end1ID=theEdge.returnEnd1();
	int end2ID=theEdge.returnEnd2();
	newString=stringArray[i];
	vEnd1=graph0.returnVertByName(end1ID);
	vEnd2=graph0.returnVertByName(end2ID);
	    
	end1C=getChildNameWithVertex(theCluster, end1ID);
	end2C=getChildNameWithVertex(theCluster, end2ID);

	int ind1=getChildIndexByName(theCluster, end1C);
        int ind2=getChildIndexByName(theCluster, end2C);

        if(theCluster.children.retrieve(ind1).returnType()==2 ||
           theCluster.children.retrieve(ind2).returnType()==2)
	{	
		outputString+=newString;
        	if(i<(length-1)) outputString+=",";
                continue;
	}

        setValueInCluster(graph0,theCluster.children.retrieve(ind1), vEnd1);
        setValueInCluster(graph0,theCluster.children.retrieve(ind2), vEnd2);

	if (end1C==-1 || end2C==-1) return "";
	if (end1C!=end1ID) 
	  {
	    newString=switchString(vEnd1, a, end1ID, end1C, newString, end1C!=withHeldCluster);
	    if(end1C!=withHeldCluster)
	      newString=switchString(vEnd1, a, end1ID, end1C, newString, false);
	  }
	if (end2C!=end2ID)
	  {
	    newString=switchString(vEnd2, c, end2ID, end2C, newString, end2C!=withHeldCluster);
	    if(end2C!=withHeldCluster)
	      newString=switchString(vEnd2, c, end2ID, end2C, newString, false);
	  }
	outputString+=newString;
	if(i<(length-1)) outputString+=",";
      }
	
    cout<<"outputString="<<outputString<<endl;
    return outputString;
  }


  // Searches the Tree En-Block in user search mode
  void search_Tree()
  {      
    int no_of_searches=0;        // No. of searches to be done 
    vector <Ord_Set *> Searches; // The dynamic list for Search operation 
    vector <char *> t_str;
    
    int t_par[500];
    int flag_arr[500];
    char contin = 'y';
    
    while ((contin=='y') || (contin=='Y'))  // What should be searched?? Get the inputs
      {
	int a,b,c,d,e;
	no_of_searches++;
	cout << "Give conditions for search : " ;
	cin >> a >> b >> c >> d >> e;
	cout << "Search No." << no_of_searches << ": " <<a<< "," <<b<< "," <<c<< "," <<d<< "," <<e;
	Searches.resize(no_of_searches);
	t_str.resize(no_of_searches);
	Searches[no_of_searches-1]=new Ord_Set;
	Searches[no_of_searches-1]->Set_Val(a,b,c,d,e);
	t_str[no_of_searches-1] = new char[200];
	t_par[no_of_searches-1] = 0;
	flag_arr[no_of_searches-1] = 0;
	cout << "\nContinue (Y/N): ";
	cin >> contin;
      }
    
    contin='y';
    glo_flag=0;
    while (contin=='y')
      {	
	for (int x=0; x<no_of_searches; x++) // Search the Tree
	  {
	    int a,b,c,d,e;
	    a=Searches[x]->ret_1();
	    b=Searches[x]->ret_2();
	    c=Searches[x]->ret_3();
	    d=Searches[x]->ret_4();
	    e=Searches[x]->ret_5();
	    cout << "Search No." << x+1 << ": (" <<a<< "," <<b<< "," <<c<< "," <<d<< "," <<e<<  "): ";
	    cout << "Parent: " << t_par[x] << " " << Block[t_par[x]]->Ret_label() << endl;
	    //cout << "\nGlobal: " << glo_flag;
	    strcpy(t_str[x],Block[t_par[x]]->Find_Eqn(a,b,c,d,e)) ;
	    cout << t_str[x] ; 
	    int i = 0; 
	    int flag = 0;
	    while ((flag==0) && (i<node_count))            
	      {
		if (strcmp(Block[i]->Get_Eqn(),t_str[x])==0)   
		  {
		    /*
		      cout << "\nBlidsfdsf : " << Block[i]->Get_Eqn();
		      cout << "\nt_str: " << t_str[x] << endl;
		    */
		    flag=1;
		    t_par[x]=i;
		    //cout << "Parent: " << t_par[x];
		  }
		i++;
	      }
	    if (glo_flag==1)
	      {
		flag_arr[x]=0;
		cout << "  Search ends\n";
	      }
	    else
	      {
		flag_arr[x]=1;
		cout << "  Search can continue\n";
	      }
	  }
	
	int sig_flag=0; 
	for (int x=0; x<no_of_searches; x++)
	  {
	    if (flag_arr[x]==1)
	      { 
		sig_flag=1; 
		//continue;
	      }
	  }
	if (sig_flag==1)
	  {
	    cout << "Continue? (y/n)" ;
	    cin >> contin; cout << endl;
	    if ((contin=='y') || (contin=='Y'))
	      contin = 'y';
	    else
	      contin = 'n';
	  }
	else contin = 'n';
      }
    return;
  }
  

};  // End of class the_Tree

/*
  cout << Block[0]->Find_Eqn(3,2,3,2,1,0) << endl; 
  cout << Block[0]->Find_Eqn(3,2,0,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(2,1,0,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,2,2,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(4,3,3,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,1,3,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(1,0,3,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,0,0,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,0,5,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,0,5,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,2,1,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(5,2,3,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,2,3,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(5,1,3,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(2,1,1,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(2,0,2,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(2,0,5,1,1,0) << endl;
  cout << Block[0]->Find_Eqn(2,0,3,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,0,5,2,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,2,2,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(5,1,2,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(5,2,2,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(6,4,6,5,1,0) << endl;
  cout << Block[0]->Find_Eqn(5,3,2,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(3,1,1,0,1,0) << endl;
  cout << Block[0]->Find_Eqn(1,0,4,3,1,0) << endl;
*/




//Useful in FIND operation. Finds the equation given the end entities and the constraints
char* Node::Find_Eqn_def_mode(int a, int b, int c, int d, int e)
{
  int j=1; 
  int count=1;
  while ((count<=child_count) && (j==1))
    {
      int i=0;
      i=(cond[count-1]->Check_Cond(a,b,c,d,e));  
      if (i==1)
	{
	  return(ptr[count-1]->Find_Eqn(a,b,c,d,e));
	  j=0;
	}
      else count++;
    }
  if (j==1) 
    return eqn;
  else return("No equation exists");
}

//Useful in FIND operation. Finds the equation given the end entities and the constraints
//  This procedure helps in stepping through the Tree during Search
char* Node::Find_Eqn(int a, int b, int c, int d, int e)
{
  int j=1; 
  int count=1;
  while ((count<=child_count) && (j==1))
    {
      int i=0;
      i=(cond[count-1]->Check_Cond(a,b,c,d,e));  
      //cout<<"CHECK CONDITION i = "<<i<<endl;
      if (i==1)
	{
	  glo_flag = 0;
	  return(ptr[count-1]->eqn);
	  j=0;
	}
      else count++;
    }
  if (j==1) 
    {
      glo_flag = 1;
      return eqn;
    }
  else
    {
      glo_flag = 2;
      return("No equation exists");
    }
}


//Useful during FIND operation, checks if any of the conditions are present
int Condition::Check_Cond(int a, int b, int c, int d, int e)
{
  int j=1;
  int count=1;
  while ((count<=cond_count) && (j==1))
    {
      int i=0;
      i=(list[count-1]->Check_List(a,b,c,d,e));
      //cout<<"Cond_Count="<<cond_count<<endl;
      //cout<<"In CHECK CONDITION i = "<<i<<endl;
      if (i==1)
	{
	  return 1;
	  j=0;
	}
      else count++;
    }
  if (j==1) return 0;
  return 0;
}



// Useful during FIND operation, checks if entities, parts and constraints match
int Ord_Set::Check_List(int a, int b, int c, int d, int e)
{
  //cout<<"The ordered sets are: "<<type1<<part1<<type2<<part2<<const_type<<endl;
  if ((type1==a) && (part1==b) && (type2==c) && (part2==d) && (const_type==e))
    return 1;
  else
    return 0;
}


void Condition::copyCondIntoArray(int* array)
{
  int index = array[0];
  array[index++]=cond_count;
  array[0]=index;
  for (int i=0; i<cond_count; i++)
    {
      list[i]->copyOrdSetIntoArray(array);
    }
  //array[0]=index;
  //cout << "Index in Condition : " << index << "\n";
  //return *index;
}


void Condition::copyArrayIntoCond(int &startI, int* array)
{
  int index = startI;
  int count = array[index++];
  startI=index;
  for (int i=0; i<count; i++)
    {
      list.resize(i+1);
      list[i]= new Ord_Set;
      list[i]->copyArrayIntoOrdSet(startI,array);
      index = startI;
    }
  cond_count = count;
}


void Ord_Set::copyOrdSetIntoArray(int* array)
{
  int index = array[0];
  array[index++]=ret_1();
  array[index++]=ret_2();
  array[index++]=ret_3();
  array[index++]=ret_4();
  array[index++]=ret_5();
  array[0]=index;
} 

void Ord_Set::copyArrayIntoOrdSet(int &startI, int* array)
{
  int index = startI;
  int a = array[index++];
  int b = array[index++];
  int c = array[index++];
  int d = array[index++];
  int e = array[index++];
  Set_Val(a,b,c,d,e);
  startI = index;
}
