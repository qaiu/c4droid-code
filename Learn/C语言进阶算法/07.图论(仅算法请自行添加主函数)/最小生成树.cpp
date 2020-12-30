#include <stdio.h>

#define MAX  100000
#define VNUM  10+1                                             //这里没有ID为0的点,so id号范围1~10

int edge[VNUM][VNUM]={/*输入的邻接矩阵*/};
int lowcost[VNUM]={0};                                         //记录Vnew中每个点到V中邻接点的最短边
int addvnew[VNUM];                                             //标记某点是否加入Vnew
int adjecent[VNUM]={0};                                        //记录V中与Vnew最邻近的点


void prim(int start)
{
     int sumweight=0;
     int i,j,k=0;

     for(i=1;i<VNUM;i++)                                      //顶点是从1开始
     {
        lowcost[i]=edge[start][i];
        addvnew[i]=-1;                                         //将所有点至于Vnew之外,V之内，这里只要对应的为-1，就表示在Vnew之外
     }

     addvnew[start]=0;                                        //将起始点start加入Vnew
     adjecent[start]=start;
                                                 
     for(i=1;i<VNUM-1;i++)                                        
     {
        int min=MAX;
        int v=-1;
        for(j=1;j<VNUM;j++)                                      
        {
            if(addvnew[j]!=-1&&lowcost[j]<min)                 //在Vnew之外寻找最短路径
            {
                min=lowcost[j];
                v=j;
            }
        }
        if(v!=-1)
        {
            printf("%d %d %d\n",adjecent[v],v,lowcost[v]);
            addvnew[v]=0;                                      //将v加Vnew中

            sumweight+=lowcost[v];                             //计算路径长度之和
            for(j=1;j<VNUM;j++)
            {
                if(addvnew[j]==-1&&edge[v][j]<lowcost[j])      
                {
                    lowcost[j]=edge[v][j];                     //此时v点加入Vnew 需要更新lowcost
                    adjecent[j]=v;                             
                }
            }
        }
    }
    printf("the minmum weight is %d",sumweight);
}

int main()
{
	return 0;
}