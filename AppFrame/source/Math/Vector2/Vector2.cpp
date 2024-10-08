#include "Vector2.h"
#include "../MyMath/mymath.h"

const Vector2 operator+(const Vector2& left, const Vector2& right)
{
	return Vector2(left) += right;
}

const Vector2 operator-(const Vector2& left, const Vector2& right)
{
	return Vector2(left) -= right;
}

const Vector2 operator*(const Vector2& left, const Vector2& right)
{
	return Vector2(left) *= right;
}

const Vector2 operator*(const Vector2& left, const float right)
{
	return Vector2(left) *= right;
}

const Vector2 operator*(const float left, const Vector2& right)
{
	return Vector2(right) *= left;
}

const Vector2 operator/(const Vector2& left, const Vector2& right)
{
	return Vector2(left) /= right;
}

const Vector2 operator/(const Vector2& left, const float right)
{
	return Vector2(left) /= right;
}

const Vector2 operator/(const float left, const Vector2& right)
{
	return Vector2(right) /= left;
}

Vector2 Vector2::operator+=(const Vector2& right) 
{
	x = x + right.x;
	y = y + right.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& right)
{
	x = x - right.x;
	y = y - right.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& right)
{
	x = x * right.x;
	y = y * right.x;
	return *this;
}

Vector2 Vector2::operator*=(const float right)
{
	x = x * right;
	y = y * right;
	return *this;
}

Vector2 Vector2::operator/=(const Vector2& right)
{
	x = x / right.x;
	y = y / right.y;
	return *this;
}

Vector2 Vector2::operator/=(const float right)
{
	x = x / right;
	y = y / right;
	return *this;
}

bool Vector2::operator==(const Vector2& right)
{
	return (x == right.x && y == right.y);
}

bool Vector2::operator!=(const Vector2& right)
{
	return (x != right.x || y != right.y);
}

Vector2 Vector2::Normalize(const Vector2& v)
{
	const float len = Length(v);
	if (len == 0.0f) return Vector2(0, 0);
	return v / len;
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Length(const Vector2& v)
{
	return sqrt(SquareLength(v));
}

float Vector2::Length(const Vector2& v1, const Vector2& v2)
{
	Vector2 tmp = v1 - v2;
	return Length(tmp);
}

float Vector2::SquareLength(const Vector2& v)
{
	return (v.x * v.x) + (v.y * v.y);
}

float Vector2::SquareLength(const Vector2& v1, const Vector2& v2)
{
	Vector2 tmp = v1 - v2;
	return SquareLength(tmp);
}

float Vector2::Angle(const Vector2& v1, const Vector2& v2)
{
	float sqLen1 = SquareLength(v1);
	float sqLen2 = SquareLength(v2);
	if(sqLen1 > 0.000000f && sqLen2 > 0.000000f) {
		float cos = Dot(v1, v2) / (sqrt(sqLen1) * sqrt(sqLen2));
		return acos(cos);
	}
	return 0.0f;
}

