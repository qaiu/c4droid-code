#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

const  char *d1 = "+-/*",*d2="1234567890.";
char s[MAX];
char s1[MAX];
typedef struct
{
	double x[MAX];
	char *f[MAX];
	int m;
	int n;
}FJ;

FJ fenjie(char *c)
{
	FJ fj;
	char *p1,*p2;
	int i=0;
	strcpy(s1,c);
    fj.x[i]=atof(strtok(c, d1));
    while ((p1 = strtok(NULL, d1)))
    {
    	i++;
        fj.x[i]=atof(p1);
   }
   fj.m=i;
   i=0;
   p2=strtok(s1,d2);
   fj.f[i]=p2;
   while ((p2 = strtok(NULL, d2)))
    {
    	i++;
    	fj.f[i]=p2;
    }
    fj.n=i;
    
    return fj;
}

double js(FJ fj)
{
	int i=0,k=0;
	double add=0;
	
	if (fj.m==fj.n)
	k=-1;
	for (i=0;i<=fj.n;i++)
	{
		if (fj.f[i][1]=='-')
		fj.x[i+k+1]=-fj.x[i+k+1];
		if (fj.f[i][0]=='*')
		{
			if (fj.f[i+1][0]=='*'||fj.f[i+1][0]=='/')
			{
				fj.x[i+k+1]*=fj.x[i+k];
				fj.x[i+k]=0;
			}
			else
			{
				fj.x[i+k]*=fj.x[i+k+1];
				fj.x[i+k+1]=0;
			}
		}
		if (fj.f[i][0]=='/')
		{
			if (fj.f[i+1][0]=='/'||fj.f[i+1][0]=='*')
			{
				fj.x[i+k+1]=fj.x[i+k]/fj.x[i+k+1];
				fj.x[i+k]=0;
			}
			else
			{
				fj.x[i+k]/=fj.x[i+k+1];
				fj.x[i+k+1]=0;
			}
		}
		if (fj.f[i][0]=='-')
		fj.x[i+k+1]=-fj.x[i+k+1];
	}
	 for (i=0;i<=fj.m;i++)	
    add+=fj.x[i];
    return add;
}

int main()
{
	printf("请输入算式：\n");
	while(1)
	{
		scanf("%s",s);
		if (strchr(s,'+')||strchr(s,'-')||strchr(s,'*')||strchr(s,'/'))
		printf("=%lf\n",js(fenjie(s)));
		else
		printf("=%lf\n",atof(s));
	}
	return 0;
}