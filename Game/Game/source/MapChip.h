#pragma once
#include "ObjectBase.h"
#include "Animation.h"

class MapChip : public ObjectBase
{
public:
	enum class TYPE
	{
		NONE,
		FLOOR,
		WALL,
		HOLE,
	};

	MapChip(ModeBase* mode);
	~MapChip();

	void AnimProcess();
	void Draw();
	void DrawDebug();

	void SetChipType(TYPE chipType) { _chipType = chipType; }
	TYPE GetType() const { return _chipType; }

	void SetPos(Vector3 vPos) { _vPos = vPos; }
	Vector3 GetPos() const { return _vPos; }

	void SetDrawOffset(Vector3 vDrawOffset) { _vDrawOffset = vDrawOffset; }
	Vector3 GetDrawOffset() const { return _vDrawOffset; }

	void AddAnimInfo(AnimationInfo* animInfo) { _anim->AddAnimInfo(animInfo); }

protected:
	TYPE _chipType;

	// アニメーション
	Animation* _anim;

};
