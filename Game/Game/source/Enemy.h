#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(ModeBase* mode);
	virtual ~Enemy() override;

protected:

};