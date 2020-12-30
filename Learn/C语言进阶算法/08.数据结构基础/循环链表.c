#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define  OK 1
typedef int ElemType;
typedef struct Node
{
  ElemType data;
  struct Node *next;
}Node,*PNode;
typedef struct CList
{
  PNode first;
  PNode last;
  size_t size;
}CList;
void InitCList(CList *clist)//初始化
{
  (*clist).first=(*clist).last=(Node*)malloc(sizeof(Node));
  assert((*clist).first!=NULL);
  (*clist).last->next=(*clist).first;
  (*clist).size=0;
}
void PushFront(CList *clist,ElemType x)//头插
{
  Node *s=(Node*)malloc(sizeof(Node));
  assert (s!=NULL);
  s->data=x;
  s->next=(*clist).first->next;
  (*clist).first->next=s;
  if((*clist).first==(*clist).last)
  {
    (*clist).last=s;
  }
  (*clist).size++;
}
void PushBack(CList *clist,ElemType x)//尾插
{
  Node *s=(Node*)malloc(sizeof(Node));
  s->data=x;
  (*clist).last->next=s;
  s->next=(*clist).first;
  (*clist).last=s;
  (*clist).size++;
}
void ShowCList(CList clist)//显示链表
{
  if(clist.first->next==clist.last->next)
  {
    printf("show over\n");
  }
  else
  {
    Node *p=clist.first->next;
    while(p!=clist.first)
    {
      printf("%d-->",p->data);     
      p=p->next;
    }
    printf("show over\n");
  }
}
PNode FindByVal(CList clist,ElemType x)//按值查寻
{
  Node *p=clist.first->next;
  while(p!=clist.first&&p->data!=x)
  {
    p=p->next;
  }
  if(p==clist.first)
  {
    return NULL;
  }
  return p;
}
PNode FindByPos(CList clist,int pos )//按位置插入
{
  int i = 1;
  Node *p=clist.first->next;
  for (i=1;i<pos;i++)
  {
    p=p->next;
  }
  return p;
}
void DeleteBypos(CList *clist,int pos)//按位置删除
{
  PNode p=(*clist).first;
  int j=1;
  while( p&&j<pos)
  {
    p = p->next;
    ++j;
  }
  if(pos>(*clist).size)
  {
    printf("删除位置无效\n"); 
  }
  Node *q=p->next;
  p->next=p->next->next;
  if(p->next->next == (*clist).first)
  {
    (*clist).last = p;
  }
  free(q);
  (*clist).size--;
}
void InsertCListByVal(CList *clist,ElemType e)//按值插入
{
  Node *p=(*clist).first;
  while(p->next!=(*clist).first&&p->next->data<e)
  {
    p=p->next;
  }
  Node *s=(Node*)malloc(sizeof(Node));
  s->data=e;
  s->next=p->next;
  p->next=s;
  (*clist).size++;
}
void InsertCListByPos(CList *clist,int pos,ElemType x)//按位置插入
{
  int i = 1;
  Node *p=(*clist).first;
  for (i=1;i<pos;i++)
  {
    p=p->next;
  }
  Node *s=(Node*)malloc(sizeof(Node));
  s->data=x;
  s->next=p->next;
  p->next=s;
  (*clist).size++;
}
void MergeCList(CList *La,CList *Lb,CList *Lc)//合并
{
  InitCList(La);
  InitCList(Lb);
  InitCList(Lc);
  Node *p=(*La).first;
  (*La).last->next=(*Lb).first->next;
  (*Lb).last->next=(*La).first;
  (*Lc).size=(*La).size+(*Lb).size;
  free((*Lb).first);
}
int DestroyList( CList *clist)//销毁
{
  int i = 0;
  Node *p=(*clist).first->next;
  for(i=0;i<=(*clist).size;++i)
  {
    (*clist).first->next=p->next;
    free(p);
    p=(*clist).first->next;
  }
  return OK;
}
int ClearList( CList *clist)//清空
{
  (*clist).size=0;     
  return OK;
}
PNode FindNextElemByVal(CList clist,ElemType x)//查找后继
{
  Node *p=clist.first;
  while(p->next!=clist.first&&p->next->data!=x)
  {
    p=p->next;
  }
  p=p->next;
  if (p->next==clist.first)
  {
    printf("输入的前驱无效\n"); 
  }
  return p;
}
PNode FindPriorElemByVal(CList clist,ElemType x)//查找前驱
{
  Node *p=clist.first;
  while(p->next!=clist.first&&p->next->data!=x)
  {
    p=p->next;
  }
  if (p->next==clist.first->next)
  {
    printf("输入的后继无效\n"); 
  }
  if(p->next==clist.first)
  {
    return NULL;
  }
  return p;
}
void DeleteByVal(CList *clist,ElemType x)//按值删除
{
  PNode p=FindPriorElemByVal(*clist, x);//要删除元素的前驱
  if(p==NULL)
  {
    printf("删除元素无效\n"); 
    return;
  }
  Node *q=p->next;
  p->next=p->next->next;
  if(p->next->next == (*clist).first)
  {
    (*clist).last = p;
  }
  free(q);
  (*clist).size--;
}
int main(void)
{
  CList mylist,La,Lb;
  InitCList(&mylist);
  int select=1;
  int pos;
  int item;
  PNode res;
  printf("******************************\n");
  printf("* 1.输入数据H   2.输入数据B *\n");
  printf("* 3.显示链表    4.按值查询  *\n");
  printf("* 5.按位置查询  6.按值删除  *\n");
  printf("* 7.按位置删除  8.按值插入  *\n");
  printf("* 9.按位置插入  10.排序     *\n");
  printf("* 11.逆置       12.合并     *\n");
  printf("* 13.销毁链表   14.清除链表 *\n");
  printf("* 15.获取后继   16.获取前驱 *\n");
  printf("******************************\n");
  while (select!=0)
  {
    printf("请选择:>\n");
    scanf("%d",&select);
    switch(select)
    {
      case 1:
        printf("请输入数据，-1结束.\n");
        scanf("%d",&item);
        while(item != -1)
        {
          PushFront(&mylist,item);
          scanf("%d",&item);
        }    
        break;
      case 2:
        printf("请输入数据，-1结束.\n");
        scanf("%d",&item);
        while(item != -1)
        {
          PushBack(&mylist,item);
          scanf("%d",&item);
        }            
        break;
      case 3:
        ShowCList(mylist);
        break;
      case 4:
        printf("请输入要查询的值:>");
        scanf("%d",item);
        res = FindByVal(mylist,item);
        if(res)
        {
          printf("要查询的元素存在\n");
        }
        else
        {
          printf("要查询的元素不存在\n");
        }
        break;
      case 5:
        printf("请输入要查询位置:>");
        scanf("%d",&pos);
        res = FindByPos(mylist,pos);
        if(res)
        {
          printf("要查询的元素存在\n");
        }
        else
        {
          printf("要查询的元素不存在\n");
        }
        break;
      case 6:
        printf("请输入要删除的值:>");
        scanf("%d",&item);
        DeleteByVal(&mylist,item);
        break;
      case 7:
        printf("请输入要删除的位置");
        scanf("%d",&pos);
        DeleteBypos(&mylist, pos );
        break;
      case 8:
        printf("请输入要插的数据");
        scanf("%d",&item);
        InsertCListByVal(&mylist,item);
        break;
      case 9:
        printf("请输入要插入的位置:>");
        scanf("%d",&pos);
        printf("请输入要插入的值:>");
        scanf("%d",&item);
        InsertCListByPos(&mylist,pos,item);
        break;
      case 12:
        printf("请输入La的元素\n");
        scanf("%d",&item);
        while(item != -1)
        {
          PushFront(&mylist,item);
          scanf("%d",&item);
        }            
        printf("请输入La的元素\n");
        scanf("%d",&item);
        while(item != -1)
        {
          PushFront(&mylist,item);
          scanf("%d",&item);
        }            
        MergeCList(&La,&Lb,&mylist);
        break;
      case 13:
        DestroyList(&mylist);
        break;
      case 14:
        ClearList(&mylist);
        break;
      case 15:
        printf("请输入前驱\n");
        scanf("%d",&item);
        res=FindNextElemByVal(mylist,item);
        printf("后继是:%d\n",res->next->data);
        break;
      case 16:
        printf("请输入后继\n");
        scanf("%d",&item);
        res=FindPriorElemByVal(mylist,item);
        printf("前驱是:%d\n",res->data);
        break;
      default:
        break;
    }
  }
  return 0;
}