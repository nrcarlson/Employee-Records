#include <iostream> // cin cout and other I/O
#include <fstream>  // file handling
#include <iomanip>  // formatting output setw( )
#include <cstring>  // using c-string methods
#include <cstdlib>  // exit() ...

using namespace std;

//Global variables
const char binary_file_name[] = "C:/cygwin/home/DSU/csc300data/empl.dat"; //global variable for filepath
const bool CLEAR_SCREEN = false; // true- use system  false- uses blank lines
const int  BLANK_LINES  = 25;   // # of blank lines useds with CLEAR_SCREEN = false

struct REC_TYPE
{
	int empid; // Empl company id {Unique}
	char name[51]; // Empl name can have blanks in it
	double salary; // Empl salary
	char phone[40]; // Empl office phone
	int tax_number; // Empl tax code number {Unique}
	int dept_num; // Empl departmental number
};

struct INDEX_REC{
	int id;		//key field
	long int rid;	//record id
};

//Prototypes
//=====================================================
int main(void);
void clearScreen();
void printQuitOption();
void printMainOption();
int mainMenu();
int viewEmployee();
int editdptNum1();
int editdptNum2();
int listallEmpl();
int console_reader(char * prompt, int input_type, char &c1, int &i1, double &d1);
void build_index(INDEX_REC index[],const int max,int &count);
void sort_index(INDEX_REC index[], const int count);
void update_record(const long int rid, const REC_TYPE r);
void read_record(const long int rid, REC_TYPE &r);
long int get_rid(const INDEX_REC index[], const int count, const int id);
void print_record(INDEX_REC index[], const int count);
void print_all_records(INDEX_REC index[], const int count);
bool valid_menu_input(const int convert_flag, const int min, const int max, const char * valchar_list,const char c, const int n);

// To clear the screen
// ======================================================
void clearScreen()
{
  if (CLEAR_SCREEN)
    cout << "\033[2J";
  else
    for (int i=0; i < BLANK_LINES; i++)
	    cout << endl;
}

// The option to quit
// ======================================================
void printQuitOption()
{
	cout << endl << endl << "You may enter Q to Quit." << endl;
}

// The option to return to main menu
// ======================================================
void printMainOption()
{
	cout << endl << endl << "You may enter M to return to Main Menu." << endl;
}

//console reader function takes in data from user and tests it for type
// =======================================================
int  console_reader(char * prompt, int input_type, char &c1, int &i1, double &d1)
{
   int convert_flag = 0;  // set to invalid 
   char line_buffer[121]; // used to read a line of text up to 120 useable characters;
   //char junk[11];
   cout << prompt;
   cin.getline(line_buffer,120,'\n');
   
   // clear output parameters
   c1 = 'X';  i1 = -999; d1 = -999.99;
   
   char fchar = line_buffer[0]; // first character entered by user
   
   if (!isalnum(fchar))  // check first character for letter or number
        return convert_flag; // invalid user input
   
   if (isalpha(fchar))  // user typed a letter
     input_type = 1;
   
   switch (input_type)
   {
      case 1:// expecting a single character
	          c1 = fchar;
			  convert_flag = 1;
			  break;
			  
	  case 2: // expecting an int
	          if (isdigit(fchar))
			  {
					i1 = atoi(line_buffer);
					convert_flag = 2;
              }
	          break;
			  
	  case 3: // expecting a double
	          if (isdigit(fchar))
			  {
			     d1 = atof(line_buffer);
			     convert_flag = 3;
              }				 
	          break;
			  
	  default:
	          cout << "Prog ERROR invalid input_type  CONTACT DEVELOPER !" << endl;
              exit(1);   //violation of pre-condition
	}
    
	return convert_flag; // what was actually converted
}

//function that is called for main menu, this is the first thing the user sees onscreen
//=======================================================
int mainMenu(){
	int flag;
    int num;
    int n = 0;
    char c = 'x';
    double d = 999.99;
	
	cout<<""<<endl<<endl<<endl;
	cout<<"Main Menu"<<endl;
	cout<<"{Nick Carlson}"<<endl<<endl;
	cout<<"1) VIEW an employee"<<endl;
	cout<<"2) EDIT an employee's department number"<<endl;
	cout<<"3) LIST all employees"<<endl<<endl;
	printQuitOption();
	do{
	flag = console_reader("Please enter your selection from the menu [1-3 or Q]: ",2,c,n,d);
	}while (!valid_menu_input(flag,1,3,"Qq",c,n));
	
	if (flag == 2)
   {
      num = n;
	  cout << "Num is:" << n << endl;
   }else{
		if (flag == 1)
		{
			return 0;
		}
	}
	
	return num;
}

//function that allows the user to enter an employee ID and view their record
//=======================================================
int viewEmployee(){
	int flag;
    int n = 0;
    char c = 'x';
    double d = 999.99;
	int id;
	
	cout<<""<<endl<<endl<<endl;
	cout<<"View Employee"<<endl<<endl;
	cout<<"To view an employee you will need to"<<endl;
	cout<<"enter their employee ID number."<<endl;
	printMainOption();
	cout<<""<<endl;
	do{
	flag = console_reader("Please enter an employee ID number: ",2,c,n,d);
	}while (!valid_menu_input(flag,1,100000,"Mm",c,n));
	
	if (flag == 2)
   {
      id = n;
	  return id;
   }else{
	  return 0;
   }
   
}

//function that asks the user for the ID number of the employee whose department number they wish to edit
//=======================================================
int editdptNum1(){
	int flag;
    int n = 0;
    char c = 'x';
    double d = 999.99;
	int id;
	
	cout<<""<<endl<<endl<<endl;
	cout<<"Edit"<<endl;
	cout<<"Employee Department Number"<<endl<<endl;
	cout<<"To edit an employee department number you will need to"<<endl;
	cout<<"enter the employee's ID number."<<endl;
	printMainOption();
	cout<<endl<<endl;
	do{
	flag = console_reader("Please enter an employee ID number: ",2,c,n,d);
	}while (!valid_menu_input(flag,1,100000,"Mm",c,n));
	
	if (flag == 2)
   {
      id = n;
	  return id;
   }else{
	  return 0;
   }
   
}

//function that asks the user for a new department number and edits the file
//=======================================================
int editdptNum2(const int rid){
	int flag;
    int n = 0;
    char c = 'x';
    double d = 999.99;
	char junk[11];
	REC_TYPE temp;
	read_record(rid,temp);
	
	cout<<""<<endl<<endl<<endl;
	cout<<"Edit"<<endl;
	cout<<"Employee Department Number (cont.)"<<endl<<endl;
	cout<<"Employee: "<<temp.name<<endl<<endl;
	cout<<"Current Department Number: "<<temp.dept_num<<endl<<endl;
	cout<<"To change the department number you will need to"<<endl;
	cout<<"enter the new department number."<<endl;
	printMainOption();
	cout<<endl<<endl;
	do{
	flag = console_reader("Please enter the NEW department number: ",2,c,n,d);
	}while (!valid_menu_input(flag,1,100000,"Mm",c,n));
	
	if (flag == 2)
   {
      temp.dept_num = n;
	  update_record(rid,temp);
	  cout<<endl<<endl;
	  cout<<temp.name<<"'s new department number is: "<<temp.dept_num<<endl;
	  cout<<endl;
	  cout<<"Hit enter to continue:"<<endl;
	  cin.getline(junk,10,'\n');
   }
	
}

//function that lists all employee records in sorted order
//=======================================================
int listallEmpl(){
	int flag;
    int n = 0;
    char c = 'x';
    double d = 999.99;
	int num;
	
	cout<<""<<endl<<endl<<endl;
	cout<<"List All Employees"<<endl<<endl;
	cout<<"You may list all the employees orderd in several different"<<endl;
	cout<<"ways. All will be in ascending order."<<endl;
	cout<<endl;
	cout<<"1) List ordered by employee ID number"<<endl;
	cout<<"2) List ordered by employee name {sorry, not available}"<<endl;
	printMainOption();
	cout<<endl<<endl;
	do{
	flag = console_reader("Please enter your selection from the menu [1-2 or M]: ",2,c,n,d);
	}while (!valid_menu_input(flag,1,1,"Mm",c,n));
	
	if (n == 1)
   {
      num = n;
	  return num;
   }
   
}

//function that builds the index for us
//========================================================
void build_index(INDEX_REC index[],const int max,int &count){
	REC_TYPE temp;
	//1)
	fstream infile;
	//2) and 3)
	infile.open(binary_file_name,ios::in|ios::binary);
	if(!infile){
		cout<<"FILE error empl.dat"<<endl;
		exit(1);
	}
	//4)
	count = 0;
	infile.read((char *) &temp, sizeof(REC_TYPE));
	while(!infile.fail()){
		//build the index record
		index[count].id = temp.empid;
		index[count].rid = count;
		count++;
		//check for max reads
		if(count >= max){
			break;
		}
		infile.read((char *) &temp, sizeof(REC_TYPE));
	}
	//5)
	infile.close();
}

//function that sorts the index in ascending order using a bubble sort
//=========================================================
void sort_index(INDEX_REC index[], const int count)
{
  // this is a classic bubble sort (good for small sorts)
  bool flag = true;  //did a swap happen
  INDEX_REC temp;    // using in 3 phase swap
  
  // bubble whole index or until no swap occurs
  for(int i=1; i <= count; i++) //loop count times
  {
     flag = false;   // clear the swap flag each pass
	 for (int j=0; j < count-1; j++) // up to one less then count
	 {
	    if (index[j+1].id < index[j].id) // need to bubble it up
		{
		  // classic 3-phase swap
		  temp = index[j];   // move the whole record (NOT just fields)
		  index[j] = index[j+1];
		  index[j+1] = temp;
		  flag = true;  // swap occured
		}
	 }
  }
  return;
}

//function that re-writes the file to update a record
//======================================================
void update_record(const long int rid, const REC_TYPE r)
{
   REC_TYPE temp;
  
   temp = r;
  
  //1)
   fstream outfile;
  // 2) 3)
   outfile.open(binary_file_name,ios::in|ios::out|ios::binary);  // OUTPUT !!!
   if (!outfile)
   {
     cout<< "FILE ERROR " << endl;
     exit(1);
   }
   
   // 4)
   outfile.seekp(rid * sizeof(REC_TYPE),ios::beg);  // USE seekp !!!!!
   outfile.write((char *)&temp,sizeof(REC_TYPE));   // write the record
   
   // 5)
   outfile.close();
}

//function that accepts a RID and reads in the corresponding record
// ======================================================
void read_record(const long int rid, REC_TYPE &r)
{
  REC_TYPE temp;
  
  //1)
   fstream infile;
  // 2) 3)
   infile.open(binary_file_name,ios::in|ios::binary);  // INPUT
   if (!infile)
   {
     cout<< "FILE ERROR " << endl;
     exit(1);
   }
   
   // 4)
   infile.seekg(rid * sizeof(REC_TYPE),ios::beg); // use seekg !!!
   infile.read((char *)&temp,sizeof(REC_TYPE));   // read the record
   
   r = temp;
   
   // 5)
   infile.close( );
}

//function that accepts an ID and gives back the corresponding RID
//========================================================
long int get_rid(const INDEX_REC index[], const int count, const int id){
	for(int i = 0; i<count; i++){
		if(index[i].id == id){
			return index[i].rid;
		}
	}
	
	return -1;	//not found
}

// function that prints one record out in a formated report
//======================================================
void print_record(INDEX_REC index[], const int count, const int rid)
{
   REC_TYPE temp;
   char junk[11];
   
   cout << endl;
   cout << "=====================================================================" << endl;
   cout << "  ID         NAME          SALARY        PHONE      TAXNUM   DEPTNUM" << endl;
   cout << " ___  __________________ _________  ______________  _______  _______" << endl;
   
     read_record(rid,temp);
     cout <<  setw(4)  << temp.empid;
     cout <<  setw(20) << temp.name;
     cout <<  setprecision(2) << fixed << setw(10)  << temp.salary;
	 cout <<  setw(16)  << temp.phone;
	 cout <<  setw(9)  << temp.tax_number;
     cout <<  setw(9)  << temp.dept_num << endl;
    cout << "=====================================================================" << endl;
	cout<<""<<endl;
	cout<<"Press enter to continue:"<<endl;
	cin.getline(junk,10,'\n');
}

// function that prints all of the records out in a formated report
//======================================================
void print_all_records(INDEX_REC index[], const int count)
{
   REC_TYPE temp;
   int rid;
   char junk[11];
   
   cout << endl;
   cout << "=====================================================================" << endl;
   cout << "  ID         NAME          SALARY        PHONE      TAXNUM   DEPTNUM" << endl;
   cout << " ___  __________________ _________  ______________  _______  _______" << endl;
   
   for (int i=0; i < count; i++)
   {
     rid = get_rid(index,count,index[i].id);
     read_record(rid,temp);
     cout <<  setw(4)  << temp.empid;
     cout <<  setw(20) << temp.name;
     cout <<  setprecision(2) << fixed << setw(10)  << temp.salary;
	 cout <<  setw(16)  << temp.phone;
	 cout <<  setw(9)  << temp.tax_number;
     cout <<  setw(9)  << temp.dept_num << endl;	 
    }
    cout << "=====================================================================" << endl;
	cout<<""<<endl;
	cout<<"Press enter to continue:"<<endl;
	cin.getline(junk,10,'\n');
}

//function that prevents bad user input from crashing our program
// =====================================================================================================
bool valid_menu_input(const int convert_flag, const int min, const int max, const char * valchar_list, const char c, const int n)
{
    int length = strlen(valchar_list);
   // each case results in the function returning !!!!!
   switch (convert_flag)
   {
      case 0: return false;  // was invalid to start with
	  case 3:  // check double range
	             return true;
			 
	  case 2: if ((n >= min) && (n <= max)) // check int range
	             return true;
			  else
			     return false;
	  case 1: for( int i = 0; i < length; i++) // check for valid char in list
              {
                 if (valchar_list[i] == c)
                      return true;
              }
              return false;
    }
}

//main driver
//=======================================================
int main(void){
	int currMenu = 1; //main menu
	int viewEmp = 0; //view employee
	int editDpt1 = 0; //edit employee department # (find)
	int editDpt2 = 0; //edit employee departmen # (found)
	int listEmp = 0; //list all employees
	int operation = 0;
	int get_back_VE = 0;
	int get_back_VAE = 0;
	int get_back;
	int count = 0;
	int rid;
	
	REC_TYPE rec;					//sets up our records
	INDEX_REC index[100];			//sets up our index
	build_index(index,100,count);	//builds the index inside of our struct
	sort_index(index,count);		//as a precondition, we sort the index before we do anything else
	
	do{clearScreen();
		switch(currMenu){
		case 1: operation = mainMenu();		//start with the main menu
				if(operation == 0){
					currMenu = 0;
				}else{
					operation = operation+1;	//accept user input and move forward
					currMenu = operation;
					}
				break;
		case 2: get_back_VE = viewEmployee();	//go to view employee screen
				if(get_back_VE == 0){
					currMenu = 1;
				}else{
				rid = get_rid(index,count,get_back_VE);		//use ID from user to find record
				print_record(index,count,rid);				//display record
				currMenu = 1;
				}
				break;
		case 3: get_back = editdptNum1(); 					//go the the edit dept number screen
				if(get_back == 0){
					currMenu = 1;
				}else{
					clearScreen();
					rid = get_rid(index,count,get_back);	//we pass the ID from screen 1 to screen 2 and pull up the record
					editdptNum2(rid); 						//this will execute if get_back has a nonzero return and perform the edit
					}
				currMenu = 1;
				break;
		case 4: get_back_VAE = listallEmpl();				//go to the print all records screen
				if(get_back_VAE == 1){						//we only want to print the records if the user enters a 1
				print_all_records(index,count);
				}
				currMenu = 1;
				break;
	}
	}while(currMenu != 0);
	
	return 0;
}
