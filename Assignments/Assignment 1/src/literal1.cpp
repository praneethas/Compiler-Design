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
void valiDateLiteral(string);
void validateOctal(string);
void validateHex(string);
void validateString(string);
void validateChar(string);
void validateFloat(string);
void validateInt(string);
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
    char outputFilename[] = "literal.txt" ; // Not Used
	char cSourceFile[] = "test.c";          // Input File for the code
	string keyWords[] = {"int","char","while","for","float","long",
        "double","long long int","int*","int *","FILE*","FILE *"}; // Not Used
	char *buffer;       // Stores each of line of code for processing
	char ch;
	ifp.open(cSourceFile, ios::in);
	if (!ifp.is_open()) { // File Check
	  cerr << "Can't open input file " << cSourceFile << endl;
	  exit(1);
	}
	ofp.open(outputFilename, ios::out);
	int count = 1;
	if (!ofp.is_open()) { // File Check
	  cerr << "Can't open output file " << outputFilename << endl;
	  exit(1);
	}
    char c;
    while((c=ifp.get())!=EOF) // While file has not reached its end
    {
        int i = 0;
        if(c == '\n')
        {
            continue;
        }
        buffer = (char*)malloc(1000);
        buffer[i++] = c;
        while((c=ifp.get()) !='\n')        // While file has not reached enf of line
        {
            if(c == '\t')
            {
                continue;
            }
            buffer[i++]=c;
        }
        buffer[i]='\0';                    // Store line in buffer
        int len = strlen(buffer);
        char array[len+2];
        strcpy(array, trimSpaces(buffer)); // Trim trailing whitespaces
        checkLiterals(array);              // Check for literals in the stored line
        free(buffer);                      // Free memory after usage
    }
    ifp.close();                           // Close file after use
    return 0;
}

void stringTokenize(char array[])         // Might be used for tokenizing string based on delimiters
{                                         // Not used in this code
    int len = strlen(array);
    if(strstr(array, "#define") != NULL) {
        char *tokens;
        int i=1;
        tokens = strtok (array," ");
        while (tokens != NULL)
        {
          tokens = strtok (NULL, " ");
        }
    }
    else
    {
        char *tokens;
        int i=1;
        tokens = strtok (array,"\"");
        while (tokens != NULL)
        {
          tokens = strtok (NULL, "\"");
        }
    }
}

void checkLiterals(char array[])
{
	int len = strlen(array), i;
	string buffer = "";
	for(i = 0; i < len; i++)
	{
		int statps = 0;
		if((strstr (array,"printf") != NULL) || (strstr (array,"scanf") != NULL))
		{
			statps = 1;
		}
		if(array[i]>='0' && array[i]<='9' )
		{
			if(array[i-1] == '+' || array[i-1] == '-')
            {
                buffer+=array[i-1];
            }
			while((array[i] != ';' && array[i] != ' ' &&
			     array[i] != '\n' && array[i] != ',' &&
			     array[i] != '(' && array[i] != ')' &&
			     array[i] != '{' && array[i] != '}' &&
			     array[i] != '|' && array[i] != '&' &&
			     array[i] != '^' && array[i] != '%')&& (i < len))
			{
				buffer+=array[i];
				i++;
			}
			valiDateLiteral(buffer);
			buffer = "";
		}
		if(array[i] == '\"' && statps != 1)
		{
			buffer+=array[i];
			i++;
			while(array[i] != '\"')
			{
				if(array[i] == '\\' && array[i+1] == '\"')
				{
					buffer+=array[i];buffer+=array[i+1];
					i += 2;
				}
				else
				{
					buffer+=array[i];
					i++;
				}
			}
			buffer+=array[i];
			valiDateLiteral(buffer);
			buffer="";
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
			if(array[i+1] == '\'')
            {
                buffer+=array[i];buffer+=array[i+1];
                i += 2;
                valiDateLiteral(buffer);
                buffer="";
            }
			else
            {
                buffer+=array[i];buffer+=array[i+1];buffer+=array[i+2];
                i += 3;
                valiDateLiteral(buffer);
                buffer="";
		    }
		}
		if(array[i] == '[')
		{
			i++;
			if(array[i] == ']')
			{}
			else
			{
				buffer+=array[i];
				i++;
				valiDateLiteral(buffer);
				buffer="";
			}
		}
	}
}

void valiDateLiteral(string buffer)
{
	int i = 0;
	if(buffer[i] == '0' && (buffer[i+1] != 'x' && buffer[i+1] != 'X') && buffer.length() > 1)
	{
		validateOctal(buffer);
	}
	else if(buffer[i] == '0' && (buffer[i+1] == 'x' || buffer[i+1] == 'X') && buffer.length() > 2)
	{
		validateHex(buffer);
	}
	else if(buffer.find("\"") != -1)
	{
		validateString(buffer);
	}
	else if(buffer.find("\'") != -1)
    {
        validateChar(buffer);
    }
    else if(buffer.find(".") != -1)
    {
        validateFloat(buffer);
    }
    else
    {
        validateInt(buffer);
    }
}

void validateOctal(string buffer)
{
    int stat = 1, i = 0, len = buffer.length();
    if(buffer[i] != '0')
    {
        stat = 0;
    }
    else
    {
        for(i = 1; i < len; i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='7'))
            {
                stat = 0;
                break;
            }
        }
    }
    if(stat)
    {
        cout << "Valid Octal char " << buffer << endl;
    }
    else
    {
        cout << "Invalid Octal char " << buffer << endl;
    }
}

void validateHex(string buffer)
{
    int stat = 1, i = 0, len = buffer.length();
    if(buffer[i] != '0' && (buffer[i+1] != 'x' || buffer[i+1] != 'X'))
    {
        stat = 0;
    }
    else
    {
        for(i = 2; i < len; i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && !(buffer[i]>='A' && buffer[i]<='F') && !(buffer[i]>='a' && buffer[i]<='f'))
            {
                cout << buffer[i];
                stat = 0;
                break;
            }
        }
    }
    if(stat)
    {
        cout << "Valid Hex char " << buffer << endl;
    }
    else
    {
        cout << "Invalid Hex char " << buffer << endl;
    }
}

void validateString(string buffer)
{
    cout << "Valid string " << buffer << endl;
}

void validateChar(string buffer)
{
    int i = 0, stat = 1;
    if(buffer.length() > 4)
    {
        stat = 0;
    }
    if(buffer.length() == 3 && buffer[1] != '"' && buffer[1] != '\'')
    {
        stat = 1;
    }
    if(buffer.length() == 2)
    {
        stat = 1;
    }
    if(buffer.length() == 4 && buffer[1] == '\\')
    {
        if(buffer[2]=='r'||buffer[2]=='n'||buffer[2]=='t'||buffer[2]=='f')
        {
            stat = 1;
        }
    }
    if(stat)
    {
        cout << "Valid char " << buffer << endl;
    }
    else
    {
        cout << "Invalid char " << buffer << endl;
    }
}

void validateFloat(string buffer)
{
    int i, len = buffer.length(), stat = 1;

    if(buffer[len-1] == 'f' || buffer[len-1] == 'F')
    {
        //cout << "IF1" << endl;
        int i, k,stat = 1, len = buffer.length(), dot = 0;
        if(buffer[0] == '+' || buffer[0] == '-')
        {
            k = 1;
        }
        else
        {
            k = 0;
        }
        for(i=k;i<len-1;i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]!='.')
            {
                stat = 0;
            }
            if(buffer[i] == '.')
            {
                dot++;
            }
            if(dot > 1)
            {
                stat = 0;
            }
        }
        if(stat)
        {
            cout << "Valid float " << buffer << endl;
        }
        else
        {
            cout << "Invalid float " << buffer << endl;
        }
    }
    else if(buffer[len-1] == 'L' || buffer[len-1] == 'l')
    {
        //cout << "IF2" << endl;
        int i, k,stat = 1, len = buffer.length(), dot = 0;
        if(buffer[0] == '+' || buffer[0] == '-')
        {
            k = 1;
        }
        else
        {
            k = 0;
        }
        for(i=k;i<len-1;i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]!='.')
            {
                stat = 0;
            }
            if(buffer[i] == '.')
            {
                dot++;
            }
            if(dot > 1)
            {
                stat = 0;
            }
        }
        if(stat)
        {
            cout << "Valid float " << buffer << endl;
        }
        else
        {
            cout << "Invalid float " << buffer << endl;
        }
    }
    else if((buffer[len-1] == 'L' && buffer[len-2] == 'D')||(buffer[len-1] == 'l' && buffer[len-2] == 'd'))
    {
        //cout << "IF3" << endl;
        int i, k,stat = 1, len = buffer.length(), dot = 0;
        if(buffer[0] == '+' || buffer[0] == '-')
        {
            k = 1;
        }
        else
        {
            k = 0;
        }
        for(i=k;i<len-2;i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]!='.')
            {
                cout<<"stat1";stat = 0;
            }
            if(buffer[i] == '.')
            {
                dot++;
            }
            if(dot > 1)
            {
                cout<<"stat2";stat = 0;
            }
        }
        if(stat)
        {
            cout << "Valid float " << buffer << endl;
        }
        else
        {
            cout << "Invalid float " << buffer << endl;
        }
    }
    else if(buffer.find("E") != -1 || buffer.find("e") != -1)
    {
        //cout << "IF4" << endl;
        int i, k,stat = 1, len = buffer.length(), dot = 0;
        if(buffer[0] == '+' || buffer[0] == '-')
        {
            i = 1;
        }
        else
        {
            i = 0;
        }
        while(buffer[i]!='E' && buffer[i]!='e') // TODO
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]!='.')
            {
                stat = 0;
                //cout << "stat1";
            }
            if(buffer[i] == '.')
            {
                dot++;
            }
            if(dot > 1)
            {
                stat = 0;
            }
            i++;
        }
        i++;
        if(buffer[i] == '+' || buffer[i] == '-')
        {
            i += 1;
            k = i;
        }
        for(i=k;i<len;i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]=='.')
            {
                stat = 0;
            }
        }
        if(stat)
        {
            cout << "Valid float " << buffer << endl;
        }
        else
        {
            cout << "Invalid float " << buffer << endl;
        }
    }
    else
    {
        //cout << "IF5" << endl;
        int i, k,stat = 1, len = buffer.length(), dot = 0;
        if(buffer[0] == '+' || buffer[0] == '-')
        {
            k = 1;
        }
        else
        {
            k = 0;
        }
        for(i=k;i<len;i++)
        {
            if(!(buffer[i]>='0' && buffer[i]<='9') && buffer[i]!='.')
            {
                stat = 0;
            }
            if(buffer[i] == '.')
            {
                dot++;
            }
            if(dot > 1)
            {
                stat = 0;
            }
        }
        if(stat)
        {
            cout << "Valid float " << buffer << endl;
        }
        else
        {
            cout << "Invalid float " << buffer << endl;
        }
    }
}

void validateInt(string buffer)
{
    int i, k,stat = 1, len = buffer.length();
    if(buffer[0] == '+' || buffer[0] == '-')
    {
        k = 1;
    }
    else
    {
        k = 0;
    }
    for(i = k; i < len; i++)
    {
        if(!(buffer[i]>='0' && buffer[i]<='9'))
        {
            stat = 0;
            break;
        }
    }
    if(stat)
    {
        cout << "Valid int " << buffer << endl;
    }
    else
    {
        cout << "Invalid int " << buffer << endl;
    }
}
