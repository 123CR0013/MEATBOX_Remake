#pragma once
#include "GameObject.h"

class TutorialObject : public GameObject
{
public:
	TutorialObject(ModeBase* mode, int id);
	virtual ~TutorialObject();

	int GetID() { return _id; }

protected:
	int _id;
};