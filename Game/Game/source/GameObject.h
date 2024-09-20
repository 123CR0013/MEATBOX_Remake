#pragma once
#include "ObjectBase.h"
#include "Animation.h"
#include "Map.h"
#include "MapChip.h"

// オブジェクトの下に描画
#define DRAW_ORDER_UNDERLAP_OBJECT -1
// オブジェクトの上に描画
#define DRAW_ORDER_OVERLAP_OBJECT 10000

class GameObject : public ObjectBase
{
public:
	enum class TYPE
	{
		NONE,
		PLAYER,
		MEATBOX,
		ENEMY,
		BEAM_STAND,
		BEAM_BODY,
		STICKY,
		STICKYGROUP
	};

	GameObject(ModeBase* mode);
	virtual ~GameObject();
	void Destroy();

	virtual void Init();
	virtual void Process();
	virtual void ProcessChildObjects();
	virtual bool CheckMove(Vector3 vMove);
	virtual void AnimProcess();
	virtual void Draw();
	virtual void DrawDebug();

	bool GetUse() { return _bUse; }
	virtual void SetUse(bool bUse) { 
		_bUse = bUse;
		
		if (_mapData != nullptr && _bSetToMap == true)
		{
			if(_bUse == true)
			{
				_mapData->SetGameObject(this, _vPos);
			}
			else
			{
				_mapData->EraseGameObject(this);
			}
		}
	}

	TYPE GetType() { return _objectType; }

	Vector3 GetPos() { return _vPos; }
	void SetPos(Vector3 vPos) override { 
		_vPos = vPos; 
		if(_mapData != nullptr && _bSetToMap == true)
		{
			_mapData->SetGameObject(this, _vPos);
		}
	}

	Vector3 GetDrawOffset() const { return _vDrawOffset; }
	void SetDrawOffset(Vector3 vDrawOffset) { _vDrawOffset = vDrawOffset; }

	void AddAnimInfo(AnimationInfo* animInfo) { _anim->AddAnimInfo(animInfo); }
	void SetAnimIndex(int index) { _anim->SetAnimIndex(index); }

	void SetAnimZoom(float zoom) { _anim->SetZoom(zoom); }
	void SetAnimAngle(float angle) { _anim->SetAngle(angle); }
	void SetAnimWidth(float width) { _anim->SetWidth(width); }
	void SetAnimHeight(float height) { _anim->SetHeight(height); }
	void SetAnimSize(float width, float height) { _anim->SetSize(width, height); }

	int GetDrawOrder() { return _drawOrder; }
	void SetDrawOrder(int drawOrder) { _drawOrder = drawOrder; }

	void SetDrawWithScreenPos(bool bDrawWithScreenPos) { _anim->SetDrawWithScreenPos(bDrawWithScreenPos); }

	void AddChildObject(GameObject* object) { _childObjects.push_back(object); }

protected:
	bool _bUse;

	Map* _mapData;
	bool _bSetToMap = true;

	TYPE _objectType;

	// アニメーション
	Animation* _anim;

	// 描画順
	// -1: オブジェクトの下に描画（エフェクト）
	// 0 : y座標が小さいものから描画（オブジェクト）
	// 10000: オブジェクトの上に描画（エフェクト）
	int _drawOrder;

	std::vector<GameObject*> _childObjects;
};