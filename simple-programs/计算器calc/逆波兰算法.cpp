/**
 * 逆波兰(后缀)表达式计算器，仅支持double精度的四则运算
 * 转自某CSDN博客，稍作修改
 * 
 * 输入表达式回车进行计算，\q 退出
 * @collector QAIU
 * @date：17/04/22
 */
#include <cstring>
#include <stdexcept>
#include <iostream>
using namespace std;//使用std命名空间
template<class T = char, size_t SIZE = 256>
//定义模板类
class Stack
 {
 //桟操作相关函数
public:
    Stack (void) : m_uTop (0) {}
    void Push (const T& tData) 
    {
        if (Full ())
            throw overflow_error ("堆栈满！");
        m_tData[m_uTop++] = tData;
    }
    
    T Pop (void)
    {
        if (Empty ())
            throw underflow_error ("堆栈空！");
        return m_tData[--m_uTop];
    }
    const T& Top (void) const 
    {
        if (Empty ())
            throw underflow_error ("堆栈空！");
        return m_tData[m_uTop-1];
    }
    bool Full (void) const 
    {
        return m_uTop >= SIZE;
    }
    bool Empty (void) const 
    {
        return m_uTop == 0;
    }
private:
    T m_tData[SIZE];
    size_t m_uTop;
};
bool Prior (char cOp1, char cOp2) 
{
//符号判断函数，返回布尔类型
    return (cOp1 == '*' || cOp1 == '/') && (cOp2 == '+' || cOp2 == '-');
}
// 将中缀表达式转换为后缀（逆波兰）表达式
string Infix2Suffix (const string& strInfix) 
{
    string strSuffix;
    string::size_type uLength = strInfix.length ();
    Stack<> stack;
    for (string::size_type i=0; i<uLength; i++)
    {
        // 数字和小数点
        if (isdigit(strInfix[i])||strInfix[i]=='.')
            // 直接入表达式
            strSuffix += strInfix[i];
        // 左括号
        else if (strInfix[i] == '(')
            // 直接入栈
            stack.Push (strInfix[i]);
        // 右括号
        else if (strInfix[i] == ')') {
            // 将相应左括号后的运算符栈入表达式
            while (stack.Top () != '(')
                (strSuffix += ' ') += stack.Pop ();
            stack.Pop (); // 最后弹出左括号
        }
        // 运算符
        else 
        if (strchr ("+-*/", strInfix[i]))
        {
            strSuffix += ' ';
            // 该运算符不优先于栈顶，栈入表达式
            while (! stack.Empty () && stack.Top () != '(' && ! Prior (strInfix[i], stack.Top ()))
                (strSuffix += stack.Pop ()) += ' ';
            // 优者入栈
            stack.Push (strInfix[i]);
        }
    }
    // 余者栈入表达式
    while (! stack.Empty ())
        (strSuffix += ' ') += stack.Pop ();
    return strSuffix;
}
// 从后缀表达式中提取运算数
double ExtractOperand (const string& strSuffix, string::size_type& uPos)
{
    double fOperand = 0;
    // 处理整数部分
    for (; isdigit (strSuffix[uPos]); uPos++)
        fOperand = fOperand * 10 + strSuffix[uPos] - '0';
    // 处理小数部分
    if (strSuffix[uPos] == '.')
    {
        string::size_type uDecimals = 0;
        for (uPos++; isdigit (strSuffix[uPos]); uPos++)
        {
            fOperand = fOperand * 10 + strSuffix[uPos] - '0';
            uDecimals++;
        }
        for (; uDecimals; uDecimals--)
            fOperand /= 10;
    }
    return fOperand;
}
//计算逆波兰表达式
double CalSuffix (const string& strSuffix)
{
    string::size_type uLength=strSuffix.length ();
    Stack<double> stack;
    for (string::size_type i = 0; i<uLength;i++) 
   {
        if (isdigit (strSuffix[i]))
            stack.Push(ExtractOperand(strSuffix,i));
        else
        if (strchr (" \t\r\n", strSuffix[i]))
            i++;
        else 
        if (strchr ("+-*/", strSuffix[i])) 
       {
            double fOperand2 = stack.Pop ();
            double fOperand1 = stack.Pop ();
            switch (strSuffix[i])
            {
                case '+':
                    stack.Push(fOperand1+fOperand2);
                    break;
                case '-':
                    stack.Push(fOperand1-fOperand2);
                    break;
                case '*':
                    stack.Push(fOperand1*fOperand2);
                    break;
                case '/':
                    stack.Push(fOperand1/fOperand2);
                    break;
            }
            i++;
        }
    }
 
    return stack.Top ();
}

int main(void)
{
	string str;
	while (true)
	{
		cout << "\n请输入表达式：";
		cin >> str;
		if (str == "\q") break;
		cout << "\t后缀表达式：" << Infix2Suffix(str) << endl;
		cout << "\t计算结果=" << CalSuffix(Infix2Suffix(str)) << endl;
	}
	return 0;
}

