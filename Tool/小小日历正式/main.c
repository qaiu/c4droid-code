/*小小日历
by千百度
date 2017.8.10

本程序天文计算部分参考寿星万年历(js源文件)
复活节日期参考<<天文算法>>
请使用GCC/G++编译器
请调低输入法高度，或使用悬浮输入法

交流 学习 bug反馈QQ736226400
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#include "GeographicalData.h"
#include "MapData.h"
#include "DateInit.h"

int timemap[17][30],timenum[12],cursor[6][7]={};
int dx,dy,dx0,dy0;
char monthviewch[18][7][32]={};
int flag=1,flag0=1,k,mode=0;
LunarDate lunar[32],lunar0;
SolarDate solar,solar0,osolar;
int hourgan,hourzhi;
JINGWEI jw={
	-116.3833333,	39.900000,	"默认","北京"
};

void inittime(void );//初始化实时时钟
void BaZhi(void );//八字
void jumpdate(void );//日期跳转
void CalcQiShuo(int year,int k);//计算气朔
void GetLeapYear(void);//获取闰年
int CalcNaYin(int g,int z);//计算纳音五行
void Menu(void);//菜单
void event(int md);//按键
void initmap(int mode);//初始化地图

void getagoCh(SolarDate solar1,SolarDate solar2,char *chago);
//获取经过的天数
void print_0(void );//打印地图数据
void print_1(void );
void print_2(void );
void drawmap(int mode);//绘图

struct tm *utc;
time_t timer;
int hour0=-1;


int main(void )
{
	printf("\033[?25l"); //隐藏光标
	printf("\nloading...\n如果显示不全，请调整输入法高度或者设置终端字号\n");//伪加载
	usleep(1000000);
	while (1)
	{
		inittime();
		event(mode);
		initmap(mode);
		clrscr();//清屏
		drawmap(mode);//打印地图
		usleep(100000);//延时0.1秒
	}
	return 0;
}

void inittime(void)
{//获取本地时间
	timer = time(NULL);
	utc = localtime(&timer);
	timenum[2]=timenum[5]=10;
	timenum[0]=utc->tm_hour/10;
	timenum[1]=utc->tm_hour%10;
	timenum[3]=utc->tm_min/10;
	timenum[4]=utc->tm_min%10;
	timenum[6]=utc->tm_sec/10;
	timenum[7]=utc->tm_sec%10;
	timenum[8]=osolar.year=utc->tm_year + 1900;
	timenum[9]=osolar.month=utc->tm_mon+1;
	timenum[10]=osolar.day=utc->tm_mday;
	timenum[11]=utc->tm_wday;
//	osolar.day=28;
	if (hour0!=utc->tm_hour)
	{
    	lunar0=GenratedataLunar(osolar);
	    dayganzhi(&lunar0.daygan,&lunar0.dayzhi,osolar);
	    hoursganzhi(&hourgan,&hourzhi,lunar0.daygan,utc->tm_hour);
	}
	hour0=utc->tm_hour;
	if (flag)
	solar=osolar,flag=0;
}

void event(int md)
{
	if (md)
	{
		while (kbhit())
		{
			int key = getch();
			switch (key)
			{
				case '0':
				mode=0;
				break;
			}		
		}
	}
	else
	{
		while (kbhit())
		{
			char key = getch();
			if (key=='1'||key=='3'||key=='7'||key=='9'||key=='0')
			flag0=1;
			if (key=='5')
			{
				int a[6][7]={};
				memcpy(cursor,a,sizeof (cursor));
			}
			switch (key)
			{
				case '0':
		        	solar=osolar;
				break;
				case '1':
				if (solar.month==1)
				solar.month=12;
				else
				solar.month--;
				break;
				case '3':
				if (solar.month==12)
				solar.month=1;
				else
				solar.month++;
				break;
				case '7':
				if (solar.year==-4700)
				solar.year=2000;
				else
				solar.year--;
				break;
				case '9':
				if (solar.year==10000)
				solar.year=2000;
				else
				solar.year++;
				break;
				case '2':
				if (cursor[dy-1][dx]&&dy>0)
				dy--;
				break;
				case '4':
				if (cursor[dy][dx-1]&&dx>0)
				dx--;
				break;
				case '6':
				if (cursor[dy][dx+1]&&dx<6)
				dx++;
				break;
				case '8':
				if (cursor[dy+1][dx]&&dy<5)
				dy++;
				break;
				case '5':
				Menu();
				break;
			}
		}
	}
}

int month=0;
int year=0;
void initmap(int mode)
{
	if (mode){
	for (int i=0;i<17;i++)
	for (int j=0;j<30;j++)
	{
		if (i==0||i==16)
		timemap[i][j]=2;
	}
	for (int k=0;k<8;k++)
	for (int i=timy[k];i<timy[k]+7;i++)
	for (int j=timx[k];j<timx[k]+4;j++)
	{
		if (num[timenum[k]][i-timy[k]][j-timx[k]])
		timemap[i][j]=1;
		else
		timemap[i][j]=0;
	}
	}
	else
	{
		solar.day=1;
		int day,day0=0,day1=0;
		k=GetWeekDay(solar);
		if (flag0)
		{
			int a[6][7]={};
			memcpy(cursor,a,sizeof (cursor));
			dx=k,dy=flag0=0;
			if (osolar.year==solar.year&&osolar.month==solar.month)
			{
				dx=dx0=(osolar.day+k)%7==0?6:(osolar.day+k)%7-1;
				dy=dy0=(osolar.day+k)%7==0?(osolar.day+k)/7-1:(osolar.day+k)/7;
			}
			else 
			dx0=dy0=-1;
		}
		solar0.day=dy?7-k+dx+1+(dy-1)*7:dx-k+1;
		solar0.month=solar.month;
		solar0.year=solar.year;	//用于显示顶部日期
		if (leapyear(solar.year)&&solar.month==2)
	    day=29;
		else
		day = days[solar.month];
		
		lunar[0]=lunar[solar0.day];//用于显示顶部日期
		dayganzhi(&lunar[0].daygan,&lunar[0].dayzhi,solar0);
		
		for (int i=1;i<=day&&(year!=solar.year||month!=solar.month);i++)
		{
			lunar[i]=GenratedataLunar(solar);
			if (i>0&&lunar[i].solarterms==8)
			{
			    lunar[i-1].LFestival=1;
			}
			lunar[i].SFestival=GetSolarFestival(solar);
			lunar[i].LFestival=GetLunarFestival(lunar[i]);
			solar.day++;
		}	
		month=solar.month;
		year=solar.year;
		for (int i=0;i<18;i++)
    	for (int j=0;j<7;j++)
    	{
    		if ((i+1)%3==0)
    		strcpy(monthviewch[i][j],"\e[37m--------");
        	else if (i<2)
        	strcpy(monthviewch[i][j],"        ");
        	else if (i>=2)
        	strcpy(monthviewch[i][j],"\0");
    	}
    	for (int i=0,j=k;i<((k+day)/7+((k+day)%7!=0))*3;i++)
    	{
    	for (;j<7;j++)
    	{
    		char str[]={};
    		if ((i+1)%3==1&&day0<day)
    		{
    			strcpy(monthviewch[i][j],ChDay0[day0++]);
    			cursor[i/3][j]=1;
    		}
    		if ((i+1)%3==2&&day1<day)    		
    		{			
    			if (lunar[day1+1].day==1)
    			{
    				strcpy(monthviewch[i][j],ChMonth0[lunar[day1+1].month]);
    			    monthviewch[i][j][11]=ChMonthSize[lunar[day1+1].monthsize][0];
    			    monthviewch[i][j][12]=ChMonthSize[lunar[day1+1].monthsize][1];
    			    monthviewch[i][j][13]=ChMonthSize[lunar[day1+1].monthsize][2];
    			    if (lunar[day1+1].isleap)
    			    {
    			    	monthviewch[i][j][14]=leapmon[1][0];
    			    	monthviewch[i][j][15]=leapmon[1][1];
    			    	monthviewch[i][j][16]=leapmon[1][2];
    			    }
    			}
    			else
    			strcpy(monthviewch[i][j],ChDay1[lunar[day1+1].day]);
  
    			lunar[day1+1].SFestival?strcpy(monthviewch[i][j],ChFestivalSolar[lunar[day1+1].SFestival]):0;
    			lunar[day1+1].LFestival?strcpy(monthviewch[i][j],ChFestival[lunar[day1+1].LFestival]):0;
    			lunar[day1+1].solarterms?strcpy(monthviewch[i][j],ChSolarterms[lunar[day1+1].solarterms]):0;
    			day1++;
    			solar.day++;
    		}
    	}	
        	j=i>0?0:k;
    	}
	}
}

void drawmap(int mode)
{
	switch (mode)
	{
		case 0:
		print_1();
		break;
		case 1:
		print_0();
		break;
		case 2:
		print_2();
		break;
	}
}

int CalcNaYin(int g,int z)
{
	return (z+((g-z)<0?g-z+10:g-z)*6)/2;
}

void BaZhi(void )
{
	double Y,M,D,h,m;
	printf("\t(本算法仅提供八字五行星座，不作过多解读)\n\t请输入:年 月 日 时 分(取北京时间) \n\t如 1999 2 1 17 30(1999年2月1日17时30分)\n\t");
	scanf("%lf %lf %lf %lf %lf",&Y,&M,&D,&h,&m);
	
	Date dat={Y,M,D,h,m,0};
	
	printf("\n北京时间(平太阳时):%02.0f:%02.0f:%02.0f\n",dat.h,dat.m,dat.s);
	setbuf(stdin, NULL);
	int hourgan,hourzhi;
	double dt0=toJD(dat);
	double dt=BJStoUTC(toJD(dat));
	double sc=SC_calc(dt-JD2000);
	dt=toJD(dat)-(120+jw.J)/360+sc;
	dat=setFromJD(dt);
	SolarDate solar={dat.Y,dat.M,dat.D};
	LunarDate lunar=GenratedataLunar(solar);
	dayganzhi(&lunar.daygan,&lunar.dayzhi,solar);
	hoursganzhi(&hourgan,&hourzhi,lunar.daygan,dat.h);
	int Week=GetWeekDay(solar);
	printf("本地时间(真太阳时):%02.0f:%02.0f:%02.0f\n",dat.h,dat.m,dat.s);
	printf("阳历:公元%04d年%02d月%02d日 星期%s %s\n",solar.year,solar.month,solar.day,wday0[Week],ChConstellations[GetConstellations(solar)]);
	printf("农历:%s%s年%s%s月%s 生肖:%s\n",tgan[lunar.yeargan],dzhi[lunar.yearzhi],leapmon[lunar.isleap],ChMonth[lunar.month],ChDay[lunar.day],CZodiac[lunar.yearzhi]);
	if (lunar.solarterms)
	{//当生日和节令月重合，比较具体时间
		if (solarterms[lunar.solarterms-1]>dt0)
		{
			if (lunar.monthgan==0) lunar.monthgan=9;
			else lunar.monthgan=(lunar.monthgan-1)%10;
			if (lunar.monthzhi==0) lunar.monthzhi=11;
			else lunar.monthzhi=(lunar.monthzhi-1)%12;
		}
	}
	printf("\n\e[35;1m\t八字:  %s%s  %s%s  %s%s  %s%s\e[0m\n",tgan[lunar.yeargan],dzhi[lunar.yearzhi],tgan[lunar.monthgan],dzhi[lunar.monthzhi],tgan[lunar.daygan],dzhi[lunar.dayzhi],tgan[hourgan],dzhi[hourzhi]);
	
	int wx0=CalcNaYin(lunar.yeargan,lunar.yearzhi);
	int wx1=CalcNaYin(lunar.monthgan,lunar.monthzhi);
	int wx2=CalcNaYin(lunar.daygan,lunar.dayzhi);
	int wx3=CalcNaYin(hourgan,hourzhi);
	printf("\n纳音五行: %s %s %s %s \n",NaYinWuXing[wx0],NaYinWuXing[wx1],NaYinWuXing[wx2],NaYinWuXing[wx3]);
}

void CalcQiShuo(int year,int k)
{
	double lm[16]={};
	double st[25]={};
	GetSolarTermsAndLunarMonth(year,st,lm);
	Date dat;
	if (year>9999||year<-3000||(k!=1&&k!=0))
		printf("呵呵:(");
	else if (k==1)
	{
		for (int i=0;i<24;i++)
		{
			printf("%2d:%s 视黄经:%3d ",i+1,ChSsolarterms0[i+1],(i+18)%24*15);
			dat = setFromJD(UTCtoBJS(st[i]));
			printf ("%04g-%02g-%02g %02g:%02d:%02d\n",dat.Y,dat.M,dat.D,dat.h,(int)dat.m,(int)dat.s);
		}
	}
	else if (k==0)
	{
		printf("月日视黄经差0º\n");	
		for (int i=0;i<14;i++)
		{
			dat = setFromJD(UTCtoBJS(lm[i]));
			printf ("%02d: %04g-%02g-%02g %02g:%02d:%02d\n",i+1,dat.Y,dat.M,dat.D,dat.h,(int)dat.m,(int)dat.s);
		}
	}
}

void GetLeapYear(void)
{
		int y,k;
	double lm[16]={};
	double st[25]={};
	
	clrscr();
	printf("\n请输入起始年份+计算年数");
	scanf("%d %d",&y,&k);
	getch();
	if (y>9999||y<-4700)
	{
		printf("呵呵:(");
		return;
	}
	clrscr();
	for (int i=0,j=0;i<k;i++)
	{
		lm[15]=0;
		GetSolarTermsAndLunarMonth(y+i,st,lm);
		int leap=lm[15];
		if (leap)
		{
			j++;
    		printf("%2d %04d年闰%02d月 ",j,y+i,leap<3?leap+10:leap-2);
    		if (j%3==0)
			printf("\n");
		}
	}
}

void jumpdate(void )
{
	SolarDate solarx;
	kk: clrscr();
	printf("请输入阳历日期\n范围:-4700-9999年\n如2017 8 1注意空格\n\e[35;1m因为采用定气定朔的现代天文算法\n公元1700年之前的古代农历可能与历史不符\e[0m\n");
	scanf("%d%d%d",&solar.year,&solar.month,&solar.day);
	if (solar.year>-4701&&solar.year<10000)
	{
		if ((leapyear(solar.year)&&solar.month==2?solar.day>29||solar.day<1:solar.day<1||solar.day>days[solar.month])||solar.month<0||solar.month>13)
		{
    		printf("\n该日期不存在，请重新输入:\n");
    		goto kk;
		}
	}
	else
	{
		printf("\n超出范围，请从新输入:\n");
		goto kk;
	}
	solarx=solar;
	solarx.day=1;
	k=GetWeekDay(solarx);
	dx=dx0=(solar.day+k)%7==0?6:(solar.day+k)%7-1;
	dy=dy0=(solar.day+k)%7==0?(solar.day+k)/7-1:(solar.day+k)/7;	
}

void TWZB(void )
{
	printf("\n\t请输入:年 月 日 时 分 秒 (力学时)\n\t如 1999 2 1 17 30 0 (1999-2-1 17:30:0)\n\n\t");
	double Y,M,D,h,m,s;
	scanf("%lf %lf %lf %lf %lf %lf",&Y,&M,&D,&h,&m,&s);
	setbuf(stdin, NULL);
	clrscr();
	Date d={Y,M,D,h,m,s};
	printf("\033[32mTT力学时:%02.0f-%02.0f-%02.0f %02.0f:%02.0f:%02.0f\n",d.Y,d.M,d.D,d.h,d.m,d.s);
	double jd=toJD(d);
	zb_calc(jd-JD2000,0,jw);
	zb_calc(jd-JD2000,1,jw);
	printf("\033[32m当前位置:%s %s\n地理坐标:东经%0.4f 北纬%0.4f\n",jw.s,jw.x,jw.J,jw.W);
}

void Menu(void)
{
	printf("\033[?25h");//显示光标
	clrscr();
	printf("\n\t\t小小日历v1.0正式版\n\t\t---------------\n\t\t0 数字时钟\n\t\t1 日期跳转\n\t\t2 定气定朔\n\t\t3 八字推算\n\t\t4 天文坐标\n\t\t5 闰年计算\n\t\t---------------\n\t\t6 设置位置(天文坐标,八字会用到)\n");
	gotoxy(25,21);
	printf("\e[32;1m-------------------------------\n");
	gotoxy(25,22);
	printf("©2017 千百度.All rights reserved\e[0m");
	switch(getch())
	{
		case '0':
		mode=1;
		break;
		
		case '1':
		jumpdate();
		break;
		
		case '2':
		{
			int y,k;
			clrscr();
			printf("\n请输入年份+类型(1定气, 0定朔)\n如:2017 0\n");
			scanf("%d %d",&y,&k);
			getch();
			clrscr();
		    CalcQiShuo(y,k);
		    getch();
		}
		break;
		
		case '3':
	    	clrscr();
		    BaZhi();
			getch();
		break;
		
		case '4':
		{
			clrscr();
			printf("\n\t\t请选择:\n\t\t---------------\n\t\t0 月日实时天文坐标\n\t\t1 手动输入指定日期\n");
			char key=getch();
			if (key=='0')
			mode=2;
			else if (key=='1')
			{
				TWZB();
				getch();
			}
		}
		break;
		
		case '5':
		GetLeapYear();
		getch();
		break;
		
		case '6':
		clrscr();
		jw=GeographicalPosition() ;  
		printf("设置成功2s后返回\n");
	    sleep(2);
		break;
	}
	printf("\033[?25l");//隐藏光标
	clrscr();
}

void getagoCh(SolarDate solar1,SolarDate solar2,char *chago)
{
	int ago=CalcAgo(solar1,solar2);
	int num;
	if (ago>=-1&&ago<=1)
		sprintf(chago,"       %s",ChAgo[ago+1]);
	else
	{
		if (ago>1) num=3; 
		else if (ago<-1) num=4; 
        sprintf(chago,"%7d%s",abs(ago),ChAgo[num]); 
    }
}

void print_0(void )
{
	//呃。。。又是一大坨，下面还有。。用于顶部信息栏显示
	int jd=toJD2(osolar);
	printf("\e[47;30m公元%d年%02d月%02d日 周%s %s JD%d(%.0f)  ",osolar.year,osolar.month,osolar.day,wday0[utc->tm_wday],ChConstellations[GetConstellations(osolar)],jd,jd-JD2000);
	printf("\e[47;35m%02d:%02d:%02d \n",utc->tm_hour,utc->tm_min,utc->tm_sec);
	printf("\e[41;37m开元%4d年%s%s月[%s]%s",lunar0.year,leapmon[lunar0.isleap],ChMonth[lunar0.month],ChMonthSize[lunar0.monthsize],ChDay[lunar0.day]);
	printf(" %s%s[%s]年 %s%s月 %s%s日 %s%s时 \e[0m\n",tgan[lunar0.yeargan],dzhi[lunar0.yearzhi],CZodiac[lunar0.yearzhi],tgan[lunar0.monthgan],dzhi[lunar0.monthzhi],tgan[lunar0.daygan],dzhi[lunar0.dayzhi],tgan[hourgan],dzhi[hourzhi]);
	printf("\e[40;1m\t %s  %s\e[0m\n",ChFestivalSolar[GetSolarFestival(osolar)],ChFestival[GetLunarFestival(lunar0)]);
	
	for (int i=0;i<17;i++)
	{
		for (int j=0;j<30;j++)
		{
			switch (timemap[i][j])
			{//绘制数字时钟
        		case 1:
        		printf("\e[32;4%dm@@\e[0m",j==13?3:5);
        		break;
        		case 2:
	        	printf("\e[37m┄┄\e[0m");
	        	break;
	        	case 0:
	        	printf("  ");
        		break;
    		}
    	}
	    if (i<16)
        	printf("\n");
	}
	printf("\e[47;31m\n'0'返回月历视图\e[0m\n");
}

void print_1(void )
{
	SolarDate huili=getHuiLi(solar0);
	int wx0=CalcNaYin(lunar[0].yeargan,lunar[0].yearzhi);
	int wx1=CalcNaYin(lunar[0].monthgan,lunar[0].monthzhi);
	int wx2=CalcNaYin(lunar[0].daygan,lunar[0].dayzhi);
		
	char BC=32;
	int year0;
	if (solar0.year>0) year0=solar0.year;
	else year0 = 1-solar0.year,BC='B';
		
	char chago[16]={};
	getagoCh(solar0,osolar,chago);
	/*顶部信息栏显示*/
	printf("\e[47;30m公元%c%04d年%02d月%02d日 周%s %s \e[35m%s ",BC,year0,solar0.month,solar0.day,wday0[GetWeekDay(solar0)],ChConstellations[GetConstellations(solar0)],chago);
	printf("\e[35m%s%02d:%02d:%02d \e[0;1m\e[0m\n",ChTimeQuantum[GetTimeQuantum(utc->tm_hour)],utc->tm_hour,utc->tm_min,utc->tm_sec);
	printf("\e[41;37m开元 %4d年%s%s月[%s]%s",lunar[0].year,leapmon[lunar[0].isleap],ChMonth[lunar[0].month],ChMonthSize[lunar[0].monthsize],ChDay[lunar[0].day]);
	printf(" %s%s%s年 %s%s月 %s%s日 冲%s煞%s\e[0m \n",tgan[lunar[0].yeargan],dzhi[lunar[0].yearzhi],CZodiac[lunar[0].yearzhi],tgan[lunar[0].monthgan],dzhi[lunar[0].monthzhi],tgan[lunar[0].daygan],dzhi[lunar[0].dayzhi],CZodiac[(lunar[0].dayzhi+6)%12],ChAzimuth[lunar[0].dayzhi%4]);
	printf("\e[44;37m回历 %4d年%02d月%02d日JD%07d(%.0f) %s %s %s\e[0m\n",huili.year,huili.month,huili.day,toJD2(solar0),toJD2(solar0)-JD2000,NaYinWuXing[wx0],NaYinWuXing[wx1],NaYinWuXing[wx2]);
	/*绘制日历界面*/
	for (int k=0;k<7;k++)
		printf("\e[3%d;1m周%s    ",(k&&k<6)?3:5,wday0[k]);
	printf("\n");
	for (int k=0;k<28;k++)
		printf("\e[33m──");
	printf("\n");
	for (int i=0;i<18;i++)
	{		
		for (int j=0;j<7;j++)
		{
			if ((dy*3==i||dy*3+1==i)&&dx==j)
	    		printf("\e[40;33;1;5m%s\e[0m",monthviewch[i][j]);
			else 
	    		if ((dy0*3==i||dy0*3+1==i)&&dx0==j)
				printf("\e[40;3%d;1m%s\e[0m",j||(i+1)%3==0?2:5,monthviewch[i][j]);
			else	printf("\e[38;5;19%d;1m%s",(j&&j<6)||(i+1)%3==0?1:9,monthviewch[i][j]);
		}
		if (i<17)
			printf("\n");
	}
	/*其他信息*/
	gotoxy(35,21);
	printf("\e[0;35m%s %s",jw.s,jw.x);
	gotoxy(35,22);
	printf("东经%0.2f 北纬%0.2f\n",jw.J,jw.W);
	printf("\e[0;47;31m\n2/4/6/8移动光标 1/3上/下一月 7/9上/下一年 5菜单 0返回今天\e[0m\n");
	if (lunar[0].day==1)
	{
		gotoxy(1,24); 
		Date d=setFromJD(lunar[0].lunarmonthtime);
		printf("\e[41;37m定朔:%02.0f:%02.0f:%02.0f  \e[0m\n",d.h,d.m,d.s);
	}
	if (lunar[0].solarterms)
	{
		gotoxy(1,24);
		Date d=setFromJD(solarterms[lunar[0].solarterms-1]);
		printf("\e[44;37m定%s:%02.0f:%02.0f:%02.0f \e[0m\n",ChSsolarterms0[lunar[0].solarterms],d.h,d.m,d.s);
	}
	DrawFestivalTXT(lunar[0].SFestival,lunar[0].LFestival);//彩蛋
}

void print_2(void )
{//显示实时天文数据
		Date date={utc->tm_year + 1900,utc->tm_mon+1,utc->tm_mday,utc->tm_hour,utc->tm_min,utc->tm_sec};
		double jd=BJStoUTC(toJD(date));
		Date d=setFromJD(jd);
		printf("北京时间:%02.0f:%02.0f:%02.0f\n",date.h,date.m,date.s);
		gotoxy(30,1);
		printf("TT力学时:%02.0f:%02.0f:%02.0f\n",d.h,d.m,d.s);
		zb_calc(jd-JD2000,0,jw);
		zb_calc(jd-JD2000,1,jw);
		printf("\033[32m当前位置:%s %s\n地理坐标:东经%0.4f 北纬%0.4f\n",jw.s,jw.x,jw.J,jw.W);		
}