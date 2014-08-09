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
        //printf("Tokens:  ");
        while (tokens != NULL)
        {
          //printf ("%d:%s  ",i++,tokens);
          tokens = strtok (NULL, " ");
        }
        //printf("\n");
    }
    else
    {
        char *tokens;
        int i=1;
        tokens = strtok (array,"\"");
        //printf("Tokens1:  ");
        while (tokens != NULL)
        {
          //printf ("%d:%s  ",i++,tokens);
          tokens = strtok (NULL, "\"");
        }
        //printf("\n");
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
			//cout << "condition 1" << endl;
			statps = 1;
		}
		if(array[i]>='0' && array[i]<='9' )
		{
			//cout << "condition 2" << endl;
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
				//printf("%c", array[i]);
				i++;
			}
			//cout << "Buffer1 = "<< buffer << endl;
			valiDateLiteral(buffer);
			buffer = "";
			//printf("\n");
		}
		if(array[i] == '\"' && statps != 1)
		{
			//cout << "condition 3" << endl;
			buffer+=array[i];
			//printf("%c", array[i]);
			i++;
			while(array[i] != '\"')
			{
				if(array[i] == '\\' && array[i+1] == '\"')
				{
					//printf("%c%c",array[i],array[i+1]);
					buffer+=array[i];buffer+=array[i+1];
					i += 2;
				}
				else
				{
					//printf("%c", array[i]);
					buffer+=array[i];
					i++;
				}
			}
			buffer+=array[i];
			//printf("%c\n", array[i]);
			//cout << "Buffer2 = " << buffer << endl;
			valiDateLiteral(buffer);
			buffer="";
		}
		if(array[i] == '\"' && statps == 1)
		{
			//cout << "condition 4" << endl;
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
			//cout << "condition 5" << endl;
			if(array[i+1] == '\'')
            {
                buffer+=array[i];buffer+=array[i+1];
                i += 2;
                //cout << "Buffer3 = " << buffer << endl;
                valiDateLiteral(buffer);
                buffer="";
            }
			else
            {
                buffer+=array[i];buffer+=array[i+1];buffer+=array[i+2];
                //printf("%c%c%c\n",array[i],array[i+1],array[i+2]);
                i += 3;
                //cout << "Buffer3 = " << buffer << endl;
                valiDateLiteral(buffer);
                buffer="";
		    }
		}
		if(array[i] == '[')
		{
			//cout << "condition 6" << endl;
			i++;
			if(array[i] == ']')
			{}
			else
			{
				buffer+=array[i];
				//printf("%c\n",array[i]);
				i++;
				//cout << "Buffer4 = " << buffer << endl;
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
		//cout << "oct" << endl;
		validateOctal(buffer);
	}
	else if(buffer[i] == '0' && (buffer[i+1] == 'x' || buffer[i+1] == 'X') && buffer.length() > 2)
	{
		//cout << "hex" << endl;
		validateHex(buffer);
	}
	else if(buffer.find("\"") != -1)
	{
		//cout << "string" << endl;
		validateString(buffer);
	}
	else if(buffer.find("\'") != -1)
    {
        //cout << "char" << endl;
        validateChar(buffer);
    }
    else if(buffer.find(".") != -1)
    {
        //cout << "float" << endl;
        validateFloat(buffer);
    }
    else
    {
        //cout << "int" << endl;
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
