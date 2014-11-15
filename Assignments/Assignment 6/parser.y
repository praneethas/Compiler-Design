%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
char* printMessage(int,char*,char,char*);
int start=1;
char s[10]="TK";
%}

%union{
	char value[1000];
}

%token INTEGER
%token IDENTIFIER
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%type <value>statement
%type <value>expr


%%
statement: expr {printf("\n Final Answer: %s\n",$$);}
		 ;
expr: INTEGER      { }
      | IDENTIFIER	      { }
      | expr '+' expr { strcpy($$,printMessage(start,$1,'+',$3));start++;}
	  | expr '-' expr { strcpy($$,printMessage(start,$1,'-',$3));start++;}
      | expr '*' expr { strcpy($$,printMessage(start,$1,'*',$3));start++;}
	  | expr '/' expr { strcpy($$,printMessage(start,$1,'/',$3));start++;}
	  | expr '%' expr { strcpy($$,printMessage(start,$1,'%',$3));start++;}
	  | '(' expr ')' { strcpy($$,$2);}
	  | '-' expr %prec UMINUS {strcpy($$,$2);}
      ;
%%

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

char* printMessage(int T, char* c1, char symb, char* c2)
{
	s[1]=T+'0';
	printf("\n %s = %s %c %s ",s,c1,symb,c2);
	return s;
}