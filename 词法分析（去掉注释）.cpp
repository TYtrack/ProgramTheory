#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "iostream"
using namespace std;


static char reserveWord[34][20] = {
	"dim","if","do","stop","auto", "break", "case", "char", "const", "continue",
	"default",  "double", "else", "enum", "extern",
	"float", "for", "goto",  "int", "long",
	"register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void",
	"volatile", "while"
};



static char operatorOrDelimiter[37][10] = {
	"=","+","*", "**", ",", "(", ")","-","/","<","<=",">",">=","==",
	"!=",";","^","\"","\'","#","&",
	"&&","|","||","%","~","<<",">>","[","]","{",
	"}","\\",".","\?",":","!"
};

 /********查找保留字*****************/
int searchReserve(char reserveWord[][20], char s[])
{
	for (int i = 0; i < 34; i++)
	{
		if (strcmp(reserveWord[i], s) == 0)
		{
			return i + 1;
		}
	}

	return -1;
}


/*********************判断是否为字母********************/
bool IsLetter(char letter)
{
	if (letter >= 'a'&&letter <= 'z' || letter >= 'A'&&letter <= 'Z' || letter == '_')
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*****************判断是否为数字************************/
bool IsDigit(char digit)
{
	if (digit >= '0'&&digit <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}


/********************编译预处理，取出无用的字符和注释**********************/
void filterResource(char r[], int pProject)
{
	char tempString[10000];
	int count = 0;
	for (int i = 0; i <= pProject; i++)
	{
		if (r[i] == '/'&&r[i + 1] == '/')
		{
			while (r[i] != '\n')
			{
				i++;
			}
		}
		if (r[i] != '\n'&&r[i] != '\t'&&r[i] != '\v'&&r[i] != '\r')
		{
			tempString[count++] = r[i];
		}
	}
	tempString[count] = '\0';
	strcpy(r, tempString);
}



/****************************DFA算法***********************/
void Scanner(int &syn, char resourceProject[], char token[], int &pProject)
{
	int i, count = 0;
	char ch;
	ch = resourceProject[pProject];
	while (ch == ' ')
	{
		pProject++;
		ch = resourceProject[pProject];
	}
	for (i = 0; i<20; i++)
	{
		token[i] = '\0';
	}
	if (IsLetter(resourceProject[pProject]))
	{
		token[count++] = resourceProject[pProject];
		pProject++;
		while (IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject]))
		{
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		token[count] = '\0';
		syn = searchReserve(reserveWord, token);
		if (syn == -1)
		{
			syn = 36;
		}
		return;
	}
	else if (IsDigit(resourceProject[pProject]))
	{
		while (IsDigit(resourceProject[pProject]))
		{
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		token[count] = '\0';
		syn = 35;
	}
	else if (ch == '=' || ch == '+'|| ch == ',' || ch == '(' || ch == ')'|| ch == '-' || ch == '*' 
			||ch == '/' || ch == ';'  || ch == '^'|| ch == '\"' || ch == '\'' || ch == '~'
			|| ch == '#' || ch == '%' || ch == '['|| ch == ']' || ch == '{' || ch == '}' || ch == '\\' 
		    || ch == '.' || ch == '\?' || ch == ':')
	{
		token[0] = resourceProject[pProject];
		token[1] = '\0';
		for (i = 0; i<37; i++)
		{
			if (strcmp(token, operatorOrDelimiter[i]) == 0)
			{
				syn = 37 + i;
				break;
			}
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '<')
	{
		pProject++;
		if (resourceProject[pProject] == '=')
		{
			syn = 47;
		}
		else if (resourceProject[pProject] == '<')
		{
			syn = 63;
		}
		else
		{
			pProject--;
			syn = 46;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '>')
	{
		pProject++;
		if (resourceProject[pProject] == '=')
		{
			syn = 49;
		}
		else if (resourceProject[pProject] == '>')
		{
			syn = 64;
		}
		else
		{
			pProject--;
			syn = 48;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '=')
	{//
		pProject++;
		if (resourceProject[pProject] == '=')
		{
			syn = 50;
		}
		else
		{
			pProject--;
			syn = 37;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '!')
	{
		pProject++;
		if (resourceProject[pProject] == '=')
		{
			syn = 51;
		}
		else
		{
			syn = 73;
			pProject--;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '&')
	{
		pProject++;
		if (resourceProject[pProject] == '&')
		{
			syn = 58;
		}
		else
		{
			pProject--;
			syn = 57;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '|')
	{
		pProject++;
		if (resourceProject[pProject] == '|')
		{
			syn = 60;
		}
		else
		{
			pProject--;
			syn = 59;
		}
		pProject++;
		return;
	}
	else if (resourceProject[pProject] == '*')
	{

		pProject++;

		if (resourceProject[pProject] == '*')
		{
			syn = 40;
		}
		else
		{
			pProject--;
			syn = 39;
		}
		pProject++;
		return;
	}
	else  if (resourceProject[pProject] == '$')
	{
		syn = 74;
	}
	else
	{
		printf("error：there is no exist %c \n", ch);
		system("pause");
		exit(0);
	}
}


int main()
{
	char resourceProject[10000];
	char token[20] = { 0 };
	int syn = -1, i;
	int pProject = 0;
	FILE *fp, *fp1;
	if ((fp = fopen("D:\\zyr_rc.txt", "r")) == NULL)
	{
		cout << "can't open this file";
		exit(0);
	}
	resourceProject[pProject] = fgetc(fp);
	while (resourceProject[pProject] != '$')
	{
		pProject++;
		resourceProject[pProject] = fgetc(fp);
		cout << resourceProject[pProject] << "";
	}
	resourceProject[++pProject] = '\0';
	fclose(fp);
	cout << endl << "源程序为:" << endl;
	cout << resourceProject << endl;
	
	filterResource(resourceProject, pProject);

	cout << endl << "过滤之后的程序:" << endl;
	cout << resourceProject << endl;
	pProject = 0;

	if ((fp1 = fopen("D:\\zyr_compile.txt", "w+")) == NULL)
	{
		cout << "can't open this file";
		exit(0);
	}
	while (syn != 74)
	{
		
		Scanner(syn, resourceProject, token, pProject);
		if (syn == 36)
		{

			printf("(标识符  ,%s)\n", token);
			fprintf(fp1, "(标识符   ,%s)\n", token);
		}
		else if (syn >= 1 && syn <= 34)
		{
			printf("(%s   ,  %d)\n", reserveWord[syn - 1],  syn);
			fprintf(fp1, "(%s   ,  %d)\n", reserveWord[syn - 1], syn);
		}
		else if (syn == 35)
		{
			printf("(常数   ,   %s)\n", token);
			fprintf(fp1, "(常数   ,   %s)\n", token);
		}
		else if (syn >= 37 && syn <= 73)
		{
			printf("(%s   ,   %d)\n", operatorOrDelimiter[syn - 37], syn);
			fprintf(fp1, "(%s   ,   %d)\n", operatorOrDelimiter[syn - 37], syn);
		}
		else if (syn==74)
		{
			printf("($   ,   74)\n");
			fprintf(fp1, "($   ,   74)\n");
		}

	}

	fclose(fp1);
	system("pause");
	return 0;
}