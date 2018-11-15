#ifndef EVALUATE_H
#define EVALUATE_H

#include<iostream>
#include <fstream>

using namespace std;


class E {
	public:
        bool areParanthesisBalanced(string expr);
        string lineOperation(string afterEquals);
        bool isVariablePresent(string value);
        bool isOperatorPresent(string value);
        string ReplaceString(string subject, const string& search, const string& replace);
        bool isoperator(string str);
        bool isoperator(char ch);
        void mapEvaluation(string filename, string k);
        string evaluate(vector<string>  &y);
        float eval(float x1, float x2, string sign);
        vector<string> tokenize(string x);
        vector<string> infix2postfix(vector<string> &x);
        int convertOpToInt (string str);
        bool isleq(string opA, string opB);
        void writeToFile(string line, string filename);
};

#endif
