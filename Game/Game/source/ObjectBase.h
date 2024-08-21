#pragma once
#include "appframe.h"

class ObjectBase
{
public:
	ObjectBase(ModeBase* mode);
	~ObjectBase();

	virtual void SetPos(Vector3 pos) { _vPos = pos; }
	Vector3 GetPos() { return _vPos; }

	void SetDrawOffset(Vector3 offset) { _vDrawOffset = offset; }
	Vector3 GetDrawOffset() { return _vDrawOffset; }

protected:
	ModeBase* _mode;

	// �}�b�v�`�b�v���W
	Vector3 _vPos;
	// �`��I�t�Z�b�g
	Vector3 _vDrawOffset;
};

