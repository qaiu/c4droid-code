/*c4droid代码手册
 *选项
 *黑雪路西法编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include <iostream>
#include <conio.h>
using namespace std;
int choose(int, int, int);		// 记得用gotoxy完成选项
int main()
{								// gotoxy的作用是跳转光标的坐标横右纵下为正
	gotoxy(20, 1);
	cout << "选项";
	gotoxy(20, 2);
	cout << "1:A";
	gotoxy(20, 3);
	cout << "2:B";				// 你的箭头在选项前一格，,故横坐标减一
	switch (choose(19, 2, 2))
	{
	case 1:
		{
			clrscr();
			// 作用是清屏
			cout << "你选择的是A";
			break;
		}
	case 2:
		{
			clrscr();
			cout << "你选择的是B";
			break;
		}
	default:
		break;
	}
	return 0;
}

int choose(int x, int y, int c)
{
	gotoxy(x, y);
	cout << ">";
	int start = y, end = y + c - 1;
	char ch;
	bool quit = false;
	while (!quit)
	{
		ch = getch();
		switch (ch)
		{
		case 'w':
			{
				if (y > start)
				{
					y--;
					gotoxy(x, y + 1);
				}
				else
				{
					y = end;
					gotoxy(x, start);
				}
				cout << " ";
				gotoxy(x, y);
				cout << ">";
				break;
			}
		case 's':
			{
				if (y < end)
				{
					y++;
					gotoxy(x, y - 1);
				}
				else
				{
					y = start;
					gotoxy(x, end);
				}
				cout << " ";
				gotoxy(x, y);
				cout << ">";
				break;
			}
		case '\n':
			{
				quit = true;
				break;
			}
		}
	}
	return y - c + 1;
}

