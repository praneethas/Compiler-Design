%{
#include <iostream>
#include <map>
#include "assign2.h"
int numLiterals = 0, numIdentifiers = 0,a;

using namespace std;

map<string,int> freq;

%}

DIGIT [0-9]
LETTER [A-Za-z]
ODIGIT [0-7]
HDIGIT ([0-9]|[a-f]|[A-F])
SIGN [+-]
USCORE [_]
CHARSPEC [\']
STRINGSPEC [\"]
EXPONENT e|E
SUFFIX f|F|u|U|l|L|d|D
FSUFFIX ld|LD|LLD|lld
BIN [0-1]
FLOAT {DIGIT}+.{DIGIT}*
KEYWORDS  #define|#pragma|#ifdef|#endif|#ifndef|auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|printf|scanf|main\(.*\)
DATATYPES  int|float|char|double|long|void|short
POINTERS {DATATYPES}(\*)+
NEGLECT #include<*.*>|printf(.*);|scanf(.*);|\;|=|[ ]|[\t]|[\n]|\{|\}|\(|\)
LOGICAL "||"|"|"|"&"|"&&"|"^"|"~"|"!"|"!="
OPERATORS "++"|"--"|"*="|"\="|"+="|"-="

%%
{KEYWORDS}|{NEGLECT}|{POINTERS}|{LOGICAL}                 ;
({FLOAT}|{DIGIT}+){EXPONENT}{DIGIT}+   {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return EXPONENTIAL;}
{BIN}+([bB])               {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return BINARY;}
(0)([xX]){HDIGIT}+         {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return HEXADECIMAL;}
[0]{ODIGIT}+            {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return OCTAL;}
{SIGN}?{DIGIT}+        {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return INTEGER;}
\'.\'                  {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return CHARACTER;}
{FLOAT}{SUFFIX}           {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return FLOAT;}
{FLOAT}{FSUFFIX}           {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return LONG;}
\"(\\.|[^\\"])*\"	 {a = freq[yytext];freq[yytext] = a+1;numLiterals++;return STRING;}
({USCORE}|{LETTER})({LETTER}|{DIGIT}|{USCORE})* {a = freq[yytext];freq[yytext] = a+1;numIdentifiers++;return IDENTIFIER;}
({USCORE}|{LETTER})({LETTER}|{DIGIT}|{USCORE})*\( ;
{FLOAT}                     {a = freq[yytext];freq[yytext] = a+1;numLiterals++;}
%%

int yywrap(void)
{
	return 1;
}
