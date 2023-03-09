#include <math.h>

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


#define WGS84 84    //WGS84坐标系（GPS 坐标）
#define BJ54 54     //北京54坐标系
#define XIAN80 80   //西安80坐标系
#define ZONEWIDE3 3 //投影带宽度 3
#define ZONEWIDE6 6 //投影带宽度 6


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

//---------------------------------------------------------------------------
void BLTOXY(CRDCARTESIAN * pcc, CRDGEODETIC * pcg, int Datum, int zonewide)
{
    double B = pcg->latitude; //纬度
    double L = pcg->longitude; //经度//纬度度数

    double L0; //中央经线度数
    double N; //卯酉圈曲率半径 
    double q2;
    double x; //高斯平面纵坐标
    double y; //高斯平面横坐标
    double s; //赤道至纬度B的经线弧长
    double f; //参考椭球体扁率
    double e1; //椭球第一偏心率
    double a; //参考椭球体长半轴
    //double b;    //参考椭球体短半轴
    double a1, a2, a3, a4;
    double b1, b2, b3, b4;
    double c0, c1, c2, c3;

    const double IPI = 0.0174532925199433333333; //3.1415926535898/180.0

    int prjno = 0; //投影带号   
    // zonewide 投影带宽带， 3 或者是 6
    if (zonewide == 6)
    {
        prjno = (int) (L / zonewide) + 1;
        L0 = prjno * zonewide - 3;
    }
    else
    {
        prjno = (int) ((L - 1.5) / 3) + 1;
        L0 = prjno * 3;
    }

    /*
     * 北京 54
     * 长半轴a=6378245m
     * 短半轴b=6356863.0188m
     * 扁率α=1/298.3
     * 第一偏心率平方 =0.006693421622966
     * 第二偏心率平方 =0.006738525414683 
     * 
     * 西安80
     * 长半轴a=6378140±5（m）
     * 短半轴b=6356755.2882m
     * 扁率α=1/298.257
     * 第一偏心率平方 =0.00669438499959
     * 第二偏心率平方=0.00673950181947 
     * 
     * WGS84
     * 长半轴a=6378137± 2（m）
     * 短半轴b=6356752.3142m
     * 扁率α=1/298.257223563
     * 第一偏心率平方 =0.00669437999013
     * 第二偏心率平方 =0.00673949674223
     * 
     */

    //Datum 投影基准面类型：北京54基准面为54，西安80基准面为80，WGS84基准面为84
    if (Datum == 84)
    {
        a = 6378137;
        f = 1 / 298.257223563;
    }
    else if (Datum == 54)
    {
        a = 6378245;
        f = 1 / 298.3;
    }
    else if (Datum == 80)
    {
        a = 6378140;
        f = 1 / 298.257;
    }

    e1 = 2 * f - f*f; //(a*a-b*b)/(a*a) 椭球第一偏心率

    L0 = L0*IPI; // 转为弧度
    L = L*IPI; // 转为弧度
    B = B*IPI; // 转为弧度

    double sinB = sin(B); //sinB
    double cosB = cos(B); //cosB
    double tanB = tan(B); //tanB

    double l = L - L0; //L-L0l
    double m = l * cosB; //ltanB

    N = a / sqrt(1 - e1 * pow(sinB, 2));
    q2 = e1 / (1 - e1) * pow(cosB, 2);

    a1 = 1 + 3.0 / 4.0 * e1 + 45.0 / 64.0 * pow(e1, 2) + 175.0 / 256.0 * pow(e1, 3)
            + 11025.0 / 16384.0 * pow(e1, 4) + 43659.0 / 65536.0 * pow(e1, 5);

    a2 = 3.0 / 4.0 * e1 + 15.0 / 16.0 * pow(e1, 2) + 525.0 / 512.0 * pow(e1, 3)
            + 2205.0 / 2048.0 * pow(e1, 4) + 72765.0 / 65536.0 * pow(e1, 5);
    
    a3 = 15.0 / 64.0 * pow(e1, 2) + 105.0 / 256.0 * pow(e1, 3) + 2205.0 / 4096.0
            * pow(e1, 4) + 10359.0 / 16384.0 * pow(e1, 5);
    
    a4 = 35.0 / 512.0 * pow(e1, 3) + 315.0 / 2048.0 * pow(e1, 4) + 31185.0 / 13072.0
            * pow(e1, 5);
    b1 = a1 * a * (1 - e1);
    b2 = -1.0 / 2.0 * a2 * a * (1 - e1);
    b3 = 1.0 / 4.0 * a3 * a * (1 - e1);
    b4 = -1.0 / 6.0 * a4 * a * (1 - e1);
    c0 = b1;
    c1 = 2 * b2 + 4 * b3 + 6 * b4;
    c2 = -(8 * b3 + 32 * b4);
    c3 = 32 * b4;
    s = c0 * B + cosB * (c1 * sinB + c2 * pow(sinB, 3) + c3 * pow(sinB, 5));
    
    x = s + 0.5 * N * tanB * pow(m, 2) + 1.0 / 24.0 * (5 - pow(tanB, 2) + 9 * q2 + 4
            * pow(q2, 2)) * N * tanB * pow(m, 4) + 1.0 / 720.0 * (61 - 58 * pow(tanB, 6))
            * N * tanB * pow(m, 6);
    
    y = N * m + 1.0 / 6.0 * (1 - pow(tanB, 2) + q2) * N * pow(m, 3) + 1.0 / 120.0
            * (5 - 18 * tanB * tanB + pow(tanB, 4) - 14 * q2 - 58 * q2 * pow(tanB, 2)) * N * pow(m, 5);

    y = y + 1000000 * prjno + 500000;
    pcc->x = x;
    pcc->y = y - 38000000;
    pcc->z = 0;
}


double BLDistance(CRDGEODETIC *pcg1, CRDGEODETIC *pcg2, int Datum, int zonewide)
{
    CRDCARTESIAN pcc1, pcc2;
    BLTOXY(&pcc1, pcg1, Datum, zonewide);
    BLTOXY(&pcc2, pcg2, Datum, zonewide);

    double xdes = fabs(pcc1.x - pcc2.x);
    double ydes = fabs(pcc1.y - pcc2.y);
    double des = sqrt(xdes * xdes + ydes * ydes);

    return des;
}

#include <stdio.h>
int main() {
    
    static CRDGEODETIC 
    c1={
        longitude: 114,
        latitude: 39.8 
    },
    c2={
        longitude: 115,
        latitude: 40.3
    };
    
    double d0 = get_distance(c1.latitude, c1.longitude,c2.latitude,c2.longitude);
    double d1=BLDistance(&c1 , &c2, WGS84, ZONEWIDE6);
    printf(">> d1=%0.3f,  d2=%0.3f,  Δ=%0.3f\n",d0,d1,d0-d1);
    
    printf(">> %lf\n",(d0-d1)/d1);
}
