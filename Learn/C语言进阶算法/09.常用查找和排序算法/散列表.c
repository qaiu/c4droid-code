#include <stdio.h>  
#include <stdlib.h> 
#define MAXSIZE 17  
#define NULLKEY -32768  
/* 关键字结点 */
typedef struct node{
  /* 关键字 */
  int data;
  /* 指向下一个结点 */
  struct node *next;
}Node;  
typedef struct Table{ 
  /* 动态分配哈希表 */
  Node *table;
  /* 哈希表的关键字个数 */
  int count; 
}HashTable; 
/* 初始化哈希表 */
void InitHashTable(HashTable *H)
{  
  /* 初始化哈希表的大小 */
  H->count = MAXSIZE; 
  /* 创建哈希表 */
  H->table = (Node *)malloc(sizeof(Node) * H->count);
  /* 初始化哈希表的数据 */
  for(int i = 0;i < H->count;++i)
  {  
    H->table[i].data = NULLKEY;  
    H->table[i].next = NULL;  
  }  
}
/* 哈希表函数 */
int Hash(int key)
{  
  return key % MAXSIZE;  
}
/* 将关键字key插入哈希表 */
void InsertHashTable(HashTable *H,int key)
{  
  int addr = Hash(key);
  /* 如果为真，创建单链表 */
  if(H->table[addr].data != key && H->table[addr].data != NULLKEY)
  {  
    Node *temp = (Node *)malloc(sizeof(Node));  
    temp->next = H->table[addr].next;  
    temp->data = key;  
    H->table[addr].next = temp;  
  }  
  /* 则否直接填入 */
  else if(H->table[addr].data == NULLKEY)
    H->table[addr].data = key;  
}  
/* 查找关键字key的值 */
int SearchHashTable(HashTable *H,int key)
{  
  int addr = Hash(key);  
  if(H->table[addr].data == key)  
    return 1;  
  Node *p = H->table[addr].next;
  /* 查找单链表 */
  while(p != NULL)
  {  
    if(p->data == key)  
      return 1;  
    else  
      p = p->next;  
  }  
  return 0;  
}
int main()  
{  
  HashTable H;  
  InitHashTable(&H);  
  for(int i = 0;i < MAXSIZE * 2;i += 2)  
    InsertHashTable(&H,i);  
  printf("请输入要查找的内容:\n");  
  int key;  
  scanf("%d",&key);  
  if(SearchHashTable(&H,key))  
    printf("在哈希表中找到关键字:%d\n",key);  
  else  
    printf("在哈希表中未找到关键字:%d\n",key);
  return 0;  
}