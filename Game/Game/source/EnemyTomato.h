#pragma once
#include "Enemy.h"
#include <vector>

class EnemyTomato : public Enemy
{
public:
	EnemyTomato(ModeBase* mode);
	~EnemyTomato() override;

	void Init() override;
	void Process() override;


	void SetMoveRoute(std::vector<Vector3> route);

protected:
	//-------------------------------------
	// 移動処理
	void MoveProcess();

	// 次の移動先を設定
	void CheckNextPos();

	// MoveAreaEffectの設定
	void SetMoveAreaEffect();

	// 移動ルート
	std::vector<Vector3> _moveRoute;
	// 移動ルートの配列のサイズ
	int _routeSize;

	// 現在のインデックス
	int _routeIndex;
	// 移動ルートをどう進むか 1:正順 -1:逆順
	int _moveOrder;

	Vector3 _vNextPos;

	//-------------------------------------

};