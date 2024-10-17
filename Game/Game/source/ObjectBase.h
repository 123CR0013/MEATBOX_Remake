#pragma once
#include "appframe.h"

class ObjectBase
{
public:
	ObjectBase(ModeBase* mode);
	~ObjectBase();

	virtual void SetPos(Vector3 pos) { _vPos = pos; }
	Vector3 GetPos() { return _vPos; }

protected:
	ModeBase* _mode;

	// マップチップ座標
	Vector3 _vPos;
};

