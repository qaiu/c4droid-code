#include<stdio.h>
#include<iostream>
using namespace std;
int main(void)
{       //本程序每四位数输出结果，如果请求计算的位数不是4的整数倍，最后输出可能会少1~3位
	long a[2]={956,80},b[2]={57121,25},i=0,j,k,p,q,r,s=2,t,u,v,N,M=10000;
	printf("%9cMachin%6cpi=16arctan(1/5)-4arctan(1/239)\nPlease input a number.\n",32,32);
	cin>>N,N=N/4+3;
	long *pi=new long[N],*e=new long[N];
	while(i<N)pi[i++]=0;
	while(--s+1)
	{
		for(*e=a[k=s],i=N;--i;)e[i]=0;
		for(q=1;j=i-1,i<N;e[i]?0:++i,q+=2,k=!k)
			for(r=v=0;++j<N;pi[j]+=k?u:-u)u=(t=v*M+(e[j]=(p=r*M+e[j])/b[s]))/q,r=p%b[s],v=t%q;
	}
	while(--i)(pi[i]=(t=pi[i]+s)%M)<0?pi[i]+=M,s=t/M-1:s=t/M;
	for(cout<<"3.";++i<N-2;)printf("%04ld",pi[i]);
	delete []pi,delete []e,cin.ignore(),cin.ignore();
	return 0;
}