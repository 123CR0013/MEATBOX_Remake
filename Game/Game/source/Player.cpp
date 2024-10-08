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
	// �ړ����͂����������ǂ���
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

		bool bSuccess = false;

		// �A�j���[�V�����ݒ�
		if (vMove.x != 0) {
			_anim->SetAnimIndex(vMove.x == 1 ? 0 : 1);
		}

		//----------------------------------------
		// �}�b�v�`�b�v�Ƃ̔���

		// �ړ���̃}�b�v�`�b�v�������ǂ���
		bool bExistFloor = false;

		// �ړ���̃}�b�v�`�b�v���擾
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
		// �ړ���̃I�u�W�F�N�g�Ƃ̔���

		// �ړ���ɏ�������ꍇ
		if (bExistFloor) {
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
			// �ړ���ɃI�u�W�F�N�g���Ȃ��ꍇ
			else {
				bSuccess = true;
			}
		}
		//----------------------------------------


		// �ړ�������
		if (bSuccess) {
			// �v���C���[�̃X�e�b�v�J�E���g�𑝂₷
			_mode->AddPlayerStepCnt();

			// �G�t�F�N�g�ݒ�
			CreateEffect(Effect::TYPE::MOVE, vOldPos, _mode);

			// �}�b�v�f�[�^�̍X�V
			_mapData->SetGameObject(this, _vPos);
		}
		else {
			_vPos = vOldPos;
			CreateEffect(Effect::TYPE::QUESTION, _vPos, _mode);
		}

	}
}
