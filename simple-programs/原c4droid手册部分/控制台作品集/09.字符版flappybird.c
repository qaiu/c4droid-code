/*c4droid代码手册
*字符版flappy bird。
*作者 2002a1116
*转载请注明出处
*/
#include<iostream>
#include<conio.h>
#include<stdlib.h>
//作者2002a1116
//参考自be148TTN前辈的参考自啊o额iu鱼前辈的贪吃蛇的字符版跑酷！     @_@好长。
//按任意键向上飞，死亡后按任意键继续。
using namespace std;
const int h=12,w=15;
int wo[h][w];
int *bi=&wo[h/2-1][1];
int hd=h/2-1;
int wou=4;
int wu=0;
int tim=4;
int lwu;
int fly=0;
int s;
void mw()
{
	for(int i=0;i<h;i++)
	{
		for(int f=0;f<w;f++)
		{
			if(wo[i][f]==2)
			{
			wo[i][f-1]=wo[i][f];
			wo[i][f]=0;
			}
		}
	}
    for(int ss=0;ss<h;ss++)
    {
    	if(wo[ss][0]==2)
    	{
    		s++;
    		break;
    	}
    }
	for(int kk=0;kk<w;kk++)
	{
		wo[kk][0]=0;
	}
}
void doza()
{
	tim++;
	if(!(tim%4))
	{
	int a=wu+7;
	for(int i=a;i<h;i++)
	wo[i][14]=2;
	for(int f=wu;f>=0;f--)
	wo[f][14]=2;
	}
}
void dowou()
{
      do
      {
      	wu=random()%4;
      }while((wu-wou)>2||(wou-wu)>2||wu==wou);
      wou=wu;
      
}
void dowo()
{
	for(int i=0;i<h;i++)
	{
		for(int f=0;f<w;f++)
		{
			wo[i][f]=0;
		}
	}
	*bi=1;
}
void out()
{
	usleep(500*20);
	clrscr();
	for(int i=0;i<h;i++)
	{
		for(int f=0;f<w;f++)
		{
			switch(wo[i][f])
			{
				case 1:
			cout<<"  "<<"\033[;43m \033[0m"<<flush;
			break;
			case 2:
			cout<<"\033[;41m   \033[0m"<<flush;
			break;
			case 0:
			cout<<"   "<<flush;
			break;
			different:
			break;
			}
		}
		cout<<endl;
	}
	usleep(5000);
}
bool mbi()
{
	while(kbhit())
	getch();
	usleep(50000*5);
	if(fly)
	{
		fly--;
		goto e;
	}
	else if(kbhit())
	{
		fly=2;
		e:
		if(*bi!=1)
		{
			return false;
		}
			*bi=0;
		bi=&wo[hd--][1];
		if(*bi!=0||hd<0)
		{
			return false;
		}
		else
		{
		*bi=1;
		out();
		return true;
		}
	}
	else
	{
		if(*bi!=1)
		{return false;
		}
		*bi=0;
		bi=&wo[++hd][1];
		if(*bi!=0||hd>=h)
		{
			return false;
		}
		else
		{
		*bi=1;
		out();
		return true;
		}
	}
	
}
int main()
{
	cout<<"按任意键上升"<<endl;
	sleep(2);
	do
	{
		clrscr();
		s=0;
		 bi=&wo[h/2-1][1];
hd=h/2-1;
wou=4;
wu=0;
tim=4;
lwu;
fly=0;
	dowo();
	do
	{
		dowou();
	doza();
	mw();
	}while(mbi());
	clrscr();
	gotoxy(6,7);
	cout<<s<<endl;
	getch();
	}while(true);
}