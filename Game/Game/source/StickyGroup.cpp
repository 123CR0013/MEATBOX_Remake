#include "StickyGroup.h"
#include "Sticky.h"
#include "MeatBox.h"
#include "Effect.h"

int tmpId = 0;

StickyGroup::StickyGroup(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKYGROUP;
	_bSetToMap = false;

	_bExistHole = false;
	_bExistFloor = false;
	_bGoThroughHole = false;

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
	// 以下の3つのフラグはミートボックスの_CheckMove関数内で更新される

	// グループ内のミートボックスの移動先に穴があるか
	_bExistHole = false;
	// グループ内のミートボックスの移動先に床があるか
	_bExistFloor = false;
	// 穴を通り抜けられるか
	// ミートボックスの_CheckMove関数内で一度でもfalseになったら、このフラグはfalseのまま
	_bGoThroughHole = true;

	// このグループ内のミートボックスがすべて移動できるかどうかを調べる
	bool bCanMove = true;
	for(auto pMB : _pMeatBox)
	{
		bCanMove = pMB->_CheckMove(vMove) && bCanMove;
	}

	// 以下のどちらかの場合に移動できる
	// ①すべてのミートボックスが移動できる場合
	// ②グループ内のミートボックスの一部が移動可能であり、穴を超えられる場合
	bool bResult = bCanMove || (_bExistHole && _bExistFloor && _bGoThroughHole);
	if (bResult)
	{
		// ミートボックスを移動
		for(auto pMB : _pMeatBox)
		{
			pMB->Move(vMove);
		}

		// 押しつぶせる敵を削除
		int num = 0;
		for (auto pEnemy : _pDeleteEnemyList)
		{
			// 5フレームごとに破裂エフェクトを再生
			Vector3 vEnPos = pEnemy->GetPos();
			std::function<void()> func = [=]() {CreateEffect_Explosion(vEnPos, _mode, num); };
			ModeTimeTable::Add(func, 5 * num);
			num++;

			pEnemy->Destroy();
		}
	}

	_pDeleteEnemyList.clear();

	return bResult;
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

bool StickyGroup::AddMeatBox(MeatBox* pMeatBox)
{
	if (pMeatBox == nullptr) return false;

	// すでに追加されている場合は追加しない
	for (auto pMB : _pMeatBox)
	{
		if (pMB == pMeatBox) return false;
	}

	StickyGroup* tmpSG = pMeatBox->GetStickyGroup();
	if (tmpSG != nullptr) {
		// ミートボックスが別のStickyGroupに所属している場合、結合する
		if (tmpSG != this) {
			Merge(tmpSG);
			return true;
		}
	}
	// ミートボックスがまだStickyGroupに所属していない場合
	else {
		_pMeatBox.push_back(pMeatBox);
		pMeatBox->SetStickyGroup(this);
		return true;
	}

	return false;
}
