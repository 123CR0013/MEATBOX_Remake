#include "ObjectBase.h"

ObjectBase::ObjectBase(ModeBase* mode)
{
	_bUse = true;
	_mode = mode;

	_vPos = Vector3(0, 0, 0);
}

ObjectBase::~ObjectBase()
{
}