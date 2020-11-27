/*
简易单步骤计算器v1.0
by:千百度
创建于2017-3-1
计算精度为15位
*/

//使用库函数
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//宏定义用于判断计算结果是否为整数
#define PD jsjg-(int)jsjg

char a[100]={0x00};//存放输入的表达式
double jsjg;//计算结果

/*计算函数*/
double jsq(char*a)
{
	
	char *d;//分隔符字符串
	char c;//运算符
	double x,y,b;
	//x,y:用于计算的两个浮点变量
	//b:函数返回值计算结果
	if (strchr(a,'-')!=NULL)
	//判断运算符是否为"-"，下同。
	{
		d="-";
		//将"-"赋值给分隔符用于分割表达式
		c='-';
		//运算符赋值用于计算
	}
	if (strchr(a,'+')!=NULL)
	{
		d="+";
		c='+';
	}
	if (strchr(a,'*')!=NULL)
	{
		d="*";
		c='*';
	}
	if (strchr(a,'/')!=NULL)
	{
		d="/";
		c='/';
	}
	if (strstr(a,"--")!=NULL)
	{
		d="--";
		c='+';
	}
		
	x=atof(a);
	//将输入的字符串转为算式第一个数值
	/*PS:之所以不采用分割后的字符串转
换，因为第一个数值做减法运算时会因分割而丢失符号*/
	strtok(a,d);	//分割字符串
	y=atof(strtok(NULL,d));
	//字符串第二段转换为算式第二个数值
	
	switch (c)
	//通过运算符变量的值决定计算方式
	{
		case '+':
		b=x+y;//加
		break;
		case '-':
		b=x-y;
		break;
		case '*':
		b=x*y;
		break;
		case '/':
		b=x/y;
		break;
		default:
		printf("输入有误\n");
		//不支持的运算符
		break;
	}
	return b;//返回运算结果
}
/*主函数*/
int main(void)
{
	while (1)//无限循环
	{
	printf("请输入算式\n");
	scanf("%s",a);//输入算式字符串
	jsjg=jsq(a);//调用计算函数
	if (PD!=0)//判断计算结果是否为整数
	printf("=%lf\n", jsjg);
	//如果是小数，打印浮点值
	else
	printf("=%d\n", (int)jsjg);
	//打印整数值
	}
	return 0;
}