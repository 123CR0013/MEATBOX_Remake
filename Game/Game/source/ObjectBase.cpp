#include "ObjectBase.h"

ObjectBase::ObjectBase(ModeBase* mode)
{
	_mode = mode;

	_vPos = Vector3(0, 0, 0);
}

ObjectBase::~ObjectBase()
{
}