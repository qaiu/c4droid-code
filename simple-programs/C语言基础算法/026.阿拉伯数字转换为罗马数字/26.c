/*
例程26. 阿拉伯数字转化成罗马数字
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/21
*/
#include <stdlib.h>
#include <conio.h>
#define ROWS 4
#define COLS 4
void checknum(int val);
void  to_roman(int decimal,char roman[ ]);
int nums[ROWS][COLS]={{1000,1000,1000,1000},
		         {900,500,400,100},
		         {90,50,40,10},
		         {9,5,4,1}};
char *roms[ROWS][COLS]={{"m","m","m","m"},
		              {"cm","d","cd","c"},
		              {"xc","l","xl","x"},
		              {"ix","v","iv","i"}};

int main(int argc,char *argv[])
{
	int low,high;
	char roman[25];
	
 /*若使用命令行参数运行，请将0改为1*/
#if 0
	if(argc<2)
	{	printf("Usage:roman decimal_number\n");/*运行程序需带整数参数*/
	return -1;
	}
	high=low=atoi(argv[1]);/*将第一个参数转换成整数*/
	checknum(low);
	if(argc>2)
	{/*带两个参数*/
	high=atoi(argv[2]);
	checknum(high);
	if(low>high)
		{
			low=high;
			high=atoi(argv[1]);
		}
	}
	else
		low=1;
#else
    printf("Please enter to convert the min num integer(n>0)\n");
    scanf("%d",&low);
    printf("Please enter to convert the max num integer(n2>n)\n");
    scanf("%d",&high);
#endif
	for(;low<=high;low++)
	{
		to_roman(low,roman);
		printf("%d\t%s\n",low,roman);
	}
}

void checknum(int val)/*检查参数合理性*/
{
	if(val<1||val>9999)
	{
		printf("The number must be in range 1..9999.\n");
		// exit(0);
	}
}
void  to_roman(int decimal,char roman[ ])/*将整数转换成罗马数字表示*/
{
	int power,index;
	roman[0]='\0';
	for(power=0;power<ROWS;power++)
		for(index=0;index<COLS;index++)
			while(decimal>=nums[power][index])
			{
				strcat(roman,roms[power][index]);
				decimal-=nums[power][index];
			}
}