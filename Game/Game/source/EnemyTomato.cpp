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

	// ArrowEffectの設定
	SetArrowEffect();
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

// ArrowEffectの設定
void EnemyTomato::SetArrowEffect()
{
	// Arrowの位置を設定
	_childObjects[0]->SetPos(_vNextPos);
	// Arrowの色と向きを設定	
	// index
	//	0:上 1:下 2:左 3:右 （黄色+0 赤+4）
	int dirIndex = 0;
	if (_vPos.y > _vNextPos.y) {
		dirIndex = 0;
	}
	else if (_vPos.y < _vNextPos.y) {
		dirIndex = 1;
	}
	else if (_vPos.x > _vNextPos.x) {
		dirIndex = 2;
	}
	else if (_vPos.x < _vNextPos.x) {
		dirIndex = 3;
	}

	int colorIndex = 0;
	if (_mode->GetPlayerStepCnt() == 1) {
		colorIndex = 4;
	}

	_childObjects[0]->SetAnimIndex(dirIndex + colorIndex);
}
