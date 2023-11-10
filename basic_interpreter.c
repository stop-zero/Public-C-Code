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
struct node
{
	int type;		   // 노드의 타입
	har exp_data;	   // 문자 타입으로 주로 한글자의 데이터를 저장한다.
	int val;		   // 정수 값, 노드에 연결된 값에 대한 정보 저장한다.
	int line;		   // 노드가 속한 라인의 정보, 코드 어느 부분에서 노드가 생성되었는지 추척한다.
	struct node *next; // 연결 리스트의 다음 노트를 가리키는 포인터이다.
};

// typeof : 이미 존재하는 타입에 새로운 이름을 설정한다.
typedef struct node Node;

// 연결 리스트를 기반으로 스택 구조체를 구현한다.
struct stack
{
	Node *top; // 연결리스트 노드의 top를 가리키는 포인터이며 연결리스트를 스택 형식으로 변환한다.
};

// stack의 이름은 Stack이다. Stack을 사용하여 변수 및 포인터를 선언할 수 있다.
typedef struct stack Stack;

// 연산자 노드 구조체
struct opnode
{
	char op;			 // 연산자를 저장하는 문자형 필드이다.
	struct opnode *next; // 다음 연산자 노드를 가리키는 포인터이다.
};

// opnode의 이름은 opNode이다. opNode을 사용하여 변수 및 포인터를 선언할 수 있다.
typedef struct opnode opNode;

// 연산자 스택 구조체
struct opstack
{
	opNode *top; // 스택의 top 연산자 노드를 가리키는 포인터이다.
};

// opstack의 이름은 OpStack이다. OpStack을 사용하여 변수 및 포인터를 선언할 수 있다.
typedef struct opstack OpStack;

// 후위 표기법 노드 구조체
struct postfixnode
{
	int val;
	struct postfixnode *next; // 다음 후위 표기법 노드를 가리키는 포인터이다.
};

// postfixnode의 이름은 Postfixnode이다. Postfixnode을 사용하여 변수 및 포인터를 선언할 수 있다.
typedef struct postfixnode Postfixnode;

// 후위 표기법 스택 구조체
struct postfixstack
{
	Postfixnode *top; // 스택의 top 연산자 노들르 가리키는 포인터이다.
};

// postfixstack의 이름은 PostfixStack이다. PostfixStack을 사용하여 변수 및 포인터를 선언할 수 있다.
typedef struct postfixstack PostfixStack;

int GetVal(char, int *, Stack *);
int GetLastFunctionCall(Stack *);

Stack *FreeAll(Stack *);

Stack *Push(Node sNode, Stack *stck) // Push 함수 : 스택에 새로운 노드를 추가
{
	Node *newnode;

	if ((newnode = (Node *)malloc(sizeof(Node))) == NULL)
	{
		printf("ERROR, Couldn't allocate memory...");
		return NULL; // 메모리 할당에 실패한 경우 NULL 반환 후 함수 종료
	}
	else // 메모리 할당에 성공한 경우
	{
		// 새 노드의 데이터를 채우는 과정
		newnode->type = sNode.type;
		newnode->val = sNode.val;
		newnode->exp_data = sNode.exp_data;
		newnode->line = sNode.line;
		// 새 노드를 스택의 맨 위에 추가
		newnode->next = stck->top;
		stck->top = newnode;
		// 변경된 스택 반환
		return stck;
	}
}

OpStack *PushOp(char op, OpStack *opstck)
{
	opNode *newnode;
	if ((newnode = (opNode *)malloc(sizeof(opNode))) == NULL)
	{
		printf("ERROR, Couldn't allocate memory...");
		return NULL;
	}
	else
	{
		newnode->op = op;
		newnode->next = opstck->top;
		opstck->top = newnode;
		return opstck;
	}
}

char PopOp(OpStack *opstck)
{
	opNode *temp;
	char op;
	if (opstck->top == NULL)
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		op = opstck->top->op;
		temp = opstck->top;
		opstck->top = opstck->top->next;
		free(temp);
		return op;
	}
	return NULL;
}

PostfixStack *PushPostfix(int val, PostfixStack *poststck)
{
	Postfixnode *newnode;
	if ((newnode = (Postfixnode *)malloc(sizeof(Postfixnode))) == NULL)
	{
		printf("ERROR, Couldn't allocate memory...");
		return NULL;
	}
	else
	{
		newnode->val = val;
		newnode->next = poststck->top;
		poststck->top = newnode;
		return poststck;
	}
}

char PopPostfix(PostfixStack *poststck)
{
	Postfixnode *temp;
	int val;
	if (poststck->top == NULL)
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		val = poststck->top->val;
		temp = poststck->top;
		poststck->top = poststck->top->next;
		free(temp);
		return val;
	}
	return NULL;
}

void Pop(Node *sNode, Stack *stck) // Pop : 해당 노드의 데이터를 sNode로 복사한다.
{
	Node *temp;

	if (stck->top == NULL) // 스택이 비어있으면, 오류 메시지를 출력하고 함수를 종료한다.
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		// 스택의 top 노드의 데이터를 sNode로 복사한다.
		sNode->exp_data = stck->top->exp_data;
		sNode->type = stck->top->type;
		sNode->line = stck->top->line;
		sNode->val = stck->top->val;
		// 스택에서 top 노드를 제거하고 메모리 공간을 해제한다.
		temp = stck->top;
		stck->top = stck->top->next;
		free(temp);
	}
}

int isStackEmpty(OpStack *stck)
{
	if (stck->top == 0)
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
	if ((operator== '+') | (operator== '-'))
		return 1;
	else if ((operator== '/') | (operator== '*'))
		return 2;
	return 0;
}

int main(int argc, char **argv)
{
	// 문자열 데이터를 저장하는 배열
	char line[4096];
	char dummy[4096];
	char lineyedek[4096];
	char postfix[4096];

	char *firstword;

	// 정수형 변수, 정보 저장
	// int i;
	int val1;
	int val2;

	int LastExpReturn;
	int LastFunctionReturn = -999;
	int CalingFunctionArgVal;

	Node tempNode; // 연결 리스트이 임시 노드로 Node 구조체 변수

	OpStack *MathStack; // OpStack의 주소값을 MathStack에 저장

	FILE *filePtr; // 처리할 파일을 가리키는 파일 포인터

	PostfixStack *CalcStack; // PostfixStack의 주소값을 CalcStack에 저장

	int resultVal;

	Stack *STACK; // Stack의 주소값을 STACK에 저장

	int curLine = 0;
	int foundMain = 0;
	int WillBreak = 0;

	MathStack->top = NULL;
	CalcStack->top = NULL;
	STACK->top = NULL;
	clrscr(); // 화면 초기화

	// argc는 main 함수에 전달되는 데이터의 갯수를 의미한다.
	//  2가 아니면 "Incorrect arguments!"메시지를 출력하고 종료한다.
	if (argc != 2)
	{
		/* if argument count is =1 */
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>", argv[0]);
		return 1;
	}

	/* open the file */
	// 전달된 파일 이름을 읽기 모드(r)로 연다.
	// 파일을 열지 못하면 NULL 값을 반환한다.
	if ((filePtr = fopen(argv[1], "r")) == NULL)
	{
		printf("Can't open %s. Check the file please", argv[1]);
		return 2;
	}

	while (!feof(filePtr)) // filePtr 끝까지 반복
	{

		int k = 0; // while 문을 위한 정수형 k 초기화

		fgets(line, 4096, filePtr); // 최대 4096 문자를 한줄에 읽음

		while (line[k] != '\0') // k번째가 \0이 아닐 동안 반복문
		{
			if (line[k] == '\t') // k번째가 \t 라면
			{
				line[k] = ' '; // 탭 문자를 공백으로 변경
			}

			k++; // 다음 문자를 위한 k 증가
		}

		strcpy(lineyedek, line); // 문자열 복사, 현재 라인을 lineyedek에 복사
		curLine++;				 // while문 돌 때마다 현재 라인 번호를 증가
		tempNode.val = -999;	 // tempNode의 val 필드 초기화
		tempNode.exp_data = ' '; // tempNode의 exp_data 필드 초기화
		tempNode.line = -999;	 // tempNode의 line 필드 초기화
		tempNode.type = -999;	 // tempNode의 type 필드 초기화

		// strcmpi 계산이 0이 되면 문자열이 같은 것, 1이 되면 다른 경우이다.
		// if 문이 참이 되려면 안에 있는 값이 0이 아닌 값이어야 한다.
		// 두 문자열이 같으면 0이 반환되기에 이를 1로 바꾸기 위해 not 연산자를 붙였다.
		if (!strcmpi("begin\n", line) | !strcmpi("begin", line)) // line이 begin\n 또는 begin이라면 수행한다.
		{
			if (foundMain) // int foundMain=0으로 설정되어 있는 상태
			{
				tempNode.type = 4;			   // tempNode의 타입 속성을 4로 설정한다.
				STACK = Push(tempNode, STACK); // STACK은 현재 가장 top을 말하고, STACK의 top dp tempNode를 Push한다.
			}
		}
		else if (!strcmpi("end\n", line) | !strcmpi("end", line)) // line이 end 또는 end\n라면 수행한다.(대소문자를 구분하지 않음)
		{
			if (foundMain) // int foundMain=0으로 설정되어 있는 상태
			{
				int sline; // int형 변수 sline 생성

				tempNode.type = 5;			   // tempNode의 유형을 5로 설정하고
				STACK = Push(tempNode, STACK); // STACK에 tempNode를 푸시한다.

				sline = GetLastFunctionCall(STACK); // 마지막 함수를 가져와서 sline에 넣는다.

				if (sline == 0) // sline의 값이 0이면 (연산자는 같으면 1, 다르면 0을 반환한다.)
				{
					/* WE FOUND THE RESULT! */
					printf("Output=%d", LastExpReturn); // int 변수 값을 출력한다.
				}
				else // sline의 값이 아니라면
				{
					int j;
					int foundCall = 0;
					LastFunctionReturn = LastExpReturn; // 현재 LastFunctionReturn=-999, int 변수인 LastExpReturn에 대입한다.
					/* get to the last line that have been a function calling */

					fclose(filePtr);			   // filePtr 파일을 닫는다.
					filePtr = fopen(argv[1], "r"); // 파일을 열고 argv[1]을 읽는다.
					curLine = 0;				   // curLine = 0을 설정해두면 파일 시작 위치로 이동한다.

					/* file reversed to start postion */
					/* now go codeline lines to go, to the functions line */
					for (j = 1; j < sline; j++) // j가 1dlrh 총 라인의 수를 반복하는 반복문
					{
						fgets(dummy, 4096, filePtr); /* read the file by Line by Line */
						curLine++;					 // 다 읽을 때까지 curLine에 1씩 추가된다.
					}

					/* clear all the stack up to the last function call */
					while (foundCall == 0) // foundCall 값이 0이면
					{
						Pop(&tempNode, STACK);	// tempNode를 가리키는 포인터 연산자 &, 하나씩 삭제한다.
						if (tempNode.type == 3) // tempNode가 3이면
						{
							foundCall = 1; // foundCall이 1로 바뀐다.
						}
					}
				}
			}
		}
		else	// begin 또는 end가 없다면
		{
			// we need to tokinize
			firstword = strtok(line, " ");	// line 별로 공백을 기준으로 문자열을 잘라서 firstword에 할당한다.

			if (!strcmpi("int", firstword))	// 자른 문자열 중 'int'와 같다면 0인데 ! 붙여서 if문을 참(1)으로 만든다.
			{
				if (foundMain)
				{
					tempNode.type = 1; // tempNode의 타입을 1로 설정
					firstword = strtok(NULL, " ");	// 이전에 구분자 찾았던 문자열 주소에서부터 토근화하여
					tempNode.exp_data = firstword[0];	// exp_data에 저장한다.

					firstword = strtok(NULL, " ");	// 다음 문자열부터 공백을 기준으로 구분자를 다시 찾기 시작한다. 

					/* check for = */
					if (!strcmpi("=", firstword))	// 찾다가 = 를 발견하면
					{
						firstword = strtok(NULL, " ");	// = 다음 단어를 토큰화하고,
					}

					tempNode.val = atoi(firstword);	// firstword를 정수 타입으로 반환하고
					tempNode.line = 0;	// 0번째 줄로 설정한다.
					STACK = Push(tempNode, STACK);	// 스택에 Push하여 해당 정보를 저장한다. 
				}
			}
			else if (!strcmpi("function", firstword))
			{
				tempNode.type = 2;
				firstword = strtok(NULL, " ");
				tempNode.exp_data = firstword[0];
				tempNode.line = curLine;
				tempNode.val = 0;
				STACK = Push(tempNode, STACK);

				if ((firstword[0] == 'm') & (firstword[1] == 'a') & (firstword[2] == 'i') & (firstword[3] == 'n'))
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain = 1;
				}
				else
				{
					if (foundMain)
					{
						firstword = strtok(NULL, " ");
						tempNode.type = 1;
						tempNode.exp_data = firstword[0];
						tempNode.val = CalingFunctionArgVal;
						tempNode.line = 0;
						STACK = Push(tempNode, STACK);
					}
				}
			}
			else if (firstword[0] == '(')
			{

				if (foundMain)
				{

					int i = 0;
					int y = 0;

					MathStack->top = NULL;
					/* now make the postfix calculcation */

					while (lineyedek[i] != '\x0')
					{
						/* evulate the function */
						if (isdigit(lineyedek[i]))
						{
							postfix[y] = lineyedek[i];
							y++;
						}
						/*	else if (lineyedek[i]=='(')
					   {
						   MathStack=PushOp(lineyedek[i],MathStack);
					   }*/

						else if (lineyedek[i] == ')')
						{
							if (!isStackEmpty(MathStack))
							{
								postfix[y] = PopOp(MathStack);
								y++;
							}
						}
						else if ((lineyedek[i] == '+') | (lineyedek[i] == '-') | (lineyedek[i] == '*') | (lineyedek[i] == '/'))
						{
							/*operators*/
							if (isStackEmpty(MathStack))
							{
								/* if stack empty push the operator to stack */

								MathStack = PushOp(lineyedek[i], MathStack);
							}
							else
							{
								/* check for presedence */
								if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op))
								{
									/* higher presedence for example + < *  */
									/* pop the last operator */

									/* add the poped operator to the postfix */
									postfix[y] = PopOp(MathStack);
									y++;

									MathStack = PushOp(lineyedek[i], MathStack);
								}
								else
								{
									/* lower presedence for example / > + */
									/* push to stack */

									MathStack = PushOp(lineyedek[i], MathStack);
								}
							}
						}
						else if (isalpha(lineyedek[i]) > 0)
						{
							int codeline = 0;
							int dummyint = 0;
							/*look if it's a variable or function call 						*/
							int retVal = 0;
							retVal = GetVal(lineyedek[i], &codeline, STACK);
							if ((retVal != -1) & (retVal != -999))
							{
								/* if variable */
								postfix[y] = retVal + 48; /* in ascii table numeric values start from 48 */
								y++;
							}
							else
							{

								if (LastFunctionReturn == -999)
								{

									/* if function */
									/* add to our system stack that we are making a call to function */
									int j;
									tempNode.type = 3;
									tempNode.line = curLine;
									STACK = Push(tempNode, STACK);

									/* get function's arguments value */
									CalingFunctionArgVal = GetVal(lineyedek[i + 2], &dummyint, STACK);

									fclose(filePtr);
									filePtr = fopen(argv[1], "r");
									curLine = 0;
									/* file reversed to start postion */
									/* now go codeline lines to go, to the functions line */

									/* !!!!!!!!!!!!!!! j <= codeline olabilir */
									for (j = 1; j < codeline; j++)
									{
										fgets(dummy, 4096, filePtr); /* read the file by Line by Line */
										curLine++;
									}

									WillBreak = 1;
									break;
								}
								else

								{

									postfix[y] = LastFunctionReturn + 48; /* in ascii table numeric values start from 48 */
									y++;
									i = i + 3;
									LastFunctionReturn = -999;
								}
							}
						}

						i++;
					}

					if (WillBreak == 0)
					{
						/* get out items left in the mathstack */
						while (isStackEmpty(MathStack) == 0)
						{

							/* add the poped operator to the postfix */
							postfix[y] = PopOp(MathStack);
							y++;
						}

						postfix[y] = '\0';

						// MathStack=FreeAll(MathStack);

						/* now calculate the postfix */
						/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/

						i = 0;

						CalcStack->top = NULL;
						while (postfix[i] != '\x0')
						{
							if (isdigit(postfix[i]))
							{
								/* push to stack */
								CalcStack = PushPostfix(postfix[i] - '0', CalcStack);
							}
							else if ((postfix[i] == '+') | (postfix[i] == '-') | (postfix[i] == '*') | (postfix[i] == '/'))
							{
								val1 = PopPostfix(CalcStack);

								val2 = PopPostfix(CalcStack);

								switch (postfix[i])
								{
								case '+':
									resultVal = val2 + val1;
									break;
								case '-':
									resultVal = val2 - val1;
									break;
								case '/':
									resultVal = val2 / val1;
									break;
								case '*':
									resultVal = val2 * val1;
									break;
								}

								CalcStack = PushPostfix(resultVal, CalcStack);
							}
							i++;
						}

						// CalcStack=FreeAll(CalcStack);
						LastExpReturn = CalcStack->top->val;
					}
					WillBreak = 0;
				}
			}
		}
	}

	fclose(filePtr);

	// printAllStack(STACK);
	STACK = FreeAll(STACK);

	printf("\nPress a key to exit...");
	getch();
	return 0;
}

Stack *FreeAll(Stack *stck)
{
	Node *temp;
	Node *head;

	if (stck->top != NULL)
	{
		head = stck->top;
		do
		{

			temp = head;
			head = head->next;
			free(temp);

		} while (head->next != NULL);
	}

	return NULL;
}
int GetLastFunctionCall(Stack *stck) // int형 변수 GetLastFunctionCall 생성, 스택 포인터
{
	Node *head; // Node형 head 변수의 주소를 알려주는 포인터

	if (stck->top == NULL) // 스택이 비어있는 경우 오류 메시지를 출력하고 0을 반환한다.
	{
		printf("ERROR, empty stack..."); // 오류 메시지 출력
	}
	else
	{
		head = stck->top; // head가 스택의 최상단으로 이동한다.
		do				  // 스택의 최상단 노드부터 시작
		{
			if (head->type == 3) // head가  3이라면
			{
				return head->line; // head 라인 번호를 반환한다.
			}
			else
			{
				head = head->next; // 3이 아니라면 다음 노드로 넘어간다.
			}
		} while (head->next != NULL); // next가 NULL인 노드가 나올 때까지 이동한다.
	}

	return 0; // 마지막 함수 호출을 찾지 못하면 0을 반환한다.
}

int GetVal(char exp_name, int *line, Stack *stck)
{

	Node *head;
	*line = 0;
	if (stck->top == NULL)
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head = stck->top;
		do
		{
			if (head->exp_data == exp_name)
			{

				if (head->type == 1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type == 2)
				{
					*line = head->line;
					return -1;
					/* it's a function so return -1 */
				}
			}
			else
			{
				head = head->next;
			}
		} while (head->next != NULL);
		/* check agin once more */
		if (head->exp_data == exp_name)
		{

			if (head->type == 1)
			{
				/* return the variables value */
				return head->val;
			}
			else if (head->type == 2)
			{
				*line = head->line;
				return -1;
				/* it's a function so return -1 */
			}
		}
	}

	return -999;
}
