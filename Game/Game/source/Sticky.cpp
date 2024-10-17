#include "Sticky.h"
#include "StickyGroup.h"
#include "MeatBox.h"

Sticky::Sticky(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKY;

	_pStickyGroup = nullptr;
	_pRootMB = nullptr;
	_bMBExist.fill(false);
}

Sticky::~Sticky()
{
	_pStickyGroup = nullptr;
	_pRootMB = nullptr;
	_bMBExist.fill(false);
}

void Sticky::Process()
{
	GameObject::Process();

	// ねばねば本体がミートボックスにくっついている場合、ねばねば本体の座標をミートボックスの座標に合わせる
	if (_bMBExist[0] == true)
	{
		// MBが移動中でない場合
		if (_pRootMB->IsMove() == false)
		{
			_vPos = _pRootMB->GetPos();

			// 周囲4マスにミートボックスがあるかを調べる
			CheckMeatBoxExistAround();
		}
	}
	else {
		// ねばねば本体がミートボックスにくっついていない場合、ねばねば本体の座標をマップに登録する
		// プレイヤーが上に乗ったときにねばねば本体が消えないようにするため
		GameObject* obj = _mapData->GetGameObject(_vPos);
		if (obj == nullptr)
		{
			_mapData->SetGameObject(this, _vPos);
		}
		// 上にミートボックスがある場合
		// BeamBodyの下にねばねばマスがあり、そのマスにミートボックスが押された場合にくっつく様にするため
		else if (obj->GetType() == TYPE::MEATBOX)
		{
			AddMeatBox(static_cast<MeatBox*>(obj), POSITION::ROOT);
		}
	}
}

bool Sticky::AddMeatBox(MeatBox* pMeatBox, Sticky::POSITION pos)
{
	if(_bMBExist[(int)pos] == false && pMeatBox != nullptr)
	{
		// StickyGroupにミートボックスを追加
		bool bSuccess = _pStickyGroup->AddMeatBox(pMeatBox);
		if (bSuccess == false) return false;

		_bMBExist[(int)pos] = true;

		if(pos == POSITION::ROOT)
		{
			_pRootMB = pMeatBox;
			_mapData->EraseGameObject(this);
		}

		return true;
	}
	return false;
}

// ねばねば本体の周囲4マスにミートボックスがあるかを調べる
void Sticky::CheckMeatBoxExistAround()
{
	// ねばねば本体がミートボックスにくっついている場合
	if (_bMBExist[0] == true)
	{
		// ねばねば本体の座標をミートボックスの座標に合わせる
		_vPos = _pRootMB->GetPos();

		std::array<Vector3, 4> vPos = {
			_vPos + VGet(0, -1, 0),
			_vPos + VGet(0, 1, 0),
			_vPos + VGet(-1, 0, 0),
			_vPos + VGet(1, 0, 0)
		};

		// 上下左右にミートボックスがあるかどうかを調べる
		for (int i = 1; i < 5; i++)
		{
			if (_bMBExist[i] != false) continue;

			GameObject* obj = _mapData->GetGameObject(vPos[i - 1]);
			if (obj != nullptr && obj->GetType() == TYPE::MEATBOX)
			{
				AddMeatBox(static_cast<MeatBox*>(obj), static_cast<POSITION>(i));
			}
		}
	}
}
