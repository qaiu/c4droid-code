#include <stdio.h>
#include <string.h>
#include <math.h>

/* 
功能说明： 
    将绝对高斯坐标(y,x)转换成绝对的地理坐标(wd,jd)。        
*/
// double y;     输入参数: 高斯坐标的横坐标，以米为单位
// double x;  输入参数: 高斯坐标的纵坐标，以米为单位
// short  DH;     输入参数: 带号，表示上述高斯坐标是哪个带的
// double *L;     输出参数: 指向经度坐标的指针，其中经度坐标以秒为单位
// double *B;     输出参数: 指向纬度坐标的指针，其中纬度坐标以秒为单位
void GaussToGeo(double y, double x, short DH, double *L, double *B, double LP)
{
	double l0;		//  经差
	double tf;		//  tf = tg(Bf0),注意要将Bf转换成以弧度为单位
	double nf;		//  n = y * sqrt( 1 + etf ** 2) / c, 其中etf = e'**2 * cos(Bf0) ** 2
	double t_l0;		//  l0，经差，以度为单位
	double t_B0;		//  B0，纬度，以度为单位
	double Bf0;		//  Bf0
	double etf;		//  etf,其中etf = e'**2 * cos(Bf0) ** 2
	double X_3;

	double PI = 3.14159265358979;
	double b_e2 = 0.0067385254147;
	double b_c = 6399698.90178271;

	X_3 = x / 1000000.00 - 3;	// 以兆米（1000000）为单位
	// 对于克拉索夫斯基椭球，计算Bf0
	Bf0 = 27.11115372595 + 9.02468257083 * X_3 - 0.00579740442 * pow(X_3, 2)
	    - 0.00043532572 * pow(X_3, 3) + 0.00004857285 * pow(X_3, 4)
	    + 0.00000215727 * pow(X_3, 5) - 0.00000019399 * pow(X_3, 6);
	tf = tan(Bf0 * PI / 180);	//  tf = tg(Bf),注意这里将Bf转换成以弧度为单位
	etf = b_e2 * pow(cos(Bf0 * PI / 180), 2);	//  etf = e'**2 * cos(Bf) ** 2
	nf = y * sqrt(1 + etf) / b_c;	//  n = y * sqrt( 1 + etf ** 2) / c
	// 计算纬度，注意这里计算出来的结果是以度为单位的
	t_B0 = Bf0 - (1.0 + etf) * tf / PI * (90.0 * pow(nf, 2)
					      - 7.5 * (5.0 + 3 * pow(tf, 2) + etf - 9 * etf * pow(tf, 2)) * pow(nf, 4)
					      + 0.25 * (61 + 90 * pow(tf, 2) + 45 * pow(tf, 4)) * pow(nf, 6));
	// 计算经差，注意这里计算出来的结果是以度为单位的
	t_l0 = (180 * nf - 30 * (1 + 2 * pow(tf, 2) + etf) * pow(nf, 3)
		+ 1.5 * (5 + 28 * pow(tf, 2) + 24 * pow(tf, 4)) * pow(nf, 5))
	    / (PI * cos(Bf0 * PI / 180));
	l0 = (t_l0 * 3600.0);	//  将经差转成秒

	if (LP == -1000) {
		*L = (double) ((DH * 6 - 3) * 3600.0 + l0);	// 根据带号计算出以秒为单位的绝对经度，返回指针
	} else {
		*L = LP * 3600.0 + l0;	// 根据带号计算出以秒为单位的绝对经度，返回指针
	}
	//----------------------------------

	*B = (double) (t_B0 * 3600.0);	//  将纬差转成秒，并返回指针
}


/* 
功能说明： 
    （1）将地理坐标(wd,jd)转换成绝对的高斯坐标(y,x)
    （2）本函数支持基于六度带（或三度带）、克拉索夫斯基椭球进行转换 

适用范围： 
    本函数适用于将地球东半球中北半球（即东经0度到东经180度，北纬0度至90度）范围
    内所有地理坐标到高斯坐标的转换            

 使用说明： 调用本函数后返回的结果应在满足精度的条件下进行四舍五入      
// double jd;         输入参数: 地理坐标的经度，以秒为单位
// double wd;         输入参数: 地理坐标的纬度，以秒为单位
// short  DH;      输入参数: 三度带或六度带的带号

  六度带(三度带)的带号是这样得到的：从东经0度到东经180度自西向东按每6度(3度)顺序编号
(编号从1开始)，这个顺序编号就称为六度带(三度带)的带号。因此，六度带的带号的范围是1-30，
三度带的带号的范围是1-60。
  如果一个点在图号为TH的图幅中，那麽该点所处的六度带的带号就可以这样得到：将该图号的
第3、4位组成的字符串先转换成数字，再减去30。例如某点在图幅06490701中，该点所在的带号就
是49-30，即19。
  如果调用本函数去进行一般的从地理坐标到基于六度带高斯坐标的变换（非邻带转换），则参
数DH的选取按前一段的方法去确定。                
  如果调用本函数去进行基于六度带邻带转换，则参数DH的选取先按上述方法去确定，然后看是
往前一个带还是后一个带进行邻带转换再确定是加1还是减1。
        
*/
void GeoToGauss(double jd, double wd, short DH, short DH_width, double *y, double *x, double LP)
{
	double t;		//  t=tgB
	double L;		//  中央经线的经度
	double l0;		//  经差
	double jd_hd, wd_hd;	//  将jd、wd转换成以弧度为单位
	double et2;		//  et2 = (e' ** 2) * (cosB ** 2)
	double N;		//  N = C / sqrt(1 + et2)
	double X;		//  克拉索夫斯基椭球中子午弧长
	double m;		//  m = cosB * PI/180 * l0
	double tsin, tcos;	//  sinB,cosB

	double PI = 3.14159265358979;
	double b_e2 = 0.0067385254147;
	double b_c = 6399698.90178271;

	jd_hd = jd / 3600.0 * PI / 180.0;	// 将以秒为单位的经度转换成弧度
	wd_hd = wd / 3600.0 * PI / 180.0;	// 将以秒为单位的纬度转换成弧度

	// 如果不设中央经线（缺省参数: -1000），则计算中央经线，
	// 否则，使用传入的中央经线，不再使用带号和带宽参数
	//L = (DH - 0.5) * DH_width ;      // 计算中央经线的经度
	if (LP == -1000) {
		L = (DH - 0.5) * DH_width;	// 计算中央经线的经度
	} else {
		L = LP;
	}

	l0 = jd / 3600.0 - L;	// 计算经差
	tsin = sin(wd_hd);	// 计算sinB
	tcos = cos(wd_hd);	// 计算cosB
	// 计算克拉索夫斯基椭球中子午弧长X
	X = 111134.8611 / 3600.0 * wd - (32005.7799 * tsin + 133.9238 * pow(tsin, 3)
					 + 0.6976 * pow(tsin, 5) + 0.0039 * pow(tsin, 7)) * tcos;
	et2 = b_e2 * pow(tcos, 2);	//  et2 = (e' ** 2) * (cosB ** 2)
	N = b_c / sqrt(1 + et2);	//  N = C / sqrt(1 + et2)
	t = tan(wd_hd);		//  t=tgB
	m = PI / 180 * l0 * tcos;	//  m = cosB * PI/180 * l0
	*x = X + N * t * (0.5 * pow(m, 2)
			  + (5.0 - pow(t, 2) + 9.0 * et2 + 4 * pow(et2, 2)) * pow(m, 4) / 24.0 + (61.0 - 58.0 * pow(t, 2) + pow(t, 4)) * pow(m, 6) / 720.0);
	*y = N * (m + (1.0 - pow(t, 2) + et2) * pow(m, 3) / 6.0 + (5.0 - 18.0 * pow(t, 2) + pow(t, 4) + 14.0 * et2 - 58.0 * et2 * pow(t, 2)) * pow(m, 5) / 120.0);

}

#include <stdio.h>


#define PI                      3.1415926
#define EARTH_RADIUS            6378137        //地球近似半径(米)

// 求弧度
double radian(double d)
{
    return d * PI / 180.0;   //角度1˚ = π / 180
}

//计算地理经纬坐标距离
double get_distance(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = radian(lat1);
    double radLat2 = radian(lat2);
    double a = radLat1 - radLat2;
    double b = radian(lng1) - radian(lng2);
    
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));
    
    dst = dst * EARTH_RADIUS;
    return dst;
}


//高斯平面坐标系
typedef struct
{
    double x;
    double y;
    double z;
} CRDCARTESIAN;

//大地坐标系（可以是 北京54坐标系，西安80坐标系，WGS84坐标系（GPS 坐标））
typedef struct
{
    double longitude; //经度
    double latitude; //纬度 
    double height; //大地高,可设为0
} CRDGEODETIC;


// 两点之间的距离
double twoPointDistance(double x1,double y1,double x2,double y2) {
    return sqrt(pow((y2 - y1),2) + pow((x2 - x1),2));
}

int main() {
    
    static CRDGEODETIC 
    c1={
        longitude: 105,
        latitude: 3.8 
    },
    c2={
        longitude: 115,
        latitude: 30.3
    };
    
    double d0 = get_distance(c1.latitude, c1.longitude,c2.latitude,c2.longitude);
    
    double X1,Y1,X2,Y2;
    GeoToGauss(c1.longitude*3600 , c1.latitude*3600, 0, 0,&X1,&Y1,110);
    GeoToGauss(c2.longitude*3600 , c2.latitude*3600, 0, 0,&X2,&Y2,110);
    double d1 = twoPointDistance(X1,Y1,X2,Y2);

    printf(">> d0=%0.3f,  d1=%0.3f,  Δ=%0.3f\n",d0,d1,d0-d1);
    
    printf(">> %lf\n",(d0-d1)/d1);
}

