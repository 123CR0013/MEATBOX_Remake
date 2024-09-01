#pragma once
#include "GameObject.h"
class Sticky;
class MeatBox;

class StickyGroup : public GameObject
{
public:
	StickyGroup(ModeBase* mode);
	~StickyGroup() override;

	bool CheckMove(Vector3 vMove) override;

	void Merge(StickyGroup* other);
	void AddSticky(Sticky* pSticky);
	void AddMeatBox(MeatBox* pMeatBox);

	void AddDeleteEnemy(GameObject* pEnemy) { _pDeleteEnemyList.push_back(pEnemy); }

	int GetID() { return id; }

protected:
	std::vector<Sticky*> _pSticky;
	std::vector<MeatBox*> _pMeatBox;

	std::vector<GameObject*> _pDeleteEnemyList;

	int id;
};