#pragma once

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	virtual ~Vector2() {};

	// �I�y���[�^
	Vector2 operator+=(const Vector2& right);
	Vector2 operator-=(const Vector2& right);
	Vector2 operator*=(const Vector2& right);
	Vector2 operator*=(const float right);
	Vector2 operator/=(const Vector2& right);
	Vector2 operator/=(const float right);

	// �I�y���[�^
	friend const Vector2 operator+(const Vector2& left, const Vector2& right);
	friend const Vector2 operator-(const Vector2& left, const Vector2& right);
	friend const Vector2 operator*(const Vector2& left, const Vector2& right);
	friend const Vector2 operator*(const Vector2& left, const float right);
	friend const Vector2 operator*(const float left, const Vector2& right);
	friend const Vector2 operator/(const Vector2& left, const Vector2& right);
	friend const Vector2 operator/(const Vector2& left, const float right);
	friend const Vector2 operator/(const float left, const Vector2& right);
	friend const Vector2 operator*(const Vector2& left, const class Matrix3& right);

	bool operator==(const Vector2& right);
	bool operator!=(const Vector2& right);

	// ���K��
	static Vector2 Normalize(const Vector2& v);
	// ����
	static float Dot(const Vector2& v1, const Vector2& v2);
	// �傫��
	static float Length(const Vector2& v);
	// 2�_�Ԃ̋���
	static float Length(const Vector2& v1, const Vector2& v2);
	// �傫����2��
	static float SquareLength(const Vector2& v);
	// 2�_�Ԃ̋�����2��
	static float SquareLength(const Vector2& v1, const Vector2& v2);
	// 2�̃x�N�g���̂Ȃ��p
	static float Angle(const Vector2& v1, const Vector2& v2);

	static Vector2 Lerp(const Vector2& start, const Vector2& end, const float rate);


	// Reflect V about (normalized) N
	static Vector2 Reflect(const Vector2& v, const Vector2& n)
	{
		return v - 2.0f * Vector2::Dot(v, n) * n;
	}

	// Transform vector by matrix
	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);
public:
	float x, y;
};

