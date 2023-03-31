#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_
#include <cmath>
class Vector_2D
{
public:
	double x;
	double y;
public:
	Vector_2D():x(0.0),y(0.0){}
	Vector_2D(Vector_2D& sou):x(sou.x),y(sou.y) {}
	Vector_2D(double tx,double ty):x(tx),y(ty){}
public:
	const Vector_2D& operator=(const Vector_2D& sou)
	{
		x=sou.x;
		y=sou.y;
		return *this;
	}
	const Vector_2D operator*(const double real)
	{
		Vector_2D result(x*real,y*real);
		return result;
	}
	void operator+=(const Vector_2D& add)
	{
		x+=add.x;
		y+=add.y;
	}
	const Vector_2D operator-(const Vector_2D& sub)
	{
		Vector_2D result
		{
			x-sub.x,
			y-sub.y
		};
		return result;
	}
	double mod()
	{
		return sqrt(x*x+y*y);
	}
	Vector_2D unit_vector()
	{
		double L=mod();
		Vector_2D result
		{
			x/L,
			y/L
		};
		return result;
	}
};

#endif