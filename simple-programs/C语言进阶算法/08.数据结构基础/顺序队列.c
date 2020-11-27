#include <stdio.h>
#include <stdlib.h>
typedef int elemType;
struct sequeue{
  /* 指向存储队列的数组空间 */
  elemType *queue;
  /* 队首指针（下标），队尾指针（下标），队列长度变量 */
  int front, rear, len;
  /* queue数组长度 */
  int maxSize;
};
void againMalloc(struct sequeue *q)
{
  /* 空间扩展为原来的2倍，原内容被自动拷贝到p所指向的存储空间中 */
  elemType *p;
  p = (elemType *)realloc(q->queue, 2 * q->maxSize * sizeof(elemType));
  /* 动态存储空间分配，若失败则退出运行 */
  if(!p){
    printf("空间分配失败！ ");
    exit(1);
  }
  /* 使queue指向新的队列空间 */
  q->queue = p;
  /* 把原队列的尾部内容后移maxSize个位置 */
  if(q->rear != q->maxSize -1){
    int i;
    for(i = 0; i <= q->rear; i++){
      q->queue[i+q->maxSize] = q->queue[i];
    }
    /* 队尾指针后移maxSize个位置 */
    q->rear += q->maxSize;
  }
  /* 把队列空间大小修改为新的长度 */
  q->maxSize = 2 * q->maxSize;
  return;
}
/* 1.初始化队列 */
void initQueue(struct sequeue *q, int ms)
{
  /* 检查ms是否有效，若无效则退出运行 */
  if(ms <= 0){
    printf("ms值非法! ");
    exit(1);
  }
  /* 置队列空间大小为ms */
  q->maxSize = ms;
  /* 动态存储空间分配，若失败则退出运行 */
  q->queue = (elemType *)malloc(ms * sizeof(elemType));
  if(!q->queue){
    printf("内存空间分配失败！ ");
    exit(1);
  }
  /* 初始置队列为空 */
  q->front = q->rear = 0;
  return;
}
/* 2.向队列中插入元素x */
void enQueue(struct sequeue *q, elemType x)
{
  /* 当队列满时进行动态生分配 */
  if((q->rear + 1) % q->maxSize == q->front){
    againMalloc(q);
  }
  /* 求出队尾的下一个位置 */
  q->rear = (q->rear + 1) % q->maxSize;
  /* 把x的值赋给新的队尾 */
  q->queue[q->rear] = x;
  return;
}
/* 3.从队列中删除元素并返回 */
elemType outQueue(struct sequeue *q)
{
  /* 若队列为空则终止运行 */
  if(q->front == q->rear){
    printf("队列为空，无法删除！ ");
    exit(1);
  }
  /* 使队首指针指向下一个位置 */
  q->front = (q->front +1) % q->maxSize;
  /* 返回队首元素 */
  return q->queue[q->front];
}
/* 4.读取队首元素，不改变队列状态 */
elemType peekQueue(struct sequeue *q)
{
  /* 若队列为空则终止运行 */
  if(q->front == q->rear){
    printf("队列为空，无法删除！ ");
    exit(1);
  }
  /* 队首元素是队首指针的下一个位置中的元素 */
  return q->queue[(q->front +1) % q->maxSize];
}
/* 5.检查一个队列是否为空，若是则返回1,否则返回0 */
int emptyQueue(struct sequeue *q)
{
  if(q->front == q->rear){
    return 1;
  }else{
    return 0;
  }
}
/* 6.清除一个队列，并释放动态存储空间 */
void clearQueue(struct sequeue *q)
{
  if(q->queue != NULL){
    free(q->queue);
    /* 设置队列空间指针为空 */
    q->queue = NULL;
    /* 设置队列为空 */
    q->front = q->rear = 0;
    /* 设置队列大小为0 */
    q->maxSize = 0;
  }
  return;
}
int main(int argc, char* argv[])
{
  struct sequeue q;
  int a[8] = {3, 8, 5, 17, 9, 30, 15, 22};
  int i;
  initQueue(&q, 5);
  for(i = 0; i < 8; i++){
    enQueue(&q, a[i]);
  }
  printf("%d ", outQueue(&q));
  printf("%d  ", outQueue(&q));
  enQueue(&q, 68);
  printf("%d ", peekQueue(&q));
  printf("%d  ", outQueue(&q));
  while(!emptyQueue(&q)){
    printf("%d ", outQueue(&q));
  }
  printf(" ");
  clearQueue(&q);
  return 0;
}