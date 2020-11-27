#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
/* 线性表的双向链表存储结构 */
typedef struct DuLNode
{
  /* 数据域 */
  ElemType data;
  /* 前驱后继指针 */
  struct DuLNode *prior,*next;
}DuLNode,*DuLinkList;
/* 产生空的双向循环链表L */
int InitList(DuLinkList *L)
{
  /* L指向头结点 */
  *L=(DuLinkList)malloc(sizeof(DuLNode));
  if(*L)
  {
    /* 将头结点的前驱后继都指向头结点，这样构成了一个空表 */
    (*L)->next=(*L)->prior=*L;
    return 1;
  }
  else
    return 0;
}
/* 销毁双向循环链表L */
int DestroyList(DuLinkList *L)
{
  /* p指向第一个结点 */
  DuLinkList q,p=(*L)->next;
  /* p没到表头 */
  while(p!=*L)
  {
    q=p->next;
    free(p);
    p=q;
  }
  free(*L);
  *L=NULL;
  return 1;
}
/* 将L重置为空表 */
int ClearList(DuLinkList L)
{
  /* p指向第一个结点 */
  DuLinkList q,p=L->next;
  /* p没到表头 */
  while(p!=L)
  {
    q=p->next;
    free(p);
    p=q;
  }
  /* 头结点的两个指针域均指向自身，构成空表 */
  L->next=L->prior=L;
  return 1;
}
/* 若L为空表（空表就是头结点的前驱后继都指向头结点），则返回1，否则返回0 */
int ListEmpty(DuLinkList L)
{
  if(L->next==L&&L->prior==L)
    return 1;
  else
    return 0;
}
/* 返回L中数据元素个数 */
int ListLength(DuLinkList L)
{
  int i=0;
  /* p指向第一个结点 */
  DuLinkList p=L->next;
  /* p没到表头 */    
  while(p!=L)
  {
    i++;
    p=p->next;
  }
  return i;
}
/* 当第i个元素存在时,其值赋给e并返回1,否则返回0 */
int GetElem(DuLinkList L,int i,ElemType *e)
{
  /* j为计数器 */
  int j=1;
  /* p指向第一个结点 */
  DuLinkList p=L->next;
  /* 顺指针向后查找,直到p指向第i个元素或p指向头结点 */
  while(p!=L&&j<i)
  {
    p=p->next;
    j++;
  }
  /* 第i个元素不存在 */
  if(p==L||j>i)
    return 0;
  /* 取第i个元素 */
  *e=p->data;
  return 1;
}
/* 返回L中第1个与e满足关系compare()的数据元素的位序。 
 * 若这样的数据元素不存在，则返回值为0 */
int LocateElem(DuLinkList L,ElemType e,int(*compare)(ElemType,ElemType))
{
  int i=0;
  /* p指向第1个元素 */
  DuLinkList p=L->next;
  while(p!=L)
  {
    i++;
    /* 找到这样的数据元素 */
    if(compare(p->data,e))
      return i;
    p=p->next;
  }
  return 0;
}
/* 若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱 */
int PriorElem(DuLinkList L,ElemType cur_e,ElemType *pre_e)
{    
  /* p指向第2个元素 */
  DuLinkList p=L->next->next;
  /* p没到表头 */    
  while(p!=L)
  {
    if(p->data==cur_e)
    {
      *pre_e=p->prior->data;
      return 1;
    }
    p=p->next;
  }
  return 0;
}
/* 若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继 */
int NextElem(DuLinkList L,ElemType cur_e,ElemType *next_e)
{    
  /* p指向第2个元素 */
  DuLinkList p=L->next->next;
  /* p没到表头 */
  while(p!=L)
  {
    if(p->prior->data==cur_e)
    {
      *next_e=p->data;
      return 1;
    }
    p=p->next;
  }
  return 0;
}
/* 在双向链表L中返回第i个元素的位置指针  */
DuLinkList GetElemP(DuLinkList L,int i)
{
  int j;
  DuLinkList p=L;
  for(j=1;j<=i;j++)
    p=p->next;
  return p;
}
/* 在带头结点的双链循环线性表L中第i个位置之前插入元素e，
// i的合法值为1≤i≤表长+1 */
int ListInsert(DuLinkList L,int i,ElemType e)
{
  DuLinkList p,s;
  /* i值不合法 */
  if(i<1||i>ListLength(L)+1)
    return 0;
  /* 在L中确定第i-1个元素的位置指针p */
  p=GetElemP(L,i-1);
  /* p=NULL,即第i-1个元素不存在 */
  if(!p)
    return 0;
  s=(DuLinkList)malloc(sizeof(DuLNode));
  if(!s)
    return 0;
  /* 在第i-1个元素之后插入 */
  s->data=e;
  s->prior=p;
  s->next=p->next;
  p->next->prior=s;
  p->next=s;
  return 1;
}
/* 删除带头结点的双链循环线性表L的第i个元素,i的合法值为1≤i≤表长+1 */
int ListDelete(DuLinkList L,int i,ElemType *e) 
{
  DuLinkList p;
  /* i值不合法 */
  if(i<1||i>ListLength(L))
    return 0;
  /* 在L中确定第i个元素的位置指针p */
  p=GetElemP(L,i); 
  /* p=NULL,即第i个元素不存在 */
  if(!p)
    return 0;
  *e=p->data;
  p->prior->next=p->next;
  p->next->prior=p->prior;
  free(p);
  return 1;
}
/* 由双链循环线性表L的头结点出发,正序对每个数据元素调用函数visit() */
void ListTraverse(DuLinkList L,void(*visit)(ElemType))
{
  /* p指向头结点 */
  DuLinkList p=L->next;
  while(p!=L)
  {
    visit(p->data);
    p=p->next;
  }
  printf("\n");
}
/* 由双链循环线性表L的头结点出发,逆序对每个数据元素调用函数visit() */
void ListTraverseBack(DuLinkList L,void(*visit)(ElemType))
{
  /* p指向尾结点 */
  DuLinkList p=L->prior;
  while(p!=L)
  {
    visit(p->data);
    p=p->prior;
  }
  printf("\n");
}
/* 数据元素判定函数(判定相等) */
int compare(ElemType c1,ElemType c2) 
{
  if(c1==c2)
    return 1;
  else
  return 0;
}
/* ListTraverse()调用的函数(类型一致) */
void vd(ElemType c)
{
  printf("%d ",c);
}
int main()
{
  DuLinkList L;
  int i,n;
  int j;
  ElemType e;
  InitList(&L);
  for(i=1;i<=5;i++)
  {
    /* 在第i个结点之前插入i */
    ListInsert(L,i,i);
  }
          
  printf("正序输出链表：");
  /* 正序输出 */
  ListTraverse(L,vd);
  printf("逆序输出链表：");
  /* 逆序输出 */
  ListTraverseBack(L,vd);
  n=2;
  /* 删除并释放第n个结点 */
  ListDelete(L,n,&e);
  printf("删除第%d个结点，值为%d，其余结点为：",n,e);
  /* 正序输出 */
  ListTraverse(L,vd);
  printf("链表的元素个数为%d\n",ListLength(L));
  printf("链表是否空：%d(1:是 0:否)\n",ListEmpty(L));
  /* 清空链表 */
  ClearList(L);
  printf("清空后，链表是否空：%d(1:是 0:否)\n",ListEmpty(L));
  for(i=1;i<=5;i++)
  {
    /* 重新插入5个结点 */
    ListInsert(L,i,i);
  }
  /* 正序输出 */
  ListTraverse(L,vd);
  n=3;
  /* 将链表的第n个元素赋值给e  */
  j=GetElem(L,n,&e);
  if(j)
    printf("链表的第%d个元素值为%d\n",n,e);
  else
    printf("不存在第%d个元素\n",n);
  n=4;
  i=LocateElem(L,n,compare);
  if(i)
    printf("等于%d的元素是第%d个\n",n,i);
  else
    printf("没有等于%d的元素\n",n);
  j=PriorElem(L,n,&e);
  if(j)
    printf("%d的前驱是%d\n",n,e);
  else
    printf("不存在%d的前驱\n",n);
  j=NextElem(L,n,&e);
  if(j)
    printf("%d的后继是%d\n",n,e);
  else
    printf("不存在%d的后继\n",n);
  DestroyList(&L);
  system("pause");
  return 0;
}