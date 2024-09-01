#pragma once
#include "GameObject.h"
class StickyGroup;
class MeatBox;

class Sticky : public GameObject
{
public:
	Sticky(ModeBase* mode);
	~Sticky() override;

	void Process() override;


	enum class POSITION {
		ROOT,
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	bool AddMeatBox(MeatBox* pMeatBox, Sticky::POSITION pos);


	//bool CheckMove(Vector3 vMove) override;

	StickyGroup* GetStickyGroup() { return _pStickyGroup; }
	void SetStickyGroup(StickyGroup* pStickyGroup) { _pStickyGroup = pStickyGroup; }

protected:
	StickyGroup* _pStickyGroup;

	MeatBox* _pRootMB;
	std::array<bool, 5> _bMBExist;

	// ねばねば本体の周囲4マスにミートボックスがあるかを調べる
	void CheckMeatBoxExistAround();
};