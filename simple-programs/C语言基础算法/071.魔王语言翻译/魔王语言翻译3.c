//【实现】将需要解释的魔王语言当成一个字符数组，入栈1。再依次出栈，入栈2，依次处理顶端字符，若是开括号逐一出栈入队列，直至闭括号出栈。再逐一出队列，按照规则2解释，再重新入栈2。出栈2，按照规则1解释。
//原文链接 http://m.blog.csdn.net/sinat_35954614/article/details/71565627

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct Node  //节点
{
	char data;
	struct Node *next;
}Node,*NodePtr;

typedef struct Stak		//栈链表
{
	NodePtr top;
	NodePtr bot;
	int count;
}Stak;

//初始化栈
void InitStak(Stak *ts)		
{
	ts->top = (NodePtr)malloc(sizeof(Node));
	if (NULL == ts->top)
	{
		printf("分配失败");
		exit(-1);
	}
	ts->bot = ts->top;
	ts->count = 0;
	ts->top->next = NULL;
	
}

//入栈
void Push(Stak *S, char e)		
{
	NodePtr pnew = (NodePtr)malloc(sizeof(Node));
	pnew->data = e;
	pnew->next = S->top;
	S->top = pnew;
	S->count++;
}

//出栈
char Pop(Stak *S)		
{
	
	NodePtr p;
	if (S->bot == S->top)
	{//空时返回1 删除失败
		exit(1);
	}
	p = S->top;
	char e = p->data;
	S->top = S->top->next;
	free(p);
	S->count--;
	return e;

}

//打印栈
void prin(Stak *S)
{
	NodePtr p;
	p = S->top;

	while (p != S->bot)
	{
		printf("%c\n",p->data);
		p = p->next;
	}
}

bool EmptyStack(Stak *S)
//判断是否空栈
{
	if (S->count == 0)
		return 1;
	return 0;
}

typedef struct QNode	//队列节点
{
	char data;
	struct QNode *next;
}QNode,*QNodePtr;

typedef struct LinkQueue	//队列链表
{
	QNodePtr front, rear;
}Queue;

//队列初始化
void InitQue(Queue *Q)
{
	QNodePtr p = (QNodePtr)malloc(sizeof(QNode));	//p为头节点
	p->data = NULL;
	p->next = NULL;
	Q->front = p;
	Q->rear = p;
}

//入队列
void EnQue(Queue *Q, char e)
{
	QNodePtr p = (QNodePtr)malloc(sizeof(QNode));
	p->data = e;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
}

char DeQue(Queue *Q)
//出队列
{
	QNodePtr p;
	char c;
	if (Q->front == Q->rear)
	{//空时
		exit(1);
	}
	p = Q->front->next;
	c = Q->front->next->data;
	Q->front->next = p->next;
	if (p == Q->rear)
	{
		Q->rear = Q->front;
	}
	free(p);

	return c;
}

void pri(Queue *Q)
//打印队列
{
	QNodePtr p;
	p = Q->front;
	if (p != NULL)
	{
		while (p != Q->rear)
		{
			printf("%c", p->next->data);
			p = p->next;
		}
	}
	
}

bool EmptyQue(Queue *Q)
{
	if (Q->front == Q->rear)
		return 1;
	return 0;
}

void Reverse(char M[], Stak *S)
{
	int i;
	int len = strlen(M);
	int l = 0, r = 0;
	for (i =0; i <len; i++)
	{
		Push(S, M[i]);
		if (M[i] == '(')
			l++;
		if (M[i] == ')')
			r++;
	}
	if (l != r)
		exit(1);
}

void EnA(Queue *Q)
//规则1
{
	EnQue(Q, 's');
	EnQue(Q, 'a');
	EnQue(Q, 'e');
}

void EnB(Queue *Q)
//规则1
{
	EnQue(Q, 't');
	EnA(Q);
	EnQue(Q, 'd');
	EnA(Q);
}

void  Fpri(Queue *Q)
{
	char c;
	while (!EmptyQue(Q))
	{
		c = DeQue(Q);
		switch (c)
		{
		case 't':printf("天"); break;
		case 'd':printf("地"); break;
		case 's':printf("上"); break;
		case 'a':printf("一只"); break;
		case 'e':printf("鹅"); break;
		case 'z':printf("追"); break;
		case 'g':printf("赶"); break;
		case 'x':printf("下"); break;
		case 'n':printf("蛋"); break;
		case 'h':printf("恨"); break;
		default: printf("Error");
		}

	}
}

void Tran(Stak *Sbe,Stak *Saf,Queue *Q )
{
	char c;
	char d;
	int i=0;
	while (Sbe->count != 0)
	{
		
		c = Pop(Sbe);

		 if (c == ')')
			i = Saf->count;
		else if (c == '(')
		{
			int j = Saf->count;
			while ( j > i )
			{
				d = Pop(Saf);
				EnQue(Q, d);
				j--;
			}
			char e = Q->front->next->data;
			DeQue(Q);
			while (!EmptyQue(Q))
			//规则2
			{
				char dl = DeQue(Q);
				Push(Saf, e);
				Push(Saf, dl);
			}
			Push(Saf, e);
		}
		else
			Push(Saf, c);
	}

	while (Saf->count>0)
	{
		char en;
		en = Pop(Saf);
		if (en == 'A')
			EnA(Q);
		else if (en == 'B')
			EnB(Q);
		else
			EnQue(Q,en);
	}
	Fpri(Q);
}


int main() 
{
	Stak Sf,St;
	InitStak(&Sf);
	InitStak(&St);
	Queue Qu1;
	InitQue(&Qu1);

	char M[] = "B(ehnxgz)B";
	Reverse(M, &Sf);
	
	Tran(&Sf, &St, &Qu1);
	getchar();
	return 0;
}