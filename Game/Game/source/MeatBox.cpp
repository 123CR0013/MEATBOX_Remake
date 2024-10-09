#include "MeatBox.h"
#include "Sticky.h"
#include "StickyGroup.h"
#include "Effect.h"


MeatBox::MeatBox(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::MEATBOX;
	_pStickyGroup = nullptr;
}

MeatBox::~MeatBox()
{
	_pStickyGroup = nullptr;
}

bool MeatBox::CheckMove(Vector3 vMove)
{
	// �ړ��ł��邩�ǂ���
	bool bCanMove = false;

	// �˂΂˂΃I�u�W�F�N�g�����Ă���ꍇ
	if(_pStickyGroup != nullptr)
	{
		bCanMove = _pStickyGroup->CheckMove(vMove);
	}
	// �˂΂˂΃I�u�W�F�N�g�����Ă��Ȃ��ꍇ
	else
	{
		bCanMove = _CheckMove(vMove);
		if(bCanMove)
		{
			Move(vMove);
		}
	}

	return bCanMove;
}

bool MeatBox::_CheckMove(Vector3 vMove)
{
	// �ړ���̍��W
	Vector3 vNextPos = _vPos + vMove;

	// �ړ��ł��邩�ǂ���
	bool bCanMove = false;

	// �ړ���̃}�b�v�`�b�v���擾
	MapChip* mapChip = CheckMapChip(vNextPos);
	if (mapChip != nullptr) {
		// �ړ��悪���̏ꍇ
		if (mapChip->GetType() == MapChip::TYPE::FLOOR) {
			GameObject* obj = CheckObject(vNextPos);
			// �ړ���ɃI�u�W�F�N�g������ꍇ
			if (obj != nullptr) {
				// �ړ���̃I�u�W�F�N�g���G�̏ꍇ
				if (obj->GetType() == GameObject::TYPE::ENEMY) {

					// �����1�}�X��̃}�b�v�`�b�v�ƃI�u�W�F�N�g�𒲂ׂ�
					Vector3 vTmpPos = vNextPos + vMove;
					MapChip* nextMapChip = CheckMapChip(vTmpPos);

					// �G�������Ԃ������
					// �v���C���[���������~�[�g�{�b�N�X�Ƈ@�܂��͇A�œG������
					// �@�ǁi�}�b�v�`�b�v�������`�悳��Ă��Ȃ��}�X�͕ǂƂ��Ĉ����j
					// �A�~�[�g�{�b�N�X

					// �@�̏�������
					if (nextMapChip != nullptr) {
						if (CheckMapChip(vTmpPos)->GetType() == MapChip::TYPE::NONE ||
							CheckMapChip(vTmpPos)->GetType() == MapChip::TYPE::WALL)
						{
							bCanMove = true;
						}
					}
					// �����Ȃ��}�X
					else {
						bCanMove = true;
					}

					if (!bCanMove) {
						// �A�̏�������
						GameObject* nextObj = CheckObject(vTmpPos);
						if (nextObj != nullptr && nextObj->GetUse())
						{
							if (CheckObject(vTmpPos)->GetType() == GameObject::TYPE::MEATBOX) {
								bCanMove = true;
							}
						}
					}

					// �G�������Ԃ���ꍇ
					if (bCanMove) {
						if (_pStickyGroup != nullptr) {
							// �폜����G��StickyGroup�ɓo�^
							_pStickyGroup->AddDeleteEnemy(obj);
						}
						else {
							// �G�t�F�N�g�𐶐�
							CreateEffect(Effect::TYPE::EXPLOSION, vNextPos, _mode);
							// �G���폜
							obj->Destroy();
						}
					}
				}
				// �ړ���̃I�u�W�F�N�g���~�[�g�{�b�N�X�̏ꍇ
				else if (obj->GetType() == GameObject::TYPE::MEATBOX) {
					// ����StickyGroup�ɏ������Ă���ꍇ�͈ړ��ł���
					if (_pStickyGroup != nullptr && _pStickyGroup == static_cast<MeatBox*>(obj)->GetStickyGroup()) {
						bCanMove = true;
					}
				}
				// �ړ���̃I�u�W�F�N�g���r�[����̏ꍇ
				else if (obj->GetType() == GameObject::TYPE::BEAM_STAND) {
					// �ړ��ł��Ȃ�
					bCanMove = false;
				}
				// �ړ���̃I�u�W�F�N�g���r�[���{�̂̏ꍇ
				else if (obj->GetType() == GameObject::TYPE::BEAM_BODY) {
					// �ړ��ł���
					bCanMove = true;
				}
				// �ړ���̃I�u�W�F�N�g���˂΂˂Ζ{�̂̏ꍇ
				else if (obj->GetType() == GameObject::TYPE::STICKY) {
					// �ړ��ł���
					bCanMove = true;
					Sticky* pSticky = static_cast<Sticky*>(obj);
					pSticky->AddMeatBox(this, Sticky::POSITION::ROOT);
				}
			}
			// �ړ���ɃI�u�W�F�N�g���Ȃ��ꍇ
			else {
				bCanMove = true;
			}

			if (bCanMove) {
				SetExistFloor(true);
			}
			else {
				SetGoThroughHole(false);
			}
		}
		else if (mapChip->GetType() == MapChip::TYPE::HOLE) {
			bCanMove = false;
			SetExistHole(true);
		}else if(mapChip->GetType() == MapChip::TYPE::WALL) {
			bCanMove = false;
			SetGoThroughHole(false);
		}
	}
	else {
		bCanMove = false;
		_pStickyGroup->SetGoThroughHole(false);
	}



	return bCanMove;
}

MapChip* MeatBox::CheckMapChip(Vector3 vPos)
{
	return _mapData->GetMapChip(vPos);
}

GameObject* MeatBox::CheckObject(Vector3 vPos)
{
	return _mapData->GetGameObject(vPos);
}

void MeatBox::SetExistHole(bool bExistHole)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetExistHole(bExistHole);
	}
}

void MeatBox::SetExistFloor(bool bExistFloor)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetExistFloor(bExistFloor);
	}
}

void MeatBox::SetGoThroughHole(bool bGoThroughHole)
{
	if (_pStickyGroup != nullptr)
	{
		_pStickyGroup->SetGoThroughHole(bGoThroughHole);
	}
}

void MeatBox::Move(Vector3 vMove)
{
	// �ړ��O�̍��W��ۑ�
	_vOldPos = _vPos;

	// ���W���X�V
	_vPos = _vPos + vMove;
	// �}�b�v�f�[�^�̍X�V
	_mapData->SetGameObject(this, _vPos);

	// �G�t�F�N�g�𐶐�
	CreateEffect(Effect::TYPE::IMPACT, _vOldPos + (vMove / 2.0f), _mode);

	//�ړ��t���O
	_isMove = true;

	//�ړ�������W���X�V
	_vDestiPos = _vPos;

	//�`�悷��ʒu���ړ��O�̍��W�ɖ߂�
	_vPos = _vOldPos;

	_frameCount = 0;
}

void MeatBox::DrawDebug()
{
	// StickyGroup���̕`��
	if (_pStickyGroup != nullptr)
	{
		DrawFormatString(_vPos.x * CHIP_W, _vPos.y * CHIP_H + 50, COLOR_GREEN, "SG:%d", _pStickyGroup->GetID());
	}
}
