#include "BeamBody.h"

BeamBody::BeamBody(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::BEAM_BODY;
	_direction = BeamStand::DIRECTION::UP;
}

BeamBody::~BeamBody()
{
	
}

void BeamBody::AnimProcess()
{
	if (!_bUse) return;

	GameObject::AnimProcess();
}

void BeamBody::Draw()
{
	if (!_bUse) return;

	GameObject::Draw();
}

void BeamBody::SetDirection(BeamStand::DIRECTION direction)
{
	_direction = direction;
	switch (_direction)
	{
	case BeamStand::DIRECTION::UP:
		GetAnimationClass(0)->SetAnimIndex(1);
		GetAnimationClass(1)->SetAnimIndex(1);
		break;
	case BeamStand::DIRECTION::DOWN:
		GetAnimationClass(0)->SetAnimIndex(1);
		GetAnimationClass(1)->SetAnimIndex(1);
		break;
	case BeamStand::DIRECTION::LEFT:
	case BeamStand::DIRECTION::RIGHT:
		GetAnimationClass(0)->SetAnimIndex(0);
		GetAnimationClass(1)->SetAnimIndex(0);
	default:
		break;
	}
}
