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

	float GetZoom() { return _zoom; }
	void SetZoom(float zoom) { _zoom = zoom; }

	float GetAngle() { return _angle; }
	void SetAngle(float angle) { _angle = angle; }

	float GetWidth() { return _width; }
	void SetWidth(float width) { _width = width; }

	float GetHeight() { return _height; }
	void SetHeight(float height) { _height = height; }

	Vector2 GetSize() { return Vector2(_width, _height); }
	void SetSize(float width, float height) { _width = width; _height = height; }
	void SetSize(Vector2 size) { _width = size.x; _height = size.y; }

	void SetDrawWithScreenPos(bool bDrawWithScreenPos) { _bDrawWithScreenPos = bDrawWithScreenPos; }


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

	// _parentObjのvPosをスクリーン座標として扱うか
	bool _bDrawWithScreenPos = false;
};


