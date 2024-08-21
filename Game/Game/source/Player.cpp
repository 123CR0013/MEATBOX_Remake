#include "Player.h"
#include "ModeGame.h"
#include "Effect.h"

Player::Player(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::PLAYER;
	_dirX = 1;
}

Player::~Player()
{
}

void Player::Process()
{
	MoveProcess();
}

void Player::MoveProcess()
{
	// 移動したかどうか
	bool bMoved = false;
	// 移動前の座標
	Vector3 vOldPos = _vPos;

	// 移動値
	Vector3 vMove = VGet(0, 0, 0);

	if (global._trg & PAD_INPUT_LEFT)
	{
		_vPos.x -= 1;
		vMove.x = -1;
		_dirX = -1;
		bMoved = true;
	}
	else if (global._trg & PAD_INPUT_RIGHT)
	{
		_vPos.x += 1;
		vMove.x = 1;
		_dirX = 1;
		bMoved = true;
	}
	else if (global._trg & PAD_INPUT_UP)
	{
		_vPos.y -= 1;
		vMove.y = -1;
		bMoved = true;
	}
	else if (global._trg & PAD_INPUT_DOWN)
	{
		_vPos.y += 1;
		vMove.y = 1;
		bMoved = true;
	}


	if (bMoved) {
		// プレイヤーのステップカウントを増やす
		_mode->AddPlayerStepCnt();

		// アニメーション設定
		if (vMove.x != 0) {
			_anim->SetAnimIndex(vMove.x == 1 ? 0 : 1);
		}

		// エフェクト設定
		CreateEffect(Effect::TYPE::MOVE, vOldPos, _mode);

		//----------------------------------------
		// マップチップとの判定

		// 移動先のマップチップを取得
		MapChip* mapChip = _mapData->GetMapChip(_vPos);
		if (mapChip == nullptr)
		{
			_vPos = vOldPos;
			return;
		}

		switch (mapChip->GetType())
		{
		default:
		case MapChip::TYPE::NONE:
		case MapChip::TYPE::WALL:
		case MapChip::TYPE::HOLE:
			_vPos = vOldPos;
			break;
		case MapChip::TYPE::FLOOR:
			break;
		}

		//----------------------------------------



		//----------------------------------------
		// 移動先のオブジェクトとの判定

		if (_vPos != vOldPos) {
			// 移動先のオブジェクトを取得
			GameObject* obj = _mapData->GetGameObject(_vPos);
			if (obj != nullptr)
			{
				switch (obj->GetType())
				{
				default:
					break;
				case GameObject::TYPE::NONE:
					break;
				case GameObject::TYPE::MEATBOX:
					if (obj->CheckMove(vMove) == false) {
						_vPos = vOldPos;
						CreateEffect(Effect::TYPE::QUESTION, _vPos, _mode);
						return;
					}
					break;
				case GameObject::TYPE::ENEMY:
				case GameObject::TYPE::BEAM_BODY:
					_mode->SetGameOver();
					break;
				case GameObject::TYPE::BEAM_STAND:
					_vPos = vOldPos;
					break;
				}
			}
		}
		//----------------------------------------

		// マップデータの更新
		_mapData->SetGameObject(this, _vPos);

	}
}
