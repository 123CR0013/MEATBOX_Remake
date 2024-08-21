#pragma once

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	virtual ~Vector2() {};

	// オペレータ
	Vector2 operator+=(const Vector2& right);
	Vector2 operator-=(const Vector2& right);
	Vector2 operator*=(const Vector2& right);
	Vector2 operator*=(const float right);
	Vector2 operator/=(const Vector2& right);
	Vector2 operator/=(const float right);

	bool operator==(const Vector2& right);
	bool operator!=(const Vector2& right);

	// 正規化
	static Vector2 Normalize(const Vector2& v);
	// 内積
	static float Dot(const Vector2& v1, const Vector2& v2);
	// 大きさ
	static float Length(const Vector2& v);
	// 2点間の距離
	static float Length(const Vector2& v1, const Vector2& v2);
	// 大きさの2乗
	static float SquareLength(const Vector2& v);
	// 2点間の距離の2乗
	static float SquareLength(const Vector2& v1, const Vector2& v2);
	// 2つのベクトルのなす角
	static float Angle(const Vector2& v1, const Vector2& v2);


public:
	float x, y;
};


// オペレータ
const Vector2 operator+(const Vector2& left, const Vector2& right);
const Vector2 operator-(const Vector2& left, const Vector2& right);
const Vector2 operator*(const Vector2& left, const Vector2& right);
const Vector2 operator*(const Vector2& left, const float right);
const Vector2 operator*(const float left, const Vector2& right);
const Vector2 operator/(const Vector2& left, const Vector2& right);
const Vector2 operator/(const Vector2& left, const float right);
const Vector2 operator/(const float left, const Vector2& right);