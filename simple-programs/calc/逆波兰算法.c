// 来自百度百科
#include<stdlib.h>
#include<stdio.h>
#define max 100
char ex[max];					/* 存储后缀表达式 */

void trans()
{								/* 将算术表达式转化为后缀表达式 */
	char str[max];				/* 存储原算术表达式 */
	char stack[max];			/* 作为栈使用 */
	char ch;
	int sum, i, j, t, top = 0;
	printf("*****************************************\n");
	printf("*输入一个求值的表达式，以#结束。*\n");
	printf("******************************************\n");
	printf("算数表达式：");
	i = 0;						/* 获取用户输入的表达式 */
	do
	{
		i++;
		// cin>>str[i];/*此步我用的是C++C语言的话在后面之所以用这个有一点区别都*/
		scanf("%c", &str[i]);
	}
	while (str[i] != '#' && i != max);
	sum = i;
	t = 1;
	i = 1;
	ch = str[i];
	i++;
	// 
	while (ch != '#')
	{
		switch (ch)
		{
		case '(':				/* 判定为左括号 */
			top++;
			stack[top] = ch;
			break;
		case ')':				/* 判定为右括号 */
			while (stack[top] != '(')
			{
				ex[t] = stack[top];
				top--;
				t++;
			}
			top--;
			break;
		case '+':				/* 判定为加减号 */
		case '-':
			while (top != 0 && stack[top] != '(')
			{
				ex[t] = stack[top];
				top--;
				t++;
			}
			top++;
			stack[top] = ch;
			break;
		case '*':				/* 判定为乘除号 */
		case '/':
			while (stack[top] == '*' || stack[top] == '/')
			{
				ex[t] = stack[top];
				top--;
				t++;
			}
			top++;
			stack[top] = ch;
			break;
		case ' ':
			break;
		default:
			while (ch >= '0' && ch <= '9')
			{					/* 判定为数字 */
				ex[t] = ch;
				t++;
				ch = str[i];
				i++;
			}
			i--;
			ex[t] = ' ';
			t++;
		}
		ch = str[i];
		i++;
	}
	while (top != 0)
	{
		ex[t] = stack[top];
		t++;
		top--;
	}
	ex[t] = ' ';
	printf("\n\t原来表达式：");
	for (j = 1; j < sum; j++)
		printf("%c", str[j]);
	printf("\n\t逆波兰式：", ex);
	for (j = 1; j < t; j++)
		printf("%c", ex[j]);
}

void compvalue()
{								/* 计算后缀表达式的值 */
	float stack[max], d;		/* 作为栈使用 */
	char ch;
	int t = 1, top = 0;			/* t为ex下标，top为stack下标 */
	ch = ex[t];
	t++;
	while (ch != ' ')
	{
		switch (ch)
		{
		case '+':
			stack[top - 1] = stack[top - 1] + stack[top];
			top--;
			break;
		case '-':
			stack[top - 1] = stack[top - 1] - stack[top];
			top--;
			break;
		case '*':
			stack[top - 1] = stack[top - 1] * stack[top];
			top--;
			break;
		case '/':
			if (stack[top] != 0)
				stack[top - 1] = stack[top - 1] / stack[top];
			else
			{
				printf("\n\t除零错误!\n");
				// exit(0);/*异常退出*/
			}
			top--;
			break;
		default:
			d = 0;
			while (ch >= '0' && ch <= '9')
			{
				d = 10 * d + ch - '0';	/* 将数字字符转化为对应的数值 */
				ch = ex[t];
				t++;
			}
			top++;
			stack[top] = d;
		}
		ch = ex[t];
		t++;
	}
	printf("\n\t计算结果:%g\n", stack[top]);
}

int main()
{
	trans();
	compvalue();
	// system("pause");//win平台使用此函数
	return 0;
}