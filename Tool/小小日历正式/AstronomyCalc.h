#ifndef ASTRONOMY_CALC_H
#define ASTRONOMY_CALC_H
#include "planetdata.h"
/*
天文计算部分
如非必要，请勿乱动
*/

typedef struct 
{
	double x,y,z;
}POSITION; //星体位置

typedef struct
{
	double dL,dE;
}NUTATION;//章动值

/*角度转为0到360度之间*/
double Mod360Degree(double deg)
{	
	double k =(int)deg%360+(deg-(int)deg);
	return k>0?k:360+k;
}

double rad2mrad(double v)
{   //对超过0-2PI的角度转为0-2PI
  v=fmod(v,2*PI);
  if (v<0) 
  return v+2*PI;
  return v;
}

double rad2rrad(double v)
{//对超过-PI到PI的角度转为-PI到PI
  v=fmod(v,2*PI);
  if (v<=-PI) return v+2*PI;
  if (v>PI) return v-2*PI;
  return v;
}
/*输出度分秒格式保留两位小数秒*/
void printrad(double jd)
{
//	jd=Mod360Degree(jd);
	int d=(int)jd;
	double f=(jd-(int)jd)*60;
	double m=(f-(int)f)*60;
	if (f<0) f=-f;
	if (m<0) m=-m;
	printf("%dº%02d′%.2f″\n",d,(int)f,m);
}
//弧度转角度
double htoj(double a)
{
	return 180*a/PI;
}
//月球地心视黄经 
double M_calc_0(MONTH *month,int count,double dt)
{
	double val = 0.0;
	double  dt2=T2/1e4,dt3=T3/1e8,dt4=T4/1e8;
    for(int i = 0; i < count; i++)
    
val+=month[i].a*cos(month[i].b +dt*month[i].c +dt2*month[i].d +dt3*month[i].e +dt4*month[i].f);
    return val;
}

double ML_calc(double dt)
{
	dt*=10;
	double Tx=T-10,v=0;
    v += (3.81034409 + 8399.684730072*T -3.319e-05*T2 + 3.11e-08*T3 - 2.033e-10*T4)*RAD; //月球平黄经(角度)
    v += 5028.792262*T + 1.1124406*T2 + 0.00007699*T3 - 0.000023479*T4 -0.0000000178*T5;  //岁差(角秒)
    if (Tx>0) 
    v += -0.866 +1.43*Tx +0.054*Tx*Tx;
	double L0 = M_calc_0(ML0, sizeof(ML0) / sizeof(MONTH), dt);
    double L1 = M_calc_0(ML1, sizeof(ML1) / sizeof(MONTH), dt);
    double L2 = M_calc_0(ML2, sizeof(ML2) / sizeof(MONTH), dt);
    v+=L2*T2+L1*T+L0;
    v=Mod360Degree((v/3600));
    return v;
}

double MLa_calc(double dt)
{
	dt*=10;
	double B_0 = M_calc_0(MB0, sizeof(MB0) / sizeof(MONTH), dt);
    double B1 = M_calc_0(MB1, sizeof(MB1) / sizeof(MONTH), dt);
    return (B1*T+B_0)/3600;
}

 double MR_calc(double dt)
{
	dt*=10;
	double R0 = M_calc_0(MR0, sizeof(MR0) / sizeof(MONTH), dt);
    double R1 = M_calc_0(MR1, sizeof(MR1) / sizeof(MONTH), dt);
    double R2 = M_calc_0(MR2, sizeof(MR2) / sizeof(MONTH), dt);
    return R2*T2+R1*T+R0;
}

//半解析计算各轨道系数
double S_calc_0(VSOP87_COEFFICIENT *coff, int count, double dt)
{
    double val = 0.0;
    for (int i = 0; i < count; i++)
        val += (coff[i].A * cos((coff[i].B + coff[i].C * dt)));
    return val;
 }
/*******************************************************
函数名：VSOP87行星理论计算行星日心黄经
参数： 儒略千数
返回值：太阳的地心黄经
*******************************************************/
double SL_calc(double dt,int m)
{
    double L0 = S_calc_0(Earth_L0, sizeof(Earth_L0) / sizeof(VSOP87_COEFFICIENT), dt);
    double L1 = S_calc_0(Earth_L1, sizeof(Earth_L1) / sizeof(VSOP87_COEFFICIENT), dt);
    double L2 = S_calc_0(Earth_L2, sizeof(Earth_L2) / sizeof(VSOP87_COEFFICIENT), dt);
    double L3 = S_calc_0(Earth_L3, sizeof(Earth_L3) / sizeof(VSOP87_COEFFICIENT), dt);
    double L4 = S_calc_0(Earth_L4, sizeof(Earth_L4) / sizeof(VSOP87_COEFFICIENT), dt);
    double L5 = S_calc_0(Earth_L5, sizeof(Earth_L5) / sizeof(VSOP87_COEFFICIENT), dt);
 
    double L = (((((L5 * dt + L4) * dt + L3) * dt + L2) * dt + L1) * dt + L0) / 10000000000;
    double k=(-0.0728 -2.7702*T -1.1019*T2 -0.0996*T3) / 3600;
    /*地心黄经 = 日心黄经 + 180度*/
    if (m)   return Mod360Degree(L/GDE+k+180);
    else return L+k*GDE;
}
 /*******************************************************
函数名：VSOP87行星理论计算地球日心黄维
参数： 儒略千数
返回值：太阳的地心黄维
*******************************************************/
double SLa_calc(double dt,int m)
{	
    double B_0 = S_calc_0(Earth_B0, sizeof(Earth_B0) / sizeof(VSOP87_COEFFICIENT), dt);
    double B1 = S_calc_0(Earth_B1, sizeof(Earth_B1) / sizeof(VSOP87_COEFFICIENT), dt);
    double B2 = S_calc_0(Earth_B2, sizeof(Earth_B2) / sizeof(VSOP87_COEFFICIENT), dt);
    double B = ((B2 * dt + B1) * dt + B_0) / 10000000000.0; 
    double k = (+0.0000 +0.0004*T +0.0004*T2 -0.0026*T3) / RAD;
    /*地心黄纬 = －日心黄纬*/
    if (m) return (-B-k)/GDE;
    else return B+k;
 }
 /******************************************************
函数名：VSOP87行星理论计算日地距离单位au
参数： 儒略千数
返回值：太阳到地球距离
*****************************************************/
 double SR_calc(double dt)
{	
    double R0 = S_calc_0(Earth_R0, sizeof(Earth_R0) / sizeof(VSOP87_COEFFICIENT), dt);
    double R1 = S_calc_0(Earth_R1, sizeof(Earth_R1) / sizeof(VSOP87_COEFFICIENT), dt);
    double R2 = S_calc_0(Earth_R2, sizeof(Earth_R2) / sizeof(VSOP87_COEFFICIENT), dt);
    double R3 = S_calc_0(Earth_R3, sizeof(Earth_R3) / sizeof(VSOP87_COEFFICIENT), dt);
    double R4 = S_calc_0(Earth_R4, sizeof(Earth_R4) / sizeof(VSOP87_COEFFICIENT), dt);
    double R5 = S_calc_0(Earth_R5, sizeof(Earth_R5) / sizeof(VSOP87_COEFFICIENT), dt);
    double k = (-0.0020 +0.0044*T +0.0213*T2 -0.0250*T3) / 1000000;
    double R = ((((((R5*dt)+R4 * dt) + R3) * dt + R2) * dt + R1) * dt + R0) / 10000000000.0;
    return R+k;
 } 
/*高精度算章动*/	
NUTATION Nu_calc(double t,int zq)
{ //章动计算,t是J2000.0起算的儒略千数,zq表示只计算周期天于zq(天)的项
    t*=10;
    NUTATION nutation;
    double t2=t*t, t3=t2*t, t4=t3*t; //t的二、三、四次方
    double l = 485868.249036 + 1717915923.2178*t + 31.8792*t2 + 0.051635*t3 - 0.00024470*t4;
    double l1=1287104.79305 + 129596581.0481*t -  0.5532*t2 - 0.000136*t3 - 0.00001149*t4;
    double F = 335779.526232 + 1739527262.8478*t - 12.7512*t2 - 0.001037*t3 + 0.00000417*t4;
    double D =1072260.70369 + 1602961601.2090*t -  6.3706*t2 + 0.006593*t3 - 0.00003169*t4;
    double Om= 450160.398036 -    6962890.5431*t +  7.4722*t2 + 0.007702*t3 - 0.00005939*t4;
    double c,q;
    for (int i=0;i<77*11;i+=11){ //周期项取和计算
    c = (nuTab[i]*l + nuTab[i+1]*l1 + nuTab[i+2]*F +nuTab[i+3]*D +nuTab[i+4]*Om)/RAD;
    if (zq)
    { //只算周期大于zq天的项
        q = 36526*2*M_PI*RAD/(1717915923.2178*nuTab[i] + 129596581.0481*nuTab[i+1] + 1739527262.8478*nuTab[i+2] + 1602961601.2090*nuTab[i+3] + 6962890.5431*nuTab[i+4]);
        if (q<zq) continue;
    }
     nutation.dL += (nuTab[i+5]+nuTab[i+6]*t) * sin(c) + nuTab[i+7]  * cos(c);
     nutation.dE += (nuTab[i+8]+nuTab[i+9]*t) * cos(c) + nuTab[i+10] * sin(c);
}
    nutation.dL/=36000000000;
    nutation.dE/=36000000000;
    return nutation;//返回IAU2000B章动值, dL是黄经章动,dE是交角章动
}

double nutB[]={
	2.1824,  -33.75705, 36e-6,-1720,920,
   3.5069, 1256.66393, 11e-6,-132, 57,
   1.3375,16799.4182, -51e-6, -23, 10,
   4.3649,  -67.5141,  72e-6,  21, -9,
   0.04,   -628.302,   0,     -14,  0,
   2.36,   8328.691,   0,       7,  0,
   3.46,   1884.966,   0,      -5,  2,
   5.44,  16833.175,   0,      -4,  2,
   3.69,  25128.110,   0,      -3,  0,
   3.55,    628.362,   0,       2,  0};
NUTATION nutation(double t)
{ //中精度章动计算,t是儒略千
 t*=10;
  double c,a, t2=t*t;
  NUTATION nu;
  for (int i=0;i<sizeof (nutB)/sizeof (double);i+=5)
  {
   c = nutB[i]+nutB[i+1]*t+nutB[i+2]*t2;
   if (i==0) a=-1.742*t; else a=0;
   nu.dL+=(nutB[i+3]+a)*sin(c);
   nu.dE+= nutB[i+4]*cos(c);
  }
  nu.dL=nu.dL/100/RAD;  //黄经章动
  nu.dE=nu.dE/100/RAD;  //交角章动
  return nu;
}

double gxc_sunLon(double t)
{ //太阳光行差,t是千数
    t*=10;
	double v =-0.043126+ 628.301955*t -0.000002732*t*t; //平近点角
    double e= 0.016708634-0.000042037*t-0.0000001267*t*t;
    return  ( -20.49552 * (1+e*cos(v))/(1.000001018 *(1 - e*e)))/3600; //黄经光行
}
    
double gxc_moonLat(double t)
{ //月球纬度光行差
	return 0.063*sin(0.057+8433.4662*t+0.000064*t*t)/3600;
}
/*计算太阳地心视黄经   
参数：儒略千(浮点数)
返回值：太阳地心视黄经，单位是角度*/
POSITION S_get(double dt)
{
	POSITION pos;
	NUTATION nut=Nu_calc(dt,0);
	pos.x = SL_calc(dt,1)+nut.dL+ gxc_sunLon(dt);
    pos.y = SLa_calc(dt,1);
    pos.z = SR_calc(dt);
    return pos;
}
POSITION S_get0(double dt)
{//地球日心黄经
	POSITION pos={SL_calc(dt,0),SLa_calc(dt,0),SR_calc(dt)};
	return pos;
}
 //计算月球视黄经
POSITION M_get(double dt)
{
	POSITION pos;
	NUTATION nut=Nu_calc(dt,0);
	pos.x = ML_calc(dt) +nut.dL +gxc_moonLon;
    pos.y = MLa_calc(dt)+ gxc_moonLat(dt);
    pos.z = MR_calc(dt);
    return pos;
}
POSITION M_get0(double dt)
{
	POSITION pos={ML_calc(dt)*GDE,MLa_calc(dt)*GDE,MR_calc(dt)};
	return pos;
}
//太阳地心视黄经
double ML_get(double dt)
{
	return  SL_calc(dt,1)+Nu_calc(dt,0).dL+ gxc_sunLon(dt);
}

double MS_aLon(double dt)
{ //月日视黄经的差值
    return (ML_calc(dt) +gxc_moonLon-(SL_calc(dt,1)+ gxc_sunLon(dt)));
}
double pGST0(double jd,double dt)
{ //传入jd是2000年首起算的日数(Ujd),dt是deltatjd(日),精度要求不高时dt可取值为0  //返回格林尼治平恒星时(不含赤经章动及非多项式部分),即格林尼治子午圈的平春风点起算的赤经
 double t=(jd+dt)/36525,t2=t*t, t3=t2*t, t4=t3*t;
  return PI2*(0.7790572732640 + 1.00273781191135448*jd) + (0.014506 + 4612.15739966*t + 1.39667721*t2 - 0.00009344*t3 + 0.00001882*t4)/RAD;
    //jd是UT,下一行的t是力学时(世纪数)
}
double pGST(double jd)
{ //传入力学时J2000起算日数，返回平恒星时
 double dt=deltatT2(jd);
 return pGST0(jd-dt,dt);
}
double hcjj(double dt)
{ //返回P03黄赤交角,t儒略千
  double t=dt*10;
  double t2=t*t, t3=t2*t,t4=t3*t,t5=t4*t;
  return (84381.4060 -46.836769*t -0.0001831*t2 +0.00200340*t3 -5.76e-7*t4 -4.34e-8*t5)/RAD;
}

POSITION llrConv(POSITION JW,double E)
{ //球面坐标旋转
  //黄道赤道坐标变换,赤到黄E取负
  POSITION r;
  double J=JW.x,W=JW.y;
  r.x=atan2(sin(J)*cos(E) - tan(W)*sin(E),cos(J));
  r.y=asin(cos(E)*sin(W) + sin(E)*cos(W)*sin(J));
  r.z=JW.z;
  r.x=rad2mrad(r.x);
  return r;
}

POSITION llr2xyz(POSITION JW)
{ //球面转直角坐标
  POSITION r={};
  double J=JW.x,W=JW.y,R=JW.z;
  r.x=R*cos(W)*cos(J);
  r.y=R*cos(W)*sin(J);
  r.z=R*sin(W);
  return r;
}

 POSITION xyz2llr(POSITION xyz)
{ //直角坐标转球
  POSITION r={};
  double x=xyz.x,y=xyz.y,z=xyz.z;
  r.z = sqrt(x*x+y*y+z*z);
  r.y = asin(z/r.z);
  r.x = rad2mrad( atan2(y,x) );
  return r;
}

POSITION h2g(POSITION z,POSITION a)
{ //日心球面转地心球面,Z星体球面坐标,A地球球面坐标
  a = llr2xyz(a); //地球
  z = llr2xyz(z); //星体
  z.x-=a.x; z.y-=a.y; z.z-=a.z;
  return xyz2llr(z);
}

double MQC (double h)
{
	return  0.0002967/tan( h +0.003138/( h+0.08919));
} //大气折射,h是真高度

POSITION parallax(POSITION z,double H,double fa,double high)
{ //视差修正
  //z赤道坐标,fa地理纬度,H时角,high海拔(千米)
  double dw=1;
  if (z.z<500) 
  dw=cs_AU;
  z.z*=dw;
  double r0,x0,y0,z0, f=cs_ba;
  double u=atan(f*tan(fa)), g=z.x+H;
  r0 = cs_rEar*cos(u)+ high*cos(fa); //站点与地地心向径的赤道投影长度
  z0 = cs_rEar*sin(u)*f+ high*sin(fa); //站点与地地心向径的轴向投影长度
  x0 = r0*cos(g);
  y0 = r0*sin(g);
  POSITION s =llr2xyz(z);
  s.x-=x0, s.y-=y0, s.z-=z0;
  s = xyz2llr(s);
  s.z/=dw;
  return s;
}

void printtim(double gde)
{
	unsigned int h=gde/15;
	unsigned int m=(gde/15-h)*60;
	 double s=((gde/15-h)*60-m)*60;
	printf("%02dh%02dm%0.2fs\n",h,m,s);
}

double SC_calc(double jd)
{
    double dt=jd/365250;
    double dL= -17.2*sin(2.1824-337.5705*dt)/RAD; //黄经章
    double dE=9.2*cos(2.1824-337.5705*dt)/RAD; //交角章
    double E= hcjj(dt)+dE;
    POSITION z = S_get0(dt);   
    z.x  = rad2mrad(z.x+PI+gxc_sunLon(dt)*GDE+dL);
    z.y  =-z.y; 
    z = llrConv( z, E ); 
    double t=dt, t2=t*t,t3=t2*t,t4=t3*t,t5=t4*t;
    double Lon = ( 1753470142 + 6283319653318*t + 529674*t2 + 432*t3 - 1124*t4 - 9*t5 )/1000000000 + PI - 20.5/RAD;
    Lon = rad2rrad( Lon - (z.x-dL*cos(E))); 
    return Lon/PI2;
}

void zb_calc(double t,int k,JINGWEI v)
{
	double L = v.J*GDE;
	double fa = v.W*GDE;
	
	double dt=t/365250;
	NUTATION nu=nutation(dt);
	double j=hcjj(dt);
	double E=j+nu.dE;
	double gstPing = pGST(t);
	double gst= gstPing + nu.dL*cos(E);
	double r0,r1,r2;
	POSITION z,z0,a,a0;
	if (k){
			gotoxy(30,3);
			printf("\033[36m-----------月球------------\n");
	a=S_get0(dt);
	z.z=MR_calc(dt); r0=z.z;
	
	dt -= z.z*(cs_Agx/cs_AU/10); //光行时
    //求视坐标
	a0=S_get0(dt);
	z=M_get0(dt); r2=z.z;
	//求光行距
	a0 = h2g(a,a0); a0.z *= cs_AU;
    z0 = h2g(z,a0); r1 = z0.z;
    //地心黄道及地心赤道
	z.x= rad2mrad(z.x+nu.dL); 
		gotoxy(30,4);
	printf("视黄经:");
	printrad(z.x/GDE);
		gotoxy(30,5);
	printf("视黄纬:");
	printrad(z.y/GDE);
		gotoxy(30,6);
	printf("地心距:%0.8f\n\n",r0);

	z0= llrConv(z,E);
		gotoxy(30,8);
	printf("视赤经:");
	printtim(rad2mrad(z0.x)/GDE);
		gotoxy(30,9);
	printf("视赤纬:");
	printrad(z0.y/GDE);
		gotoxy(30,10);
	printf("光行距:%0.8f\n\n",r1);
		double sj = rad2rrad(gst-L-z0.x); //得到天体时角
	z0=parallax(z0, sj,fa, 0); //视差修正
		gotoxy(30,12);
	printf("站赤经:");
	printtim(z0.x/GDE);
		gotoxy(30,13);
	printf("站赤纬:");
	printrad(z0.y/GDE);
		gotoxy(30,14);
	printf("视距离:%0.8f\n\n",r2);

	z0.x += PI/2-gst+L;  //修正了视差的赤道坐标;
	z0= llrConv(z0,PI/2-fa); //转到时角坐标转到地平坐标
	z0.x = rad2mrad(PI/2-z0.x );
    if (z0.y>0) z0.y += MQC(z0.y); //大气折射修正
    	gotoxy(30,16);
	printf("方位角:");
    printrad(z0.x/GDE);
    	gotoxy(30,17);
    printf("高度角:");
    printrad(z0.y/GDE);
	printf("\033[0m\n");
	}	
	else{		
	printf("平恒星时:");
	printtim(Mod360Degree(gstPing/GDE));
	gotoxy(30,2);
	printf("真恒星时:");
	printtim(Mod360Degree(gst/GDE));
		printf("\033[33m------------地球------------\n");
	a=S_get0(dt);
	z.x=0,z.y=0,z.z=0;
	  //地心黄道
    z = h2g(z,a); r0 = z.z;  //ra地心距
    dt -= z.z*(cs_Agx/10); //光行时
    
    a0=S_get0(dt);
	z0.x=0,z0.y=0,z0.z=0;
	z = h2g(z0,a); r1 = z.z;
    z = h2g(z0,a0); r2 = z.z;
    
	z.x= rad2mrad(z.x+nu.dL); 
	printf("视黄经:");
	printrad(z.x/GDE);
	printf("视黄纬:");
	printrad(z.y/GDE);
	printf("地心距:%0.8f\n\n",r0);
	
	z0= llrConv(z,E);
	printf("视赤经:");
	printtim(rad2mrad(z0.x)/GDE);
	printf("视赤纬:");
	printrad(z0.y/GDE);
	printf("光行距:%0.8f\n\n",r1);
	double sj = rad2rrad(gst-L-z0.x); //得到天体时角
	z0=parallax(z0, sj,fa, 0); //视差修正
	
	printf("站赤经:");
	printtim(z0.x/GDE);
	printf("站赤纬:");
	printrad(z0.y/GDE);
	printf("视距离:%0.8f\n\n",r2);

	z0.x += PI/2-gst+L;  //修正了视差的赤道坐标;
	z0= llrConv(z0,PI/2-fa); //转到时角坐标转到地平坐标
	z0.x = rad2mrad(PI/2-z0.x );
    if (z0.y>0) z0.y += MQC(z0.y); //大气折射修正
    
	printf("方位角:");
    printrad(z0.x/GDE);
    printf("高度角:");
    printrad(z0.y/GDE);
	printf("\033[0m\n");
	}
}
#endif