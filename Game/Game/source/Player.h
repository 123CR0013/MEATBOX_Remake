#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(ModeBase* mode);
	~Player() override;

	void Process() override;
	void AnimProcess() override;

protected:
	void MoveProcess();

	// キャラクターの向き
	// -1:左 1:右
	int _dirX;
};