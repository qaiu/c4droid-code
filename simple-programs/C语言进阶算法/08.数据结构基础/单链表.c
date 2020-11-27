#include <stdio.h>
#include <stdlib.h>
/* 单链表的最大长度 */
#define MAXSIZE 100
/* 单链表结点类型定义 */
/* 定义elemtype为int类型 */
typedef int elemtype;
/* 单链表的结点类型 */
typedef struct link_node{
  elemtype elem;
  struct link_node *next;
}list,*linklist;
/* 1、初始化单链表
* 参数: 链表(L)。
* 返回值: 成功返回1,否则返回0
*/
int initList(linklist *L)
{
  /* 头结点申请内存 */
  (*L)=(linklist)malloc(sizeof(list));
  /* 判断有无申请到空间 */
  if(!(*L))
  {
    /* 没有申请到内存返回0 */
    return 0;
  }
  (*L)->next=NULL;
  (*L)->elem=0;
  return 1;
}
/* 2、清空单链表
* 参数: 链表(L)。
* 返回值: 成功返回1
*/
int clearList(linklist *L)
{
  linklist p,q;
  if((*L) == NULL || (*L)->next!=NULL)
    return 1;
  p=(*L)->next;
  /* 当p非空时，删除p */
  while(p)
  {
    q=p;
    p=p->next;
    free(q);
  }
  /* 只剩头指针，所以L->next=NULL */
  (*L)->next=NULL;
  /* 清空后链表中元素为0 */
  (*L)->elem=0;
  return 1;
}
/* 3、求链表长度。
* 参数: 链表(L)。
* 返回链表的长度
*/
int getLength(linklist L)
{
  linklist p;
  int len=0;
  if(L==NULL||L->next==NULL)
    return 0;
  p=L->next;
  while(p)
  {
    /* 统计链表中元素 */
    len++;
    p=p->next;
  }
  return len;
}
/* */
/* 4、判断链表是否为空。
* 参数: 链表(L)。
* 返回值: 为空返回1,否则返回0
*/
int isEmpty(linklist L)
{
  if(L==NULL||L->next==NULL)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
/* 5、判断链表是否为满。
* 参数: 链表(L)。
* 返回值: 为满时返回1,否则返回0
*/
int isFull(linklist L)
{
  if(getLength(L)<=MAXSIZE)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
/* 6、遍历链表,输出每个节点的elem值
* 参数: 链表(L)
*/
void printList(linklist L)
{
  linklist p;
  if(L==NULL||L->next==NULL)
    return ;
  p=L->next;
  /* 链表为空 */
  if(isEmpty(L)==1)//当链表为空时则输出链表为空
  {
    printf("链表为空! \n");
  }
  /* 链表不为空 */
  while(p)
  {
    printf("%d  ",p->elem);
    p=p->next;
  }
  printf("\n");
}
/* 7、定位元素在表中的位置
* 参数: 链表(L)
*       给定元素(elem)
* 返回值: 表中有给定元素i则返回元素的位置,没有则返回0
*/
int locatElem(linklist L,int e)
{
  linklist p;
  int loc=0;
  if(L==NULL||L->next==NULL)
    return 0;
  p=L->next; 
  while(p)
  {
    loc++;
    /* 判断有无元素e */
    if(p->elem==e)
    {
      /* 有返回其位置 */
      return loc;
    }
    p=p->next;
  }
  /* 没有则返回0 */
  return 0;
}
/* 8、向链表插入元素
* 参数: 链表(L)
*       位置(loc)
*       元素(e)。
* 返回值: 成功返回1,否则返回0
*/
int insertElem(linklist L,int loc,elemtype e)
{
  linklist p,s;
  int j = 0;
  if(L==NULL)
    initList(&L);
  p = L;
  while(p&&j<loc-1)
  {
    p = p->next;
    j++;
  }
  /* 不符合条件返回0 */
  if(j>loc-1||!p)
  {
    return 0;
  }
  /* 给节点s分配内存 */
  s=(linklist)malloc(sizeof(list));
  s->elem=e;
  s->next=p->next;
  p->next=s;
  /* 头结点的elem加1，作为链表的长度*/
  L->elem++;
  return 1;
}
/* 9、在指定位置删除元素
* 参数: 链表(linklist?L)
*       位置(loc)
* 返回值: 成功返回1,否则返回0
*/
int delElem(linklist L,int loc)
{
  linklist p,q;
  int j=0;
  p=L;
  /* 查找第loc个元素的前驱 */
  while(p->next&&j<loc-1)
  {
    p=p->next;
    j++;
  }
  /* 没找到返回0 */
  if(j>loc-1||!(p->next))
  {
    return 0;
  }
  q=p->next;
  /* 删除操作 */
  p->next=q->next;
  free(q);
  /* 头结点的elem(链表的长度)减1 */
  L->elem--;
  return 1;
}
/* 显示单链表操作功能 */
void singleLink(void)
{
  printf("\n");
  printf("      单链表操作\n");
  printf("------------------------------\n");
  printf("      1 链表初始化\n");
  printf("      2 清空链表\n");
  printf("      3 求链表长度\n");
  printf("      4 链表是否为空\n");
  printf("      5 检查链表是否为满\n");
  printf("      6 遍历链表\n");
  printf("      7 从链表中查找元素\n");
  printf("      8 向链表中插入元素\n");
  printf("      9 从链表中删除元素\n");
  printf("      其他键退出\n");
  printf("------------------------------\n");
  printf("请选择要进行操作的序号(1--9):\n\n");
}
int main(void)
{
  int choice,len,e,loc;
  linklist L = NULL;
  /* 显示主界面 */
  singleLink();
  scanf("%d",&choice);
  do
  {
    switch(choice)
    {
      case 1://初始化
        if(initList(&L)==1)
          printf("初始化成功! \n");
        else
          printf("初始化失败! \n");
        break;
      case 2://链表置空
        if(clearList(&L)==1)
          printf("链表已清空! \n");
        else
          printf("链表清空失败! \n");
        break;   
      case 3://链表的长度
        len=getLength(L);
        printf("链表的长度为: %d\n",len);
        break;
      case 4://判断链表是否空
        if(isEmpty(L)==1)
          printf("链表为空! \n");
        else
          printf("链表不空! \n");
        break;
      case 5://判断链表是否满
        if(isFull(L)==1)
          printf("链表已满! \n");
        else
          printf("链表不满! \n");
        break;
      case 6://遍历链表
        printList(L);
        break;
      case 7://输出链表要查找元素的位置
        printf("您要查找的元素为: \n");
        scanf("%d",&e);
        if(locatElem(L,e)==0)
          printf("没有您要查找的元素:%d\n",e);
        else
          printf("您查找的元素[%d]在第[%d]个位置\n",e,locatElem(L,e));
        break;
      case 8:
        printf("输入你要插入的位置和元素: \n");
        scanf("%d%d",&loc,&e);
        if(loc<=0||loc>getLength(L)+1)
        {
          printf("插入位置错误! \n");
          break;
        }
        insertElem(L,loc,e);
        break;
      case 9:
        if(getLength(L)==0)
        {
          printf("链表为空无法删除! \n");
          break;
        }
        printf("输入你要删除元素的位置: \n");
        scanf("%d",&loc);
        if(loc<=0||loc>getLength(L))
        {
          printf("输入错误! \n");
          break;
        }
        delElem(L,loc);
        break;
      default:
        break;
    }
    singleLink();///显示主界面
    scanf("%d",&choice);//输入要进行操作的序号
  }while(choice>0&&choice<=9);///对进行输入的数进行判断(不在0—9则程序结束)
}