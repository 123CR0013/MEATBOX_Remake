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


public:
	float x, y;
};


// �I�y���[�^
const Vector2 operator+(const Vector2& left, const Vector2& right);
const Vector2 operator-(const Vector2& left, const Vector2& right);
const Vector2 operator*(const Vector2& left, const Vector2& right);
const Vector2 operator*(const Vector2& left, const float right);
const Vector2 operator*(const float left, const Vector2& right);
const Vector2 operator/(const Vector2& left, const Vector2& right);
const Vector2 operator/(const Vector2& left, const float right);
const Vector2 operator/(const float left, const Vector2& right);