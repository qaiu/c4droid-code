/*********************
*****最简单推箱子*****
-------------------------------------
****by一路向西到广州****
******版本:ver 2.0*********/
/*说明 ①修复了两个箱子在一起会重叠成一个的问题
②增加统计步数功能*/
#include<stdio.h>
#include<conio.h>
//函数getch()的头文件
#include<stdlib.h>
//函数exit()的头文件
#define H 15 
#define W 15
void do2(int,int);
void do8(int,int);
void do4(int,int);
void do6(int,int);
void draw();
void end();
int n=0;;//统计步数
int n1,n2,n3,n4;//计数
int a1=5,b1=5;//箱子1初始位置
int a2=5,b2=9;//箱子2
int a3=9,b3=5;//箱子3
int a4=9,b4=9;//箱子4
int x=7,y=7;//人
int u1=1,v1=1;//井1
int u2=1,v2=W-2; //井2
int u3=H-2,v3=1; //井3
int u4=H-2,v4=W-2; //井4
char direct;
int main() 
{
draw();
while(1)
{
direct=getch();
switch(direct)
{
case '2':n++;do2(--x,y);break;//按2上移
case '8':n++;do8(++x,y);break;
case '4':n++;do4(x,--y);break;
case '6':n++;do6(x,++y);break;
}}
return 0;}
void do2(int x,int y)
{
	if(x==a1&&b1==y) a1--;
	if(x==a2&&b2==y) a2--;
	if(x==a3&&b3==y) a3--; 
	if(x==a4&&b4==y) a4--; 
	//向上推箱子时，箱子上移
	draw();
	end();
}
void do8(int x,int y)
{
	if(x==a1&&b1==y) a1++;
	if(x==a2&&b2==y) a2++; 
	if(x==a3&&b3==y) a3++; 
	if(x==a4&&b4==y) a4++; 
	//向下推箱子时，箱子下移
	draw();
	end();
	}
void do4(int x,int y)
{
	if(x==a1&&y==b1) b1--;
	if(x==a2&&y==b2) b2--; 
	if(x==a3&&y==b3) b3--; 
	if(x==a4&&y==b4) b4--; 
	//向左推箱子时，箱子左移
	draw();
	end();
	}
void do6(int x,int y)
{
	if(x==a1&&b1==y) b1++;
	if(x==a2&&b2==y) b2++; 
	if(x==a3&&b3==y) b3++; 
	if(x==a4&&b4==y) b4++; 
	//向右推箱子时，箱子右移
	draw();
	end();
}
void draw()
{
	if(a1==x-1&&a1==a2&&y==b1&&b1==b2){x=a1+2;a2=a1+1;}
if(a1==x-1&&a1==a3&&y==b1&&b1==b3){x=a1+2;a3=a1+1;}
if(a1==x-1&&a1==a4&&y==b1&&b1==b4){x=a1+2;a4=a1+1;}
if(a2==x-1&&a2==a1&&y==b1&&b1==b2){x=a2+2;a1=a2+1;}
if(a2==x-1&&a2==a3&&y==b3&&b1==b2){x=a2+2;a3=a2+1;}
if(a2==x-1&&a4==a1&&y==b1&&b1==b4){x=a2+2;a4=a2+1;}
if(a3==x-1&&a3==a1&&y==b1&&b1==b3){x=a3+2;a1=a3+1;}
if(a3==x-1&&a3==a2&&y==b2&&b2==b3){x=a3+2;a2=a3+1;}
if(a3==x-1&&a3==a4&&y==b4&&b4==b3){x=a3+2;a4=a3+1;}
if(a4==x-1&&a4==a1&&y==b1&&b1==b4){x=a4+2;a1=a4+1;}
if(a4==x-1&&a4==a2&&y==b2&&b2==b4){x=a4+2;a2=a4+1;}
if(a4==x-1&&a4==a3&&y==b3&&b3==b4){x=a4+2;a3=a4+1;}
//向上推箱子，两个箱子不能重叠
if(a1==x+1&&a1==a2&&y==b1&&b1==b2){x=a1-2;a2=a1-1;}
if(a1==x+1&&a1==a3&&y==b1&&b1==b3){x=a1-2;a3=a1-1;}
if(a1==x+1&&a1==a4&&y==b1&&b1==b4){x=a1-2;a4=a1-1;}
if(a2==x+1&&a2==a1&&y==b1&&b1==b2){x=a2-2;a1=a2-1;}
if(a2==x+1&&a2==a3&&y==b3&&b1==b2){x=a2-2;a3=a2-1;}
if(a2==x+1&&a4==a1&&y==b1&&b1==b4){x=a2-2;a4=a2-1;}
if(a3==x+1&&a3==a1&&y==b1&&b1==b3){x=a3-2;a1=a3-1;}
if(a3==x+1&&a3==a2&&y==b2&&b2==b3){x=a3-2;a2=a3-1;}
if(a3==x+1&&a3==a4&&y==b4&&b4==b3){x=a3-2;a4=a3-1;}
if(a4==x+1&&a4==a1&&y==b1&&b1==b4){x=a4-2;a1=a4-1;}
if(a4==x+1&&a4==a2&&y==b2&&b2==b4){x=a4-2;a2=a4-1;}
if(a4==x+1&&a4==a3&&y==b3&&b3==b4){x=a4-2;a3=a4-1;}
//向下推箱子，两个箱子不能重叠
if(b1==y-1&&b1==b2&&x==a1&&a1==a2){y=b1+2;b2=b1+1;}
if(b1==y-1&&b1==b3&&x==a1&&a1==a3){y=b1+2;b3=b1+1;}
if(b1==y-1&&b1==b4&&x==a1&&a1==a4){y=b1+2;b4=b1+1;}
if(b2==y-1&&b2==b1&&x==a1&&a1==a2){y=b2+2;b1=b2+1;}
if(b2==y-1&&b2==b3&&x==a3&&a1==a2){y=b2+2;b3=b2+1;}
if(b2==y-1&&b2==b4&&x==a2&&a2==a4){y=b2+2;b4=b2+1;}
if(b3==y-1&&b3==b1&&x==a1&&a1==a3){y=b3+2;b1=b3+1;}
if(b3==y-1&&b3==b2&&x==a2&&a2==a3){y=b3+2;b2=b3+1;}
if(b3==y-1&&b3==b4&&x==a4&&a4==a3){y=b3+2;b4=b3+1;}
if(b4==y-1&&b4==b1&&x==a1&&a1==a4){y=b4+2;b1=b4+1;}
if(b4==y-1&&b4==b2&&x==a2&&a2==a4){y=b4+2;b2=b4+1;}
if(b4==y-1&&b4==b3&&x==a3&&a3==a4){y=b4+2;b3=b4+1;}
//向左推箱子，两个箱子不能重叠
if(b1==y+1&&b1==b2&&x==a1&&a1==a2){y=b1-2;b2=b1-1;}
if(b1==y+1&&b1==b3&&x==a1&&a1==a3){y=b1-2;b3=b1-1;}
if(b1==y+1&&b1==b4&&x==a1&&a1==a4){y=b1-2;b4=b1-1;}
if(b2==y+1&&b2==b1&&x==a1&&a1==a2){y=b2-2;b1=b2-1;}
if(b2==y+1&&b2==b3&&x==a3&&a1==a2){y=b2-2;b3=b2-1;}
if(b2==y+1&&b4==b1&&x==a1&&a1==a4){y=b2-2;b4=b2-1;}
if(b3==y+1&&b3==b1&&x==a1&&a1==a3){y=b3-2;b1=b3-1;}
if(b3==y+1&&b3==b2&&x==a2&&a2==a3){y=b3-2;b2=b3-1;}
if(b3==y+1&&b3==b4&&x==a4&&a4==a3){y=b3-2;b4=b3-1;}
if(b4==y+1&&b4==b1&&x==a1&&a1==a4){y=b4-2;b1=b4-1;}
if(b4==y+1&&b4==b2&&x==a2&&a2==a4){y=b4-2;b2=b4-1;}
if(b4==y+1&&b4==b3&&x==a3&&a3==a4){y=b4-2;b3=b4-1;}
//向右推箱子时，两个箱子不能重叠
	if(x==0)	{x=1;} 
	if(x==H-1){x=H-2;}
	if(y==0){y=1;}
	if(y==W-1){y=W-2;}
//人不能出边界
	if(x==1&&a1==0){x=2;a1=1;}
	if(x==1&&a2==0){x=2;a2=1;} 
	if(x==1&&a3==0){x=2;a3=1;} 
	if(x==1&&a4==0){x=2;a4=1;} 
	//向上推箱子，人和箱子不能出上边界
if(x==H-2&&a1==H-1){x=H-3;a1=H-2;}
if(x==H-2&&a2==H-1){x=H-3;a2=H-2;} 
if(x==H-2&&a3==H-1){x=H-3;a3=H-2;} 
if(x==H-2&&a4==H-1){x=H-3;a4=H-2;} 
//向下推箱子时，人和箱子不能出下边界
if(y==1&&b1==0){y=2;b1=1;}
if(y==1&&b2==0){y=2;b2=1;} 
if(y==1&&b3==0){y=2;b3=1;} 
if(y==1&&b4==0){y=2;b4=1;} 
//向左推箱子时，人和箱子不能出左边界
if(y==W-2&&b1==W-1){y=W-3;b1=W-2;}
if(y==W-2&&b2==W-1){y=W-3;b2=W-2;} 
if(y==W-2&&b3==W-1){y=W-3;b3=W-2;} 
if(y==W-2&&b4==W-1){y=W-3;b4=W-2;} 
//向右推箱子时，人和箱子不能出右边界
	clrscr();//清屏
	printf("    第1关   步数：%d\n",n);
for(int i=0;i<H;i++)
{
for(int j=0;j<W;j++)
{
	if(i==0||i==H-1||j==0||j==W-1)
printf("\033[42;3m墙\033[m");
else if(i==x&&j==y)
printf("\033[48;3m人\033[m");
else if(i==a1&&j==b1)
printf("\033[41;3m箱\033[m");
else if(i==a2&&j==b2)
printf("\033[41;3m箱\033[m"); 
else if(i==a3&&j==b3)
printf("\033[41;3m箱\033[m"); 
else if(i==a4&&j==b4)
printf("\033[41;3m箱\033[m"); 
else if(i==u1&&j==v1)
printf("\033[44;3m井\033[m"); 
else if(i==u2&&j==v2)
printf("\033[44;3m井\033[m");  
else if(i==u3&&j==v3)
printf("\033[44;3m井\033[m");  
else if(i==u4&&j==v4)
printf("\033[44;3m井\033[m");  
else printf("  ");
}
printf("\n");
}
}  
void end()
{
	if((a1==u1&&b1==v1)||(a2==u1&&b2==v1)||(a3==u1&&b3==v1)||(a4==u1&&b4==v1))n1=1;
	if((a1==u2&&b1==v2)||(a2==u2&&b2==v2)||(a3==u2&&b3==v2)||(a4==u2&&b4==v2))n2=1; 
	if((a1==u3&&b1==v3)||(a2==u3&&b2==v3)||(a3==u3&&b3==v3)||(a4==u3&&b4==v3))n3=1; 
	if((a1==u4&&b1==v4)||(a2==u4&&b2==v4)||(a3==u4&&b3==v4)||(a4==u4&&b4==v4))n4=1; 
	//任一箱子进入任一井，n记为1
	if(n1+n2+n3+n4==4)
{printf("\033[41;3m成功通过,游戏结束！\033[m"); 
	exit(0);}}
//四个n相加为4，表明四个箱子都进入井里，游戏结束