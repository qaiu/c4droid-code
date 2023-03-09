#ifndef DATEINIT_H
#define DATEINIT_H

#include "DateChange.h"
#include "AstronomyCalc.h"
/*基于天文算法的日历数据生成及初始化部分*/
int year0=0;
double lunarmonth[16]={};//定朔日期
double solarterms[25]={};//定气日期
double lunarmonth0[16]={};
double solarterms0[25]={};

LunarDate GenratedataLunar(SolarDate solar);//农历日期和年月干支节气
SolarDate getHuiLi(SolarDate HL);//回历计算
void dayganzhi(int *gan,int*zhi,SolarDate solar);//日干支
void hoursganzhi(int *gan,int *zhi,int daygan,int hours);//时辰干支
int GetSolarFestival(SolarDate solar);//阳历节日
int GetLunarFestival(LunarDate lunar);//农历节日
int GetConstellations(SolarDate solar);//获取星座
int GetWeekDay(SolarDate solar);//获取指定阳历的星期
int GetTimeQuantum(int h);//获取一天时间段
double intf(double f)
{//JD北京时间取整
	return floor(UTCtoBJS(f)+0.5);
}

double Estimate_ST(int year,int angle)
{//估算24节气日期
	Date date={};
	date.Y=year;
	switch (angle/15)
	{
		case 0:// 3-21前后 春分
		    date.M=3,date.D= 23;
		break;
		case 1://4月5日前后 清明
		    date.M= 4,date.D= 5;
		break;
		case 2: //谷雨 4月19-21日
		    date.M= 4,date.D= 20;
		break;
		case 3://立夏 5月5-7日
		    date.M= 5,date.D= 6;
		break;
		case 4:
		    date.M= 5,date.D= 21;
		break;
		case 5:
		    date.M= 6,date.D= 6;
		break;
		case 6:
		    date.M= 6,date.D= 21;
		break;
		case 7:
		    date.M= 7,date.D= 7;
		break;
		case 8:
		    date.M= 7,date.D= 23;
		break;
		case 9:
		    date.M= 8,date.D= 8;
		break;
		case 10:
		    date.M= 8,date.D= 23;
		break;
		case 11:
		    date.M= 9,date.D= 8;
		break;
		case 12:
		    date.M= 9,date.D= 23;
		break;
		case 13:
		    date.M= 10,date.D= 8;
		break;
		case 14:
		    date.M= 10,date.D= 23;
		break;
		case 15:
		    date.M= 11,date.D= 8;
		break;
		case 16:
		    date.M= 11,date.D= 22;
		break;
		case 17:
		    date.M= 12,date.D= 7;
		break;
		case 18:
		    date.M= 12,date.D= 22;
		break;
		case 19:
		    date.M= 1,date.D= 6;
		break;
		case 20:
		    date.M= 1,date.D= 21;
		break;
		case 21:
		    date.M= 2,date.D= 4;
		break;
		case 22:
		    date.M= 2,date.D= 19;
		break;
		case 23:
		    date.M= 3,date.D= 6;
		break;
		default :
		return 0;
		break;
	}
	double k=(toJD(date)- JD2000) / 365250;
	return k;
}

double Get_ST(int year, double angle)
{
	int sum=0;
    double JD2, JD0, JD1,stDegree=0,stDegreep=0;
    JD2=JD1 = Estimate_ST(year, angle);
    int k=angle?1:1000;
    do
    {
    	sum++;
    	if (angle==0&&(JD2-JD1)*365250>7)
    		JD2=JD1=JD2-7/365250;
        JD0 = JD1;
        stDegree =ML_get(JD1)- angle;
        stDegreep = (ML_get(JD0 + 0.000005/k)  - ML_get(JD0 - 0.000005/k)) / (0.00001/k);
       JD1 = JD0 - stDegree / stDegreep;
    }
    while(sum<5&&(fabs(JD1 - JD0) > 0.0000001)); 
    return JD1*365250+JD2000;
}
 
double Calc_MS(double tdJD)
{	//tdJD儒略千年数
    double JD0, JD1,stDegree,stDegreep;
    JD1 = tdJD;
    int sum=0;
    do
     {
     	sum++;
         JD0 = JD1;
        stDegree = MS_aLon(JD0);
        stDegreep = (MS_aLon(JD0+0.0000005) - MS_aLon(JD0-0.0000005) ) / 0.000001;
        JD1 = JD0 - stDegree / stDegreep;
   }while(sum<10&&(fabs(JD1 - JD0) > 0.0000001));
    return JD1*365250+JD2000;
}

void GetSolarTermsAndLunarMonth(int year, double *STerms,double *month)
{
	year--;
	/*农历定冬至(春分点起的第18个节气所在的月份为十一月(冬月)*/
    int st = 18;
    double monthlong=29.55;//朔望月长度
    for (int i=0; i < 25 ;i++)
    {
    	 STerms[i]=Get_ST (year, st * 15);
        if(st == 18)  year++;
        st = (st + 1) % 24;
    }
    month[0]=Calc_MS(JDDtoJDK(STerms[0]));
    if (intf(month[0])>intf(STerms[0]))
    month[0]=Calc_MS(JDDtoJDK(month[0]-monthlong));
    for (int i = 1,j=0;j<3&& i < 15; i++)
    {
    	month[i]=Calc_MS(JDDtoJDK(month[i-1]+monthlong));
    	if (month[i]==month[i-1])
    	{if (j==0)
    	i=1,monthlong=28;
    	else
    	i=1,monthlong=30;
    	j++;
    	}
    }
	if (intf(month[13])<=intf(STerms[24]))
	{
		for (int i = 1; i < 13; i++)
        {
        	if (intf(STerms[2*i])>=intf(month[i+1]))
            {//无中气置闰法判断闰月位置
            	month[15]=i;
                break;
            } 
        }
    }
}

//double dz=0;
LunarDate GenratedataLunar( SolarDate solar )
{//生成农历数据
	int lunaryear=solar.year;
	Date JDdate={};
	LunarDate lunar={};
	JDdate.Y=solar.year;
	JDdate.M=solar.month;
	JDdate.D=solar.day;
	double JD=toJD(JDdate);
	if (solar.year!=year0)
	{//初始化朔日，节气时间表
		year0=solar.year;
		for (int i=0;i<25;i++)
		{
			if (i<16)
				 lunarmonth0[i]= lunarmonth[i]=0;
			solarterms0[i]=solarterms[i]=0;
		}	GetSolarTermsAndLunarMonth(solar.year,solarterms,lunarmonth);
		for (int i=0;i<25;i++)
		{
			if (i<15)
			{				 
				 lunarmonth[i]=UTCtoBJS(lunarmonth[i]);
				 lunarmonth0[i]= floor(lunarmonth[i])+0.5;
				 lunarmonth0[i]=lunarmonth[i]>=lunarmonth0[i]?
				 lunarmonth0[i]: lunarmonth0[i]-1;
			}
			solarterms[i]=UTCtoBJS(solarterms[i]);
			solarterms0[i]=floor(solarterms[i])+0.5;
			solarterms0[i]=solarterms[i]>=solarterms0[i]?
			solarterms0[i]:solarterms0[i]-1;
		}
	}
	/*农历月日*/
	int leap=lunarmonth[15];
	for (int i=0;i<15;i++)
	{
		if (JD>= lunarmonth0[i]&&JD< lunarmonth0[i+1])
    	{
    		lunar.lunarmonthtime= lunarmonth[i];
    		lunar.year=i<2?lunaryear-1:lunaryear;
    		lunar.year+=2698;
			lunar.month=i<2?i+11:i-1;
			lunar.day=JD- lunarmonth0[i]+1;
			lunar.monthsize= lunarmonth0[i+1]- lunarmonth0[i];
		 	if (leap>2&&leap<=i)
		 	{
		 		lunar.month--;
		 		if (leap==i) lunar.isleap=1;
		 	}
	 		if (leap<=2&&leap<=i&&leap)
	 		{
	 			if (i==1) lunar.month=11;
	 			if (i==2) lunar.month=12,lunar.year--;
	 			if (i==leap) lunar.isleap=1;
	 		}
	 		lunar.monthsize= lunarmonth0[i+1]- lunarmonth0[i]-29;
 			break;
		}
	}
	/*节气，干支历*/
	int year=solar.year,month=0;
	for (int i=0;i<25;i++)
	{
		if (solarterms0[i]==JD)
		{
		//	if (i==24) lunar.solarterms=1;
			lunar.solarterms=(i%24)+1;
		}
	}
	if (JD<solarterms0[1]) month=11,year--;
	if (JD>=solarterms0[23]) month = 11;
	for (int i=1;i<24;i+=2)
	{
		if (JD>=solarterms0[i]&&JD<solarterms0[i+2])
		{
			if (i==1) month=12,year--; else month=i/2;
		}
	}
	if (year<=0)
	{//公元前干支
		year=-year+1;
		lunar.yeargan =(17- year%10)%10;
    	lunar.yearzhi = (21-year%12)%12;
		lunar.monthgan=(lunar.yeargan*2+1+month)%10;
	    lunar.monthzhi=(1+month)%12;
	}
	else 
	{
		lunar.yeargan = ((year - 4)<0?year-4+10:year-4)%10;
		lunar.yearzhi = ((year - 4)<0?year-4+12:year-4)%12;
		lunar.monthgan=(lunar.yeargan*2+1+month)%10;
		lunar.monthzhi=(1+month)%12;
	}
	return lunar;
}
/*日干支*/
void dayganzhi(int *gan,int*zhi,SolarDate solar)
{
	Date date={solar.year,solar.month,solar.day};
	int JD = ceil(toJD(date));
	*gan=(JD-11)%10;
	*zhi=(JD-11)%12;
}
/*时辰干支*/
void hoursganzhi(int *gan,int *zhi,int daygan,int hours)
{
	*zhi=((hours%23)+1)/2;
	*gan=(2*daygan+*zhi)%10;
}
SolarDate getHuiLi(SolarDate HL)
{ //回历计算
    Date JD={HL.year,HL.month,HL.day,12.0};
    SolarDate r;
    double d0 = floor(toJD(JD)) - JD2000; 
    double z,y,m,d;
    d = d0 + 503105;
    z = floor( d/10631 );       //10631为一周期(30年)
    d -= z*10631;             
    y = floor((d+0.5)/354.366); //加0.5的作用是保证闰年正确(一周中的闰年是第2,5,7,10,13,16,18,21,24,26,29年)
    d -= floor(y*354.366+0.5); m = floor((d+0.11)/29.51);  //分子加0.11,分母加0.01的作用是第354或355天的的月分保持为12月(m=11)
    d -= floor(m*29.5+0.5);
    r.year = z*30+y+1;
    r.month= m+1;
    r.day  = d+1;
    return r;
}

int GetWeekDay(SolarDate solar)
{//获取指定阳历的星期
	Date date={solar.year,solar.month,solar.day};
	int JD = toJD(date)+1.5;
    return JD%7;
}

int leapyear(int year)
{//闰年判断
	if (year>1582)
	return year%4==0&&year%100!=0||year%400==0;
	else
	return year%4==0;
}
int GetLunarFestival(LunarDate lunar)
{//获取农历节日
	if (lunar.month==12)
	    LunarFestivalDate[sizeof ( LunarFestivalDate)/sizeof (int)-1]=lunar.monthsize?1230:1229;   
	for (int i=0;i<sizeof ( LunarFestivalDate)/sizeof (int);i++)
	{
		if (!lunar.isleap&&lunar.month*100+lunar.day==LunarFestivalDate[i])
		    return i+1;
	}
    return 0;
}

void CalcEasterDate(int x,int *month,int *day)
{//计算复活节日期
	if (x>1582)
	{//格里历(公历)复活节日期
		int a=x%19;
		int b=x/100;
		int c=x%100;
		int d=b/4;
		int e=b%4;
		int f=(b+8)/25;
		int g=(b-f+1)/3;
		int h=(19*a+b-d-g+15)%30;
		int i=c/4;
		int k=c%4;
		int l=(32+2*e+2*i-h -k)%7;
		int m=(a+11*h+22*l)/451;
		int n=(h+l-7*m+114)/31;
		int p=(h+l-7*m+114)%31+1;
		*day=p,*month=n;
	}
	else if (x>0)
	{//儒略历复活节日期
		int a=x%4;
		int b=x%7;
		int c=x%19;
		int d=(19*c+15)%30;
		int e=(2*a + 4*b - d + 34)%7;
		int f=(d + e + 114)/31;
		int g=(d + e + 114)%31+1;
		*day=g,*month=f;
	}
}

int GetSolarFestival(SolarDate solar)
{//阳历节日
	int k=solar.month*100+solar.day;
	SolarDate d;
	d.year=solar.year;
	d.day=1;
	if (solar.month==6)
	{//父亲节//定周节日
    	d.month=6;
    	int week=GetWeekDay(d);
    	if (week==0) week=7;
    	SolarFestivalDate[1]=d.month*100+22-week;
	}
	if (solar.month==5)
	{//母亲节
    	d.month=5;
    	int week=GetWeekDay(d);
    	if (week==0) week=7;
    	SolarFestivalDate[2]=d.month*100+15-week;
	}
	if (solar.month==11)
	{//感恩节
    	d.month=11;
    	int week=GetWeekDay(d);
    	if (week<5) week+=7;
    	SolarFestivalDate[3]=d.month*100+33-week;
	}
	if (k<426&&k>321)
	{//复活节
		int month,day;
		CalcEasterDate(solar.year,&month,&day);
		SolarFestivalDate[0]=month*100+day;
	}
	for (int i=0;i<=20;i++)
	{//其他定期节日
		if (solar.month*100+solar.day==SolarFestivalDate[i])
	    	return i+1;
	}
	return 0;
}

int GetConstellations(SolarDate solar)
{//获取星座
	int k=solar.month*100+solar.day;
	if (k<ConstellationsDate[1]||k>=ConstellationsDate[0])
	return 0;
	for (int i=1;i<12;i++)
	{
		if (k>=ConstellationsDate[i]&&k<ConstellationsDate[i+1])
		return i;
	}
}
int GetTimeQuantum(int h)
{//获取一天时间段
	for (int i=0;i<6;i++)
	{
		if (h>=TimeQuantum[i]&&h<TimeQuantum[i+1])
		return i;
	}
}

#endif