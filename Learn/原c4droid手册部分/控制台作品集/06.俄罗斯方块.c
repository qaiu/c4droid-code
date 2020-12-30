/*c4droid代码手册
 *俄罗斯方块
 *Junye编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include <iostream>
#include<cstdlib>
#include<conio.h>
#include<ctime>
#define H 18
#define W 22
#define N 2
//每1/Ns移动一次
#define TL 'a'
#define TR 'd'
#define TD 's'
#define Revolve 'w'
//wsad控制移动和翻转
using namespace std;
void judge();
void draw();
void getcube();
void over();
int i,j,choice;
static int score;
char map[H][W];
char *p,*q;
char t;
//方块基类
class cube
{
	public:char *x[10];
	cube();
	virtual ~cube(){}
	virtual int revolve_move(){}
	int turn(char de);
};
cube::cube()
{
	for(i=0;i!=10;++i)
	x[i]=NULL;}
//方块一至七
class cube1:public cube
{
	public :cube1();
	~cube1(){}
	int revolve_move();
};
class cube2:public cube
{
	public :cube2();
	~cube2(){}
	int revolve_move();
};
class cube3:public cube
{
	public :cube3();
	~cube3(){}
	int revolve_move();
};
class cube4:public cube
{
	public:int position;
//position记录当前形态
	cube4();
	~cube4(){}
	int revolve_move();
};
class cube5:public cube
{
	public:int position;
	cube5();
	~cube5(){}
	int revolve_move();
};
class cube6:public cube
{
	public:cube6();
	~cube6(){}
	int revolve_move();
};
class cube7:public cube
{
	public:cube7();
	~cube7(){}
	int revolve_move();
};

//通用移动函数
int cube::turn(char de)
{
	int jl=1,jr=1,jd=1;
	//判断可否左，右，下移
	for(i=0;i!=8;i++)
	{
		if(*(x[i]-2)!='0'&&*(x[i]-2)!='2')jl=0;
		if(*(x[i]+2)!='0'&&*(x[i]+2)!='2')jr=0;
    	if(*(x[i]+W)!='0'&&*(x[i]+W)!='2')jd=0;	}
    switch(de)
    {
    	case TL:if(jl==1){
    		for(i=0;i!=8;i++)
    		{*x[i]='0';x[i]=x[i]-2;}
    		for(i=0;i!=8;++i){
    		*x[i]='2';}
    		x[8]=x[8]-2;x[9]=x[9]-2;}break;
    	case TR:if(jr==1){
    		for(i=0;i!=8;i++)
    		{*x[i]='0';x[i]=x[i]+2;}
    		for(i=0;i!=8;++i){
    		*x[i]='2';}
    		x[8]=x[8]+2;x[9]=x[9]+2;}break;
    case TD:
    if(jd==1){
    for(i=0;i!=8;++i){
    	*x[i]='0';x[i]=x[i]+W;}
    	for(i=0;i!=8;++i)
    	*x[i]='2';
    	x[8]=x[8]+W;x[9]=x[9]+W;}
    else if(jd==0){
    	for(i=0;i!=8;++i)
        *x[i]='1';
    	judge();
    	return 9;
    	}
    }
    return 0;
}	
//判断游戏结束和消去行
void judge()
{
//判断顶部生成区是否存在方块	
if(map[1][W/2]=='1'||map[1][W/2+1]=='1')over();
	int full[H-1];
	dele:;	
	for(i=0;i!=H-1;++i)
	{
		full[i]=1;
		for(j=1;j!=W-1;++j)
		{
			if (map[i][j]!='1')full[i]=0;
			//未满则full[i]变为0
		}
	}
	for(i=0;i!=H-1;++i)
	{
		if (full[i]==1)
		//满了便消去一行
		{
			for(int ii=i;ii!=0;--ii)
			{
				for(j=1;j!=W-1;++j)
			map[ii][j]=map[ii-1][j];
			}
		for(j=1;j!=W-1;++j)
		map[0][j]='0';
		score+=500;
		goto dele;
		//消去一行后重新检测有无满的行
		}
	}
}

//方块一类建立(倒T型)
cube1::cube1():cube()
{
	x[0]=&map[0][W/2];x[1]=&map[0][W/2+1];
	x[8]=&map[2][W/2];x[9]=&map[2][W/2+1];
    for(i=2;i!=8;++i)
    x[i]=&map[1][W/2-4+i];
    for(i=0;i!=8;++i)
    *x[i]='2';
    *x[9]=*x[8]='0';
}

//方块一旋转和移动判断
int cube1::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
	if(*x[9]=='0'&&*x[8]=='0'){
		p=x[8];q=x[9];
		x[8]=x[2];x[9]=x[3];
		x[2]=x[0];x[3]=x[1];
		x[0]=x[6];x[1]=x[7];
		x[6]=p;x[7]=q;
		for(i=0;i!=8;++i)
		*x[i]='2';
		*x[8]=*x[9]='0';break;}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}

//方块二类建立(田字型)
cube2::cube2()
{
	for(i=0;i!=4;++i)
	x[i]=&map[0][W/2-2+i];
	for(i=4;i!=8;++i)
	x[i]=&map[1][W/2-6+i];
	for(i=0;i!=8;++i)
	*x[i]='2';
	x[8]=&map[2][W/2];
	x[9]=&map[2][W/2+1];
	*x[8]=*x[9]='0';
}
//方块二旋转和移动判断
int cube2::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==TL)
	turn(TL);
	else if(t==TR)
	turn(TR);
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
//方块三类建立(一字型)
cube3::cube3():cube()
{
	for(i=0;i!=10;++i)
	{x[i]=&map[0][W/2-4+i];*x[i]='2';}
	*x[8]=*x[9]='0';
}
//方块三旋转和移动判断
int cube3::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
		if(x[0]==x[4]-4)
		{if(*(x[4]-W)=='0'&&*(x[4]-2*W)=='0'&&*(x[4]+W)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			x[0]=x[4]-2*W;x[1]=x[5]-2*W;
			x[2]=x[4]-W;x[3]=x[5]-W;
			x[6]=x[4]+W;x[7]=x[5]+W;
			for(i=0;i!=8;++i)
			*x[i]='2';
			break;
		}
		}
		else
		{if(*(x[4]-4)=='0'&&*(x[4]-2)=='0'&&*(x[4]+2)=='0')
		{
		for(i=0;i!=8;++i)
			*x[i]='0';
		x[0]=x[4]-4;x[1]=x[4]-3;
		x[2]=x[4]-2;x[3]=x[4]-1;
		x[6]=x[4]+2;x[7]=x[4]+3;
		for(i=0;i!=8;++i)
			*x[i]='2';
		break;
		}}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
//方块四类建立(L型)
cube4::cube4()
{
	position=1;
	x[0]=&map[0][W/2-2];
	x[1]=x[0]+1;
	x[8]=x[1]+1;
	x[9]=x[8]+2*W;
	*x[8]=*x[9]='0';
	*x[0]=*x[1]='2';
	for(i=2;i!=8;++i)
	{x[i]=&map[1][W/2-4+i];*x[i]='2';}
}
//方块四旋转和移动判断
int cube4::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
		switch(position%4)
	{   case 1:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[8]+2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[2]+2;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 2:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[6]+2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[2]+W;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 3:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[8]-2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[2]-2;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 0:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[6]-2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[2]-W;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
	}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
//方块五类建立(L型，与四对称)
cube5::cube5()
{
	position=1;
	x[0]=&map[0][W/2+2];
	x[1]=x[0]+1;
	x[8]=x[0]-2;
	x[9]=x[8]+2*W;
	*x[8]=*x[9]='0';
	*x[0]=*x[1]='2';
	for(i=2;i!=8;++i)
	{x[i]=&map[1][W/2-4+i];*x[i]='2';}
}
//方五旋转和移动判断
int cube5::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
		switch(position%4)
	{   case 1:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[9]+2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[6]+2;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 2:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[6]-2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[6]+W;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 3:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[9]-2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[6]-2;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
		case 0:
		if(*x[8]=='0'&&*x[9]=='0'&&*(x[6]+2)=='0')
		{
			for(i=0;i!=8;++i)
			*x[i]='0';
			p=x[9];
			x[9]=x[2];
			x[2]=x[8];x[3]=x[2]+1;
			x[8]=x[6];
			x[6]=p;x[7]=p+1;
			x[0]=x[6]-W;
			x[1]=x[0]+1;
			for(i=0;i!=8;++i)
			*x[i]='2';
			position++;break;
		}
	}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
//方块六类建立(S型)
cube6::cube6():cube()
{
    for(i=0;i!=4;++i)
    {x[i]=&map[0][W/2+i];*x[i]='2';}
    for(i=4;i!=8;++i)
    {x[i]=&map[1][W/2-6+i];*x[i]='2';}
    x[8]=x[7]+1;
    x[9]=x[8]+W;
    *x[9]=*x[8]='0';
}

//方块六旋转和移动判断
int cube6::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
		if(x[4]==x[6]-2)
		{if(*x[8]=='0'&&*x[9]=='0')
		{for(i=0;i!=8;++i)*x[i]='0';
		p=x[8];x[8]=x[4];x[4]=x[0];
		x[5]=x[4]+1;x[0]=p;x[1]=x[0]+1;
		p=x[2];x[2]=x[9];x[9]=p;
		x[3]=x[2]+1;
		for(i=0;i!=8;++i)*x[i]='2';break;}
		}
		else if(*x[8]=='0'&&*x[9]=='0')
		{for(i=0;i!=8;++i)*x[i]='0';
			p=x[8];x[8]=x[0];x[0]=x[4];
			x[1]=x[0]+1;x[4]=p;x[5]=x[4]+1;
			p=x[2];x[2]=x[9];x[9]=p;
	    	x[3]=x[2]+1;
	    	for(i=0;i!=8;++i)*x[i]='2';
		}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
//方块七类建立(S型,与六对称)
cube7::cube7():cube()
{
    for(i=0;i!=4;++i)
    {x[i]=&map[0][W/2+i-2];*x[i]='2';}
    for(i=4;i!=8;++i)
    {x[i]=&map[1][W/2-4+i];*x[i]='2';}
    x[8]=x[4]-2;
    x[9]=x[8]+W;
    *x[9]=*x[8]='0';
}

//方块七旋转和移动判断
int cube7::revolve_move()
{
	int ju;
	while(kbhit())
	{t=getch();
	if(t==Revolve)
	{
		if(x[4]==x[6]-2)
		{if(*x[8]=='0'&&*x[9]=='0')
		{for(i=0;i!=8;++i)*x[i]='0';
		p=x[8];x[8]=x[6];x[6]=x[2];
		x[7]=x[6]+1;x[2]=p;x[3]=x[2]+1;
		p=x[0];x[0]=x[9];x[9]=p;
		x[1]=x[0]+1;
		for(i=0;i!=8;++i)*x[i]='2';break;}
		}
		else if(*x[8]=='0'&&*x[9]=='0')
		{for(i=0;i!=8;++i)*x[i]='0';
			p=x[8];x[8]=x[2];x[2]=x[6];
			x[3]=x[2]+1;x[6]=p;x[7]=x[6]+1;
			p=x[0];x[0]=x[9];x[9]=p;
	    	x[1]=x[0]+1;
	    	for(i=0;i!=8;++i)*x[i]='2';
		}
	}
	else if(t==TL)
	{turn(TL);}
	else if(t==TR)
	{turn(TR);}
	else if(t==TD)
	{ju=turn(TD);
	if(ju==9)return ju;}
	usleep(1000000/25);
	clrscr();draw();}
	ju=turn(TD);
	return ju;
}
void over()
{
	clrscr();
	cout<<"Game over!!!"<<endl<<"Your scores :"<<score;
	cout<<endl<<"Bye!";
	exit(1);
}
int main()
{
	cout<<"\t/**********Welcome**********/"<<endl<<"\t\tStart game ? (y or n)\n";
	t=cin.get();
	if(t=='n')goto end;
	clrscr();
	srand(time(0));
	score=0;
	for(i=0;i!=H;++i)
	for(j=0;j!=W;++j)
	{if ((!j)||(i==H-1)||(j==W-1))
	map[i][j]='9';
	else map[i][j]='0';}
    getcube();
	clrscr();
	end:;
	cout<<"Bye!";
}
//获得一个方块
void getcube()
{
	score+=60;
	choice=rand()%7+1;
	cube *now;
	switch(choice)
	{
		case 1:now=new cube1();break;
		case 2:now=new cube2();break;
		case 3:now=new cube3();break;
		case 4:now=new cube4();break;
		case 5:now=new cube5();break;
		case 6:now=new cube6();break;
		case 7:now=new cube7();break;
	}
	clrscr();draw();
	while(1)
	{usleep(1000000/N);
	choice=now->revolve_move();
	//如果方块到底，再来一个
	if(choice==9){
		delete now;
		getcube();}
	clrscr();
	draw();
	}
}
//输出地图
void draw()
{ for(i=0;i!=H;++i){
	for(j=0;j!=W;++j){
		switch(map[i][j])
		{case '0':cout<<" ";break;
		case '1':if(j%2)cout<<"[";
		else cout<<"]";break;
		case '2':
		if(j%2)cout<<"[";
		else cout<<"]";
		break;
		case '9':cout<<"%";break;}
	}
	if(i==2)cout<<"\t\tScore :"<<score;
	cout<<endl;
}
}