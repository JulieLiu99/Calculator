#include "evaluate.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stack>
#include <vector>
#include <list> 
#include <iterator> 
#include <map> 
#include <cmath>
#include <ctype.h>
#include <typeinfo>



using namespace std;




//Add calculated variables and values to the map
void E::mapEvaluation(string filename, string outputFile){
    
    ifstream file;
    map<string, float> variableMap;  
    string afterEquals;
    string variableName;
    int lineNum;
    string line;
    string cleanExpression;
    
    //get number of lines
    file.open(filename);
    while (getline(file, line)) {
        if (line!=""){
            lineNum++;
        }
    }
    file.close();
   int k=0;
    int fileSize=0;
    while((variableMap.size()!=lineNum)&&(fileSize!=lineNum)) {//while not all variables are solved
        k++; if(k==1000){break;}

        file.open(filename);
        string lineString;
    int k=0;
        while (getline(file, line) and line!="") {
            k++; if(k==1000){break;}
        
        /*
        evaluation carried on one single expression
        */
        
            variableName = line.substr(0, line.find("=")-1); //variable name
            afterEquals = line.substr(line.find("=")+1, line.size()-1); //expression          
            cleanExpression = lineOperation(afterEquals);//expression manipulation
        
            
            if(areParanthesisBalanced(cleanExpression)){//sanity check
                //cout <<"clean expression is balanced " << cleanExpression << endl;
            
                //update variables with new values
                if(!variableMap.empty()) {
                    map<string, float>::iterator it = variableMap.begin();
                    while (it != variableMap.end()){
                        
                        cleanExpression  = ReplaceString(cleanExpression, it->first, to_string(it->second));
                        //cout<<"result of replacement, cleanExpression = "<<cleanExpression<<endl;
                        it++;
                    }
                }
                
                //postfix calculation
                if((not isVariablePresent(cleanExpression)) and (isOperatorPresent(cleanExpression))) { 
                    //cout << variableName << " is being evaluated" << endl;
                    
                    vector<string> v = tokenize(cleanExpression);//tokenize the expression into vector 
                    for(int i=0; i<v.size();i++){
                       // cout<<"i: "<<i<<" - " << v[i]<<endl;
                    }
                    vector<string> postfix = infix2postfix(v);//postfix expression
                    string result = evaluate(postfix);//result of calculation
                    //cout << variableName << " = " << result;
                    string s = variableName + " = " + result + ";";
                    if(fileSize<lineNum) {
                    //writeToFile(s,outputFile);
                }
                    fileSize+=1;
                    variableMap.insert(make_pair(variableName, stof(result))); //put (variable,value) to map
                } 
                
                //direct value
                else if((not isVariablePresent(cleanExpression)) and (not isOperatorPresent(cleanExpression))) { 
                     string s = variableName + " = " + cleanExpression + ";";
                     if(fileSize<lineNum) {
               // writeToFile(s,outputFile);
            }
                    fileSize+=1;
                    variableMap.insert(make_pair(variableName, stof(cleanExpression))); //put (variable,value) to map
                }    
                
            }

            
         /**/
   
        }

        
        //every line of expression has been visited
        file.close();

    }
    
     map <string, float>::iterator it2 = variableMap.begin();
    while (it2 != variableMap.end())
    {

        string s = it2->first + " = " + to_string(it2->second) + ";";
                writeToFile(s,outputFile);
                    
        it2++;

    }  
    
    //calculation completed
     map <string, float>::iterator it = variableMap.begin();
    while (it != variableMap.end())
    {
        
        //cout<< it->first << " = " << it->second <<endl;
        it++;
    }
 
}



float E::eval(float x1, float x2, string sign) { 

    if (sign == "+")//addition
        return x1 + x2;
  
    if (sign == "-")//subtraction
        return x1 - x2;
        
    if (sign == "*")//multiplication
        return x1 * x2;

    if (sign == "/")//division
        return x1 / x2;
        
    if (sign == "%")//integer division
        return static_cast<int>(x1 / x2);
        
    if (sign == "$")//modulus 
        return (int)x1 % (int)x2;
}

//Method which will evaluate a PostfixExpression and return the result
string E::evaluate(vector<string>  &y){

    //1. Create a stack (e.g. of type float) to store the operands
    stack <string> mystack;
    //2. Scan the postfix expression from left to right for every element
    for (int i = 0; i < y.size(); i++){
    //   a. if the element is an operand push it to the stack
        if (not isoperator(y[i])){
            mystack.push(y[i]);
        }
        else if(y[i]=="^"){
            float num = stof(mystack.top())*-1;
            mystack.pop();
            mystack.push(to_string(num));
        }
        else if(y[i]=="@"){
            float num = stof(mystack.top())-1;
            mystack.pop();
            mystack.push(to_string(num));
        }
        else if(y[i]=="#"){
            float num = pow(stof(mystack.top()), 2);
            mystack.pop();
            mystack.push(to_string(num));
        }
        else if(y[i]=="!"){
            float num = stof(mystack.top())+1;
            mystack.pop();
            mystack.push(to_string(num));
        }
        //   b. if the element is an operator pop 2 elements from the stack, 
        //      apply the operator on it and push the result back to the stack
        else{
            float x1 = stof(mystack.top());
            mystack.pop();
            float x2 = stof(mystack.top());
            mystack.pop();
            float x3 = eval(x2,x1,y[i]);
            mystack.push(to_string(x3));
            //cout << x2 << " " << x1 << " "<< y[i] << " =  " << x3 << endl;
        }    
    }
    //3. return the value from the top of the stack (i.e. the final answer) 
    return mystack.top();
}



/*
BELOW ARE HELPER FUNCTIONS
*/



//Cleaning work on the expressions
string E::lineOperation(string afterEquals){

    afterEquals = ReplaceString(afterEquals,";",""); //get rid of ;
    afterEquals = ReplaceString(afterEquals," ",""); //get rid of space
    afterEquals = ReplaceString(afterEquals,"++","!"); //replace ++ with !
    afterEquals = ReplaceString(afterEquals,"--","@"); //replace -- with @ 5+++4
    afterEquals = ReplaceString(afterEquals,"**","#"); //replace ** with #
    afterEquals = ReplaceString(afterEquals,"mod","$"); //replace mod with $ 
    return afterEquals;
}

//Sanity check function using stack
bool E::areParanthesisBalanced(string expr) { 
    stack <char> s; 
    char x; 
    // Traversing the Expression 
    for (int i=0; i<expr.length(); i++) { 
        if (expr[i]=='(') { //If the current character is a starting bracket
            s.push(expr[i]); //push it to stack
        } 
        //If the current character is a closing bracket
        else if (expr[i]==')'){
            if (s.top() == '(') s.pop();//pop from stack and check if it is the starting bracket 
            else return false;
        }
    } 
    //Balanced if no starting bracket left
    return (s.empty()); 
} 

bool E::isVariablePresent(string value){
    for (int i = 0; i < value.size(); i++) {  //iterate over ever character of input string
        if(isalpha(value[i])) { //if a letter is found
            return true;
        } 
    }
    return false;
}

bool E::isOperatorPresent(string value){
    for (int i = 0; i < value.size(); i++) {  //iterate over ever character of input string
        if(isoperator(value[i])) { //if a letter is found
            return true;
        }
    }
    return false;
}

//String replacement 
string E::ReplaceString(string subject, const string& search, const string& replace) { 
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

bool E::isoperator(char ch){
    if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='%'|| ch=='!'|| ch=='@'|| ch=='#'|| ch=='$')
        return true;
    else
        return false;
}

bool E::isoperator(string str){
    if( str=="+" || str=="-" || str=="*" || str=="/" || str=="%"|| str=="!"|| str=="@"|| str=="#"|| str=="$"|| str=="^")
        return true;
    else
        return false;
}



vector<string> E::tokenize(string x){

    vector <string> y;
    //cout << "start of tokenization" << x << endl;
    //cout<< "size of string for tokenization: "<<x.size()<<endl;
    int i=0;
    while(i<x.size()){
    
         if(i == 0 and x[i] == '-' and (isdigit(x[i+1]) or x[i+1]=='(') ){//unary minus
                //cout << i << " unary minus " << x[i] << endl;
                y.push_back("^");
                i++;
        }   
        
         if (x[i] == '-' and (isdigit(x[i+1]) or x[i+1]=='(') and (not isdigit(x[i-1])) and x[i-1]!=')' ){ //(4-2)-(-1)
                    //cout << i << " unary minus " << x[i] << endl;
                    y.push_back("^");
                    i++;
        }
        
        else if (isdigit(x[i])){//add digits to number
            string number;
            while (isdigit(x[i]) or x[i] == '.' ) {
                number += x[i];
                i++;
            }
           // cout<<"FULL NUMBER IS--"<<number<<"--"<<endl;
            //cout << "number is " << number << endl;
            y.push_back(number);
            
        }
        
        else if (isalpha(x[i])){//add letters to variable
            string variable;
            while (isalpha(x[i])){
                variable += x[i];
                i++;
            }
            //cout << "variable is " << variable << endl;
            y.push_back(variable);

        }
        
        else {// (, ), binary operators
            //cout << "binary operator" << x[i] << endl;
            //cout<<"PUSHING: --"<< x[i] << "--" << endl;
            y.push_back(string(1, x[i]));
    
            i++;
        }
    }    
    
    for(int i=0; i<y.size(); i++){
        //cout << y[i] << " ";
    }
    //cout << endl;
    return y;
}


vector<string> E::infix2postfix(vector<string> &x){

    stack <string> mystack;
    vector<string> y;
    
    //cout << "post fix begins" << endl;

    //1.    Push “(“onto Stack, and add “)” to the end of X.
    x.push_back(")");
    mystack.push("(");

    //2.    Scan X from left to right and repeat Step 3 to 6 for each element of X 
    //until the Stack is empty.
    int i=0;
    while(!mystack.empty())
    {   
        string str = x[i++];
        //3.    If an operand is encountered, add it to Y.
        if (not isoperator(str) and str!="(" and str!=")"){
            //cout << "push operand to vector: " << str << endl;
            y.push_back(str);
        }
        //4.    If a left parenthesis is encountered, push it onto Stack.
        else if(str=="("){
            //cout << "push ( to vector: " << str << endl;
            mystack.push(str);
        }
        //5.    If an operator is encountered, then: 
        else if(isoperator(str))
        {   //a.    Repeatedly pop from Stack and add to Y each operator (on the top of Stack) 
            //which has the same precedence as or higher precedence than operator.
            while (isoperator(mystack.top()) and isleq(str,mystack.top()))
            {
                //cout << "push operator to vector: " << mystack.top() << endl;
                y.push_back(mystack.top());
                mystack.pop();
            }
            //b.    Add operator to Stack.
            mystack.push(str);
        }
        //.6.   If a right parenthesis is encountered, then: 
        else if(str==")")
        {
            //a.    Repeatedly pop from Stack and add to Y each operator 
            //(on the top of Stack) until a left parenthesis is encountered.
            while(mystack.top()!="(")
            {
                //cout << "push ) to vector: "  << mystack.top() << endl;
                y.push_back(mystack.top());
                mystack.pop();
            }
            //b.    Remove the left Parenthesis.
            mystack.pop();
        }
    }
    
   // cout << "in post fix" << endl;
    for (int i=0; i<y.size(); i++){
       // cout << y[i] << " ";
    }
    //cout << endl;
    return y;
}

bool E::isleq(string opA, string opB){
    return (convertOpToInt(opA)<=convertOpToInt(opB));
}
int E::convertOpToInt (string str){
    if (str=="+" || str=="-") return 1;
    if (str=="*" || str=="/" || str=="%"|| str=="$") return 2;
    if (str=="^" || str=="!"|| str=="#"|| str=="@") return 3;
    return 0;
}
