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

	Vector3 GetDir() { return _vDir; }
	void SetDir(Vector3 vDir) { _vDir = vDir; }

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
	Vector3 _vDir;

	void CreateBeamBody(Vector3 vPos);
	void UpdateBeamBodies(Vector3 vStartPos, Vector3 vEndPos);
	void SetUnuseAllBeamBody();

	// BeamBodyのアニメーションを色ごとに同期
	void SyncBeamBodyAnim();
};