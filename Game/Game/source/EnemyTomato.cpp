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
		// �ړ���̃I�u�W�F�N�g���擾
		GameObject* obj = _mapData->GetGameObject(_vNextPos);
		// �ړ���ɃI�u�W�F�N�g������ꍇ�͈ړ��ł��Ȃ�
		if (obj != nullptr)
		{
			// �v���C���[�ɓ��������ꍇ
			if (obj->GetType() == TYPE::PLAYER)
			{
				// �Q�[���I�[�o�[
				_mode->SetGameOver();
			}
			// �r�[���{�̂ɓ��������ꍇ
			else if (obj->GetType() == TYPE::BEAM_BODY)
			{
				// �ړ�����
				_routeIndex += _moveOrder;
				_vPos = _vNextPos;
				_mapData->SetGameObject(this, _vPos);
			}
			else 
			{
				// �ړ��ł��Ȃ��ꍇ�́A�ړ������t�ɂ���
				_moveOrder *= -1;
			}
		}
		// �ړ���ɃI�u�W�F�N�g���Ȃ��ꍇ
		else {
			// �ړ�����
			_routeIndex += _moveOrder;
			_vPos = _vNextPos;
			_mapData->SetGameObject(this, _vPos);
		}

		// ���̈ړ����ݒ�
		CheckNextPos();
	}

	// MoveAreaEffect�̐ݒ�
	SetMoveAreaEffect();
}

// ���̈ړ����ݒ�
void EnemyTomato::CheckNextPos()
{
	// �ړ����������A���A�Ō�̗v�f�ɓ��B���Ă���ꍇ�A�t���ɂ���
	if (_moveOrder == 1 && _routeIndex >= _routeSize - 1)
	{
		_moveOrder = -1;
	}
	// �ړ������t���A���A�ŏ��̗v�f�ɓ��B���Ă���ꍇ�A�����ɂ���
	else if (_moveOrder == -1 && _routeIndex <= 0)
	{
		_moveOrder = 1;
	}

	int nextIndex = _routeIndex + _moveOrder;
	_vNextPos = _moveRoute[nextIndex];
}

// MoveAreaEffect�̐ݒ�
void EnemyTomato::SetMoveAreaEffect()
{
	// MoveAreaEffect�̈ʒu��ݒ�
	_childObjects[0]->SetPos(_vNextPos);
	// MoveAreaEffect�̐F��ݒ�
	int index = _mode->GetPlayerStepCnt() != 1 ? 0 : 1;
	Animation* anim = _childObjects[0]->GetAnimationClass(0);
	anim->SetAnimIndex(index);
}
