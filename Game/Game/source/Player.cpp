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
	GameObject::Process();

	MoveProcess();
}

void Player::MoveProcess()
{
	// 移動入力があったかどうか
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

		bool bSuccess = false;

		// アニメーション設定
		if (vMove.x != 0) {
			_anim->SetAnimIndex(vMove.x == 1 ? 0 : 1);
		}

		//----------------------------------------
		// マップチップとの判定

		// 移動先のマップチップが床かどうか
		bool bExistFloor = false;

		// 移動先のマップチップを取得
		MapChip* mapChip = _mapData->GetMapChip(_vPos);
		if (mapChip != nullptr)
		{

			switch (mapChip->GetType())
			{
			default:
			case MapChip::TYPE::NONE:
			case MapChip::TYPE::WALL:
			case MapChip::TYPE::HOLE:
				break;
			case MapChip::TYPE::FLOOR:
				bExistFloor = true;
				break;
			}
		}

		//----------------------------------------



		//----------------------------------------
		// 移動先のオブジェクトとの判定

		// 移動先に床がある場合
		if (bExistFloor) {
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
					if (obj->CheckMove(vMove)) 
					{
						bSuccess = true;
					}
					break;
				case GameObject::TYPE::ENEMY:
				case GameObject::TYPE::BEAM_BODY:
					_mode->SetGameOver();
					break;
				case GameObject::TYPE::BEAM_STAND:
					break;
				case GameObject::TYPE::STICKY:
					bSuccess = true;
					break;
				}
			}
			// 移動先にオブジェクトがない場合
			else {
				bSuccess = true;
			}
		}
		//----------------------------------------


		// 移動成功時
		if (bSuccess) {
			// プレイヤーのステップカウントを増やす
			_mode->AddPlayerStepCnt();

			// エフェクト設定
			CreateEffect(Effect::TYPE::MOVE, vOldPos, _mode);

			// マップデータの更新
			_mapData->SetGameObject(this, _vPos);
		}
		else {
			_vPos = vOldPos;
			CreateEffect(Effect::TYPE::QUESTION, _vPos, _mode);
		}

	}
}
