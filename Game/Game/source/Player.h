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

	// �L�����N�^�[�̌���
	// -1:�� 1:�E
	int _dirX;
};