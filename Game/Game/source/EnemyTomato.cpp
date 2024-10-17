#include "EnemyTomato.h"

EnemyTomato::EnemyTomato(ModeBase* mode) : Enemy(mode)
{
	_routeSize = 0;
	_routeIndex = 0;
	_moveOrder = 1;
}

EnemyTomato::~EnemyTomato()
{
}

void EnemyTomato::Init()
{
	CheckNextPos();
}

void EnemyTomato::Process()
{
	if (!_bUse) return;
	MoveProcess();
}

void EnemyTomato::SetMoveRoute(std::vector<Vector3> route)
{
	_moveRoute = route;
	_routeSize = _moveRoute.size();
	_routeIndex = 0;
	_moveOrder = 1;
}

void EnemyTomato::MoveProcess()
{
	if (_routeSize <= 0)
	{
		return;
	}

	if(_mode->GetPlayerStepCnt() == 2)
	{
		// 移動先のオブジェクトを取得
		GameObject* obj = _mapData->GetGameObject(_vNextPos);
		// 移動先にオブジェクトがある場合は移動できない
		if (obj != nullptr)
		{
			// プレイヤーに当たった場合
			if (obj->GetType() == TYPE::PLAYER)
			{
				// ゲームオーバー
				_mode->SetGameOver();
			}
			// ビーム本体に当たった場合
			else if (obj->GetType() == TYPE::BEAM_BODY)
			{
				// 移動する
				_routeIndex += _moveOrder;
				_vPos = _vNextPos;
				_mapData->SetGameObject(this, _vPos);
			}
			else 
			{
				// 移動できない場合は、移動順を逆にする
				_moveOrder *= -1;
			}
		}
		// 移動先にオブジェクトがない場合
		else {
			// 移動する
			_routeIndex += _moveOrder;
			_vPos = _vNextPos;
			_mapData->SetGameObject(this, _vPos);
		}

		// 次の移動先を設定
		CheckNextPos();
	}

	// MoveAreaEffectの設定
	SetMoveAreaEffect();
}

// 次の移動先を設定
void EnemyTomato::CheckNextPos()
{
	// 移動順が正順、かつ、最後の要素に到達している場合、逆順にする
	if (_moveOrder == 1 && _routeIndex >= _routeSize - 1)
	{
		_moveOrder = -1;
	}
	// 移動順が逆順、かつ、最初の要素に到達している場合、正順にする
	else if (_moveOrder == -1 && _routeIndex <= 0)
	{
		_moveOrder = 1;
	}

	int nextIndex = _routeIndex + _moveOrder;
	_vNextPos = _moveRoute[nextIndex];
}

// MoveAreaEffectの設定
void EnemyTomato::SetMoveAreaEffect()
{
	// MoveAreaEffectの位置を設定
	_childObjects[0]->SetPos(_vNextPos);
	// MoveAreaEffectの色を設定
	int index = _mode->GetPlayerStepCnt() != 1 ? 0 : 1;
	Animation* anim = _childObjects[0]->GetAnimationClass(0);
	anim->SetAnimIndex(index);
}
