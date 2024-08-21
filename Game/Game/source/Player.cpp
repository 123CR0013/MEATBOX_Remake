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
	// �ړ��������ǂ���
	bool bMoved = false;
	// �ړ��O�̍��W
	Vector3 vOldPos = _vPos;

	// �ړ��l
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
		// �v���C���[�̃X�e�b�v�J�E���g�𑝂₷
		_mode->AddPlayerStepCnt();

		// �A�j���[�V�����ݒ�
		if (vMove.x != 0) {
			_anim->SetAnimIndex(vMove.x == 1 ? 0 : 1);
		}

		// �G�t�F�N�g�ݒ�
		CreateEffect(Effect::TYPE::MOVE, vOldPos, _mode);

		//----------------------------------------
		// �}�b�v�`�b�v�Ƃ̔���

		// �ړ���̃}�b�v�`�b�v���擾
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
		// �ړ���̃I�u�W�F�N�g�Ƃ̔���

		if (_vPos != vOldPos) {
			// �ړ���̃I�u�W�F�N�g���擾
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

		// �}�b�v�f�[�^�̍X�V
		_mapData->SetGameObject(this, _vPos);

	}
}
