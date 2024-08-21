#pragma once
#include"DxLib.h"

class Vector3 {
public:
	Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(const Vector3& right) {
		*this = right;
	}

	Vector3(const VECTOR& right) {
		this->x = right.x;
		this->y = right.y;
		this->z = right.z;
	}

	virtual ~Vector3() {}

	//DxLib��VECTOR�\���݂̂����Ɏg�p�������̂�public
	float x, y, z;

	//�l�̑��
	const Vector3& operator = (const Vector3& right) {
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	//�x�N�g���̉��Z���
	const Vector3& operator += (const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	//�x�N�g���̌��Z���
	const Vector3& operator -= (const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//�x�N�g���̏�Z���
	const Vector3& operator *= (const float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	//�x�N�g���̏��Z���
	const Vector3& operator /= (const float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	//�x�N�g���̉��Z
	friend Vector3 operator+(const Vector3& left, const Vector3& right) {
		return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	//�x�N�g���̌��Z
	friend Vector3 operator-(const Vector3& left, const Vector3& right) {
		return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	//�x�N�g���̏�Z
	friend Vector3 operator * (const Vector3& left, float right) {
		return Vector3(left.x * right, left.y * right, left.z * right);
	}

	//�x�N�g���̏��Z���
	friend Vector3 operator / (const Vector3& left, float right) {
		return Vector3(left.x / right, left.y / right, left.z / right);
	}

	//��̍��W����������
	bool operator == (const Vector3& right);

	//��̍��W���������Ȃ���
	bool operator != (const Vector3& right);

	//�ǂ���̃x�N�g���̕����傫����(���_���猩��)
	bool operator < (const Vector3& right);
	bool operator > (const Vector3& right);

	//����
	static const float Dot(const Vector3& v1, const Vector3& v2);
	const float Dot(const Vector3& v);

	//�O��
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	Vector3 Cross(const Vector3& other);

	//�x�N�g���̒����̓��
	const float LengthSquare();
	static float LengthSquare(const Vector3& v1,const Vector3& v2);

	//�x�N�g���̒���
	const float Length();
	static float Length(const Vector3& vector);
	static float Length(const Vector3& v1,const Vector3& v2);

	//���K��
	Vector3 Normalize();
	static Vector3 Normalize(const Vector3& vector);
	void Normalized();

	//�d�_
	static Vector3 Emphasis(const Vector3& ver1,const Vector3& ver2,const Vector3& ver3);

	//���`�⊮
	static Vector3 Lerp(const Vector3& start,const Vector3& end,float t);

	//�@���x�N�g���ɂ��x�N�g���̔���
	static Vector3 Reflect(const Vector3& vec,const Vector3& normal);

	//��̃x�N�g���̂Ȃ��p�x�����߂�(radian)
	static float Angle(const Vector3& from, const Vector3& to);

	//�x�N�g���̓��l����
	static bool Equal(const Vector3& left, const Vector3& right, float dist);

	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
	// This will transform the vector and renormalize the w component
	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	// Transform a Vector3 by a quaternion
	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	VECTOR ToVECTOR() {
		return VGet(x, y, z);
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegUnitX;
	static const Vector3 NegUnitY;
	static const Vector3 NegUnitZ;
};
