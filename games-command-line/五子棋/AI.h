#include<math.h>
#include<stdlib.h>
#include<time.h>

#define Npoints 10 //最大分支数 
#define Ntype 17 //棋型类别数 
#define WIN 1 //4000
#define LOSE 2 //-4000
#define FLEX4 3 //2000
#define flex4 4 //-2000
#define BLOCK4 5 //1000
#define block4 6 //-1000 
#define FLEX3 7 //1000 ?
#define flex3 8 //-1000
#define BLOCK3 9 //400 ?
#define block3 10 //-600 ? 
#define FLEX2 11 //400 ?
#define flex2 12 //-600 ?
#define BLOCK2 13 //100 ?
#define block2 14 //-150 ?
#define FLEX1 15 //100 ?
#define flex1 16 //-150 ?

//棋盘
struct board
{
	char array[15][15]; //棋盘方阵 0为空点 1为黑 2为白 	
	char type1[15][Ntype]; //15条横线上的棋型 	
	int TYPE1[Ntype]; //所有横线上的总棋型 
	char type2[15][Ntype]; //15条竖线上的棋型 	
	int TYPE2[Ntype]; //所有竖线上的总棋型 	
	char type3[19][Ntype]; //19条平行于副对角线的斜线上的棋型，斜线的最小长度为6 	
	int TYPE3[Ntype]; //所有平行于副对角线的斜线上的总棋型	
	char type4[19][Ntype]; //19条平行于主对角线的斜线上的棋型，斜线的最小长度为6 	
	int TYPE4[Ntype]; //所有平行于主对角线的斜线上的总棋型	
	int eva; //棋盘相对于黑方的局势评估值 	
	char result; //棋盘结局 0为未完 1为黑胜 2为白胜 
}BOARD,ZERO;

struct route
{
	int x[120];
	int y[120];
	int step;
	char first;
}ROUTE,ZERO_ROUTE;

//将棋盘A黑白交换后写到B 
int inv_board(struct board &A,struct board &B)
{
	int INV[17]={0,2,1,4,3,6,5,8,7,10,9,12,11,14,13,16,15}; //交换映射表 
	int i,j;
	for (i=0;i<15;i++)
	{
		for (j=0;j<15;j++)
		{
			B.array[i][j]=INV[A.array[i][j]];
		}
	}
	
	for (j=0;j<Ntype;j++)
	{
		for (i=0;i<15;i++)
		{
			B.type1[i][j]=A.type1[i][INV[j]];		
			B.type2[i][j]=A.type2[i][INV[j]];	
			B.type3[i][j]=A.type3[i][INV[j]];		
			B.type4[i][j]=A.type4[i][INV[j]];
		}
		
		for (i=15;i<19;i++)
		{
			B.type3[i][j]=A.type3[i][INV[j]];
			B.type4[i][j]=A.type4[i][INV[j]];
		}
		
		B.TYPE1[j]=A.TYPE1[INV[j]];
		B.TYPE2[j]=A.TYPE2[INV[j]];
		B.TYPE3[j]=A.TYPE3[INV[j]];
		B.TYPE4[j]=A.TYPE4[INV[j]];
	}	
	B.result=INV[A.result];
	return 0;
}

//棋型辨识数组 
int score[3][3][3][3][3][3]; 
//给棋型辨识数组赋值 
int init_score()
{
	//黑五连 AI胜
	score[1][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][0]=WIN;
	score[0][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][2]=WIN;
	score[2][1][1][1][1][1]=WIN;
	//白五连 AI负
	score[2][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][0]=LOSE;
	score[0][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][1]=LOSE;
	score[1][2][2][2][2][2]=LOSE;
	//黑活四
	score[0][1][1][1][1][0]=FLEX4;
	//白活四
	score[0][2][2][2][2][0]=flex4;	
	//黑活三
	score[0][1][1][1][0][0]=FLEX3;
	score[0][1][1][0][1][0]=FLEX3;
	score[0][1][0][1][1][0]=FLEX3;
	score[0][0][1][1][1][0]=FLEX3;
	//白活三
	score[0][2][2][2][0][0]=flex3;
	score[0][2][2][0][2][0]=flex3;
	score[0][2][0][2][2][0]=flex3;
	score[0][0][2][2][2][0]=flex3;
	//黑活二
	score[0][1][1][0][0][0]=FLEX2;
	score[0][1][0][1][0][0]=FLEX2;
	score[0][1][0][0][1][0]=FLEX2;
	score[0][0][1][1][0][0]=FLEX2;
	score[0][0][1][0][1][0]=FLEX2; 
	score[0][0][0][1][1][0]=FLEX2;
	//白活二
	score[0][2][2][0][0][0]=flex2;
	score[0][2][0][2][0][0]=flex2;
	score[0][2][0][0][2][0]=flex2;
	score[0][0][2][2][0][0]=flex2;
	score[0][0][2][0][2][0]=flex2; 
	score[0][0][0][2][2][0]=flex2;
	//黑活一
	score[0][1][0][0][0][0]=FLEX1;
	score[0][0][1][0][0][0]=FLEX1;
	score[0][0][0][1][0][0]=FLEX1;
	score[0][0][0][0][1][0]=FLEX1;
	//白活一
	score[0][2][0][0][0][0]=flex1;
	score[0][0][2][0][0][0]=flex1;
	score[0][0][0][2][0][0]=flex1;
	score[0][0][0][0][2][0]=flex1;
	int p1,p2,p3,p4,p5,p6,x,y,ix,iy;
	for (p1=0;p1<3;p1++)
	{
		for (p2=0;p2<3;p2++)
		{
			for (p3=0;p3<3;p3++)
			{
				for (p4=0;p4<3;p4++)
				{
					for (p5=0;p5<3;p5++)
					{
						for (p6=0;p6<3;p6++)
						{
							x=0;
							y=0;
							ix=0;
							iy=0;
							if (p1==1) x++;
							else if (p1==2) y++;
							if (p2==1) x++,ix++;
							else if (p2==2) y++,iy++;
							if (p3==1) x++,ix++;
							else if (p3==2) y++,iy++;
							if (p4==1) x++,ix++;
							else if (p4==2) y++,iy++;
							if (p5==1) x++,ix++;
							else if (p5==2) y++,iy++;
							if (p6==1) ix++;
							else if (p6==2) iy++;
							//黑冲四 
							if ((x==4&&y==0) || (ix==4&&iy==0))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK4;
								}
							}
							//白冲四 
							else if ((x==0&&y==4) || (ix==0&&iy==4))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block4;
								}
							}
							//黑眠三 
							else if ((x==3&&y==0) || (ix==3&&iy==0))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK3;
								}
							}
							//白眠三 
							else if ((x==0&&y==3) || (ix==0&&iy==3))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block3;
								}
							}
							//黑眠二
							else if ((x==2&&y==0) ||(ix==2&&iy==0))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK2;
								}
							}						
							//白眠二
							else if ((x==0&&y==2) || (ix==0&&iy==2))
							{
								if (score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block2;
								}
							}
						}
					}
				}
			}
		}
	}	
	return 0;
}

//对棋盘A的局势进行评分 
int evaluate(struct board &A,int x,int y)
{	
	int i,j,k,s;
	for (i=0;i<Ntype;i++)
	{
		A.TYPE1[i]-=A.type1[x][i];
		A.type1[x][i]=0;
		A.TYPE2[i]-=A.type2[y][i];
		A.type2[y][i]=0;
	}
	
	for (i=0;i<10;i++)
	{
		s=score[A.array[x][i]][A.array[x][i+1]][A.array[x][i+2]][A.array[x][i+3]][A.array[x][i+4]][A.array[x][i+5]];		
		A.type1[x][s]++;	
		A.TYPE1[s]++;
		s=score[A.array[i][y]][A.array[i+1][y]][A.array[i+2][y]][A.array[i+3][y]][A.array[i+4][y]][A.array[i+5][y]];	
		A.type2[y][s]++;
		A.TYPE2[s]++;
	}
	
	int p=x+y-5;
	if (p>=0&&p<19)
	{
		for (i=0;i<Ntype;i++)
		{
			A.TYPE3[i]-=A.type3[p][i];
			A.type3[p][i]=0;
		}
		
		if (p<10)
		{
			k=p+5;
			for (i=0;i<=p;i++)
			{
				s=score[A.array[k][i]][A.array[k-1][i+1]][A.array[k-2][i+2]][A.array[k-3][i+3]][A.array[k-4][i+4]][A.array[k-5][i+5]];
				A.type3[p][s]++;
				A.TYPE3[s]++;
				k--;
			}
		}
		else
		{
			k=14;
			for (i=p-9;i<10;i++)
			{
				s=score[A.array[k][i]][A.array[k-1][i+1]][A.array[k-2][i+2]][A.array[k-3][i+3]][A.array[k-4][i+4]][A.array[k-5][i+5]];
				A.type3[p][s]++;
				A.TYPE3[s]++;
				k--;
			}
		}
	}
	
	int q=x-y+9;
	if (q>=0&&q<19)
	{
		for (i=0;i<Ntype;i++)
		{		
			A.TYPE4[i]-=A.type4[q][i];
			A.type4[q][i]=0;
		}
		if (q<10)
		{
			k=9-q;
			for (i=0;i<=q;i++)
			{
				s=score[A.array[i][k]][A.array[i+1][k+1]][A.array[i+2][k+2]][A.array[i+3][k+3]][A.array[i+4][k+4]][A.array[i+5][k+5]];
				A.type4[q][s]++;
				A.TYPE4[s]++;
				k++;
			}
		}
		else
		{
			k=0;
			for (i=q-9;i<10;i++)
			{
				s=score[A.array[i][k]][A.array[i+1][k+1]][A.array[i+2][k+2]][A.array[i+3][k+3]][A.array[i+4][k+4]][A.array[i+5][k+5]];
				A.type4[q][s]++;
				A.TYPE4[s]++;
				k++;
			}
		}
	}
	
	int weight[17]={0,4000,-4000,2000,-2000,1000,-1000,1000,-1000,400,-600,400,-600,100,-150,100,-150};
	s=0;
	int STAT[Ntype];
	for (i=1;i<Ntype;i++)
	{
		s+=(A.TYPE1[i]+A.TYPE2[i]+A.TYPE3[i]+A.TYPE4[i])*weight[i];
		STAT[i]=(A.TYPE1[i]>0)+(A.TYPE2[i]>0)+(A.TYPE3[i]>0)+(A.TYPE4[i]>0);
	}
	A.result=0;
	//胜 
	if (STAT[1]>0)
	{
		s+=100000;
		A.result=1;
	}
	
	//负 
	else if (STAT[2]>0)
	{
		s-=100000;
		A.result=2;
	}
	
	//对手冲四、活四 
	else if (STAT[4]>0||STAT[6]>0)
	{
		s-=30000;
	}
	
	//对手无冲四、活四 
	else
	{	
		//检验 冲四活三
		k=0;
		k+=A.TYPE1[5]*A.TYPE2[7];
		k+=A.TYPE1[5]*A.TYPE3[7];
		k+=A.TYPE1[5]*A.TYPE4[7];
		k+=A.TYPE2[5]*A.TYPE1[7];
		k+=A.TYPE2[5]*A.TYPE3[7];
		k+=A.TYPE2[5]*A.TYPE4[7];
		k+=A.TYPE3[5]*A.TYPE1[7];
		k+=A.TYPE3[5]*A.TYPE2[7];
		k+=A.TYPE3[5]*A.TYPE4[7];
		k+=A.TYPE4[5]*A.TYPE1[7];
		k+=A.TYPE4[5]*A.TYPE2[7];
		k+=A.TYPE4[5]*A.TYPE3[7];
		
		//活四
		if (STAT[3]>0)
		{
			s+=25000;
		} 
		
		//双冲四
		else if (STAT[5]>=2)
		{
			s+=20000;
		}
		
		//冲四活三
		else if (k>0)
		{
			s+=20000;
		} 
		
		//对手有活三 
		else if (STAT[8]>0&&STAT[5]==0)
		{
			s-=20000;
		}
		
		//双活三
		else if (STAT[7]>=2&&STAT[8]==0)
		{
			s+=10000;
		}
	}
	
	A.eva=s;
	return 0;
}

struct points
{
	int coo_x[Npoints]; //横坐标 
	int coo_y[Npoints]; //纵坐标 
	int eva[Npoints]; //评估值 
	int exi[Npoints]; //存在性 
};

//对于棋盘A，评估出最佳的N个待定落子点 
struct points seek_points(struct board &A,int N)
{
	struct points best_points;
	int i,j,k,w;
	int worth[15][15];
	int B[21][21]={0};
	for (k=0;k<Npoints;k++)
	{
		best_points.exi[k]=0;
	}
	
	if (A.result>0)
	{
		best_points.exi[0]=1;
		best_points.eva[0]=A.eva;
		for (i=0;i<15;i++)
		{
			for (j=0;j<15;j++)
			{
				if (A.array[i][j]==1)
				{
					best_points.coo_x[0]=i;
					best_points.coo_y[0]=j;
					goto the_end;
				}
			}
		}
	}
	
	for (i=0;i<15;i++)
	{
		for (j=0;j<15;j++)
		{
			if (A.array[i][j]!=0)
			{
				B[i+1][j+1]=1;
				B[i+1][j+2]=1;
				B[i+1][j+3]=1;
				B[i+1][j+4]=1;
				B[i+1][j+5]=1;
				B[i+2][j+1]=1;
				B[i+2][j+2]=1;
				B[i+2][j+3]=1;
				B[i+2][j+4]=1;
				B[i+2][j+5]=1;
				B[i+3][j+1]=1;
				B[i+3][j+2]=1;
				B[i+3][j+4]=1;
				B[i+3][j+5]=1;
				B[i+4][j+1]=1;
				B[i+4][j+2]=1;
				B[i+4][j+3]=1;
				B[i+4][j+4]=1;
				B[i+4][j+5]=1;
				B[i+5][j+1]=1;
				B[i+5][j+2]=1;
				B[i+5][j+3]=1;
				B[i+5][j+4]=1;
				B[i+5][j+5]=1;
			}
		}
	}
	
	struct board buff;
	
	//对于棋盘A上的空点，评估在该处落子后的局势 
	for (i=0;i<15;i++)
	{
		for (j=0;j<15;j++)
		{
			worth[i][j]=-1000000;	
			if (A.array[i][j]==0&&B[i+3][j+3]==1)
			{ 
				buff=A;			
				buff.array[i][j]=1;
				evaluate(buff,i,j);
				worth[i][j]=buff.eva;
			}
		}
	}
	
	//筛选最佳的N个点 
	for (k=0;k<N;k++)
	{
		w=-1000000;
		
		for (i=0;i<15;i++)
		{
			for (j=0;j<15;j++)
			{
				if (worth[i][j]>w)
				{
					w=worth[i][j];
					best_points.coo_x[k]=i;
					best_points.coo_y[k]=j;
				}
			}
		}
		if ( (k>0) && ((best_points.eva[0]-w)>3333) ) break;
		best_points.eva[k]=w;
		best_points.exi[k]=1;
		worth[best_points.coo_x[k]][best_points.coo_y[k]]=-1000000;
	}
	the_end:
	return best_points;
}

struct decision
{
	int coo_x;
	int coo_y;
	int eva;
	int num;
};

struct decision DECISION;
int level_limit=6;
int Nbranch[15]={10,9,8,7,7,6,5,5,4,4,4,3,3,3,3};

//博弈树MinMax递归分析 AlphaBeta剪枝 
int analyse(struct board &A,int level,int alpha,int beta)
{	
	if (level==level_limit)
	{
		alpha=seek_points(A,1).eva[0];
		return alpha;
	}
	
	else if (level%2==0)
	{
		struct points P;
		P=seek_points(A,Nbranch[level]);
		struct board buff;
		int i;
		for (i=0;i<Nbranch[level];i++)
		{
			if (P.exi[i]==1)
			{	
				buff=A;
				buff.array[P.coo_x[i]][P.coo_y[i]]=1;
				evaluate(buff,P.coo_x[i],P.coo_y[i]);
				int a;
				a=analyse(buff,level+1,alpha,beta);
				if (a>alpha)
				{
					alpha=a;
					if (level==0)
					{
						DECISION.coo_x=P.coo_x[i];
						DECISION.coo_y=P.coo_y[i];
						DECISION.eva=a;
						DECISION.num=i;
					}
				}
			}
			if (beta<=alpha) break;
		}
		return alpha;
	}
	
	else if (level%2==1)
	{
		struct board BUFF,buff;
		inv_board(A,BUFF);
		struct points P; 
		P=seek_points(BUFF,Nbranch[level]);
		int i;
		for (i=0;i<Nbranch[level];i++)
		{
			if (P.exi[i]==1)
			{	
				buff=A;
				buff.array[P.coo_x[i]][P.coo_y[i]]=2;
				evaluate(buff,P.coo_x[i],P.coo_y[i]);
				int a;
				a=analyse(buff,level+1,alpha,beta);
				if (a<beta)
				{
					beta=a;
				}
			}
			if (beta<=alpha) break;
		}
		return beta;
	}
}

void initboard(int x,int y,int type)
{
	BOARD.array[x][y]=type;
	if (type)
	{
		ROUTE.x[ROUTE.step]=x;
		ROUTE.y[ROUTE.step]=y;
		ROUTE.step++;
	}
	else
	ROUTE.step--;
	evaluate(BOARD,x,y);
}
/*	AI调用	 */ 
/*
	结构体 BOARD 储存当前棋盘信息 ,数组 BOARD.array[15][15]为当前棋盘的映射, 0代表空，1代表AI的棋子，2代表对手的棋子 
	
	AI调用方法:  analyse(BOARD,0,-1000000,1000000);
	
	AI决策储存于全局变量 DECISION，落子坐标为 ( DECISION.coo_x, DECISION.coo_y ) 
	
	对棋盘落子操作 (假设要落子的坐标为 X,Y)： 
	BOARD.array[X][Y]=1或2；
	evaluate(BOARD,X,Y); 
	
	修改 BOARD.array[15][15]中的元素后，必须调用一次 evaluate(BOARD,x,y); 以更新棋盘的描述信息 
	x,y为被修改的元素的坐标 
	 
*/

/*	AI参数自定义	*/
/*
	全局变量 Nbranch[15] 为AI搜索博弈树时每一层的节点分支数，
	愈大则搜索范围愈广，但程序速度愈慢 。作者猜测采用逐层递减规律时效果较佳 ，其最大值不得超过 宏Npoints 
	
	全局变量 level_limit 为AI搜索博弈树时的最大搜索深度 ，只能为偶数 ，愈大则搜索步数愈多，但程序速度愈慢 

*/ 