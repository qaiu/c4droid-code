/*
例程108. 递归四则运算
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24

对四则混合运算所提取的形式化表达式(生成式)
<exp> -> <term> { <addop> <term> }
<addop> -> + | -
<term> -> <factor> { <mulop> <factor> }
<mulop> -> * | /
<factor> -> ( <exp> ) | Number
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
char token; /*全局标志变量*/

/*递归调用的函数原型*/
double exp( void );
double term( void );
double factor( void );

void error( void ) /*报告出错信息的函数*/
{
	printf( "错误\n");
//	exit( 1 );
}

void match( char expectedToken ) /*对当前的标志进行匹配*/
{
	if( token == expectedToken ) token = getchar(); /*匹配成功，获取下一个标志*/
	else error(); /*匹配不成功，报告错误*/
}
void Message(void)
{
	printf("============================================================\n");
	printf("*             递归实现的四则运算表达式求值程序             *\n");
	printf("************************************************************\n");
	printf("使用方法:请从键盘上直接输入表达式,以回车键结束\n不输入任何数而直接按[回车]键,将退出程序\n如45*(12-2)[回车]\n");
	printf("************************************************************\n\n");
}
int main()
{
	double result;  /*运算的结果*/
	Message();
	tt:
		printf(" >> 请输入表达式: ");
	token = getchar(); /*载入第一个符号*/
	result = exp(); /*进行计算*/
	if( token == 10) /* 是否一行结束 */
		printf( " >> 表达式的计算结果为 : %lf\n", result );
	else error();/* 出现了例外的字符 */
	goto tt;
}

double exp( void )
{
	double temp = term(); /*计算比加减运算优先级别高的部分*/
	while(( token == '+' ) || ( token == '-' ))
		switch( token ) {
		case '+': match('+');     /*加法*/
			  temp += term();
			  break;
		case '-': match('-');
			  temp -= term(); /*减法*/
			  break;
		}
	return temp;
}

double term( void )
{
	int div; /*除数*/
	double temp = factor();   /*计算比乘除运算优先级别高的部分*/
	while(( token == '*' ) || ( token == '/' ))
		switch( token ) {
		case '*': match('*');  /*乘法*/
			  temp *= factor();
			  break;
		case '/': match('/');   /*除法*/
			  div = factor();
			  if( div == 0 ) /*需要判断除数是否为0*/
			  {
			  	fprintf( stderr, "除数为0.\n" );
			  	exit(1);
			  }
			  temp /= div; 
			  break;
		}
	return temp;
}

double factor( void )
{
	double temp; 
	if( token == '(' ) /*带有括号的运算*/
	{
		match( '(' );
		temp = exp();
		match(')');
	}
	else if ( isdigit( token )) /*实际的数字*/
	{
		ungetc( token, stdin ); /*将读入的字符退还给输入流*/
		scanf( "%lf", &temp ); /*读出数字*/
		token = getchar();  /*读出当前的标志*/
	}
	else if ( token='#') 
	exit(0);
	else error(); /*不是括号也不是数字*/
	return temp;
}