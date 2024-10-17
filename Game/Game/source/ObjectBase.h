#pragma once
#include "appframe.h"

class ObjectBase
{
public:
	ObjectBase(ModeBase* mode);
	~ObjectBase();

	bool GetUse() { return _bUse; }
	virtual void SetUse(bool bUse) { _bUse = bUse; }

	virtual void SetPos(Vector3 pos) { _vPos = pos; }
	Vector3 GetPos() { return _vPos; }

protected:
	ModeBase* _mode;

	bool _bUse;

	// マップチップ座標
	Vector3 _vPos;
};

