//Generic vector class for 3D math and colors

#ifndef SUDOVECTOR_H
#define SUDOVECTOR_H

#include <math.h>

#define TWOPI 6.28318530718
#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

class vec3 {
public:
	//allow component access with .r .g .b, or .x .y .z
	union {
		float x;
		float r;
	};
	union {
		float y;
		float g;
	};
	union {
		float z;
		float b;
	};

	//[ CONSTRUCTORS ]-------------------------------------

	//init with no arguments - give zero vector
	vec3() : x(0.0f), y(0.0f), z(0.0f) {}

	//init with each component specified
	vec3(float x1, float  y1, float z1) : x(x1), y(y1), z(z1) {}

	//init all components to one specified value
	vec3(float v) : x(v), y(v), z(v) {}

	//[ OVERLOADS ]----------------------------------------

	//multiplication by constant
	template <typename ntype>
	const vec3& operator*=(ntype c);

	template <typename ntype>
	vec3 operator*(ntype c);

	//division by a constant
	template <typename ntype>
	const vec3& operator/=(ntype c);

	template <typename ntype>
	vec3 operator/(ntype c);

	//addition with other vector
	const vec3& operator+=(vec3 rhs);

	const vec3 operator+(vec3 rhs);

	//subtraction with other vector
	const vec3& operator-=(vec3 rhs);

	const vec3 operator-(vec3 rhs);
};

//[ COLORS ]-------------------------------------------

const vec3 RED = vec3(1.0f,0.0f,0.0f);
const vec3 GREEN = vec3(0.0f,1.0f,0.0f);
const vec3 BLUE = vec3(0.0f,0.0f,1.0f);
const vec3 WHITE = vec3(1.0f);
const vec3 GREY = vec3(0.5f);
const vec3 BLACK = vec3(0.0f);

//[ OVERLOADS ]----------------------------------------

//multiplication by constant
template <typename ntype>
const vec3& vec3::operator*=(ntype c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

template <typename ntype>
vec3 vec3::operator*(ntype c)
{
	return vec3(*this) *= c;
}

//division by a constant
template <typename ntype>
const vec3& vec3::operator/=(ntype c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

template <typename ntype>
vec3 vec3::operator/(ntype c)
{
	return vec3(*this) /= c;
}

//addition with other vector
const vec3& vec3::operator+=(const vec3 rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

const vec3 vec3::operator+(const vec3 rhs)
{
	return vec3(*this)+=rhs;
}

//subtraction with other vector
const vec3& vec3::operator-=(const vec3 rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

const vec3 vec3::operator-(const vec3 rhs)
{
	return vec3(*this)-=rhs;
}


//[ FUNCTIONS ]----------------------------------------

//length of a vector
float mag(const vec3 &v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

//dot product of two vectors
float dot(const vec3 &v1, const vec3 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

//cross product of two vectors
vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x
		);
}

//normalize a vector, modifying it
void inormalize(vec3& v)
{
	v /= mag(v);
}

//return normalized copy
vec3 normalized(const vec3& v)
{
	vec3 v2 = vec3(v);
	inormalize(v2);
	return v2;
}

//perform linear interpolation between two points
template <typename ntype>
vec3 interp(const vec3& v1, const vec3& v2, ntype c)
{
	return v1*c + v2*(1.0f-c);
}

//project a vector from a point
template <typename ntype>
vec3 project(const vec3& point, const vec3& direction, ntype c)
{
	return point + direction*c;
}

//clamp all elements of a vector, modifying it
template <typename ntype>
void iclamp(vec3& v, ntype low, ntype high)
{
	v.x = (float)MIN(MAX(v.x,low),high);
	v.y = (float)MIN(MAX(v.y,low),high);
	v.z = (float)MIN(MAX(v.z,low),high);
}

//return clamped copy of a vector
template <typename ntype>
vec3 clamp(const vec3& v, ntype low, ntype high)
{
	vec3 v2 = vec3(v);
	iclamp(v2,low,high);
	return v2;
}

//abs all elements of a vector, modifying it
void iabs(vec3& v)
{
	v.x = abs(v.x);
	v.y = abs(v.y);
	v.z = abs(v.z);
}

//return absolute value vector
vec3 abs(const vec3& v)
{
	vec3 v2 = vec3(v);
	iabs(v2);
	return v2;
}

#endif