%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

DIGIT [0-9]
LETTER [A-Za-z]
USCORE [_]

%%
[e][x][i][t] {printf("Bye!!!\n"); break; return -1;}
[" "] {return yytext[0];}
{DIGIT}+[.]{DIGIT}*|{DIGIT}+ {strcpy(yylval.value,yytext);return INTEGER;}
({USCORE}|{LETTER})({LETTER}|{DIGIT}|{USCORE})* {strcpy(yylval.value,yytext);return IDENTIFIER;}
[\n] {return 0;}
[\t] {;}
. {return yytext[0];}
%%

int yywrap(void)
{
	return 1;
}

int main(void) {
	while(1)
	{
		printf("lex@praneethas >> ");
		yyparse();
	}
	return 0;
}

