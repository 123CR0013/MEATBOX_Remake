#include "MeatBox.h"
#include "Sticky.h"
#include "StickyGroup.h"
#include "Effect.h"


MeatBox::MeatBox(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::MEATBOX;
	_pStickyGroup = nullptr;
	_pSticky = nullptr;
}

MeatBox::~MeatBox()
{
	_pStickyGroup = nullptr;
}

bool MeatBox::CheckMove(Vector3 vMove)
{
	// 移動できるかどうか
	bool bCanMove = false;

	// ねばねばオブジェクトがついている場合
	if(_pStickyGroup != nullptr)
	{
		bCanMove = _pStickyGroup->CheckMove(vMove);
	}
	// ねばねばオブジェクトがついていない場合
	else
	{
		bCanMove = _CheckMove(vMove);
		if(bCanMove)
		{
			Move(vMove);
		}
	}

	return bCanMove;
}

bool MeatBox::_CheckMove(Vector3 vMove)
{
	// 移動先の座標
	Vector3 vNextPos = _vPos + vMove;

	// 移動できるかどうか
	bool bCanMove = false;

	// Stickyへの追加予約用（AddMeatBox()）
	_pSticky = nullptr;

	// 移動先のマップチップを取得
	MapChip* mapChip = CheckMapChip(vNextPos);
	if (mapChip != nullptr) {

		switch (mapChip->GetType())
		{
		default:
			break;
			// 移動先が床の場合
		case MapChip::TYPE::FLOOR:
		{
			GameObject* obj = CheckObject(vNextPos);
			// 移動先にオブジェクトがある場合
			if (obj != nullptr) {
				switch (obj->GetType())
				{
				default:
					break;

					// 移動先のオブジェクトがプレイヤーの場合
				case GameObject::TYPE::PLAYER:
					bCanMove = true;
					break; // End case GameObject::TYPE::PLAYER

					// 移動先のオブジェクトが敵の場合
				case GameObject::TYPE::ENEMY:
				{
					// さらに1マス先のマップチップとオブジェクトを調べる
					Vector3 vTmpPos = vNextPos + vMove;
					MapChip* nextMapChip = CheckMapChip(vTmpPos);

					// 敵を押しつぶせる条件
					// プレイヤーが押したミートボックスと①または②で敵を挟む
					// ①壁（マップチップが何も描画されていないマスは壁として扱う）
					// ②ミートボックス

					// ①の条件判定
					if (nextMapChip != nullptr) {
						MapChip::TYPE type = nextMapChip->GetType();
						if (type == MapChip::TYPE::NONE ||
							type == MapChip::TYPE::WALL)
						{
							bCanMove = true;
						}
					}
					// 何もないマス
					else {
						bCanMove = true;
					}

					if (!bCanMove) {
						// ②の条件判定
						GameObject* nextObj = CheckObject(vTmpPos);
						if (nextObj != nullptr && nextObj->GetUse())
						{
							GameObject::TYPE type = nextObj->GetType();
							if (type == GameObject::TYPE::MEATBOX ||
								type == GameObject::TYPE::BEAM_STAND)
							{
								bCanMove = true;
							}
						}
					}

					// 敵を押しつぶせる場合
					if (bCanMove) {
						if (_pStickyGroup != nullptr) {
							// 削除する敵をStickyGroupに登録
							_pStickyGroup->AddDeleteEnemy(obj);
						}
						else {
							// エフェクトを生成
							CreateEffect(Effect::TYPE::EXPLOSION, vNextPos, _mode);

							// SE再生
							global._soundServer->Play("se_kill_01");

							// 敵を削除
							obj->Destroy();

							// キルカウントをチェック
							_mode->CheckKillCnt(1);

							// 画面振動
							Animation::SetVibration(Vector3(0.05f, 0.05f, 0));
						}
					}
				}
				break; // End case GameObject::TYPE::ENEMY

				// 移動先のオブジェクトがミートボックスの場合
				case GameObject::TYPE::MEATBOX:
					// 同じStickyGroupに所属している場合は移動できる
					if (_pStickyGroup != nullptr && _pStickyGroup == static_cast<MeatBox*>(obj)->GetStickyGroup()) {
						bCanMove = true;
					}
					break; // End case GameObject::TYPE::MEATBOX

					// 移動先のオブジェクトがビーム台の場合
				case GameObject::TYPE::BEAM_STAND:
					// 移動できない
					bCanMove = false;
					break; // End case GameObject::TYPE::BEAM_STAND

					// 移動先のオブジェクトがビーム本体の場合
				case GameObject::TYPE::BEAM_BODY:
					// 移動できる
					bCanMove = true;
					break; // End case GameObject::TYPE::BEAM_BODY

					// 移動先のオブジェクトがねばねば本体の場合
				case GameObject::TYPE::STICKY:
					// 移動できる
					bCanMove = true;

					// Stickyへの追加予約
					_pSticky = static_cast<Sticky*>(obj);
					break; // End case GameObject::TYPE::STICKY

				} // End switch (obj->GetType())
			}
			// 移動先にオブジェクトがない場合
			else {
				bCanMove = true;
			}

			if (bCanMove) {
				SetExistFloor(true);
			}
			else {
				SetGoThroughHole(false);
			}
		}
			break; // End case MapChip::TYPE::FLOOR

			// 移動先が穴の場合
		case MapChip::TYPE::HOLE:
			bCanMove = false;
			SetExistHole(true);
			break; // End case MapChip::TYPE::HOLE

			// 移動先が壁の場合
		case MapChip::TYPE::WALL:
			bCanMove = false;
			SetGoThroughHole(false);
			break; // End case MapChip::TYPE::WALL
		} // End switch (mapChip->GetType())
	}
	// 移動先にマップチップがない場合
	else {
		bCanMove = false;
		if (_pStickyGroup != nullptr) 
		{
			_pStickyGroup->SetGoThroughHole(false);
		}
	}



	return bCanMove;
}

MapChip* MeatBox::CheckMapChip(Vector3 vPos)
{
	return _mapData->GetMapChip(vPos);
}

GameObject* MeatBox::CheckObject(Vector3 vPos)
{
	return _mapData->GetGameObject(vPos);
}

void MeatBox::SetExistHole(bool bExistHole)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetExistHole(bExistHole);
	}
}

void MeatBox::SetExistFloor(bool bExistFloor)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetExistFloor(bExistFloor);
	}
}

void MeatBox::SetGoThroughHole(bool bGoThroughHole)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetGoThroughHole(bGoThroughHole);
	}
}

void MeatBox::Move(Vector3 vMove)
{
	// 移動前の座標を保存
	_vOldPos = _vPos;

	// 座標を更新
	_vPos = _vPos + vMove;
	// マップデータの更新
	_mapData->SetGameObject(this, _vPos);

	// エフェクトを生成
	CreateEffect(Effect::TYPE::IMPACT, _vOldPos + (vMove / 2.0f), _mode);

	//移動フラグ
	_isMove = true;

	//移動する座標を更新
	_vDestiPos = _vPos;

	//描画する位置を移動前の座標に戻す
	_vPos = _vOldPos;

	_frameCount = 0;

	if (_pStickyGroup == nullptr)
	{
		// SE再生
		global._soundServer->Play("se_attack_01");
	}

	// _CheckMove()内で予約したStickyへの追加を行う
	if (_pSticky != nullptr)
	{
		_pSticky->AddMeatBox(this, Sticky::POSITION::ROOT);
	}
	_pSticky = nullptr;
}

void MeatBox::DrawDebug()
{
	// StickyGroup情報の描画
	if (_pStickyGroup != nullptr)
	{
		DrawFormatString(_vPos.x * CHIP_W, _vPos.y * CHIP_H + 50, COLOR_GREEN, "SG:%d", _pStickyGroup->GetID());
	}
}
