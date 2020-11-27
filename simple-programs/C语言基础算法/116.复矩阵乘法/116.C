/*
例程116. 复矩阵乘法
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/


#include "stdio.h"
#include <conio.h>
#define MAX 255
/*
复矩阵相乘
m是矩阵A的行数，n是矩阵B的行数，k是矩阵B的列数
ar,br,cr分别是矩阵A,B,C的实部，ai,bi,ci分别是矩阵A,B, C的虚部
*/

void CMatrixMul(double ar[],double ai[],double br[],double bi[],int m,int n,int k,double cr[],double ci[]) 
{
	int i,j,l,u,v,w;
    	double p,q,s;
    	for (i=0; i<=m-1; i++)
		for (j=0; j<=k-1; j++)
		{
			u=i*k+j;
        		cr[u]=0.0; ci[u]=0.0;
        		for (l=0; l<=n-1; l++)
			{
				v=i*n+l; w=l*k+j;
				p=ar[v]*br[w];
				q=ai[v]*bi[w];
				s=(ar[v]+ai[v])*(br[w]+bi[w]);
				cr[u]=cr[u]+p-q;
				ci[u]=ci[u]+s-p-q;
			}
		}
	return;
}
void print_matrix(double A[],int m,int n)/*打印的矩阵A(m*n)的元素*/ /*矩阵的阶数*/
 /*矩阵A*/
{
	int i,j;
	for (i=0; i<m; i++)
	{
		for (j=0; j<n; j++)
			printf("%13.7f\t",A[i*n+j]);
		printf("\n");
	}
}
int main()
{
	int i,j,n,m,k;
	double Ar[MAX],Br[MAX],Cr[MAX],Ai[MAX],Bi[MAX],Ci[MAX];
	static double cr[3][4],ci[3][4];
	static double ar[3][4]={ {1.0,2.0,3.0,-2.0}, /*矩阵A的实部*/
							{1.0,5.0,1.0,3.0},
							{0.0,4.0,2.0,-1.0}};
	static double ai[3][4]={ {1.0,-1.0,2.0,1.0}, /*矩阵A的虚部*/
							{-1.0,-1.0,2.0,0.0},
							{-3.0,-1.0,2.0,2.0}};
	static double br[4][4]={ {1.0,4.0,5.0,-2.0}, /*矩阵B的实部*/
							{3.0,0.0,2.0,-1.0},
							{6.0,3.0,1.0,2.0},
							{2.0,-3.0,-2.0,1.0}};
	static double bi[4][4]={ {-1.0,-1.0,1.0,1.0}, /*矩阵B的虚部*/
							{2.0,1.0,0.0,5.0},
							{-3.0,2.0,1.0,-1.0},
							{-1.0,-2.0,1.0,-2.0}};
	clrscr();
	puts("**********************************************************");
	puts("*    This is a complex-matrix-multiplication program.    *");
	puts("*    It calculate the two matrixes C(m*k)=A(m*n)B(n*k).  *");
	puts("**********************************************************");
	printf(" >> Please input the number of rows in A, m= ");
	scanf("%d",&m);
	printf(" >> Please input the number of cols in A, n= ");
	scanf("%d",&n);
	printf(" >> Please input the number of cols in B, k= ");
	scanf("%d",&k);
	printf(" >> Please input the %d elements in Ar one by one:\n >> ",m*n);
	for(i=0;i<m*n;i++)
		scanf("%lf",&Ar[i]);
	printf(" >> Please input the %d elements in Ai one by one:\n >> ",m*n);
	for(i=0;i<m*n;i++)
		scanf("%lf",&Ai[i]);
	printf(" >> Please input the %d elements in Br one by one:\n >> ",n*k);
	for(i=0;i<n*k;i++)
		scanf("%lf",&Br[i]);
	printf(" >> Please input the %d elements in Bi one by one:\n >> ",n*k);
	for(i=0;i<n*k;i++)
		scanf("%lf",&Bi[i]);
	CMatrixMul(Ar,Ai,Br,Bi,m,n,k,Cr,Ci); /*进行计算*/
	/*输出乘积结果的实部*/
	printf(" Real part of C(%d*%d)=A(%d*%d)*B(%d*%d):\n",m,k,m,n,n,k);
	print_matrix(Cr,m,k);
	/*输出乘积结果的虚部*/
	printf(" Complex part of C(%d*%d)=A(%d*%d)*B(%d*%d):\n",m,k,m,n,n,k);
	print_matrix(Ci,m,k);
	printf(" Press any key to quit...");
	getch();
	return 0;
}