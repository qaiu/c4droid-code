﻿/*c4droid代码手册
 *石头剪刀布
 *TTHHR编写
 *转载请说明出处
*/
#include<iostream>
#include <cstdlib>
#include<ctime>
using namespace std;
int main()
{
	char *c[3] = { "剪刀", "石头", "布" };
	int pc, df,user, result;
	cout<<"           石头剪刀布";
	cout<<endl<<"初始基金为30，赢+10，输-10。为零时结束游戏";
	cout<<endl;
	df=30;
	while (df)
	{
		srand((unsigned)time(NULL));
		pc = rand() % 3;
		cout << "0=剪刀，1=石头，2=布" << endl << "该你出手了:";
	if	(!(cin >> user))
		{
			cout<<"错误输入，请不要输入非数字！！"<<endl;
			exit(1);
			}
			else
			{
		if (user > 2)
			user = 2;
		cout << "对手出" << c[pc] << ",你出" << c[user] << endl;
		result = pc - user;
		switch (result)
		{
		case -1:
		case 2:
			cout << "\033[45;32m   你胜了##" << endl << endl;
			df=df+10;
			break;
		case 0:
			cout << "\033[35;34m   打成平手。。。\t\t" << endl << endl << endl;
			break;
		default:
			cout << "\033[49;34m   对手胜了～～\t\t" << endl << endl;df=df-10;
			break;
		}
		cout<<"\033[45;33m目前基金为："<<df<<endl;
		if (df==100)
		{
			cout<<"\n\033[45;32m  你赢了！！！"<<endl;
			exit(0);
			}
	}
	}
	cout<<"\033[38;31m     游戏结束";

}