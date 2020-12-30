/* 排列组合生成 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 100
int count;

/* 产生后调用的函数,传入a[]和n,a[0]..a[n-1]为一次产生的结果 */
void dummy(int* a,int n){
  int i;
  printf("%d: ",count++);
  for(i=0;i<n-1;i++)
    printf("%d ",a[i]);

  printf("%d \n",a[n-1]);
}

void _gen_perm(int* a,int n,int m,int l,int* temp,int* tag){
  int i;
  if (l==m)
    dummy(temp,m);
  else
    for (i=0;i<n;i++)
      if (!tag[i]){
        temp[l]=a[i],tag[i]=1;
        _gen_perm(a,n,m,l+1,temp,tag);
        tag[i]=0;
      }
}

/* 产生字典序排列P(n,m) */
void gen_perm(int n,int m){
  int a[MAXN],temp[MAXN],tag[MAXN]={0},i;
  for (i=0;i<n;i++)
    a[i]=i+1;
  _gen_perm(a,n,m,0,temp,tag);
}

void _gen_comb(int* a,int s,int e,int m,int* count,int* temp){
  int i;
  if (!m)
    dummy(temp,count);
  else
    for (i=s;i<=e-m+1;i++){
      temp[count++]=a[i];
      _gen_comb(a,i+1,e,m-1,count,temp);
      count--;
    }
}

/* 产生字典序组合C(n,m) */
void gen_comb(int n,int m){
  int a[MAXN],temp[MAXN],count=0,i;
  for (i=0;i<n;i++)
    a[i]=i+1;
  _gen_comb(a,0,n-1,m,count,temp);
}

void _gen_perm_swap(int* a,int n,int l,int* pos,int* dir){
  int i,p1,p2,t;
  if (l==n)
    dummy(a,n);
  else{
    _gen_perm_swap(a,n,l+1,pos,dir);
    for (i=0;i<l;i++){
      p2=(p1=pos[l])+dir[l];
      t=a[p1],a[p1]=a[p2],a[p2]=t;
      pos[a[p1]-1]=p1,pos[a[p2]-1]=p2;
      _gen_perm_swap(a,n,l+1,pos,dir);
    }
    dir[l]=-dir[l];
  }
}

/*gen_perm_swap产生相邻位对换全排列P(n,n)
 *产生元素用1..n表示
 */
void gen_perm_swap(int n){
  int a[MAXN],pos[MAXN],dir[MAXN],i;
  for (i=0;i<n;i++)
    a[i]=i+1,pos[i]=i,dir[i]=-1;
  _gen_perm_swap(a,n,0,pos,dir);
}


int main(void)
{
  int n = 5,m = 3;

  printf("p(5,3):\n");
  gen_perm(5,3);

  count = 0;
  printf("c(5,3):\n");
  gen_comb(5,3);

  count = 0;
  printf("p(5,5):\n");
  gen_perm_swap(5);

  return 0;
}