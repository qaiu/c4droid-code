/*
例程188. 字符左右排序
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/25
*/

#include <stdio.h>
#include <conio.h>
int cnt,sum;
void countvalue()
{
	int i;
	for(i=666;i<=999;i=i+2)
		if((i/10%10==(3*i/2)/100%10)&&((3*i/2)/1000==(3*i/2)%100/10)) /*以I为准*/
		{
			cnt++;
			sum+=i+3*i/2;
		}
}

void writeDat()
{
	FILE *fp;
	fp=fopen("OUT188.DAT","w");
	fprintf(fp,"%d\n%d\n",cnt,sum);
	fclose(fp);
}

void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}

int main()
{
	cnt=sum=0;
	clrscr();
	puts(" This program is to find the numbers of SIX and NINE,");
	puts(" which satisfy the formula SIX+SIX+SIX=NINE+NINE,");
	puts(" where S,I,X,N,E stand for digits between 0 and 9. ");
	countvalue();
	printf(" >> The Number of satisfied is %d.\n",cnt);
	printf(" >> The Sum of all SIX and NINE is %d.\n",sum);
	writeDat();
	PressKeyToQuit();
}