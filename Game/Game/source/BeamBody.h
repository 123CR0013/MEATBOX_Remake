#pragma once
#include "GameObject.h"

class BeamBody : public GameObject
{
public:
	BeamBody(ModeBase* mode);
	~BeamBody() override;

	void AnimProcess() override;
	void Draw() override;

	enum class DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};
	void SetDirection(BeamBody::DIRECTION direction);

	Animation* GetSubAnimation() override { return _subAnim; }

protected:
	BeamBody::DIRECTION _direction;
	Animation* _subAnim;
};