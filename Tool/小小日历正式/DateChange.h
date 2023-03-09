#ifndef DATE_CHANGE_H
#define DATE_CHANGE_H
#define JD2000 2451545.0

//时间结构体
typedef struct
{//儒略历 年 月 日 时 分 秒 (浮点型)
	double Y,M,D,h,m,s;
}Date;
typedef struct
{//农历和重要节日
	int year,month,day;//农历年月日
	int yeargan,yearzhi, monthgan,monthzhi, daygan, dayzhi;
	//年月日干支
	int isleap;//闰年旗帜
	int solarterms;//节气
	int monthsize;//大小月
	double lunarmonthtime;//朔月时间
	int SFestival,LFestival;//农历节日和公历节日
}LunarDate;

typedef struct
{//公历年月日
	int year, month, day;
}SolarDate;
/*函数声明*/
double deltatT(double y);//力学时TT和协调UTC时的差值返回秒
double  deltatT2(int t);//J2000起的儒略日力学时和世界时差
double toJD(Date date); //公历(儒略历)转儒略日
int toJD2(SolarDate solar);//阳历转儒略日
Date  setFromJD(double jd); //儒略日转为公历
double UTCtoBJS(double dt);//UTC力学时转北京时间
double JDDtoJDK(double dt);//儒略日转儒略千 J2000起
int CalcAgo(SolarDate d1,SolarDate d0);//任意两个阳历时间间隔天数
double BJStoUTC(double dt);

double  DTS[]=
{
-4000,108371.7,-13036.80,392,0,
-500,17201.0,-627.82,16.170,-0.3413,
-150,12200.6,-346.41,5.403,-0.1593,
150,9113.8,-328.13,-1.647,0.0377,
500,5707.5,-391.41,0.915,0.3145,
900,2203.4,-283.45,13.034,-0.1778,
1300,490.1,-57.35,2.085,-0.0072,
1600,120.0,-9.81,-1.532,0.1403,
1700,10.2,-0.91,0.510,-0.0370,
1800,13.4,-0.72,0.202,-0.0193,
1830,7.8,-1.81,0.416,-0.0247,
1860,8.3,-0.13,-0.406,0.0292,
1880,-5.4,0.32,-0.183,0.0173,
1900,-2.3,2.06,0.169,-0.0135,
1920,21.2,1.69,-0.304,0.0167,
1940,24.2,1.22,-0.064,0.0031,
1960,33.2,0.51,0.231,-0.0109,
1980,51.0,1.29,-0.026,0.0032,
2000,63.87,0.1,0,0,
2005
};
double  deltatExt(double y,double jsd)
{
	double dy=(y-1820)/100;
	return -20+jsd*dy*dy; 
}//二次曲线外推

double deltatT(double y)
{ //计算世界时与原子时之差,传入年
   if(y>=2005)
   {
     //sd是2005年之后几年（一值到y1年）的速度估计。
     //sjd是y1年之后的加速度估计。瑞士星历表jsd=31,NASA网站jsd=32,skmap的jsd=29
     double y1=2014, sd=0.4, jsd=31;
     if(y<=y1)
         return 64.7 + (y-2005) *sd; //直线外推
     double v = deltatExt(y,jsd);        //二次曲线外推
     double dv= deltatExt(y1,jsd) - ( 64.7+(y1-2005)*sd ); //y1年的二次外推与直线外推的差
     if(y<y1+100  )
         v -= dv*(y1+100-y)/100;
     return v; 
   }
   int i;
   for (i=0;i<sizeof (DTS)/sizeof (double);i+=5)
       if(y<DTS[i+5])
       break;
   double t1=(y-DTS[i])/(DTS[i+5]-DTS[i])*10, t2=t1*t1, t3=t2*t1;
   return DTS[i+1] +DTS[i+2]*t1 +DTS[i+3]*t2 +DTS[i+4]*t3;
}
  
double  deltatT2(int t)
{ //传入儒略日(J2000起算),计算TD-UT(单位:日)
    return deltatT(t/365.2425+2000)/86400.0;
}
double toJD(Date date)
{ //公历转儒略日
   double y=date.Y, m=date.M, n=0; //取出年月
 //  if (y<=0) y++; //处理公元前
   if(m<=2) m+=12,y--;
   if(date.Y*372+date.M*31+date.D>=588829)
   //判断是否为格里高利历日1582*372+10*31+15
   n =(int)(y/100), n =2-n+(int)(n/4);//加百年闰
   n +=(int)(365.25*(y+4716)+0.01);    //加上年引起的偏移日数
   n +=(int)(30.6*(m+1))+date.D;       //加上月引起的偏移日数及日偏移数
   n +=((date.s/60+date.m)/60+date.h)/24 - 1524.5;
   return n;
}

int toJD2(SolarDate solar)
{
	Date date={solar.year,solar.month,solar.day};
	return floor(toJD(date)+0.5);
}

Date  setFromJD(double jd)
{ //儒略日数转公历
   jd+=0.5;
   Date date;
   double A=(int)(jd), F=jd-A, D;  //取得日数的整数部份A及小数部分F
   if(A>=2299161)
   D=(int)((A-1867216.25)/36524.25),A+=1+D-(int)(D/4);
   A += 1524; //向前移4年零2个月
   date.Y =(int)((A-122.1)/365.25);//年
   D=A-(int)(365.25*date.Y); //去除整年日数后余下日数
   date.M =(int)(D/30.6001);       //月数
   date.D =D-(int)(date.M*30.6001);//去除整月日数后余下日数
   date.Y-=4716;
   date.M--;
   if (date.M>12) date.M-=12;
   if (date.M<=2) date.Y++;
   //日的小数转为时分秒
   F*=24; date.h=(int)(F); F-=date.h;
   F*=60; date.m=(int)(F); F-=date.m;
   F*=60; date.s=F;
   return date;
}
  
double UTCtoBJS(double dt)
{//UTC时间转为北京时间,已修正力学时误差,传入儒略日，返回转换后的儒略日,结合儒略日转公历函数使用.
    return dt+8.0/24.0-deltatT2(dt -JD2000);
}
double BJStoUTC(double dt)
{
	return dt-8.0/24.0+deltatT2(dt -JD2000);
}

double JDDtoJDK(double dt)
{
	return (dt-JD2000)/365250;
}

int CalcAgo(SolarDate d1,SolarDate d0)
{
	return toJD2(d1)-toJD2(d0);
}
#endif