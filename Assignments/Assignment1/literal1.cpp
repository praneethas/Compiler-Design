/**

      Course: Compiler design
Assignment 1: Design a lexer to identify valid literals
    Lecturer: Dr. Dinesh Gopalani

**/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctype.h>
#include <fstream>

using namespace std;

#define MAX 100

/**

func  : To check for literals in a c source code.
param : String(char []). A line of c code.
return: void

**/
void stringTokenize(char []);

void checkLiterals(char []);
/**

func  : To trim starting spaces in a c string
param : String(char *). A line of c code.
return: char*. Trimmed line

*/
char *trimSpaces(char *str)
{
  int r; /* next character to be read */
  int w; /* next character to be written */

  r=w=0;
  while (str[r])
  {
    if (isspace(str[r]) || iscntrl(str[r]))
    {
      if (w > 0 && !isspace(str[w-1]))
        str[w++] = ' ';
    }
    else
      str[w++] = str[r];
    r++;
  }
  str[w] = 0;
  return str;
}

/**

func  : main
return: int

**/
int main()
{
    fstream ifp, ofp;
    char outputFilename[] = "literal.txt" ;
	char cSourceFile[] = "test.c";
	string keyWords[] = {"int","char","while","for","float","long",
        "double","long long int","int*","int *","FILE*","FILE *"};
	char *buffer;
	char ch;
	ifp.open(cSourceFile, ios::in);
	if (!ifp.is_open()) {
	  cerr << "Can't open input file " << cSourceFile << endl;
	  exit(1);
	}
	ofp.open(outputFilename, ios::out);
	int count = 1;
	if (!ofp.is_open()) {
	  cerr << "Can't open output file " << outputFilename << endl;
	  exit(1);
	}
	/*while((ch=getc(ifp))!=EOF)
    {
        fscanf(ifp, "%[^\n]s", buffer);
        //fgets(buffer, sizeof(buffer), ifp);
        printf("String = %s\n", buffer);
    }*/
    char c;
    while((c=ifp.get())!=EOF)
    {
        int i = 0;
        if(c == '\n')
        {
            continue;
        }
        buffer = (char*)malloc(1000);
        buffer[i++] = c;
        while((c=ifp.get()) !='\n')
        {
            if(c == '\t')
            {
                continue;
            }
            buffer[i++]=c;
        }
        buffer[i]='\0';
        //printf("Buffer = %s\n",buffer);
        int len = strlen(buffer);
        //printf("New String = %s\n", trimSpaces(buffer));
        char array[len+2];
        strcpy(array, trimSpaces(buffer));
        //printf("Array = %s\n",array);
        //stringTokenize(array);
        checkLiterals(array);
        free(buffer);
    }
    ifp.close();
    return 0;
}

void stringTokenize(char array[])
{
    int len = strlen(array);
    //printf("Length of String = %d\n", len);
    if(strstr(array, "#define") != NULL) {
        char *tokens;
        int i=1;
        tokens = strtok (array," ");
        printf("Tokens:  ");
        while (tokens != NULL)
        {
          printf ("%d:%s  ",i++,tokens);
          tokens = strtok (NULL, " ");
        }
        printf("\n");
    }
    else
    {
        char *tokens;
        int i=1;
        tokens = strtok (array,"\"");
        printf("Tokens1:  ");
        while (tokens != NULL)
        {
          printf ("%d:%s  ",i++,tokens);
          tokens = strtok (NULL, "\"");
        }
        printf("\n");
    }
}

void checkLiterals(char array[])
{
	int len = strlen(array), i;
	string buffer = "";
	//printf("Length of String is %d: %s\n", len, array);
	for(i = 0; i < len; i++)
	{
		//printf("Array = %c\n", array[i]);
		int statps = 0;
		if((strstr (array,"printf") != NULL) || (strstr (array,"scanf") != NULL))
		{
			statps = 1;
		}
		if(array[i]>='0' && array[i]<='9')
		{
			while((array[i] != ';' && array[i] != ' ' && 
			     array[i] != '\n' && array[i] != ',' && 
			     array[i] != '(' && array[i] != ')' &&
			     array[i] != '|' && array[i] != '&' &&
			     array[i] != '^' && array[i] != '%')&& (i < len))
			{
				//buffer += array[i];	
				printf("%c", array[i]);
				i++;
			}
			//printf("Buffer = %c", buffer[0]);
			printf("\n");
		}
		if(array[i] == '\"' && statps != 1)
		{
			printf("%c", array[i++]);
			while(array[i] != '\"')
			{
				if(array[i] == '\\' && array[i+1] == '\"')
				{
					printf("%c%c",array[i],array[i+1]);
					i += 2;
				}
				else
				{
					printf("%c", array[i]);
					i++;
				}
			}
			printf("%c\n", array[i]);
		}
		if(array[i] == '\"' && statps == 1)
		{
			while(array[i] != '\"')
			{
				if(array[i] == '\\' && array[i+1] == '\"')
				{
					i += 2;
				}
				else
				{
					i++;
				}
			}
		}
		if(array[i] == '\'')
		{
			printf("%c%c%c\n",array[i],array[i+1],array[i+2]);
			i += 3;
		}
		if(array[i] == '[')
		{
			i++;
			printf("%c\n",array[i++]);
		}
	}

}
