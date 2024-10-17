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
	// �ړ�����
	void MoveProcess();

	// ���̈ړ����ݒ�
	void CheckNextPos();

	// MoveAreaEffect�̐ݒ�
	void SetMoveAreaEffect();

	// �ړ����[�g
	std::vector<Vector3> _moveRoute;
	// �ړ����[�g�̔z��̃T�C�Y
	int _routeSize;

	// ���݂̃C���f�b�N�X
	int _routeIndex;
	// �ړ����[�g���ǂ��i�ނ� 1:���� -1:�t��
	int _moveOrder;

	Vector3 _vNextPos;

	//-------------------------------------

};