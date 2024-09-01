#include "StickyGroup.h"
#include "Sticky.h"
#include "MeatBox.h"
#include "Effect.h"

int tmpId = 0;

StickyGroup::StickyGroup(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKYGROUP;
	_bSetToMap = false;

	id = tmpId;
	tmpId++;
}

StickyGroup::~StickyGroup()
{
	_pSticky.clear();
	_pMeatBox.clear();
	_pDeleteEnemyList.clear();
}

bool StickyGroup::CheckMove(Vector3 vMove)
{
	// このグループ内のミートボックスがすべて移動できるかどうかを調べる
	bool bCanMove = true;
	for(auto pMB : _pMeatBox)
	{
		if (pMB->_CheckMove(vMove) == false)
		{
			bCanMove = false;
			break;
		}
	}

	// すべてのミートボックスが移動できる場合
	if (bCanMove)
	{
		// ミートボックスを移動
		for(auto pMB : _pMeatBox)
		{
			pMB->Move(vMove);
		}

		// 押しつぶせる敵を削除
		for (auto pEnemy : _pDeleteEnemyList)
		{
			CreateEffect(Effect::TYPE::EXPLOSION, pEnemy->GetPos(), _mode);
			pEnemy->Destroy();
		}
	}

	_pDeleteEnemyList.clear();

	return bCanMove;
}

// 他のStickyGroupと結合する
void StickyGroup::Merge(StickyGroup* other)
{
	if (other == nullptr || this == other) return;

	// 所属するStickyGroupを更新
	for (auto pSticky : other->_pSticky)
	{
		pSticky->SetStickyGroup(this);
	}
	// vectorの結合
	_pSticky.reserve(_pSticky.size() + other->_pSticky.size());
	_pSticky.insert(_pSticky.end(), other->_pSticky.begin(), other->_pSticky.end());

	// 所属するStickyGroupを更新
	for(auto pMB : other->_pMeatBox)
	{
		pMB->SetStickyGroup(this);
	}
	// vectorの結合
	_pMeatBox.reserve(_pMeatBox.size() + other->_pMeatBox.size());
	_pMeatBox.insert(_pMeatBox.end(), other->_pMeatBox.begin(), other->_pMeatBox.end());

	// 他のStickyGroupを削除
	_mode->RemoveGameObject(other);
}

void StickyGroup::AddSticky(Sticky* pSticky)
{
	if (pSticky == nullptr) return;

	_pSticky.push_back(pSticky);
	pSticky->SetStickyGroup(this);
}

void StickyGroup::AddMeatBox(MeatBox* pMeatBox)
{
	if (pMeatBox == nullptr) return;

	StickyGroup* tmpSG = pMeatBox->GetStickyGroup();
	if (tmpSG != nullptr) {
		// ミートボックスが別のStickyGroupに所属している場合、結合する
		if (tmpSG != this) {
			Merge(tmpSG);
		}
	}
	// ミートボックスがまだStickyGroupに所属していない場合
	else {
		_pMeatBox.push_back(pMeatBox);
		pMeatBox->SetStickyGroup(this);
	}
}
