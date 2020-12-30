#include <stdio.h> 
#include <stdlib.h>
 
#define SIZE 100 
 
/* 定义二叉树节点结构 */
typedef struct BiTNode
{
  /* 数据域 */
  char data;
  /* 左右孩子指针域 */
  struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
 
 
//建立二叉树
void CreateBiTree(BiTree T)
{
  char ch;
 
  /* 读入一个字符 */
  scanf("%c",&ch);
  if(ch==' ')
    T=NULL;
  else
  {
    /* 生成一个新结点 */
    T=(BiTNode *)malloc(sizeof(BiTNode));
    T->data=ch;
    /* 生成左子树 */
    CreateBiTree(T->lchild);
    /* 生成右子树 */
    CreateBiTree(T->rchild);
  }
}
 
/* 先序遍历的递归 */
void PreOrder(BiTree T)
{
  if(T)
  {
    /* 访问结点 */
    printf("%c ",T->data);
    /* 遍历左子树 */
    PreOrder(T->lchild);
    /* 遍历右子树 */
    PreOrder(T->rchild);
  }
}
 
/* 中序遍历的递归 */
void InOrder(BiTree T)
{
  if(T)
  {
    /* 遍历左子树 */
    InOrder(T->lchild);
    /* 访问结点 */
    printf("%c ",T->data);
    /* 遍历右子树 */
    InOrder(T->rchild);
  }
}
 
//后序遍历的递归
void PostOrder(BiTree T)
{
  if(T) {
    /* 遍历左子树 */
    PostOrder(T->lchild);
    /* 访问结点 */
    PostOrder(T->rchild);
    /* 遍历右子树 */
    printf("%c ",T->data);
  }
}
 
int visit(BiTree T)
{
  if(T)
  {
    printf("%c ",T->data);
    return 1;
  }
  else
    return 0;
}
 
//主函数
int main(void)
{
  BiTree T;
  char j;
  int flag=1;
  printf("请建立二叉树。\n");
  printf("建树将以三个空格后回车结束。\n");
  printf("例如:1 2 3 4 5 6   (回车)\n");
  /* 初始化队列 */
  CreateBiTree(T);
  getchar();
  while(flag)
  {
    printf("\n");
    printf("请选择:\n");
    printf("1.递归先序遍历\n");
    printf("2.递归中序遍历\n");
    printf("3.递归后序遍历\n");
    printf("0.退出程序\n");
    scanf("%c",&j);
    switch(j)
    {
    case '1':
      if(T) {
        printf("递归先序遍历二叉树:");
        PreOrder(T);
        printf("\n");
      }
      else
        printf("二叉树为空!\n");
      break;
    case '2':
      if(T){
        printf("递归中序遍历二叉树:");
        InOrder(T);
        printf("\n");
      }
      else
        printf("二叉树为空!\n");
      break;
    case '3':
      if(T) {
        printf("递归后序遍历二叉树:");
        PostOrder(T);
        printf("\n");
      }
      else
        printf("二叉树为空!\n");
      break;
    default:
      flag=0;
      printf("程序运行结束，按任意键退出!\n");
    }
  }
 
  return 0;
}