#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"

class Plane
{
public:
	Plane::Plane(): normal(1.0f, 0.0f, 0.0f),offset(0){}

	Plane::Plane(Vector3& _pointOnPlane, Vector3& _normal)
	{
		float length = _normal.Magnitude();
		if (length <= MIN_FLOAT_THRESHOLD)
		{
			normal.Set(1, 0, 0);
		}
		else
		{
			normal = _normal.Normalize();
			offset = normal.Dot(_pointOnPlane);
		}
	}


	static float Plane::PointAndPlaneDistance(Plane& _plane, Vector3 _point)
	{
		/*
		plane origin = (x0, y0, c0) and a random point on plane = (x1, y1, z1)
		point = (x2, y2, z2)
		Min distance between a point and a plane is given by the formula
		d=(ax2 + by2 + cz2 + d)/sqrt(a^2+b^2+c^2);
		where the plane is defined by ax1 + by1 + cz1 + d = 0
		where d = -(ax0 + by0 + cz0)

		distance = p.dot(n) - offset; where n is unit vector*/

		float distance = 0.0f;
		distance = abs(_point.Dot(_plane.normal) - _plane.offset);
		return distance;
	}

	Vector3 GetNormal(){ return normal; }

private:
	Vector3 normal;
	float offset;
};

#endif


