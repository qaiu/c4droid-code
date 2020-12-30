/*
>>控制台科学计算器源码
>>部分源码参考c4droid代码手册
>>by:千百度
>>转载请注明来源
>>date:2017.3.20
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

//初始化系统常量
#define N 1000
#define P M_PI
#define E M_E
#define MAX INT_MAX
#define MIN INT_MIN
#define MIN_XS 0.000001
#define PD jg-(int)jg

//初始化界面文本
const char *S1="───────────────────────────────\n->科学计算器v1.2\n->by 千百度(部分源码参考c4droid代码手册)\n->QQ:736226400\n───────────────────────────────\n\033[47;35m->查看函数说明请输入:hs\n->数据统计模式请输入:tj\n->输入表达式开始计算\n\033[47;34m───────────────────────────────\n\033[47;30m",*S2="\033[47;35m───────────────────────────────\n->三角函数(x为角度):sin(x),cos(x),tan(x)\n->反△函数(值为角度):asin(x),acos(x),atan(x)\n->双曲函数:sinh(x),cosh(x),tanh(x)\n->常用对数和自然对数:lg(x),ln(x)\n->开方函数和e^x:sqrt(x),exp(x)\n->在(0,1)之间产生一个随机小数*x:rand(x)\n->运算符及优先级:()大于%%^ 大于*/大于 +-\n\033[47;31m->特殊规定:\n---->1,负数要加括号除非在第一位例如-5+(-3)\n---->2,函数后为指数应用括号指明优先级例如:\n---->(sin45)^3不加括号系统默认sin(45^3)\n---->3,整数部分超出10位将以科学记数法表示\n---->表达式不允许出现空格\n---->运算结果取值-1.7E-308～1.7*E308\n───────────────────────────────\033[47;30m\n",*S3="\033[47;34m───────────────────────────────\n->统计计算模式说明:\n───────────────────────────────\n\033[47;35m->数据统计函数：tj(x1;x2;...;xn)\n->相关关系函数：gx(x1,y1;x2,y2;...xn,yn)\n->二项分布:pxb(k;n;p)\n->参数要求(n>=k,1=>p>=0)\n->超几何分布:pxh(k;N;M;n)\n->参数要求(N>=M,n>=k,M>=k,N>=n)\n->请注意符号，并严格按照格式输入\n->输入exit退出统计模式\n\033[47;34m───────────────────────────────\033[47;30m\n",*d1 = "abcdefghijklmnopqrstuvwxyz^!();",*d2="1234567890.#";

double FS(void);//数值及括号处理函数
char token[N];  // 存放表达式字符串数组 
char str[N];	//获取输入字符串数组 
int n = 0, m = 0, t = 0,x=0;
int c = 0;  //判断是否已经计算0表示没有

//角度转弧度
double jtoh(double a)
{
	return P*a/180;
}
//弧度转角度
double htoj(double a)
{
	return 180*a/P;
}
//阶乘
double jc(int a)
{
	double b=1;
	if (a)
	{
		for (int i=1;i<=a;i++)
		b*=i;
	}
	return b;
}
//排列
double npr(int a,int b)
{
	if (b<=a)
	return jc(a)/jc(a-b);
	else
	{
		printf("参数有误error\n");
		return -1;
	}
}
//组合
double ncr(int a,int b)
{
	if (b<=a)
	return npr(a,b)/jc(b);
	else
	{
		printf("参数有误error\n");
		return -1;
	}
}

//数据统计
void tj(double a[N][2],int n)
{
printf("\033[47;34m───────────────────────────────\n->数据统计\n───────────────────────────────\n\033[47;31m");
	int i;
	double x=0,s=0;
	for (i=0; i<n;i++)
	{
		printf("x%d=%g\n",i+1,a[i][0],i+1);
		x=x+a[i][0];
	}
	int p=0;
	printf("->求和: %.3f",x);
	x=x/n;
	printf("\n->平均值为:%.3f",x);
	for (p;p<n;p++)
		s=s+(a[p][0]-x)*(a[p][0]-x);
		s=s/n;
	printf("\n->方差为:%.3f",s);
	s=sqrt(s);
	printf("\n->标准差为:%.3f\n元素个数n=%d\n\033[47;30m",s,n);
	printf("───────────────────────────────\n");
}

 //变量间相关关系 回归方程
void gx(double a[N][2],int n)
 {
 int i;
double s1=0,x=0,y=0;
double c,b,s2=0,s3=0,r;	printf("\033[47;34m───────────────────────────────\n->变量相关关系分析\n───────────────────────────────\033[47;31m\n");
	for (i=0;i<n;i++)
	{
		printf("->x%d=%f->y%d=%f\n",i+1,a[i][0],i+1,a[i][1]);
//		printf("%f>>%f\n",a[i][0],a[i][1]);
		s1=s1+a[i][0]*a[i][1];
		s2=s2+a[i][0]*a[i][0];
		s3=s3+a[i][1]*a[i][1];
		x=x+a[i][0];y=y+a[i][1];
	}
	x=x/n;y=y/n;
	s1=s1-x*y*n;
	s2=s2-n*x*x;
	s3=s3-n*y*y;
	b=s1/s2;
	c=y-b*x;
	printf("->回归方程:y=%.3fx+%.3f",b,c);
	r=s2*s3;
	r=s1/sqrt(r);
	printf("\n->相关系数r=%g",r);
	if (r<-0.75)
	printf("\n->负相关关系很强");
	else
	if (r>0.75)
	printf("\n->正相关关系很强");
	else
	if (-0.75<r&&r<-0.3||0.3<r&&r<0.75)
	printf("\n->相关性一般");
	else
	if (-0.3<r&&r<0.3)
	printf("\n->相关性较弱");
	else
	printf("\n->参数错误!");
	printf("\n->元素个数n=%d\n",n);
	printf("───────────────────────────────\n");
}
//二项分布求期望方差
void pxb(double a[N][2])
{
	int k=(int)a[0][0];
	int n=(int)a[1][0];
	double p=a[2][0];
	if (n>=k&&p>=0&&p<=1)
	{
	printf("\033[47;31m───────────────────────────────\n数学期望EX=%f\n",n*p);
	printf("方差DX=%f\n",n*p*(1-p));
	printf("在%d次独立重复试验中,事件A发生%d次的概率p=%f\n",n,k,ncr(n,k)*pow(p,k)*pow(1-p,n-k));
	}
	else
		printf("\033[47;31m───────────────────────────────\n->参数要求\n->n不小于k\n->p的取值范围[0,1]\n->error\n───────────────────────────────\n\033[47;30m");
}
//超几何分布
void pxh(double a[N][2])
{
	int k=(int)a[0][0];
	int n=(int)a[1][0];
	int m=(int)a[2][0];
	int n1=(int)a[3][0];
	if (n>=n1&&m>=k&&n1>=k&&n>=m)
	{
	printf("\033[47;31m───────────────────────────────\n数学期望EX=%f\n",((double)n1)*((double)m)/((double)n));
	printf("方差DX=%f\n",n1*m*(n-m)*(n-n1)/(pow(n,2)*(n-1)));
	printf("产品总数N=%d\n含有次品数M=%d\n不放回抽取件数n=%d\n其中含X件次品\n则事件{X=%d}发生的概率p(X=%d)=%f\n\033[47;30m",n,m,n1,k,k,ncr(m,k)*ncr(n-m,n1-k)/ncr(n,n1));
	}
	else
	{
		printf("\033[47;34m───────────────────────────────\n->参数要求\n->N不小于M\n->n不小于k\n->M不小于k\n->N不小于n\nerror\n───────────────────────────────\n\033[47;30m");
	}
}
//产生0~1之间的浮点随机数
double rand0(void)
{
	 /* 初始化伪随机数生成器 */    
    srand((unsigned int)time(NULL));
    return (double)rand() / RAND_MAX;
}
/*统计，概率，计算*/
void math_hs(char *str)
{
	char str0[N],*s,*s0;
	char*p[N];
	double x[N][2],d=0;
	int i=0,n=0,j=0;
	strcpy(str0,str);
	s0 = strtok(str0,d2);
	p[i] = strtok(str, d1);
    while ((s = strtok(NULL, d1)))
    {
    	i++;
    	n=i+1;
    	p[i] = s;
    	//printf(">>%s\n",p[i]);
   }
   for (j=0;j<=i;j++)
   {
   	if(strchr(p[j],','))
   	{
   		x[j][0]=atof(strtok(p[j],","));
   		x[j][1]=atof(strtok(NULL,","));
   	}
   	else
   	    x[j][0]=atof(p[j]);
   }
   
   if (strstr(s0,"tj"))
   	tj(x,n);
   if (strstr(s0,"gx"))
   	gx(x,n);
   if (strstr(s0,"pxb"))
   pxb(x);
   if (strstr(s0,"pxh"))
   pxh(x);
   if (strstr(s0,"rand"))
   printf("在0~1之间产生一个随机小数:%f\n",rand0());
   
}
//处理三角函数，对数，指数，阶乘等
double HS(void)
{
	char a[5]={0x00};
	int i=0;
	double acting = FS(),act=0;
	while (isalpha(token[m])||token[m]=='^'||token[m]=='!'||token[m]=='%')
	{
		a[i++]=str[t-1];
		switch (token[m])
		{
			case '^':
			token[++m] = str[t++];
			act=FS();
			printf("\t步骤%d: %g^%g",++x,acting,act);
			acting=pow(acting,act);
			break;
			case '!':
			token[++m] = str[t++];
			printf("\t步骤%d: %g的阶乘",++x,acting);
			acting=jc(acting);
			FS();
			break;
			case '%':
			token[++m] = str[t++];
			printf("\t步骤%d: %g%",++x,acting);
			acting/=100;
			FS();
			break;
			case 'C':
			token[++m] = str[t++];
			act=FS();
			printf("\t步骤%d: 组合%gC%g",++x,acting,act);
			acting=ncr(acting,act);
			break;
			case 'P':
			token[++m] = str[t++];
			act=FS();
			printf("\t步骤%d: 排列%gP%g",++x,acting,act);
			acting=npr(acting,act);
			break;
			default:
			token[++m] = str[t++];
			break;
		}
	}
	//三角函数
	if (islower(a[0]))
	{
		act=FS();
		printf("\t步骤%d: %s(%g)",++x,a,act);
	}
	if(strstr(a,"sin")&&a[0]!='a'&&a[3]!='h')
	acting=sin(jtoh(act));
	if(strstr(a,"cos")&&a[0]!='a'&&a[3]!='h')
	acting=cos(jtoh(act));
	if(strstr(a,"tan")&&a[0]!='a'&&a[3]!='h')
	acting=tan(jtoh(act));
	//反三角函数
	if(strstr(a,"asin"))
	acting=htoj(asin(act));
	if(strstr(a,"acos"))
	acting=htoj(acos(act));
	if(strstr(a,"atan"))
	acting=htoj(atan(act));
	//双曲函数
	if(strstr(a,"sinh"))
	acting=sinh(act);
	if(strstr(a,"cosh"))
	acting=cosh(act);
	if(strstr(a,"tanh"))
	acting=tanh(act);
	//对数函数
	if(strstr(a,"lg"))
	acting=log10(act);
	if(strstr(a,"ln"))
	acting=log(act);
	//开方，指数，随机数
	if(strstr(a,"sqrt"))
	acting=sqrt(act);
	if(strstr(a,"exp"))
	acting=exp(act);
	if(strstr(a,"rand"))
	acting=rand0()*act;

	if (isalpha(a[0])||a[0]=='^'||a[0]=='!'||a[0]=='%')
	printf("=%g\n",acting);
	return acting;
}

/* 处理乘除并计算 */

double CX(void)
{
	double cs;
	double acting = HS(),act=0;
	while ((token[m] == '*') || (token[m] == '/'))
		switch (token[m])
		{
			case '*':
			token[++m] = str[t++];
			act = HS();
			printf("\t步骤%d: %g×%g",++x,acting,act);
			acting*=act;
			printf("=%g\n",acting);
			break;
			case '/':
			token[++m] = str[t++];
			cs = HS();
			if (cs == 0)/*除数为0时*/
			{
			printf("除数不能为零");
			}
			printf("\t步骤%d: %g÷%g",++x,acting,cs);
			acting/=cs;
			printf("=%g\n",acting);
			break;
		}
	return acting;
}

/* 处理加减并计算 */
double JJ(void)
{
	double acting = CX();
	double act=0;
	while ((token[m] == '+') || (token[m] == '-'))
		switch (token[m])
		{
			case '+':
			token[++m] = str[t++];
			act = CX();
			printf("\t步骤%d: %g+%g",++x,acting,act);
			acting+=act;
			printf("=%g\n",acting);
			break;
			case '-':
			token[++m] = str[t++];
			act = CX();
			printf("\t步骤%d: %g-%g",++x,acting,act);
			acting-=act;
			printf("=%g\n",acting);
			break;
		}
	return acting;
}

/* 处理括号,数字,小数点 */
double FS(void)
{
	double acting;
	char number[N];
	int i = 0;
	if (token[m] == '(')
	{
		token[++m] = str[t++];
		acting = JJ();
		if (token[m] == ')')
			token[++m] = str[t++];
	}
	else if (isdigit(token[m]) || token[m] == '.')
	{
		while (isdigit(token[m]) || token[m] == '.')	/* 将字符串转换为浮点数  */
		{
			number[i++] = token[m++];
			token[m] = str[t++];
		}
		number[i] = '\0';
		acting = atof(number);
	}
	return acting;
}

/*主函数*/
int main(void)
{
	double jg;
	int ms;
	printf("\033[47;34;5m\033[2J");
	//界面文本初始化
	printf(S1);
	while (1)//主循环
	{
		printf("->请输入表达式以回车键结束：\n  ");
		scanf("%s",str);
		ms=0;
		if (strstr(str,"tj"))
		{
			printf(S3);
			ms=1;
			while (ms)
			{
				scanf("%s",str);
				if (strstr(str,"exit"))
				{
					printf("\033[47;35m->已退出统计模式\n───────────────────────────────\n->查看函数说明请输入:hs\n->数据统计模式请输入:tj\n->输入表达式开始计算\n───────────────────────────────\n\033[47;30m");
				ms=0;
				strcpy(str,"0");
				}
				else
				if (strpbrk(str,d2)==NULL||strpbrk(str,d1)==NULL)
				printf("->输入错误\n");
				else
				math_hs(str);
			}
		}
		if (strstr(str,"hs"))
		{
		printf(S2);
		ms=1;
		}
		if (ms==0)
		{
		if ( str[0] == '-' || str[0] == '+' || str[0] == '*' || str[0] == '/' )
		{
			for ( int b = n;b >=1;b-- )
			str[b] = str[b-1];
			str[0] = '0';
		}
		c = 1;
		m = t = 0;
		token[m] = str[t++];
		printf("\033[47;34m");
		jg=JJ();
		printf("\033[47;31m\t计算结果: ");	
		printf("\033[47;31m=%0.9g\n\033[47;30m",jg);
		printf("───────────────────────────────\n");
		if (c == 1)//计算后，str清零
		{
			strcpy(str,"\0");
			x=c=jg = 0;
		}
		}
	}
	return 0;
}