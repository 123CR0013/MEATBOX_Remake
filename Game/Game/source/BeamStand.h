#pragma once
#include "GameObject.h"
class BeamBody;

class BeamStand : public GameObject
{
public:
	BeamStand(ModeBase* mode);
	~BeamStand() override;

	void Process() override;
	void AnimProcess() override;

	enum class DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};
	Vector3 GetDir() { return _vDir; }
	void SetDir(Vector3 vDir) {
		_vDir = vDir; 
		if (_vDir.x > 0)
		{
			_direction = DIRECTION::RIGHT;
		}
		else if (_vDir.x < 0)
		{
			_direction = DIRECTION::LEFT;
		}
		else if (_vDir.y < 0)
		{
			_direction = DIRECTION::UP;
		}
		else if (_vDir.y > 0)
		{
			_direction = DIRECTION::DOWN;
		}
	}

	void SetUse(bool bUse) override
	{
		_bUse = bUse;

		if (_mapData != nullptr && _bSetToMap == true)
		{
			if (_bUse == true)
			{
				_mapData->SetGameObject(this, _vPos);
			}
			else
			{
				_mapData->EraseGameObject(this);
			}
		}

		SetUnuseAllBeamBody();
	}

protected:
	BeamStand::DIRECTION _direction;
	Vector3 _vDir;

	Vector3 _vBBEndPos;

	void CreateBeamBody(Vector3 vPos);
	void UpdateBeamBodies(Vector3 vStartPos, Vector3 vEndPos);
	void SetUnuseAllBeamBody();

	// BeamBodyのアニメーションを色ごとに同期
	void SyncBeamBodyAnim();

	// 一番端のBeamBody
	BeamBody* _pBBEnd;

	// BeamBodyが壁に当たったか
	bool _bWallExist;
};