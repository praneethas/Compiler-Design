/**
 *
 *   Assignment2
 *   Problem   : Write a program using Lex/Flex tool which shall read a C/C++ source code & identify
 *	         all the identifier names as well as literal constants present in the source code
 *	         and store them along with their frequencies in a separate file.
 *   Author    : Praneeth A S
 *   Date      : 23.08.2014
 *   Run       : chmod +x run.sh && ./run.sh
 *   Course    : Compiler Design
 *   Instructor: Dr. Dinesh Gopalani
 *   Ouput     : output.txt
 *
**/

#include <iostream>
#include "assign2.h"
#include <map>
#include <cstdio>

using namespace std;

extern int yylex();
extern int yylineno;
extern char *yytext;
extern int numLiterals;
extern int numIdentifiers;
extern map<char*,int> freq;

int main()
{
	int ntoken, vtoken, count;
	ntoken = yylex();
	while(ntoken)
	{
		ntoken = yylex();
	}
	map<char*,int>::iterator it;
	cout << "Identifiers/Literals\t\tFrequency" << endl;
	for(it=freq.begin();it!=freq.end();it++)
	{
		char* val= it->first;
		int count= it->second;
		printf("%s\t\t\t\t%d\n",val,count);
	}
	cout << "numLiterals = " << numLiterals << ", numIdentifiers = " << numIdentifiers << endl;
	return 0;
}
