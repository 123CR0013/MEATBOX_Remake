#include "GameObject.h"
#include "DrawForMapChip.h"
#include "ModeGame.h"

constexpr float MOVE_FRAME = 6.f;

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

	_isMove = false;
	_frameCount = 0;
}

GameObject::~GameObject()
{
	for (auto& anim : _anims)
	{
		delete anim;
	}
	_anims.clear();
	for(auto& object : _childObjects)
	{
		delete object;
	}
	_childObjects.clear();
}

void GameObject::Destroy()
{
	_mode->RemoveGameObject(this);
	SetUse(false);
}

void GameObject::Init()
{
	
}

void GameObject::ProcessInit()
{
	_vOldPos = _vPos;
}

void GameObject::Process()
{
}

void GameObject::ProcessFinish()
{
	if (!_isMove && _vPos != _vOldPos)
	{
		//�ړ��t���O
		_isMove = true;

		//�ړ�������W���X�V
		_vDestiPos = _vPos;

		//�`�悷��ʒu���ړ��O�̍��W�ɖ߂�
		_vPos = _vOldPos;

		_frameCount = 0;
	}

	if (_isMove)
	{
		float per = Easing::OutQuad(static_cast<float>(_frameCount++), 0.f, 1.f, MOVE_FRAME);

		_vPos = Vector3::Lerp(_vOldPos, _vDestiPos, per);

		if (per >= 1.f)
		{
			_isMove = false;
		}
	}
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
	for (auto& anim : _anims)
	{
		anim->Process();
	}
	for (auto& object : _childObjects)
	{
		object->AnimProcess();
	}
}

void GameObject::Draw()
{
	if (!_bUse) return;
	for (auto& anim : _anims)
	{
		anim->Draw();
	}

	//for(auto& object : _childObjects)
	//{
	//	object->Draw();
	//}
}

void GameObject::DrawDebug()
{
}

Animation* GameObject::AddAnimationClass()
{
	Animation* anim = new Animation(this);
	_anims.push_back(anim);
	return anim;
}

