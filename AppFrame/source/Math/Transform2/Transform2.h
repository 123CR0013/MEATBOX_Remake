#pragma once
#include"../Vector2/Vector2.h"
#include"../Matrix4/Matrix4.h"

class Transform2 {
public:

	Vector2 mLocation;
	float mRotate;
	Vector2 mScale;

	Transform2()
		:mRotate(0.f)
		,mScale(1.f,1.f)
	{}
	Transform2(const Vector2& pos)
		:mLocation(pos)
		, mRotate(0.f)
		, mScale(1.f, 1.f)
	{}
	Transform2(float rad,Vector2 zoom, const Vector2& pos)
		:mLocation(pos)
		,mRotate(rad)
		,mScale(zoom)
	{}
	Transform2(const Transform2& transform) {
		*this = transform;
	}

	void operator = (const Transform2& right) {
		mLocation = right.mLocation;
		mRotate = right.mRotate;
		mScale = right.mScale;
	}

	Matrix4 CreateMatrix4() {	
		return Matrix4::CreateRotationZ(mRotate) * Matrix4::CreateScale(mScale.x, mScale.y, 1.f) * Matrix4::CreateTranslation(Vector3(mLocation.x, mLocation.y, 0.f));
	}
};