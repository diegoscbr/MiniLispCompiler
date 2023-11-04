#include<iostream>
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
/*****************************************************************
 *                     DECLARATIONS                              *
 *****************************************************************/
typedef int NUMBER;
typedef int NAME;
const int  NAMELENG = 20;      /* Maximum length of a name */
const int  MAXNAMES = 1000;     /* Maximum number of different names */
const int  MAXINPUT = 5000;     /* Maximum length of an input */
const char*   PROMPT = "-> ";
const char*   PROMPT2 = "> ";
const char  COMMENTCHAR = ';';
const int   TABCODE = 9;        /* in ASCII */

struct EXPLISTREC;
typedef EXPLISTREC* EXPLIST;
enum EXPTYPE {VALEXP,VAREXP,APEXP};
struct EXPREC
{
	  EXPTYPE etype; //what type of expression
	  NUMBER num;
	  NAME varble;
	  NAME optr;
	  EXPLIST args;
};
typedef EXPREC* EXP;

struct EXPLISTREC
{
	  EXP head;
	  EXPLIST tail;
};


struct VALUELISTREC
{
	  NUMBER  head; //int that stores value
	  VALUELISTREC*  tail; //think of as next pointer
};

typedef VALUELISTREC* VALUELIST;

struct NAMELISTREC
{
	  NAME   head;
	  NAMELISTREC* tail;
};
typedef NAMELISTREC* NAMELIST;

struct  ENVREC
{
	   NAMELIST vars;
	   VALUELIST values;
};

typedef ENVREC* ENV;

struct  FUNDEFREC
{
	   NAME  funname;
	   NAMELIST  formals;
	   EXP  body;
	   FUNDEFREC*  nextfundef;
};
typedef FUNDEFREC* FUNDEF;

FUNDEF  fundefs;

ENV globalEnv;

EXP currentExp;

char userinput[MAXINPUT];
int   inputleng, pos;

char*   printNames[MAXNAMES];
int   numNames, numBuiltins;

int   quittingtime;



/*****************************************************************
 *                     DATA STRUCTURE OP'S                       *
 *****************************************************************/

/* mkVALEXP - return an EXP of type VALEXP with num n            */

EXP mkVALEXP ( NUMBER n)
{
    EXP e = new EXPREC;
    e->etype = VALEXP;
    e->num = n;
   return e;
}/* mkVALEXP */


/* mkVAREXP - return an EXP of type VAREXP with varble nm  */

EXP mkVAREXP ( NAME nm)
{
    EXP e = new EXPREC;
    e->etype = VAREXP;
    e->varble = nm;
   
   return e;
}/* mkVAREXP */


/* mkAPEXP - return EXP of type APEXP w/ optr op and args el     */

EXP mkAPEXP (NAME op, EXPLIST el)
{
 
    EXP e = new EXPREC;
    e->etype = APEXP;
    e->optr = op;
    e->args = el;
   return e;
}/* mkAPEXP */

/* mkExplist - return an EXPLIST with head e and tail el         */

EXPLIST mkExplist (EXP e, EXPLIST el)
{
    EXPLIST newel = new EXPLISTREC;
    newel->head = e;
    newel->tail = el;
    //do this
    return newel;
}/* mkExplist */

/* mkNamelist - return a NAMELIST with head n and tail nl        */

NAMELIST mkNamelist ( NAME nm, NAMELIST nl)
{
   NAMELIST newnl;
   newnl = new NAMELISTREC;
   newnl->head = nm;
   newnl->tail = nl;
  //do this
   return newnl;
}/* mkNamelist */

/* mkValuelist - return an VALUELIST with head n and tail vl     */

VALUELIST mkValuelist (NUMBER n,  VALUELIST vl)
{
   VALUELIST newvl;
   newvl = new VALUELISTREC;
   newvl->head = n;
   newvl->tail = vl;
   
   return newvl;
}/* mkValuelist */

/* mkEnv - return an ENV with vars nl and values vl              */

ENV mkEnv ( NAMELIST nl, VALUELIST vl)
{
    ENV rho = new ENVREC;
    rho->vars = nl;
    rho->values = vl;
    return rho;
}/* mkEnv */

/* lengthVL - return length of VALUELIST vl      */

int lengthVL ( VALUELIST vl)
{
   int i =0;
   while(vl != 0){
      i++;
      vl = vl->tail;
   }
   return i;
   
}/* lengthVL */

/* lengthNL - return length of NAMELIST nl    */

int lengthNL ( NAMELIST nl)
{
   int i =0;
   while(nl != 0){
      ++i;
      nl = nl->tail;
   }
   return i;

}/* lengthNL */

/*****************************************************************
 *                     NAME MANAGEMENT                           *
 *****************************************************************/

/* fetchFun - get function definition of fname from fundefs */

FUNDEF fetchFun( NAME fname)
{
    FUNDEF f = fundefs;
    while (f != 0 ){ 
        if (f->funname == fname)
            return f;
       f = f->nextfundef; 
    }
    return 0; //return o if the function definition is not found 
        
}/* fetchFun */


/* newFunDef - add new function fname w/ parameters nl, body e   */

void  newFunDef (NAME fname,  NAMELIST nl, EXP e)
{
    FUNDEF f = new FUNDEFREC;
    f->funname = fname;
    f->formals = nl;
    f->body = e;
    f->nextfundef = fundefs;
    fundefs = f;
}// newFunDef


/* initNames - place all pre-defined names into printNames */

void initNames()
{
   int i =0;
   fundefs = 0;
   printNames[i] = "if"; i++; //0
   printNames[i] = "while"; i++; //1
   printNames[i] = "set"; i++;//1
   printNames[i] = "begin"; i++;//3 control ops
   printNames[i] = "+"; i++;//4
   printNames[i] = "-"; i++; //built in 4- 11
   printNames[i] = "*"; i++;
   printNames[i] = "/"; i++;
   printNames[i] = "="; i++;
   printNames[i] = "<"; i++;//9. less than 
   printNames[i] = ">"; i++;//10. greater than
   printNames[i] = "print";//11
   numNames = i;     //beyond 11 =  applied user function . if it is not defined then you crash
   numBuiltins = i;
   
}//initNames

/* install - insert new name into printNames  */

NAME install ( char* nm)
{
   int i = 0;
   while (i <= numNames) {
	 if (strcmp( nm,printNames[i] ) == 0)
	    break;
     i++;
   }
   if (i > numNames)
   {
	  numNames = i;
	  printNames[i] = new char[strlen(nm) + 1];
	  strcpy(printNames[i], nm);
   }
   return i;
}// install

/* prName - print name nm              */

void prName ( NAME nm)
{
	 cout<< printNames[nm];
} //prName

/*****************************************************************
 *                        INPUT                                  *
 *****************************************************************/

/* isDelim - check if c is a delimiter   */

int isDelim (char c)
{
   return ( ( c == '(') || ( c == ')') ||( c == ' ')||( c== COMMENTCHAR) );
}

/* skipblanks - return next non-blank position in userinput */

int skipblanks (int p)
{
   while (userinput[p] == ' ')
	++p;
   return p;
}




int matches (int s, int leng,  char* nm)/* matches - check if string nm matches userinput[s .. s+leng]   */
{
   int i=0;
   while (i < leng )
   {
	 if( userinput[s] != nm[i] )
	    return 0;
	 ++i;
	 ++s;
    }
   if (!isDelim(userinput[s]) )
	  return 0;
   return 1;
}/* matches */




void nextchar (char& c)/* nextchar - read next char - filter tabs and comments */

{
    scanf("%c", &c);
    if (c == COMMENTCHAR )
    {
	  while ( c != '\n' )
		scanf("%c",&c);
    }
}


/* readParens - read char's, ignoring newlines, to matching ')' */
void readParens()
{
   int parencnt; /* current depth of parentheses */
   char c;
   parencnt = 1; // '(' just read
   do
   {
	  if (c == '\n')
	    cout <<PROMPT2;
	  nextchar(c);
	  pos++;
	  if (pos == MAXINPUT )
	  {
		cout <<"User input too long\n";
		exit(1);
	  }
	  if (c == '\n' )
		userinput[pos] = ' ';
	  else
		userinput[pos] = c;
	  if (c == '(')
		++parencnt;
	  if (c == ')')
	    parencnt--;
	}
    while (parencnt != 0 );
} //readParens

/* readInput - read char's into userinput */

void readInput()
{
    char  c;
    cout << PROMPT;
    pos = -1;
    do
	 {
	    ++pos ;
	    if (pos == MAXINPUT )
	    {
		    cout << "User input too long\n";
		    exit(1);
	    }
	    nextchar(c);
	    if (c == '\n' )
		   userinput[pos] = ' ';
	    else
		   userinput[pos] = c;
	    if (userinput[pos] == '(' )
		  readParens();
	 }
	while (c != '\n');
	inputleng = pos;
	userinput[pos+1] = COMMENTCHAR; // sentinel
}


/* reader - read char's into userinput; be sure input not blank  */

void reader ()
{
    do
    {
	  readInput();
	  pos = skipblanks(0);
    }
    while( pos > inputleng); // ignore blank lines
}

/* parseName - return (installed) NAME starting at userinput[pos]
It should read characters and make them into a string.
 Then install it in the symbol table and return the location at which the string was stored.
If it is already in the symbol table return the location where it was found.*/

NAME parseName()
{
   char nm[20]; // array to accumulate characters
   int leng; // length of name
   leng = 0;
   while ( (pos <= inputleng) && !isDelim(userinput[pos]) )
   {
	    
	    nm[leng] = userinput[pos];
	    ++pos;
	    ++leng;
       
   }
   if (leng == 0)
   {
	   cout<<"Error: expected name, instead read: "<< userinput[pos]<<endl;
	   exit(1);
   }
   nm[leng] = '\0';
   pos = skipblanks(pos); // skip blanks after name
   return ( install(nm));
}// parseName
/* isDigits - check if sequence of digits begins at pos   */
int isDigits (int pos)
{
  if ( ( userinput[pos] < '0' ) || ( userinput[pos] > '9' ) )
   return 0;
  while ( ( userinput[pos] >='0') && ( userinput[pos] <= '9') )
   ++pos;
  if (!isDelim(userinput[pos] ))
   return 0;
  return 1;
}// isDigits


/* isNumber - check if a number begins at pos  */

int isNumber (int pos) //why is isNumber not boolean
{
   return ( isDigits(pos) || ( (userinput[pos] == '-') && isDigits(pos+1)) ||( (userinput[pos] == '+') && isDigits(pos+1)));
}// isNumber
/* parseVal - return number starting at userinput[pos] 
This takes a sequence of integers possibly proceeded by a minus sign. Then convert it to an integer and return it.
(NUMBER is the same as int)  */

NUMBER parseVal() //CHECKPOINT
{
    int n=0, sign=1;
   if (userinput[pos] == '+')
      ++pos;
   if (userinput[pos] == '-'){
      sign = -1;
      ++pos;
   }
   while ( (userinput[pos] >= '0') && ( userinput[pos] <= '9') ){
      n = 10*n + userinput[pos] -'0';
      ++pos;
   }
   pos = skipblanks(pos); // skip blanks after number
   return ( (NUMBER) n*sign);
}// parseVal

EXPLIST parseEL();

/* parseExp - return EXP starting at userinput[pos]  */

EXP parseExp()
{
    pos = skipblanks(pos);
    if (userinput[pos] == '(' ) {
       pos = skipblanks(pos+1);
       NAME op = parseName();
       EXPLIST el = parseEL();
       return mkAPEXP(op, el);
    }
    if ( isNumber(pos) )
       return mkVALEXP(parseVal());
    return mkVAREXP(parseName());
}// parseExp

/* parseEL - return EXPLIST starting at userinput[pos]  */

EXPLIST parseEL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')' ) {
      pos = skipblanks(pos+1);
      return 0;
    }
    EXP e = parseExp();
    EXPLIST el = parseEL();
    return mkExplist(e, el); 
}// parseEL


/* parseNL - return NAMELIST starting at userinput[pos]  
This function return a pointer to a list of names by doing the following:
It should repeatedly call ParseName()
 (ParseName() will return an integer after installing the name in the symbol table).
 Then make a linked list containing those names (integers).
So NAMELIST is a pointer to the head of the linked list that contains names (int's).*/

NAMELIST parseNL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')') {
        pos = skipblanks(pos+1);
        return 0;
    }
    NAME nm = parseName();
    NAMELIST nl = parseNL();
    return mkNamelist(nm, nl);
}// parseNL

/* parseDef - parse function definition at userinput[pos]   */

NAME parseDef()
{
    NAME fname;        // function name
    NAMELIST nl;       // formal parameters
    EXP e;             // body
    pos = skipblanks(pos); //always skip the damn blanks
    pos = skipblanks(pos+1);
    pos = skipblanks(pos+6); //skip define
    fname = parseName();
    pos = skipblanks(pos);
    pos = skipblanks(pos+1); //skip '('
    nl = parseNL();
    e = parseExp();
    newFunDef(fname, nl, e);
   return (fname);
}// parseDef

/*****************************************************************
 *                     ENVIRONMENTS                              *
 *****************************************************************/

/* emptyEnv - return an environment with no bindings */

ENV emptyEnv()
{
   return  mkEnv(0, 0);
}

/* bindVar - bind variable nm to value n in environment rho */

void bindVar ( NAME nm,  NUMBER n,  ENV rho)/* bindVar - bind variable nm to value n in environment rho */
{ //someone gives you a new variable and you stick a new value in ENV
   rho->vars = mkNamelist(nm, rho->vars);
   rho->values = mkValuelist(n, rho->values);
}

/* findVar - look up nm in rho   */

VALUELIST findVar ( NAME nm, ENV rho) //valuelist
{
   //returning a valuelist pointer which corresponds to nm
   //max 5 lines of code!
   NAMELIST currentName = rho->vars;
   VALUELIST currentVal = rho->values;
   // Traverse the NAMELIST to find the corresponding name
    while (currentName != 0) {
       //cout<<"inside FindVar while\n";
        if (currentName->head == nm) {
            // Found the name, return the corresponding VALUELIST
            return currentVal;
        }
        currentName = currentName->tail;
        currentVal = currentVal->tail;
    }//end while

    //cout<<"FIND VAR FLG2: Name not found, return 0\n";
    return 0;
}



/* assign - assign value n to variable nm in rho   */

void  assign (NAME nm,  NUMBER n, ENV rho) //assigns value n to variable nm in rho
{
   //cout<<"Assign FLG\n";
   VALUELIST varloc; //
   varloc = findVar(nm, rho);
   //cout<<"ASSIGN FLAG2: findvar called to assign value n to variable nm in rho\n";
   varloc->head = n;
}// assign

/* fetch - return number bound to nm in rho */

NUMBER fetch ( NAME nm, ENV rho) 
{
   VALUELIST  vl;
   vl = findVar(nm, rho); 
   return (vl->head);
}

/* isBound - check if nm is bound in rho  */

int isBound ( NAME nm, ENV rho) //checks to see if nm is 
{
   //cout<<"isBound FLG\n";
   return ( findVar(nm, rho) != 0 );
}


/*****************************************************************
 *                     NUMBERS                                   *
 *****************************************************************/

/* prValue - print number n    */

void prValue ( NUMBER n )
{
   cout << n;
}
// prValue


/* isTrueVal - return true if n is a true (non-zero) value  */

int isTrueVal ( NUMBER n)
{
   return ( n != 0 );
}// isTrueVal


/* arity - return number of arguments expected by op  */

int arity ( int op)
{
	 if ( ( op > 4) && (op < 12) )
	    return 2;
	 return  1;
}// arity

/* applyValueOp - apply VALUEOP op to arguments in VALUELIST vl */
//correspnds to value operators in the symbol table 
NUMBER applyValueOp(int op, VALUELIST vl) {
   
   if (op == 11) {
      cout << vl->head << endl;
      return vl->head;
   }
    NUMBER n1 = vl->head;
    NUMBER n2 = vl->tail->head;
    switch (op) { 
        case 4: // +
            return n1 + n2;
        case 5: // minus
            return n1 - n2;
        case 6: // times
            return n1 * n2;
        case 7: // division
            if ((n1 / n2) == 0){
               cout<<"ERROR: DIVISION BY 0."<<endl;
               return -99999999999999;
            }
            else{
               return n1 / n2;
            }
            
        case 8: // =
            return n1 == n2;
        case 9: //less than
            return (n1 < n2);
        case 10: //greater than
            return (n1 > n2);
        
        default: 
         cout<<"ERROR. undefined user optr"<<endl;
         exit(1);
            // Handle unknown op here (optional)
            // For example, throw an error or return a default value.
            return 0; // You can change this to a suitable default value.
    }
}



/*****************************************************************
 *                     EVALUATION                                *
 *****************************************************************/

/* evalList - evaluate each expression in el */
NUMBER eval ( EXP e,  ENV rho);

VALUELIST evalList ( EXPLIST el, ENV rho)
{
  if(el == 0) //null set to 0, DONT USE NULL
  {
     return 0;
  }
  NUMBER n = eval(el->head, rho);
  VALUELIST vl = evalList(el->tail, rho);
  return mkValuelist(n, vl);
}// evalList

/* applyUserFun - look up definition of nm and apply to actuals  */

NUMBER applyUserFun ( NAME nm, VALUELIST actuals)
{
   FUNDEF f = fetchFun(nm);
   ENV rho = mkEnv(f->formals, actuals);
   return eval(f->body, rho);
}

NUMBER applyCtrlOp(int op, EXPLIST args, ENV rho) { //arguments is the explist pointer
    cout << "applyControl Op FLAG\n";
    int n = 0;
   /*-----IF CASE---------------*/
    if (op == 0) { // 
        n = eval(args->head, rho); 
        if (n != 0) { //if e1 evaluates to true (non-zero), then evaluate e2
            return eval(args->tail->head, rho); //evaluates e2 and returns its value
        } else {
            return eval(args->tail->tail->head, rho); //evaluates e3 and returns its value 
        }
    } 
    /*-----WHILE CASE-------------*/
    else if (op == 1){
      n = eval(args->head, rho); //evaluate e1
       //otherwise evaluate e2 and then reevaluate e1 until e1 evaluates to 0, then return 0;
         while(n!= 0){
            //evaluate e2
            n = eval(args->tail->head, rho);
            //then reevaluate e1
            n = eval(args->head, rho);
         }
         return 0;
   }
    /*-----SET CASE---------------*/
    else if (op == 2) { 
        NAME nm = args->head->varble; //name of function nm = varble in args
         n = eval(args->tail->head, rho); 
        if(isBound(nm, rho)){
           assign(nm, n, rho);
        }
        else if (isBound(nm, globalEnv)){
           assign(nm, n, globalEnv);
        }
        else{
           bindVar(nm, n, globalEnv);
        }

        return n;
   }//end set case
   /*-----BEGIN CASE-------*/ 
   else if (op == 3){
      NUMBER en = 0;
      while(args != 0){
         en = eval(args->head, rho); //evaluates e1
         args = args->tail; //increments e1->e2->...en
      }
      return en; //returns val of en
   }//end begin case
   cout<<"This case should never be reached"<<endl;
   return 0;
}// applyCtrlOp

// eval - return value of expression e in local environment rho

NUMBER eval ( EXP e,  ENV rho)
{
   int n =0;
  if(e->etype == VALEXP){ //value expression
     return e->num;
  }
  if(e->etype == VAREXP){ 
     NAME nm = e->varble;
     if(isBound(nm, rho)){
        return fetch(nm, rho);
     }
     
      else if (isBound(nm, globalEnv)){
         return fetch(nm, globalEnv);
      }
      else{
         cout<<"Undefined variable: " << nm <<endl;
         exit(1);
      }
      return e->num; //return the num
     }//if(e->etype == VAREXP)

   if (e->etype == APEXP){//returning the value of thee applied 
     // if(e->optr between 0 and 4 return applied control op
     if(e->optr < 4){
        return applyCtrlOp(e->optr, e->args, rho);
     }
     else if (e->optr <= numBuiltins){ //returns value list as long as its <= numBuiltins
        return applyValueOp(e->optr, evalList(e->args, rho));
     }
     else if (e->optr > numBuiltins){
        return applyUserFun((e->optr), evalList(e->args, rho)); //return user function
     }
     else{
        NAME ops = e->optr;
        cout<<"Undefined applied expression operator:  " << ops <<endl;
         exit(1);
     }
  }      
} //eval

/*****************************************************************
 *                     READ-EVAL-PRINT LOOP                      *
 *****************************************************************/

int main()
{
   initNames();
   globalEnv = emptyEnv();

   quittingtime = 0;
   while (!quittingtime)
   {
	 reader();
	 if ( matches(pos, 4, "quit"))
	    quittingtime = 1;
	 else if( (userinput[pos] == '(') &&
		    matches(skipblanks(pos+1), 6, "define")  )
	 {
		    prName(parseDef());
		    cout <<endl;
	 }
	 else {
			currentExp = parseExp();
			prValue(eval(currentExp, emptyEnv() ));
			cout <<endl<<endl;
		 }
	}// while
  return 0;
}