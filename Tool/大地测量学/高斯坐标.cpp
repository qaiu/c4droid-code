/*
   百度百科词条: 高斯-克吕格投影
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define PI                      3.1415926
#define EARTH_RADIUS            6378137        //地球近似半径(米)


// 高斯投影坐标转经纬度坐标
// 参数: (x，y) 高斯直角坐标 单位-米
// 返回: (longitude, latitude)经纬度坐标 单位-度
int XY2LongLat(double X,double Y, double* longitude, double* latitude)
{
	int ProjNo; 
	int ZoneWide; //带宽 
	double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
	double e1,e2,f,a, ee, NN, T,C, M, D,R,u,fai, iPI;
	iPI = 0.0174532925199433; // 3.1415926535898/180.0;
	a = 6378137.0; f = 1.0/298.257223563;//WGS84坐标系参数
	ProjNo = (int)(X/1000000L) ; //查找带号
	// 	ZoneWide = 6; //6度带宽 
	// 	longitude0 = (ProjNo-1) * ZoneWide + ZoneWide / 2; //计算每带中央子午线经度
	ZoneWide=3;   //3度带宽
	longitude0 =  ProjNo * ZoneWide;
	longitude0 = longitude0 * iPI ; //中央经线

	X0 = ProjNo*1000000L+500000L; 
	Y0 = 0; 
	xval = X-X0; yval = Y-Y0; //带内大地坐标
	e2 = 2*f-f*f;
	e1 = (1.0-sqrt(1-e2))/(1.0+sqrt(1-e2));
	ee = e2/(1-e2);
	M = yval;
	u = M/(a*(1-e2/4-3*e2*e2/64-5*e2*e2*e2/256));
	fai = u+(3*e1/2-27*e1*e1*e1/32)
		*sin(2*u)+(21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*u)
		+(151*e1*e1*e1/96)*sin(6*u)+(1097*e1*e1*e1*e1/512)*sin(8*u);
	C = ee*cos(fai)*cos(fai);
	T = tan(fai)*tan(fai);
	NN = a/sqrt(1.0-e2*sin(fai)*sin(fai));// 字串1 
	R = a*(1-e2)/sqrt((1-e2*sin(fai)*sin(fai))
		*(1-e2*sin(fai)*sin(fai))*(1-e2*sin(fai)*sin(fai)));
		
	D = xval/NN;
	//计算经度(Longitude) 纬度(Latitude)
	longitude1 = longitude0+(D-(1+2*T+C)*D*D*D/6
			+(5-2*C+28*T-3*C*C+8*ee+24*T*T)*D*D*D*D*D/120)/cos(fai);
			
	latitude1 = fai -(NN*tan(fai)/R)*(D*D/2
			-(5+3*T+10*C-4*C*C-9*ee)*D*D*D*D/24
			+(61+90*T+298*C+45*T*T-256*ee-3*C*C)*D*D*D*D*D*D/720); 
	int g=0;
	//转换为度 DD
	*longitude = longitude1 / iPI; 
	*latitude = latitude1 / iPI;

	return 1;
}

// 经纬度坐标转高斯投影坐标
// 参数: (longitude, latitude)经纬度坐标 单位-度
// 返回: (x，y) 高斯直角坐标 单位-米
int LongLat2XY(double longitude,double latitude,double *X,double *Y)
{
	int ProjNo=0; 
	int ZoneWide; //带宽 
	double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
	double a,f, e2,ee, NN, T,C,A, M, iPI;
	iPI = 0.0174532925199433; // 3.1415926535898/180.0;
	ZoneWide = 3; //3度带宽
	//ZoneWide = 6; //6度带宽 
	a = 6378137.0; f = 1.0/298.257223563;//WGS84坐标系参数
	//ProjNo = (int)(longitude / ZoneWide) ;      //6度带
	//longitude0 = ProjNo * ZoneWide + ZoneWide / 2; //6度带
	ProjNo = (int)(longitude / ZoneWide+0.5) ; 
    // ProjNo = (int)(longitude / ZoneWide) ; //--带号
	longitude0 = ProjNo * ZoneWide ; //--中央子午线
	
	
    printf("l: %0.3f,p:%d\n", longitude0,ProjNo);
	
	longitude0 = longitude0 * iPI ;//--中央子午线转化为弧度
	latitude0=0; 
	longitude1 = longitude * iPI ; //经度转换为弧度
	latitude1 = latitude * iPI ; //纬度转换为弧度
	e2=2*f-f*f;
	ee=e2*(1.0-e2);
	NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
	T=tan(latitude1)*tan(latitude1);
	C=ee*cos(latitude1)*cos(latitude1);
	A=(longitude1-longitude0)*cos(latitude1); 	

	M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1
		-(3*e2/8+3*e2*e2/32+45*e2*e2
		*e2/1024)*sin(2*latitude1)
		+(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)
		-(35*e2*e2*e2/3072)*sin(6*latitude1));

	xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
	yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
		+(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
	//X0 = 1000000L*(ProjNo+1)+500000L; //6度带
	X0 = 1000000L*ProjNo+500000L;  //3度带
	Y0 = 0; 
	xval = xval+X0; yval = yval+Y0; 

	*X= xval;
	*Y= yval;
    printf("%lf   %lf\r\n",xval,yval);
    
	return 1;
}

// 两点之间的距离
double twoPointDistance(double x1,double y1,double x2,double y2) {
    return sqrt(pow((y2 - y1),2) + pow((x2 - x1),2));
}

// 点到直线距离
double point2lineDistance(double pointx, double pointy, double x1, double y1, double x2, double y2)
{
	double a = y2 - y1;
	double b = x1 - x2;
	double x = x2 * y1 - x1 * y2;
	return fabs(a * pointx + b * pointy + x) / sqrt(a * a + b * b);
}

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

void test_get_distance(){
    
    double lat1 = 38.8;
    double lng1 = 114; //经度,纬度1
    double lat2 = 33.3;
    double lng2 = 115; //经度,纬度2
    
    // 经纬度距离
    double dst = get_distance(lat1, lng1, lat2, lng2);
    printf("dst = %0.3fm\n", dst);  //dst = 9281.165m
    
    double X1,Y1,X2,Y2;
    
    LongLat2XY(lng1,lat1, &X1,&Y1);
    LongLat2XY(lng2,lat2, &X2,&Y2);
    
    // 高斯投影平面距离
    double dst2 = twoPointDistance(X1,Y1,X2,Y2);
    printf("dst2 = %0.3fm\n", dst2);  //dst = 9294.319m
    
    // 误差分析
    printf("dt = %0.6fm\n", (dst2-dst)/dst);  //dst = 9294.319m
}


int main (int argc, const char * argv[])
{
    test_get_distance();
    return 0;
}
