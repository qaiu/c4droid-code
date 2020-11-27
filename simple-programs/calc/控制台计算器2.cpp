/* 
 *控制台版计算器
 *by 百度贴吧 @QingFLS
 *
 *设计思路：
 *1、从标准输入读取算式，并分离数字
 *和运算符；且在负号前添0
 *2、将中缀表达式转换为后缀表达式
 *3、利用后缀表达式进行运算
 *☆注：
 *第三步的运算原想通过高精度进行
 *无奈高精度有点复杂＞_＜
 *故暂且改为浮点数运算，高精度
 *留待来日改进
 */


#include <iostream>
// #include <fstream>
#include <cstdio>
#include <string>

using namespace std;

// 表达式
string formula;
const int MAXL = 100 + 5;
// 表达式中元素(符号&数字)的数量
int cnt = 0;
// 中缀表达式
string infix[MAXL];
// 后缀表达式
string suffix[MAXL];
// 临时栈
char stack1[MAXL];
string stack2[MAXL];
double stack3[MAXL];

// 错误处理
int errorbox = 0;

const int ILLEGAL_CHARACTER = 1;
const int BRACKET_ERROR = 2;
const int FORMULA_TOO_LONG = 3;

string errorstr[5] = { "NO ERROR or UNKNOWN ERROR", "ILLEGAL_CHARACTER", "BRACKET_ERROR",
	"FORMULA_IS_TOO_LONG"
};

// 判断为数字or运算符
inline bool is_num(char c)
{
	return ((c >= '0' && c <= '9') || c == '.');
}

inline bool is_saf(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}

inline bool is_blank(char c)
{
	return c == ' ';
}

// 运算符优先级(priority level)
int get_plvl(char c)
{
	switch (c)
	{
	case '(':
		return 0;
		break;
	case '+':
		return 1;
		break;
	case '-':
		return 1;
		break;
	case '*':
		return 2;
		break;
	case '/':
		return 2;
		break;
	}
}

// 初始化
void init();
// 读取并储存
void input();
// 将string转为double
double trans(string & num);
// 识别运算符进行运算
double cal(double &num1, double &num2, char &c);
// 程序主体
double calculate();

int main(void)
{
	cout << "-----控制台计算器-----\n" "----------by-QingFLS\n" "\n";
	init();
	while (1)
	{
		input();
		if (!errorbox)
		{
			cout << formula << " = \n" << calculate() << endl;
		}
		else
			cout << "ERROR!!!\n" << formula << endl << errorstr[errorbox] << endl;

		init();
	}
	return 0;
}

// 初始化
void init()
{
	formula.clear();
	// 表达式中元素(符号&数字)的数量
	cnt = 0;
	// 中缀表达式
	for (int i = 0; i != MAXL; i++)
		infix[i].clear();
	// 后缀表达式
	for (int i = 0; i != MAXL; i++)
		suffix[i].clear();
	// 临时栈
	for (int i = 0; i != MAXL; i++)
		stack1[i] = 0;
	for (int i = 0; i != MAXL; i++)
		stack2[i].clear();
	for (int i = 0; i != MAXL; i++)
		stack3[i] = 0.0;
	// 错误处理
	errorbox = 0;
}

void input()
{
	/* 从文件读取 ifstream fin("in.txt"); getline(fin, formula, '\n');
	   fin.close(); */
	cout << "PLEASE INPUT YOUR FORMULA:\n\n";
	getline(cin, formula, '\n');

	// 第一步简单错误处理
	int brk = 0;
	for (int i = 0; i != formula.length(); i++)
	{
		// 存在非法字符
		if (!(is_num(formula[i]) || is_saf(formula[i]) || is_blank(formula[i])))
		{
			errorbox = ILLEGAL_CHARACTER;
			return;
		}

		// 括号不匹配
		if (formula[i] == '(' || formula[i] == ')')
			brk++;
	}

	if (brk % 2 != 0)
	{
		errorbox = BRACKET_ERROR;
		return;
	}

	// 若第一步无错误
	// 将表达式各元素分离，存为中缀表达式
	for (int i = 0; i < formula.length() && cnt < MAXL;)
	{
		// 跳过空格
		if (formula[i] == ' ')
		{
			i++;
			continue;
		}
		// 存运算符
		if (is_saf(formula[i]))
		{
			infix[cnt] = formula[i];
			i++;
		}
		// 存数字
		else
			for (int k = 0; i < formula.length() && is_num(formula[i]); k++)
			{
				infix[cnt] += formula[i];
				i++;
			}
		cnt++;
	}

	// 处理负数（在负号前插入0）
	int p = 1;
	// 若首位为负号
	if (infix[0][0] == '-')
	{
		cnt++;
		for (int i = cnt - 1; i != 0; i--)
			infix[i] = infix[i - 1];
		infix[0] = "0";
	}
	// 在表达式其它地方寻找负号
	while (p != cnt)
	{
		if (infix[p][0] == '-' && infix[p - 1][0] == '(')
		{
			cnt++;
			for (int i = cnt - 1; i != p; i--)
				infix[i] = infix[i - 1];
			infix[p] = "0";
		}
		p++;
	}
	/* 
	   //Test,cnt为表达式中元素个数 for(int t=0;t!=cnt;t++)
	   cout<<infix[t]<<endl; cout<<"Cnt="<<cnt<<endl; */

	// 第二步简单错误处理
	// 如果表达式元素数量超出范围
	if (cnt > MAXL - 5)
	{
		errorbox = FORMULA_TOO_LONG;
		return;
	}
}

double calculate()
{
	// 将中缀表达式转为后缀表达式
	int top = 0;				// suffix栈顶
	int stk = 0;				// stack1栈顶

	for (int i = 0; i != cnt; i++)
	{
		if (is_num(infix[i][0]))
		{
			suffix[top++] = infix[i];
		}
		else
			switch (infix[i][0])
			{
			case '(':
				stack1[stk++] = '(';
				break;
			case ')':
				while (stk > 0)
					if (stack1[stk - 1] != '(')
					{
						stk--;
						suffix[top++] = stack1[stk];
					}
					else
					{
						stk--;
						stack1[stk] = '\0';
						break;
					}
				break;
			default:
				while (stk > 0 && get_plvl(infix[i][0]) <= get_plvl(stack1[stk - 1]))
				{
					suffix[top] = stack1[stk - 1];
					top++;
					stk--;
				}
				stack1[stk++] = infix[i][0];
				break;
			}
	}

	for (; stk != 0; stk--)
		suffix[top++] = stack1[stk - 1];

	// 利用后缀表达式进行栈运算
	/* 
	   //Test for(int t = 0; t != top; t++) cout<<suffix[t]<<endl;
	   cout<<"Top="<<top<<endl; */

	// 将string存储的数字转为double
	// 有较大精度损失，留待以后改进

	stk = 0;					// stack3栈顶
	for (int i = 0; i != top; i++)
	{
		if (is_num(suffix[i][0]))
		{
			// 将string转为double
			stack3[stk++] = trans(suffix[i]);
		}
		else
		{
			stk -= 2;
			stack3[stk] = cal(stack3[stk], stack3[stk + 1], suffix[i][0]);
			stk++;
		}
	}
	stk = 0;

	return stack3[0];
}

double trans(string & num)
{
	double result = 0.0;
	// 记录小数点出现
	int dot = -1;

	for (int j = 0; j != num.length(); j++)
	{
		if (num[j] == '.')
		{
			dot = j;
			continue;
		}
		result *= 10;
		result += (num[j] - 48);
	}
	for (int k = 0; dot != -1 && k != num.length() - dot - 1; k++)
		result /= 10;

	return result;
}

double cal(double &num1, double &num2, char &c)
{
	switch (c)
	{
	case '+':
		return (num1 + num2);
		break;
	case '-':
		return (num1 - num2);
		break;
	case '*':
		return (num1 * num2);
		break;
	case '/':
		return (num1 / num2);
		break;
	default:
		break;
	}
}

