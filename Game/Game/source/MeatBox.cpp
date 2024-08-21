#include "MeatBox.h"
#include "Effect.h"

MeatBox::MeatBox(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::MEATBOX;
}

MeatBox::~MeatBox()
{

}

bool MeatBox::CheckMove(Vector3 vMove)
{
	Vector3 vOldPos = _vPos;
	_vPos += vMove;

	// 移動できるかどうか
	bool canMove = false;

	// 移動先のマップチップを取得
	MapChip* mapChip = CheckMapChip(_vPos);
	// 移動先が床の場合
	if (mapChip != nullptr && mapChip->GetType() == MapChip::TYPE::FLOOR) {
		GameObject* obj = CheckObject(_vPos);
		// 移動先にオブジェクトがある場合
		if (obj != nullptr) {
			// 移動先のオブジェクトが敵の場合
			if (obj->GetType() == GameObject::TYPE::ENEMY) {

				// さらに1マス先のマップチップとオブジェクトを調べる
				Vector3 vTmp = _vPos + vMove;
				MapChip* nextMapChip = CheckMapChip(vTmp);

				// 敵を押しつぶせる条件
				// プレイヤーが押したミートボックスと①または②で敵を挟む
				// ①壁（マップチップが何も描画されていないマスは壁として扱う）
				// ②ミートボックス

				// ①の条件判定
				if (nextMapChip != nullptr) {
					if (CheckMapChip(vTmp)->GetType() == MapChip::TYPE::NONE ||
						CheckMapChip(vTmp)->GetType() == MapChip::TYPE::WALL)
					{
						canMove = true;
					}
				}
				// 何もないマス
				else {
					canMove = true;
				}

				if (!canMove) {
					// ②の条件判定
					GameObject* nextObj = CheckObject(vTmp);
					if (nextObj != nullptr && nextObj->GetUse())
					{
						if (CheckObject(vTmp)->GetType() == GameObject::TYPE::MEATBOX) {
							canMove = true;
						}
					}
				}

				// 敵を押しつぶせる場合
				if (canMove) {
					obj->Destroy();
					CreateEffect(Effect::TYPE::EXPLOSION, vTmp, _mode);
				}
			}
		}
		// 移動先にオブジェクトがない場合
		else {
			canMove = true;
		}
	}

	// 移動できる場合
	if (canMove) {
		// エフェクトを生成
		CreateEffect(Effect::TYPE::IMPACT, _vPos, _mode);

		// マップデータの更新
		_mapData->SetGameObject(this, _vPos);
		return true;
	}
	// 移動できない場合
	else {
		// 元の位置に戻す
		_vPos = vOldPos;
		return false;
	}

	return false;
}

MapChip* MeatBox::CheckMapChip(Vector3 vPos)
{
	return _mapData->GetMapChip(vPos);
}

GameObject* MeatBox::CheckObject(Vector3 vPos)
{
	return _mapData->GetGameObject(vPos);
}
