/*c4droid代码手册
 *cpu性能测试
 *佚名编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include<stdio.h>
#include<time.h>
#include<conio.h>
int n=1;
void delay()
{
  int i=0,j=0;
  for(i=0;i<1000;i++)
    for(j=0;j<15000;j++);
 }
void flash()
{
	int i;
	char c=' ';
	for(i=0;i<20;i++)
	printf("\033[1;45;33m%50c\n",c);
printf("\033[5;15H\033[1;45;36m正在第%d次测试",n);
	 delay();
printf("\033[?25l\033[8;0H\033[1;43;31m \033[0m\n");
     delay();	printf("\033[1A\033[2C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[4C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[6C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[8C\033[1;43;31m \033[0m\n");
     delay();       printf("\033[1A\033[10C\033[1;43;31m \033[0m\n");
     delay();	printf("\033[1A\033[12C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[14C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[16C\033[1;43;31m \033[0m\n");
     delay();
 printf("\033[1A\033[18C\033[1;43;31m \033[0m"); 
}
void end()
{
	printf("\033[12;8H\033[1;45;32m      ");
}
int main()
{
	int i,sum=0,ascore,score;
clock_t start,finish;
double duration;
p1:start=clock();
for(i=0;i<3;i++)
{
   flash();
   if(i!=2)
   clrscr();
}
printf("\033[5;15H\033[1;45;36m第%d次测试完毕\033[0m",n);  
finish=clock();
duration=(double)(finish-start)/CLOCKS_PER_SEC;
score=100000/duration;
sum=sum+score;
ascore=sum/n;
end();
printf("您的CPU本次得分:%d\n\n             ",score);
printf("平均得分:%d",ascore);
printf("\033[18;10H\033[1;46;31m输入任意键继续测试\033[0m");
n++;
getch();
clrscr();
  goto p1;
return 0;
}