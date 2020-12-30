/*
计算几何API
by QAIU
*/
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#ifndef M_PI
#define M_PI 3.141592654
#endif
#define INF 1E200
#define EP  1E-8
#define MAXV 300
#define PI  M_PI
#define zero(x) (((x)>0?(x):-(x))<EP)

/* 基本几何结构 */
struct POINT
{
	double x;
	double y;
	POINT(double a = 0, double b = 0)
	{
		x = a;
		y = b;
	}		 // constructor 
};
struct LINESEG
{
	POINT s;
	POINT e;
	LINESEG(POINT a, POINT b)
	{
		s = a;
		e = b;
	}
	LINESEG()
	{

	}
};

struct LINE		 // 直线的解析方程 a*x+b*y+c=0为统一表示，约定 a >= 0 
{
	double a;
	double b;
	double c;
	LINE(double d1 = 1, double d2 = -1, double d3 = 0)
	{
		a = d1;
		b = d2;
		c = d3;
	}
};

/*三维点线面*/
struct POINT3
{
	double x, y, z;
	POINT3(double a = 0, double b = 0,double c = 0)
	{
		x = a;
		y = b;
		z = c;
	}
};
struct LINE3
{
	POINT3 a, b;
	LINE3(POINT3 p1, POINT3 p2)
	{
		a = p1;
		b = p2;
	}
	LINE3()
	{

	}
};
struct PLANE3
{
	POINT3 a, b, c;
	PLANE3(POINT3 p1, POINT3 p2,POINT3 p3)
	{
		a = p1;
		b = p2;
		c = p3;
	}
	PLANE3()
	{

	}
};

//API部分
/*****************点的基本运算 *****************/
// 平面上两点之间距离
double dist(POINT p1, POINT p2);
// 平面上两点之间距离平方
double dist2(POINT p1, POINT p2);
// 判断两点是否重合 
bool equal_point(POINT p1, POINT p2);
// 矢量叉乘 (共点向量)
double multiply(POINT sp, POINT ep, POINT op);
//矢量叉乘 (任意向量)
double multiply(POINT st1, POINT ed1, POINT st2, POINT ed2);
// 矢量点乘 (共点向量)
double dotmultiply(POINT p1, POINT p2, POINT p0);
//矢量点乘 (任意键退出向量)
double dotmultiply(POINT st1, POINT ed1, POINT st2, POINT ed2);
// 判断点是否在线段上 
bool online(LINESEG l, POINT p);
//求一点饶某点旋转后的坐标 
POINT rotate(POINT o, double alpha, POINT p);
//  求矢量夹角 (共点向量)
double angle(POINT o, POINT s, POINT e);

/*************线段及直线的基本运算 *************/
// 点与线段的关系 
double relation(POINT p, LINESEG l);
// 获取点到线段的垂足坐标
POINT getRoot(POINT p, POINT st, POINT ed);
// 获取线段上某点右手垂直方向延长线的端点
POINT change(POINT st, POINT ed, POINT next, double l);
//  求点到线段所在直线垂线的垂足 
POINT perpendicular(POINT p, LINESEG l);
// 点到线段的最近点 
double ptolinesegdist(POINT p, LINESEG l, POINT & np);
//点到线段所在直线的距离 
double ptoldist(POINT p, LINESEG l);
// 点到折线集的最近距离 
double ptopointset(int vcount, POINT pointset[], POINT p, POINT & q);
// 判断圆是否在多边形内 
bool CircleInsidePolygon(int vcount, POINT center, double radius, POINT polygon[]);
// 求矢量夹角余弦 
double cosine(LINESEG l1, LINESEG l2);
// 求线段之间的夹角 
double lsangle(LINESEG l1, LINESEG l2);
// 判断线段是否相交(不含端点)
bool intersect_A(LINESEG u, LINESEG v);
// 判断线段是否相交(含端点)
bool intersect(LINESEG u, LINESEG v);
// 判断线段是否相交但不交在端点处 
bool intersect_l(LINESEG u, LINESEG v);
// 求线段的垂直平分线方程
LINE bisector(POINT a, POINT b);
// 求线段所在直线的方程 
LINE makeline(POINT p1, POINT p2);
// 求直线的斜率 
double slope(LINE l);
// 求直线的倾斜角 
double alpha(LINE l);
// 求点关于某直线的对称点 
POINT symmetry(LINE l, POINT p);
// 判断两条直线是否相交及求直线交点 
bool lineintersect(LINE l1, LINE l2, POINT & p);
// 判断线段是否相交若相交求线段交点 
bool intersection(LINESEG l1, LINESEG l2, POINT & inter);
// 点是否在直线同侧
bool SameSide(POINT p1, POINT p2, LINE line);
//线段的左右旋
int rotat(LINESEG l1, LINESEG l2);

/*************三角形基本运算 *************/
// 计算三角形面积,输入三顶点
double area_triangle(POINT p1, POINT p2, POINT p3);
// 计算三角形面积,输入三边长
double area_triangle(double a, double b, double c);
// 三角形外心
POINT circumcenter(POINT a, POINT b, POINT c);
// 三角形垂心
POINT perpencenter(POINT a, POINT b, POINT c);
//三角形 重心
POINT barycenter(POINT a, POINT b, POINT c);
// 三角形费马点
POINT fermentpoint(POINT a, POINT b, POINT c);
// 三角形求曲率半径
double RadiusCurvature(double a, double b, double c, double d);

/*************矩形的基本运算 *************/
// 已知矩形三点坐标，求第4点坐标 
POINT rect4th(POINT a, POINT b, POINT c);

/*****************圆的基本运算 *****************/
// 点是否在圆内 
bool point_in_circle(POINT o, double r, POINT p);
// 求不共线的三点所确定的圆 
bool cocircle(POINT p1, POINT p2, POINT p3, POINT & q, double &r);
// 两圆关系
int CircleRelation(POINT p1, double r1, POINT p2, double r2);
// 两圆交点
void c2point(POINT p1, double r1, POINT p2, double r2, POINT & rp1, POINT & rp2);
// 两圆相交公共面积
double c2area(POINT p1, double r1, POINT p2, double r2);
// 圆和直线关系
int clpoint(POINT p, double r, double a, double b, double c, POINT & rp1, POINT & rp2);
// 三角形内切圆： 
void incircle(POINT p1, POINT p2, POINT p3, POINT & rp, double &r);
// 求过圆外一点与圆相切的切点坐标
void cutpoint(POINT p, double r, POINT sp, POINT & rp1, POINT & rp2);
//判断圆是否在矩形内：
bool CircleRecRelation(POINT pc, double r, POINT pr1, POINT pr2, POINT pr3, POINT pr4);

/*****************多边形常用算法模块  *****************/
// 判断多边形是否简单多边形 
bool issimple(int vcount, POINT polygon[]);
//检查多边形顶点的凸凹性 
void checkconvex(int vcount, POINT polygon[], bool bc[]);
//判断多边形是否凸多边形 
bool isconvex(int vcount, POINT polygon[]);
// 求多边形面积 
double area_of_polygon(int vcount, POINT polygon[]);
//判断多边形顶点的排列方向，方法一 
bool isconterclock(int vcount, POINT polygon[]);
// 判断多边形顶点的排列方向，方法二 
bool isccwize(int vcount, POINT polygon[]);
//射线法判断点是否简单在多边形内 
int insidepolygon(int vcount, POINT Polygon[], POINT q);
//判断点是否在凸多边形内 
bool InsideConvexPolygon(int vcount, POINT polygon[], POINT q);
//寻找点集的graham算法 
void Graham_scan(POINT PointSet[], POINT ch[], int n, int &len);
//寻找点集凸包的卷包裹法 
void ConvexClosure(POINT PointSet[], POINT ch[], int n, int &len);
//判断线段是否在多边形内 
bool LinesegInsidePolygon(int vcount, POINT polygon[], LINESEG l);
//求简单多边形的重心 
POINT gravitycenter(int vcount, POINT polygon[]);
//求凸多边形的重心 
POINT gravitycenter(int vcount, POINT polygon[]);
//求肯定在给定多边形内的一个点 
POINT a_point_insidepoly(int vcount, POINT polygon[]);
//求从多边形外一点出发到该多边形的切线 
void pointtangentpoly(int vcount, POINT polygon[], POINT p, POINT & rp, POINT & lp);
//判断多边形的核是否存在 
bool core_exist(int vcount, POINT polygon[], POINT & p);

/************************球面计算************************/
//计算圆心角
double angle(double lng1, double lat1, double lng2, double lat2);
// 计算球面两点的直线距离,r为球半径
double line_dist(double r, double lng1, double lat1, double lng2, double lat2);
// 计算球面距离,r为球半径
double sphere_dist(double r, double lng1, double lat1, double lng2, double lat2);
/*****************三维立体几何计算 *****************/
// 计算cross product U x V
POINT3 xmult(POINT3 u, POINT3 v);
// 计算dot product U . V
double dmult(POINT3 u, POINT3 v);
// 矢量差 U - V
POINT3 subt(POINT3 u, POINT3 v);
// 取平面法向量
POINT3 pvec(PLANE3 s);
// 两点距离,单参数取向量大小
double distance(POINT3 p1, POINT3 p2);
// 向量大小
double vlen(POINT3 p);
// 判三点共线
int dots_inline(POINT3 p1, POINT3 p2, POINT3 p3);
// 判四点共面
int dots_onplane(POINT3 a, POINT3 b, POINT3 c, POINT3 d);
// 判点是否在线段上,包括端点和共线
int dot_online_in(POINT3 p, LINE3 l);
// 判点是否在线段上,不包括端点
int dot_online_ex(POINT3 p, LINE3 l);
// 判点是否在空间三角形上,不包括边界,三点共线无意义
int dot_inplane_ex(POINT3 p, PLANE3 s);
// 判两点在线段同侧,点在线段上返回0,不共面无意义
int same_side(POINT3 p1, POINT3 p2, LINE3 l);
// 判两点在线段异侧,点在线段上返回0,不共面无意义
int opposite_side(POINT3 p1, POINT3 p2, LINE3 l);
// 判两点在平面同侧,点在平面上返回0
int same_side(POINT3 p1, POINT3 p2, PLANE3 s);
// 判两点在平面异侧,点在平面上返回0
int opposite_side(POINT3 p1, POINT3 p2, PLANE3 s);
// 判两直线平行
int parallel(LINE3 u, LINE3 v);
// 判两平面平行
int parallel(PLANE3 u, PLANE3 v);
// 判直线与平面平行
int parallel(LINE3 l, PLANE3 s);
// 判两直线垂直
int perpendicular(LINE3 u, LINE3 v);
// 判两平面垂直
int perpendicular(PLANE3 u, PLANE3 v);
// 判直线与平面垂直
int perpendicular(LINE3 l, PLANE3 s);
// 判两线段相交,包括端点和部分重合
int intersect_in(LINE3 u, LINE3 v);
// 判两线段相交,不包括端点和部分重合
int intersect_ex(LINE3 u, LINE3 v);
// 判线段与空间三角形相交,包括交于边界和(部分)包含
int intersect_in(LINE3 l, PLANE3 s);
// 判线段与空间三角形相交,不包括交于边界和(部分)包含
int intersect_ex(LINE3 l, PLANE3 s);
// 计算两直线交点,注意事先判断直线是否共面和平行!
POINT3 intersection(LINE3 u, LINE3 v);
//计算直线与平面交点,注意事先判断是否平行,并保证三点不共线!
POINT3 intersection(LINE3 l, PLANE3 s);
//计算两平面交线,注意事先判断是否平行,并保证三点不共线!
LINE3 intersection(PLANE3 u, PLANE3 v);
// 点到直线距离
double ptoline(POINT3 p, LINE3 l);
// 直线到直线距离
double linetoline(LINE3 u, LINE3 v);
// 点到平面距离
double ptoplane(POINT3 p, PLANE3 s);
// 两平面夹角cos值
double angle_cos(PLANE3 u, PLANE3 v);
// 直线平面夹角sin值
double angle_sin(LINE3 l, PLANE3 s);


/*****************其他几何算法*****************/
//点到平面的距离： 
double P2planeDist(double x, double y, double z, double a, double b, double c, double d);
//镜面反射线： 
void reflect(double a1, double b1, double c1, double a2, double b2, double c2, double &a, double &b, double &c);
//矩形包含：  
bool r2inr1(double A, double B, double C, double D);
// 求包含点集的最小圆
void getMinAreaCir(POINT point[], int n, POINT & maxcic, double &radius);
// 求包含点集的最小矩形
double getMinAreaRect(POINT * ps, int n, POINT * ds);

#endif