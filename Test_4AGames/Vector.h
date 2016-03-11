#ifndef VECTOR_H
#define VECTOR_H

struct Vector3
{
	Vector3()
	{
		Zero();
	}

	Vector3(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{

	}
	void Set(float _x, float _y, float _z){ x = _x, y = _y, z = _z; }
	void Zero() { x = y = z = 0; }
	void Identity() { x = y = z = 1; }
	void Invert(){ x *= -1, y *= -1, z *= -1; }
	Vector3 Normalize(){ return Vector3((x == 0 ? 0 : x / Magnitude()), (y == 0 ? 0 : y / Magnitude()), (z == 0 ? 0 : z / Magnitude())); }

	Vector3 ProjectionVector(Vector3& planeNorm)
	{
		Vector3 normal = planeNorm.Normalize();
		return normal * this->Dot(normal);
	}

	Vector3 ReflectionVector(Vector3& planeNorm)
	{
		Vector3 projectionVector = ProjectionVector(planeNorm);
		return *this - projectionVector * 2;
	}

	void operator()(const Vector3& rhs){ x = rhs.x, y = rhs.y, z = rhs.z; }
	Vector3 operator+(const Vector3& rhs){ return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator+=(const Vector3& rhs) { x + rhs.x, y + rhs.y, z + rhs.z; return *this; }
	Vector3 operator-(const Vector3& rhs){ return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator*(const float scale){ return Vector3(x * scale, y * scale, z * scale); }

	Vector3 Cross(const Vector3& rhs)
	{
		return Vector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	float Dot(const Vector3& rhs)
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	float Magnitude()
	{
		return max(MIN_FLOAT_THRESHOLD, sqrt(x*x + y*y + z*z));
	}

	float x, y, z;
};

#endif