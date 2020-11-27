#include <stdio.h> 
#include <stdlib.h>
/* 表示无穷大 */
#define INFINITY 65535
/* 栈的最大空间 */
#define MAXSIZE 1000
/* 栈的数据类型 */
typedef int ElemType;
typedef struct{
  /* 栈的大小 */
  ElemType data[MAXSIZE];
  /* 栈顶的游标 */
  int top;
}Stack; 
/* 初始化栈 */
void InitStack(Stack *s)
{  
  s->top = -1;  
}
/* 判断栈是否为空 */
int IsEmpty(Stack *s)
{  
  if(s->top == -1)  
    return 1;  
  return 0;  
}
/* 返回栈顶的元素 */#include <stdio.h> 
#include <stdlib.h>
/* 表示无穷大 */
#define INFINITY 65535
/* 栈的最大空间 */
#define MAXSIZE 1000
/* 栈的数据类型 */
typedef int ElemType;
typedef struct{
  /* 栈的大小 */
  ElemType data[MAXSIZE];
  /* 栈顶的游标 */
  int top;
}Stack; 
/* 初始化栈 */
void InitStack(Stack *s)
{  
  s->top = -1;  
}
/* 判断栈是否为空 */
int IsEmpty(Stack *s)
{  
  if(s->top == -1)  
    return 1;  
  return 0;  
}
/* 返回栈顶的元素 */
ElemType Top(Stack *s)
{  
  if(!IsEmpty(s))  
    return s->data[s->top];  
  return INFINITY;  
}
/* 返回并删除栈顶的元素 */
ElemType Pop(Stack *s)
{  
  if(!IsEmpty(s))  
    return s->data[s->top--];  
  return INFINITY;  
}
/* 将元素压栈 */
void Push(Stack *s,ElemType e)
{  
  if(s->top >= MAXSIZE - 1)  
    return;  
  s->top++;  
  s->data[s->top] = e;  
}
/* 清空栈 */
void Clear(Stack *s)
{  
  s->top = -1;  
}
/* 打印栈底到栈顶的元素 */
void Print(Stack *s)
{  
  for(int i = 0;i <= s->top;++i)  
    printf("%d ",s->data[i]);  
  printf("\n");  
}
int main()  
{  
  Stack s;  
  InitStack(&s);  
  for(int i = 1;i < 20;i += 2)  
    Push(&s,i);  
  Print(&s);  
  printf("栈顶的元素为:\n");  
  int k = Top(&s);  
  printf("%d\n",k);  
  Clear(&s);  
  if(IsEmpty(&s))  
    printf("栈为空\n");  
  else  
    printf("栈不为空\n");
  system("pause");
  return 0;  
}
ElemType Top(Stack *s)
{  
  if(!IsEmpty(s))  
    return s->data[s->top];  
  return INFINITY;  
}
/* 返回并删除栈顶的元素 */
ElemType Pop(Stack *s)
{  
  if(!IsEmpty(s))  
    return s->data[s->top--];  
  return INFINITY;  
}
/* 将元素压栈 */
void Push(Stack *s,ElemType e)
{  
  if(s->top >= MAXSIZE - 1)  
    return;  
  s->top++;  
  s->data[s->top] = e;  
}
/* 清空栈 */
void Clear(Stack *s)
{  
  s->top = -1;  
}
/* 打印栈底到栈顶的元素 */
void Print(Stack *s)
{  
  for(int i = 0;i <= s->top;++i)  
    printf("%d ",s->data[i]);  
  printf("\n");  
}
int main()  
{  
  Stack s;  
  InitStack(&s);  
  for(int i = 1;i < 20;i += 2)  
    Push(&s,i);  
  Print(&s);  
  printf("栈顶的元素为:\n");  
  int k = Top(&s);  
  printf("%d\n",k);  
  Clear(&s);  
  if(IsEmpty(&s))  
    printf("栈为空\n");  
  else  
    printf("栈不为空\n");
  system("pause");
  return 0;  
}