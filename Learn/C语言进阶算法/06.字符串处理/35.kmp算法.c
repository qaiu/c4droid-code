/* KMP算法 */
#include <stdlib.h>

int next[20];
void get_nextval(char T[],int next[])
{
  int j,k;
  j=0;
  next[0] = -1;
  k=-1;
  while(T[j+1]!='\0')
  {
    if(k==-1||T[j]==T[k])
    {
      ++j,++k;
      if(T[j]!=T[k])
        next[j]=k;
      else
        next[j]=next[k];
    }
    else
      k=next[k];
  }
}

int Index_KMP(char S[],char T[],int pos)
{
  int i,j;
  i=pos-1;
  j=0;
  while(S[i]!='\0'&&T[j]!='\0')
  {
    if(j==-1||S[i]==T[j])
      i++,j++;
    else
      j=next[j];
  }
  if(T[j]=='\0')
    return (i-j);
  else
    return 0;
}

int main(void)
{
  int m;
  char S[20]="abcdfadabddfa";
  char T[20]="fa";
  get_nextval(T,next);
  m=Index_KMP(S,T,1);
  printf("%d\n",m);
  return 0;
}