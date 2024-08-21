#pragma once
#include "GameObject.h"

class MeatBox : public GameObject
{
public:
	MeatBox(ModeBase* mode);
	~MeatBox() override;
	
	bool CheckMove(Vector3 vMove) override;


protected:
	MapChip* CheckMapChip(Vector3 vPos);
	GameObject* CheckObject(Vector3 vPos);
};