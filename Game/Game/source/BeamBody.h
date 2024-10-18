#pragma once
#include "GameObject.h"
#include "BeamStand.h"

class BeamBody : public GameObject
{
public:
	BeamBody(ModeBase* mode);
	~BeamBody() override;

	void AnimProcess() override;
	void Draw() override;

	void SetDirection(BeamStand::DIRECTION direction);

protected:
	BeamStand::DIRECTION _direction;
};