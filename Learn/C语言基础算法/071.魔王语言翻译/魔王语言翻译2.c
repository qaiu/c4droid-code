/*

魔王语言的解释
一、需求分析
1.问题的描述：有一个魔王总是用自己一种非常精练而抽象的语言讲话，没有人能听懂，但他的语言可以逐步的翻译成人的语言，他的语言由以下两种翻译规则由人的语言逐步抽象上去的：
    (1) α翻译为β1β2……βm
    (2) (θδ1δ2……δn)翻译为θδnθδn-1θ……θδ1θ
   在这两种形式中从左到右表解释。要求编写一个魔王语言解释系统，把他的话翻译成人能听懂的语言
2.基本要求：实现用下述两条具体规则和上述规则（2）编写魔语解释系统。设大写字母表示魔王语言的词汇；小写字母表示人的词汇；希腊字母表示可以用大写字母或小写字母代换的变量。魔王语言可以含人的词汇。（1）B tAdA；（2）A sae.
 
字母解释mean：
t 天
d 地
s 上
a 一个
e 鹅
z 追
g 赶
x 下
n 蛋
i 恨

3.测试数据：B(ehnAgz)B翻译为tsaetsaeezegeeeaesenehetsaetsae
*/
#include <conio.h>
#include <stdlib.h>
#define LEN  20
#define L_size 100

struct 
{
	char rA[LEN];
	char ra[LEN];
}rule[LEN];

struct
{
	char mA[LEN];
	char ma[LEN];  
}mean[LEN];

//栈的操作
typedef struct  node
{
	char data;
	struct node * next;
}LinkStackNode, * LinkStack;

//初始化栈
void InitStack(LinkStack * top)
{
	(* top) = (LinkStack)malloc(sizeof(LinkStackNode));
	(* top) -> next = NULL;
}

//入栈
void Push(LinkStack top, char a)
{
	LinkStackNode * temp;	
	temp = (LinkStackNode *)malloc(sizeof(LinkStackNode));
	temp -> data = a;
	temp -> next = top -> next;
	top -> next = temp;
}

//出栈
void Pop(LinkStack top, char *x)
{   
	LinkStackNode * temp;
	temp = top -> next;
	top -> next = temp -> next;
	*x = temp -> data;
	free(temp);
}

//队列的操作
typedef struct Node
{
	char data;
	struct Node * next;
}LinkQueueNode;

typedef struct
{
	LinkQueueNode * front;
	LinkQueueNode * rear;
}LinkQueue;

//队列初始化
void InitQueue(LinkQueue *Q)
{
	Q -> front = (LinkQueueNode *)malloc(sizeof(LinkQueueNode));
	if(Q->front != NULL)
	{
		Q -> rear = Q -> front;
		Q -> front -> next = NULL;
	}
}

//入队
void EnterQueue(LinkQueue *Q, char x)
{
	LinkQueueNode * NewNode;
	NewNode=(LinkQueueNode *)malloc(sizeof(LinkQueueNode));
	if(NewNode != NULL)
	{
		NewNode -> data = x;
		NewNode -> next = NULL;
		Q -> rear -> next = NewNode;
		Q -> rear = NewNode;
	}
}

//出队
void DeleteQueue(LinkQueue *Q, char  *x)
{

	 LinkQueueNode * p;
	 p = Q -> front -> next;
	 Q -> front -> next = p -> next;
	 if(Q -> rear == p)
		Q -> rear = Q -> front;
	 *x = p -> data;
	 free(p);
}

//读规则文件
void read_rulefile()             //读取文件
{
	 FILE * fp;
	 char filename[LEN];
	 int i = 0;
	 //读取文本文件(rule.txt), 将具体规则的对应关系读入数组
	// fgets(filename, LEN, stdin);
	printf("当然规则:\n");
	 fp = fopen("rule.txt", "rt"); 
	 if(NULL == fp)
	 {
		  printf("\nFailed to open file，%sFile may not exist\n",filename);
		  printf("EXIT\n");
		  exit(1);
	 }

	 while(fscanf(fp, "%s %s", rule[i].rA, rule[i].ra) != EOF)
	 {
		  printf("%s %s\n", rule[i].rA, rule[i].ra);
		  i++;
	 }
	 fclose(fp);
	 return;
}


void read_meanfile()
{
	 FILE * fp;
 	 char filename[LEN];
     	 int i = 0;
     	 //读取文本文件(mean.txt), 将小写字母及其对应的含义读入数组
    //     fgets(filename, LEN, stdin);
 	 fp = fopen("mean.txt", "rt"); 

	 if(NULL == fp)
 	 {
 		 printf("\nFailed to open file，%sFile may not exist\n", filename);
		 printf("EXITI\n");
 		 exit(1);
 	 }

    	 while(fscanf(fp, "%s %s", mean[i].mA, mean[i].ma) != EOF)
    	 {
 		 printf("%s %s\n", mean[i].mA, mean[i].ma);
		 i++;
    	 }
	 fclose(fp) ;
	 return;
}

//写文件操作
void save_result(char L[])    //把结果写入result.txt文档中
{
	FILE * fp;
	char filename[LEN];
	int i = 0, j;
	if((fp = fopen("result.txt", "wt")) == NULL)
	{
		printf("Write file error, press any key to exit!");
		exit(1);
 	}

	while(L[i] != '\0')
	{
		for(j = 0; j < 20; j++)
		{
			if(L[i] == mean[j].mA[0])
				fprintf(fp, "%s", mean[j].ma);
		}
		i++; 
	}
	printf("\n文件已保存至result.txt，任意键退出\n");
	fclose(fp);
}

//去括号
/*思想：当没遇到闭括号时，一直压栈（top栈）。一旦遇到闭括号，首先找到与这个闭括号最近的匹配的开括号找到这两个括号“（” “）”之间的第一个字符利用队列（Q 入队）以及（top栈出栈）完成翻译，再把翻译后的队列，添加到（top栈中）把剩余的字符原样添加到top栈中在把top栈逆置到top1栈中再利用top1栈出栈，给L重新赋值。递归，看L中是否还有括号*/
void tackle_2(LinkStackNode * top, LinkStackNode * top1, LinkQueue * Q, char L[])
{
 	int i = 0, j;
 	char * a;
 	char first;
 	a = (char *)malloc(sizeof(char));
 
	InitStack(&top);
	
 	InitQueue(Q);   //辅助用来输出括号内的内容
 	InitStack(&top1);
 
    	while(L[i] != ')')
 	{
  		Push(top, L[i]);//当不等于‘）’时，往top栈中压。
  		i++;
	}
	if(L[i] == ')') //当等于'）'时
 	{
  		j = i;
  		while(L[j] != '(')
  		{
		   	j--;
   			if(L[j] == '(')
   			{
   				j++;
				break;
    			}
  		}
  		first = L[j];//找到当前‘（ ’内的第一个字符   
  		for(;j < i - 1; j++)
  		{
    			EnterQueue(Q, first);
   			Pop(top, a);
      			EnterQueue(Q, *a);
  		}
  		Pop(top, a);        //这个是括号内的第一个字符
  		EnterQueue(Q, *a);
  		Pop(top, a);    //把‘（’删掉
  		while(Q -> front -> next != NULL)
  		{
			 DeleteQueue(Q,a);
			 Push(top,*a);
   		}
    	}         //if
 	i++;     //跳过‘）’
 	while(L[i] != '\0')
 	{
	   	Push(top, L[i]);
  		i++;
 	}
 	while(top -> next != NULL)
 	{
 		Pop(top, a);
  		Push(top1, *a);
 	}

 	i = 0;
 	while(top1 -> next != NULL)
 	{
  		Pop(top1,a);
 		L[i] = *a;
  		i++;
 	}
	L[i] = '\0';

 	i = 0;
	j = 0;
 	while(L[i] != '\0') 
 	{
  		i++;
  		if(L[i] == '(' || L[i] == ')')
   			j++;
 	}
 	if(j == 0) 
  		return;
 	else
	   	tackle_2(top, top1, Q, L);
}

//实现提示:处理规则形式(1)问题并没有规定具体规则的数量.比如增加一条具体规则C→eB那么对魔王语言就要多处理一次,因此处理规则形式(1)时需要递归处理,当语言中没有大写字母后递归结束。
void tackle_1(char L[], LinkQueue * Q)
{
 	int i = 0, j, z;
 	int k = 0;
 	char * a;
 	a = (char *)malloc(sizeof(char));
 	while(L[i] != '\0')
 	{    
		z = 0;
  		for(j = 0; j < 20; j++)
  		{    
   			if(L[i] == rule[j].rA[0])
   			{
				k = 0;
        			z = 1;
    				while(rule[j].ra[k] != '\0')
               		 	{
                    			EnterQueue(Q, rule[j].ra[k]);
       					k++;
                		}
   			}
  		}
        	if(z == 1)
  		{
   			i++;
  		}
  		else 
  		{
   			EnterQueue(Q, L[i]);
   			i++;
  		} 
 	}
    	i = 0;
 	while(Q -> front -> next != NULL)
 	{
  		DeleteQueue(Q, a);
  		L[i] = (*a);
  		i++;
 	}
 	L[i] = '\0';
    	i = 0;
 	j = 0;
 	while(L[i] != '\0') 
 	{
  		if(L[i] >= 'A' && L[i] <= 'Z')
   		j++;
  		i++;
 	}
 	if(j == 0)
		return;
 	else 
		tackle_1(L, Q);////递归调用
}

void tackle_12(char L[])
{
 	int i = 0;
 	int j;
 	while(L[i] != '\0')
 	{
  		for(j = 0; j < 20; j++)
  		{
   			if(L[i] == mean[j].mA[0])
    				printf("%s", mean[j].ma);
  		}
  		i++;
 	}
 	printf("\n");
}

int main(int argc, char *argv[])
{   
	 int j = 0;
	 read_rulefile();
	 read_meanfile();
	 char L[L_size]; //魔王说的话
	 printf("(规则文件位于源码目录,请自行修改)\n请输入魔王语言，格式如下\n魔王语言+(人类语言)当人类语言为空时括号里输入# :\n如 B(ehnAgz)B\n");
	 scanf("%s", L);

	 LinkStackNode * top, * top1;
	 top = (LinkStackNode * )malloc(sizeof(LinkStackNode));
	 top1 = (LinkStackNode * )malloc(sizeof(LinkStackNode));
	 
	 LinkQueue *Q;
	 Q = (LinkQueue *)malloc(sizeof(LinkQueue));
	 
	 tackle_2(top, top1, Q, L);
	 printf("\n魔王语言为：\n%s\n", L);

	 tackle_1(L, Q);
	 printf("\n魔王语言翻译后：\n%s\n", L);
	 printf("\n魔王语言的意思：\n");
	 tackle_12(L);	 
	 save_result(L);
     
	 return EXIT_SUCCESS;
}