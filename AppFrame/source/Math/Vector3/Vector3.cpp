#include "Vector3.h"
#include "../MyMath/MyMath.h"
#include "../Matrix4/Matrix4.h"
#include "../Quaternion/Quaternion.h"

bool Vector3::operator == (const Vector3& right) {
	if (fabsf(x - right.x) > 0.f) { return false; }
	if (fabsf(y - right.y) > 0.f) { return false; }
	if (fabsf(z - right.z) > 0.f) { return false; }
	return true;
}

bool Vector3::operator != (const Vector3& right) {
	if (fabs(x - right.x) > 0.f) { return true; }
	if (fabs(y - right.y) > 0.f) { return true; }
	if (fabs(z - right.z) > 0.f) { return true; }
	return false;
}

bool Vector3::operator < (const Vector3& right) {
	return this->LengthSquare() < LengthSquare(Vector3(),right);
}
bool Vector3::operator > (const Vector3& right) {
	return this->LengthSquare() > LengthSquare(Vector3(), right);
}

const float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const float Vector3::Dot(const Vector3& v) {
	return Dot(*this, v);
}

const float Vector3::Length() {
	return sqrt(LengthSquare());
}

float Vector3::Length(const Vector3& vector) {
	return sqrt(Dot(vector, vector));
}

float Vector3::Length(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1 - v2).Length();
}
float Vector3::LengthSquare(const Vector3& v1,const Vector3& v2) {
	return Vector3(v1 - v2).LengthSquare();
}
const float Vector3::LengthSquare() {
	return (x * x + y * y + z + z);
}

Vector3 Vector3::Normalize(const Vector3& vector) {
	float length = Length(vector);
	return MyMath::NearZero(length) ? vector : vector / length;
}

Vector3 Vector3::Normalize() {
	return Normalize(*this);
}

void Vector3::Normalized() {
	*this = Normalize();
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 Vector3::Cross(const Vector3& other) {
	return Cross(*this, other);
}

Vector3 Vector3::Emphasis(const Vector3& ver1, const Vector3& ver2, const Vector3& ver3) {
	return (ver1 + ver2 + ver3) / 3;
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t) {
	return start + (end - start) * t;
}

Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal) {
	return vec - normal * (2.0f * Vector3::Dot(vec, normal));
}

float Vector3::Angle(const Vector3& from, const Vector3& to) {
	return acos(Vector3::Dot(from, to) / (Vector3::Length(from) * Vector3::Length(to)));
}

bool Vector3::Equal(const Vector3& left, const Vector3& right, float dist) {
	return LengthSquare(left, right) < (dist * dist);
}

Vector3 Vector3::Transform(const Vector3& vec, const Matrix4& mat, float w /*= 1.0f*/)
{
	Vector3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	//ignore w since we aren't returning a new value for it...
	return retVal;
}

// This will transform the vector and renormalize the w component
Vector3 Vector3::TransformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w /*= 1.0f*/)
{
	Vector3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	float transformedW = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
		vec.z * mat.mat[2][3] + w * mat.mat[3][3];
	if (!MyMath::NearZero(fabs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}
	return retVal;
}

// Transform a Vector3 by a quaternion
Vector3 Vector3::Transform(const Vector3& v, const Quaternion& q)
{
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3 qv(q.x, q.y, q.z);
	Vector3 retVal = v;
	retVal += Vector3::Cross(qv, Vector3::Cross(qv, v) + v * q.w) * 2.f;
	return retVal;
}

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.f);
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NegUnitX(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NegUnitY(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NegUnitZ(0.0f, 0.0f, -1.0f);