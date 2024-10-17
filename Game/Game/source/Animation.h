#pragma once
#include "appframe.h"
#include "AnimationInfo.h"
#include <map>

class ObjectBase;

class Animation final
{
public:
	Animation(ObjectBase* object);
	~Animation();

	void Process();
	void Draw();

	void AddAnimInfo(AnimationInfo* animInfo, int index);
	void SetAnimIndex(int index);
	int GetAnimIndex() { return _groupIndex; }
	void ChangeRandomIndex();

	int GetAnimCnt() { return _animCnt; }
	void SetAnimCnt(int cnt);
	// アニメーションカウンタを指定
	// 何枚目の画像を表示するかを指定する（0~）
	void SetAnimCntForNumOfAnim(int cnt);

	bool IsEnd() { return _bAnimEnd; }

	int GetGraphHandle();

	Vector3 GetDrawOffset() { return _vDrawOffset; }
	void SetDrawOffset(Vector3 offset) { _vDrawOffset = offset; }

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

	int GetDrawOrder() { return _drawOrder; }
	void SetDrawOrder(int drawOrder) { _drawOrder = drawOrder; }

	void SetDrawWithScreenPos(bool bDrawWithScreenPos) { _bDrawWithScreenPos = bDrawWithScreenPos; }


protected:
	ObjectBase* _parentObj;

	// アニメーション情報
	std::map<int, std::vector<AnimationInfo*>> _animInfo;
	// グループ内のアニメーション数（ランダム再生用）
	std::map<int, int> _animNum;
	// グループインデックス
	int _groupIndex;
	// グループインデックスの最大値
	int _groupIndexMax;
	// ランダム再生用のインデックス
	int _randomIndex;
	// アニメーションカウンタ
	int _animCnt;

	// アニメーション終了フラグ
	bool _bAnimEnd;

	// 描画オフセット
	// _parentObjのvPosにこの値を加算して描画する
	Vector3 _vDrawOffset;

	float _zoom;
	float _angle;
	float _width, _height;

	// 描画順
	// -1: オブジェクトの下に描画（エフェクト）
	// 0 : y座標が小さいものから描画（オブジェクト）
	// 10000: オブジェクトの上に描画（エフェクト）
	int _drawOrder;

	// _parentObjのvPosをスクリーン座標として扱うか
	bool _bDrawWithScreenPos = false;
};


