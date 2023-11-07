/* Basic Interpreter by H�seyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */

// 1. include문

// stdio.h : 소스 코드 표준 입출력과 관련되어 있으며 헤더파일을 호출하여 사용한다.
#include <stdio.h>

// conio.h : 콘솔 입출력 함수를 제공하는 헤더이다.
#include <conio.h>

// string.h : 메모리 블록이나 문자열(문자열의 조작, 복사, 비교 및 검색)을 다룰 수 있는 함수를 제공한다.
#include <string.h>

// stdlib.h : 프로그램 실행 관련 함수, 동적 메모리 관리 등의 함수 제공한다.
#include <stdlib.h>

// ctype.h : 아스키 코드 값이 문자와 숫자 등을 판별하는 함수와 대/소문자로 변환하는 함수 제공한다.
#include <ctype.h>


// 연결 리스트 노드 구조체
struct node{
	int type;	// 노드의 타입
	har exp_data;	   // 문자 타입으로 주로 한글자의 데이터를 저장한다.
	int val;		   // 정수 값, 노드에 연결된 값에 대한 정보 저장한다.
	int line;		   // 노드가 속한 라인의 정보, 코드 어느 부분에서 노드가 생성되었는지 추척한다.
	struct node *next; // 연결 리스트의 다음 노트를 가리키는 포인터이다.
};

// typeof : 이미 존재하는 타입에 새로운 이름을 설정한다.
typedef struct node Node;

// 연결 리스트를 기반으로 스택 구조체를 구현한다.
struct stack{
	Node * top;	// 연결리스트 노드의 top를 가리키는 포인터이며 연결리스트를 스택 형식으로 변환한다. 
};

// stack의 이름은 Stack이다. Stack을 사용하여 변수 및 포인터를 선언할 수 있다. 
typedef struct stack Stack;

// 연산자 노드 구조체
struct opnode{
	char op;	// 연산자를 저장하는 문자형 필드이다.
	struct opnode * next;	// 다음 연산자 노드를 가리키는 포인터이다. 
};

// opnode의 이름은 opNode이다. opNode을 사용하여 변수 및 포인터를 선언할 수 있다. 
typedef struct opnode opNode;

// 연산자 스택 구조체
struct opstack{
	opNode * top;	// 스택의 top 연산자 노드를 가리키는 포인터이다.
};

// opstack의 이름은 OpStack이다. OpStack을 사용하여 변수 및 포인터를 선언할 수 있다. 
typedef struct opstack OpStack;

// 후위 표기법 노드 구조체
struct postfixnode{
	int val;
	struct postfixnode * next;	// 다음 후위 표기법 노드를 가리키는 포인터이다.
};

// postfixnode의 이름은 Postfixnode이다. Postfixnode을 사용하여 변수 및 포인터를 선언할 수 있다. 
typedef struct postfixnode Postfixnode;

// 후위 표기법 스택 구조체
struct postfixstack{
	Postfixnode * top;	// 스택의 top 연산자 노들르 가리키는 포인터이다.
	};

// postfixstack의 이름은 PostfixStack이다. PostfixStack을 사용하여 변수 및 포인터를 선언할 수 있다. 
typedef struct postfixstack PostfixStack;





int GetVal(char,int *,Stack *);
int GetLastFunctionCall(Stack*);

Stack* FreeAll(Stack *);

Stack * Push(Node sNode,Stack *stck)
{
Node *newnode;

if ((newnode=(Node*)malloc(sizeof(Node)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->type=sNode.type;
newnode->val=sNode.val;
newnode->exp_data=sNode.exp_data;
newnode->line=sNode.line;
newnode->next=stck->top;
stck->top=newnode;
return stck;
}
}

OpStack * PushOp(char op,OpStack *opstck)
{
opNode *newnode;
if ((newnode=(opNode*)malloc(sizeof(opNode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->op=op;
newnode->next=opstck->top;
opstck->top=newnode;
return opstck;
}
}

char PopOp(OpStack *opstck)
{
opNode *temp;
char op;
if (opstck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
op=opstck->top->op;
temp=opstck->top;
opstck->top=opstck->top->next;
free(temp);
return op;
}
return NULL;
}


PostfixStack * PushPostfix(int val,PostfixStack *poststck)
{
Postfixnode *newnode;
if ((newnode=(Postfixnode*)malloc(sizeof(Postfixnode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->val=val;
newnode->next=poststck->top;
poststck->top=newnode;
return poststck;
}
}

char PopPostfix(PostfixStack *poststck)
{
Postfixnode *temp;
int val;
if (poststck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
val=poststck->top->val;
temp=poststck->top;
poststck->top=poststck->top->next;
free(temp);
return val;
}
return NULL;
}





void Pop(Node * sNode,Stack *stck)
{
Node *temp;

if (stck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
sNode->exp_data=stck->top->exp_data;
sNode->type=stck->top->type;
sNode->line=stck->top->line;
sNode->val=stck->top->val;
temp=stck->top;
stck->top=stck->top->next;
free(temp);
}
}

int isStackEmpty(OpStack *stck)
{
	if (stck->top==0)
		return 1;
return 0;
}

/*void printAllStack(Stack * stck)
{
	Node tempNode;
	printf("\n------------------\n");
	printf("dumping the stack...\n");
	Pop(&tempNode,stck);
	printf("exp=%c type=%d val=%d 	line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	while( (stck->top!=NULL) ) {
		Pop(&tempNode,stck);
		printf("exp=%c type=%d val=%d 				line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	}
} */

int Priotry(char operator)
{
	if ((operator=='+') | (operator=='-'))
		return 1;
	else if ((operator=='/') | (operator=='*'))
		return 2;
return 0;
}

int main(int argc,char ** argv)
{
	// 문자열 데이터를 저장하는 배열
	char line[4096];
	char dummy[4096];
	char lineyedek[4096];
	char postfix[4096];

	char * firstword;	


	// 정수형 변수, 정보 저장
	// int i;
	int val1;
	int val2;


	int LastExpReturn;
	int LastFunctionReturn=-999;
	int CalingFunctionArgVal;

	Node tempNode;	// 연결 리스트이 임시 노드로 Node 구조체 변수 

	OpStack * MathStack; // OpStack의 주소값을 MathStack에 저장 

	FILE *filePtr;	// 처리할 파일을 가리키는 파일 포인터

	PostfixStack * CalcStack; // PostfixStack의 주소값을 CalcStack에 저장 

	int resultVal;

	Stack * STACK;	// Stack의 주소값을 STACK에 저장 

	int curLine=0;
	int foundMain=0;
        int WillBreak=0;


	MathStack->top=NULL;
	CalcStack->top=NULL;
	STACK->top=NULL;
	clrscr();	// 화면 초기화

	//argc가 2이면
	if (argc!=2)
	{
		/* if argument count is =1 */
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>",argv[0]);
		return 1;
	}

	/* open the file */

	if ( ( filePtr=fopen(argv[1],"r") ) == NULL )
	{
		printf("Can't open %s. Check the file please",argv[1]);
		return 2;
	}


	while (!feof(filePtr))
	{

		int k=0;

		fgets(line,4096,filePtr); /* read the file by Line by Line */
		/* scan for /t characters. get rid of them! */
		while(line[k]!='\0')
		{
		 if (line[k]=='\t')
		 {
			line[k]=' ';
		 }

		 k++;
		}

		strcpy(lineyedek,line);

		curLine++;
		tempNode.val=-999;
		tempNode.exp_data=' ';
		tempNode.line=-999;
		tempNode.type=-999;



		if (!strcmpi("begin\n",line) | !strcmpi("begin",line))
		{
			if (foundMain)
			{
				tempNode.type=4;
				STACK=Push(tempNode,STACK);
			}
		}
		else if (!strcmpi("end\n",line) | !strcmpi("end",line) )
		{
			if (foundMain)
			{
				int sline;


				tempNode.type=5;
				STACK=Push(tempNode,STACK);

				sline=GetLastFunctionCall(STACK);
				if (sline==0)
				{
					/* WE FOUND THE RESULT! */
					printf("Output=%d",LastExpReturn);

				}
				else
				{
					int j;
					int foundCall=0;
					LastFunctionReturn=LastExpReturn;
					/* get to the last line that have been a function calling */

						fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */
							for(j=1;j<sline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}


						/* clear all the stack up to the last function call */
						while(foundCall==0)
						{
							Pop(&tempNode,STACK);
							if (tempNode.type==3)
							{
								foundCall=1;
							}
						}

				}


			}
		}
		else
		{
			//we need to tokinize
			firstword=strtok(line," ");


			if (!strcmpi("int",firstword))
			{
				if (foundMain)
				{
					tempNode.type=1; /*integer*/
					firstword=strtok(NULL," ");
					tempNode.exp_data=firstword[0];


					firstword=strtok(NULL," ");

					/* check for = */
					if (!strcmpi("=",firstword))
					{
						firstword=strtok(NULL," ");
					}

					tempNode.val=atoi(firstword);
					tempNode.line=0;
					STACK=Push(tempNode,STACK);
				}
			}
			else if (!strcmpi("function",firstword))
			{
				tempNode.type=2;
				firstword=strtok(NULL," ");
				tempNode.exp_data=firstword[0];
				tempNode.line=curLine;
				tempNode.val=0;
				STACK=Push(tempNode,STACK);

				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n') )
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain=1;


				}
				else
				{
					if (foundMain)
					{
						firstword=strtok(NULL," ");
						tempNode.type=1;
						tempNode.exp_data=firstword[0];
						tempNode.val=CalingFunctionArgVal;
						tempNode.line=0;
						STACK=Push(tempNode,STACK);



					}

				}


			}
			else if (firstword[0]=='(')
			{

			if (foundMain)
				{

				int i=0;
				int y=0;


				MathStack->top=NULL;
				/* now make the postfix calculcation */





				while(lineyedek[i]!='\x0')
				{
					/* evulate the function */
					if (isdigit(lineyedek[i])) {
						postfix[y]=lineyedek[i];
						y++;
					}
				     /*	else if (lineyedek[i]=='(')
					{
						MathStack=PushOp(lineyedek[i],MathStack);
					}*/

					else if (lineyedek[i]==')')
					{
						if (!isStackEmpty(MathStack))
						{
							postfix[y]=PopOp(MathStack);
							y++;
						}
					}
					else if ((lineyedek[i]=='+') | (lineyedek[i]=='-') | (lineyedek[i]=='*') | (lineyedek[i]=='/'))
					{
						/*operators*/
						if (isStackEmpty(MathStack))
						{
					/* if stack empty push the operator to stack */

							MathStack=PushOp(lineyedek[i],MathStack);
						}
						else
						{
							/* check for presedence */
							if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op) )
							{
								/* higher presedence for example + < *  */
								/* pop the last operator */

								/* add the poped operator to the postfix */
								postfix[y]=PopOp(MathStack);
								y++;


								MathStack=PushOp(lineyedek[i],MathStack);


							}
							else
							{
								/* lower presedence for example / > + */
								/* push to stack */

								MathStack=PushOp(lineyedek[i],MathStack);
							}

						}
					}
					else if (isalpha(lineyedek[i])>0)
					{
						int codeline=0;
						int dummyint=0;
						/*look if it's a variable or function call 						*/
						int retVal=0;
						retVal=GetVal(lineyedek[i],&codeline,STACK);
						if ((retVal!=-1) & (retVal!=-999))
						{
							/* if variable */
							postfix[y]=retVal+48; /* in ascii table numeric values start from 48 */
							y++;

						}
						else
						{

						    if (LastFunctionReturn==-999)
						    {

							/* if function */
							/* add to our system stack that we are making a call to function */
							int j;
							tempNode.type=3;
							tempNode.line=curLine;
							STACK=Push(tempNode,STACK);

							/* get function's arguments value */
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK);



							fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */


/* !!!!!!!!!!!!!!! j <= codeline olabilir */
							for(j=1;j<codeline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}

							WillBreak=1;
							break;
						    }
						    else

						    {


							postfix[y]=LastFunctionReturn+48; /* in ascii table numeric values start from 48 */
							y++;
							i=i+3;
							LastFunctionReturn=-999;


						    }
						}
					}

					i++;
				}

				if (WillBreak==0)
				{
				/* get out items left in the mathstack */
				while (isStackEmpty(MathStack)==0)
				{

					/* add the poped operator to the postfix */
					postfix[y]=PopOp(MathStack);
					y++;
				}

				postfix[y]='\0';

				//MathStack=FreeAll(MathStack);

				/* now calculate the postfix */
				/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/

				i=0;

				CalcStack->top=NULL;
				while(postfix[i]!='\x0')
				{
					if (isdigit(postfix[i])) {
						/* push to stack */
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack);
					}
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/'))
					{
						val1=PopPostfix(CalcStack);

						val2=PopPostfix(CalcStack);


						switch (postfix[i])
						{
							case '+': resultVal=val2+val1;break;
							case '-': resultVal=val2-val1;break;
							case '/': resultVal=val2/val1;break;
							case '*': resultVal=val2*val1;break;
						}

						CalcStack=PushPostfix(resultVal,CalcStack);
					}
					i++;
				}

				//CalcStack=FreeAll(CalcStack);
				LastExpReturn=CalcStack->top->val;


				}
				WillBreak=0;
				}
			}
		}


	}

	fclose(filePtr);

	//printAllStack(STACK);
	STACK=FreeAll(STACK);

	printf("\nPress a key to exit...");
	getch();
	return  0;
}

Stack * FreeAll(Stack * stck)
{
Node * temp;
Node * head;

	if (stck->top != NULL )
	{
		head=stck->top;
		do
		{

			temp=head;
			head=head->next;
			free(temp);

		} while (head->next!=NULL);
	}

return NULL;
}
int GetLastFunctionCall(Stack *stck)
{
Node * head;

	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head=stck->top;
		do
		{
			if ( head->type==3 )
			{
				return head->line;
			}
			else
			{
				head=head->next;
			}
		} while (head->next!=NULL);
	}

	return 0;



}

int GetVal(char exp_name,int * line,Stack *stck)
{

Node * head;
*line=0;
	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head=stck->top;
		do
		{
			if ( head->exp_data==exp_name )
			{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}

			}
			else
			{
				head=head->next;
			}
		} while (head->next!=NULL);
		/* check agin once more */
		if (head->exp_data==exp_name)
		{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}


		}
	}

	return -999;
}

