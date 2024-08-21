#pragma once
#include "appframe.h"
#include "AnimationInfo.h"

class ObjectBase;

class Animation final
{
public:
	Animation(ObjectBase* object);
	~Animation();

	void Process();
	void Draw();

	void AddAnimInfo(AnimationInfo* animInfo);
	void SetAnimIndex(int index);
	int GetAnimIndex() { return _animIndex; }

	bool IsEnd() { return _bAnimEnd; }

	int GetGraphHandle();


	void SetZoom(float zoom) { _zoom = zoom; }
	void SetAngle(float angle) { _angle = angle; }
	void SetWidth(float width) { _width = width; }
	void SetHeight(float height) { _height = height; }
	void SetSize(float width, float height) { _width = width; _height = height; }


protected:
	ObjectBase* _parentObj;

	// アニメーション情報
	std::vector<AnimationInfo*> _animInfo;
	// アニメーションインデックス
	int _animIndex;
	// アニメーションインデックスの最大値
	int _animIndexMax;
	// アニメーションカウンタ
	int _animCnt;

	// アニメーション終了フラグ
	bool _bAnimEnd;


	float _zoom;
	float _angle;
	float _width, _height;
};


