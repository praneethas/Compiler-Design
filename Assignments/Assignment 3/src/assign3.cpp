#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

vector<string> one_d;
vector< vector<string> > two_d;
vector<string> terminals, non_terminals;
vector<string> op_table_1;
vector< vector<string> > op_table_2;
int sizeTerm;

void printVec(vector< vector<string> > v)
{
    int h=v.size();
    for(int i=0;i<h;i++)
    {
        int l=v[i].size();
        for(int j=0;j<l;j++)
        {
            cout << v[i][j];
        }
        cout << endl;
    }
}

void readGrammar()
{
	fstream ingrammar;
	char input[] = "grammar.txt";
	string buffer;
	ingrammar.open(input, ios::in);
	if (!ingrammar.is_open()) { // File Check
	  cerr << "Can't open input file " << input << endl;
	  return;
	}
	while(getline(ingrammar, buffer))
	{
		size_t prev = 0, pos;
    		while ((pos = buffer.find_first_of(":| ", prev)) != string::npos)
    		{
        		if (pos > prev)
        		{
            			one_d.push_back(buffer.substr(prev, pos-prev));
            			non_terminals.push_back(one_d.front());
            	}
        		prev = pos+1;
    		}
    		if (prev < buffer.length())
        	{
        		one_d.push_back(buffer.substr(prev, string::npos));
        	}
        	two_d.push_back(one_d);
        	one_d.clear();
	}
	/*cout << non_terminals[0];
	for( vector<string>::const_iterator i = one_d.begin(); i != one_d.end(); ++i)
    		cout << *i << '?';*/
	ingrammar.close();                           // Close file after use
}

void readOpPrecTable()
{
	fstream intable;
	char input[] = "op_prec.txt";
	string buffer;
	intable.open(input, ios::in);
	if (!intable.is_open()) { // File Check
	  cerr << "Can't open input file " << input << endl;
	  return;
	}
	while(getline(intable, buffer))
	{
		size_t prev = 0, pos;
    		while ((pos = buffer.find_first_of(" ", prev)) != string::npos)
    		{
        		if (pos > prev)
        		{
            			op_table_1.push_back(buffer.substr(prev, pos-prev));
            		}
        		prev = pos+1;
    		}
    		if (prev < buffer.length())
        	{
        		op_table_1.push_back(buffer.substr(prev, string::npos));
        	}
        	op_table_2.push_back(op_table_1);
        	sizeTerm = op_table_1.size();
		/*for( vector<string>::const_iterator i = op_table_1.begin(); i != op_table_1.end(); ++i)
	    		cout << *i << ':';
	    	cout << endl;*/
	    	op_table_1.clear();
	}
	for(int i = 1;i < sizeTerm; i++)
	{
        	terminals.push_back(op_table_2[0][i]);
    	}
}

int findIndex(vector<string> s,string t)
{
        for(int i = 0; i < sizeTerm; i++)
        {
            if(s[i].compare(t) == 0)
            {
                return i;
            }
        }
        return -1;
}

int isTerminal(string s)
{
    if(findIndex(terminals, s) != -1)
        return 1;
    else
        return 0;
}

string searchNTerm(string s)
{
    int n = two_d.size(),r;
    for(int i = 0; i < n; i++)
    {
        r = two_d[i].size();
        for(int j = 1; j < r; j++)
        {
            if((two_d[i][j]).compare(s) == 0)
            {
                return two_d[i][0];
            }
        }
    }
}


int main()
{
    stack<string> termCheck;
    readGrammar();
    readOpPrecTable();
    string exp = "i+i$";
    int inLen = exp.size();
    stack<string> dummy;
    dummy.push("$");
    int ig = 0;
    int stackTop, stringTop;
    string parserOp="", expTop="", strReplace="", stackC = "$", stringR="";
    termCheck.push("$");
    while(exp.substr(ig,1).compare("$") != 0)
    {
	if(ig < inLen-1)
            expTop = exp.substr(ig,1);
        else
            break;
        stringTop = findIndex(op_table_2[0], expTop);
        stackTop = findIndex(op_table_2[0], termCheck.top());
        parserOp = op_table_2[stackTop][stringTop];
        //cout << ig<<".."<<termCheck.top()<<stackTop << expTop<<stringTop << parserOp <<  dummy.top()<<expTop<<endl;
        cout << stackC << "\t" << exp.substr(ig, inLen) << "\t" << parserOp << "\t" << "Shift" << ig << endl;
        if(parserOp=="<" || parserOp=="=") // Shift
        {
            dummy.push(expTop);
            stackC+=expTop;
            ig++;
            if(isTerminal(expTop))
            {
            	cout << "Pushing..." << expTop << endl;
        	    termCheck.push(expTop);
            }
        }
        if(parserOp==">")
        {
            int t=0;
            if(isTerminal(dummy.top()))
            {
                strReplace=dummy.top();
                dummy.pop();
                t++;
                stringR = searchNTerm(strReplace);
                dummy.push(stringR);
                string val = stackC.substr(0, stackC.size()-t);
                stackC=val+stringR;
            }
            else
            {
                while(!(isTerminal(dummy.top())))
                {
                    strReplace+=dummy.top();
                    dummy.pop();
                    t++;
                }
                strReplace+=dummy.top();
                dummy.pop();t++;
                strReplace+=dummy.top();
                dummy.pop();t++;
                stringR = searchNTerm(strReplace);
                dummy.push(stringR);
                string val = stackC.substr(0, stackC.size()-t);
                stackC=val+stringR;
            }
            termCheck.pop();
        }
    }
    cout << stackC << "\t" << exp.substr(ig, inLen) << "\t" << parserOp << "\t" << "Shift" << ig << endl;
    return 0;
}
