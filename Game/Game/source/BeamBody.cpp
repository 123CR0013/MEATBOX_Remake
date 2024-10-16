#include "BeamBody.h"

BeamBody::BeamBody(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::BEAM_BODY;
	_direction = DIRECTION::UP;

	_subAnim = new Animation(this);
}

BeamBody::~BeamBody()
{
	
}

void BeamBody::AnimProcess()
{
	if (!_bUse) return;

	GameObject::AnimProcess();
	_subAnim->Process();
}

void BeamBody::Draw()
{
	if (!_bUse) return;

	GameObject::Draw();
	_subAnim->Draw();
}

void BeamBody::SetDirection(DIRECTION direction)
{
	_direction = direction;
	switch (_direction)
	{
	case BeamBody::DIRECTION::UP:
		_anim->SetAnimIndex(1);
		_subAnim->SetAnimIndex(1);
		break;
	case BeamBody::DIRECTION::DOWN:
		_anim->SetAnimIndex(1);
		_subAnim->SetAnimIndex(1);
		break;
	case BeamBody::DIRECTION::LEFT:
	case BeamBody::DIRECTION::RIGHT:
		_anim->SetAnimIndex(0);
		_subAnim->SetAnimIndex(0);
	default:
		break;
	}
}
