#include <stdio.h> 
#include <stdlib.h>
/* 无限大 */
#define INFINITY 65535
/* 存储单元的最大值 */
#define MAXSIZE 1000
/* 栈的数据类型 */
typedef int ElemType;
typedef struct {  
  /* 存储单元 */
  ElemType data[MAXSIZE];
  int top1;  
  int top2;  
}DoubleStack; 
/* 初始化两栈 */
void InitStack(DoubleStack *s)
{  
  s->top1 = -1;  
  s->top2 = MAXSIZE;  
}  
/* 将数据元素e压栈,i为栈的序号 */
void Push(DoubleStack *s,int i,ElemType e)
{  
  if(i == 1)  
  {  
    if(s->top1 + 1 == s->top2)  
      return;  
    s->data[++s->top1] = e;  
  }  
  else if(i == 2)  
  {  
    if(s->top2 - 1 == s->top1)  
      return;  
    s->data[--s->top2] = e;  
  }  
}  
/* i为栈的序号,返回栈顶的元素 */
ElemType Top(DoubleStack *s,int i)
{  
  if(i == 1)  
  {  
    if(s->top1 == -1)  
      return INFINITY;  
    return s->data[s->top1];  
  }  
  else if(i == 2)  
  {  
    if(s->top2 == MAXSIZE)  
      return INFINITY;  
    return s->data[s->top2];  
  }  
}  
/* i为栈的序号,返回并删除栈顶的元素 */
ElemType Pop(DoubleStack *s,int i)
{  
  if(i == 1)  
  {  
    if(s->top1 == -1)  
      return INFINITY;  
    return s->data[s->top1--];  
  }  
  else if(i == 2)  
  {  
    if(s->top2 == MAXSIZE)  
      return INFINITY;  
    return s->data[s->top2++];  
  }  
}  
/* 判断栈是否为空 */
int IsEmpty(DoubleStack *s)
{  
  if(s->top1 == -1 && s->top2 == MAXSIZE)  
    return 1;  
  return 0;  
}  
/* 清空栈 */
void Clear(DoubleStack *s)
{  
  s->top1 = -1;  
  s->top2 = MAXSIZE;  
}  
/* 打印栈底到栈顶的元素,i为栈的序列 */
void Print(DoubleStack *s,int i)
{  
  if(i == 1)  
  {  
    for(int j = 0;j <= s->top1;++j)  
      printf("%d ",s->data[j]);  
    printf("\n");  
  }  
  else if(i == 2)  
  {  
    for(int j = MAXSIZE - 1;j >= s->top2;--j)  
      printf("%d ",s->data[j]);  
    printf("\n");  
  }  
} 
int main()  
{  
  DoubleStack s;  
  InitStack(&s);  
  int i;  
  for(i = 1;i < 20;i += 2)
  {
    Push(&s,1,i);
    Push(&s,2,i+1);
  }
      
  Print(&s,1);  
  Print(&s,2);  
  int top1 = Top(&s,1);  
  printf("第一个栈顶的元素:%d\n",top1);  
  int top2 = Top(&s,2);  
  printf("第二个栈顶的元素:%d\n",top2); 
  return 0;  
}