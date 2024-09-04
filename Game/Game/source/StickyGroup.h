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

	void SetExistHole(bool bExistHole) { _bExistHole = bExistHole; }
	void SetExistFloor(bool bExistFloor) { _bExistFloor = bExistFloor; }
	void SetGoThroughHole(bool bGoThroughHole) { 
		_bGoThroughHole = _bGoThroughHole && bGoThroughHole;	
	}

	int GetID() { return id; }

protected:
	std::vector<Sticky*> _pSticky;
	std::vector<MeatBox*> _pMeatBox;

	std::vector<GameObject*> _pDeleteEnemyList;

	// �O���[�v���̃~�[�g�{�b�N�X�̈ړ���Ɍ������邩
	bool _bExistHole;
	// �O���[�v���̃~�[�g�{�b�N�X�̈ړ���ɏ������邩
	bool _bExistFloor;
	// ����ʂ蔲�����邩
	bool _bGoThroughHole;

	int id;
};