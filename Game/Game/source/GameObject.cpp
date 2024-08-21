#include "GameObject.h"
#include "DrawForMapChip.h"
#include "ModeGame.h"

GameObject::GameObject(ModeBase* mode) : ObjectBase(mode)
{
	_bUse = true;

	_mapData = nullptr;
	if (_mode != nullptr)
	{
		_mapData = _mode->GetMapData();
	}

	_objectType = TYPE::NONE;

	_vPos = { 0, 0, 0 };
	_vDrawOffset = { 0, 0, 0 };
	_anim = new Animation(this);
}

GameObject::~GameObject()
{
	delete _anim;
	for(auto& object : _childObjects)
	{
		delete object;
	}
}

void GameObject::Destroy()
{
	_mode->RemoveGameObject(this);
	_bUse = false;
}

void GameObject::Init()
{
}

void GameObject::Process()
{
}

void GameObject::ProcessChildObjects()
{
	if(!_bUse) return;
	for(auto& object : _childObjects)
	{
		object->Process();
	}
}

bool GameObject::CheckMove(Vector3 vMove)
{
	return false;
}

void GameObject::AnimProcess()
{
	if (!_bUse) return;
	_anim->Process();
}

void GameObject::Draw()
{
	if (!_bUse) return;
	_anim->Draw();

	for(auto& object : _childObjects)
	{
		object->Draw();
	}
}
