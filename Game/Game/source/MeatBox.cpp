#include "MeatBox.h"
#include "Effect.h"

MeatBox::MeatBox(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::MEATBOX;
}

MeatBox::~MeatBox()
{

}

bool MeatBox::CheckMove(Vector3 vMove)
{
	Vector3 vOldPos = _vPos;
	_vPos += vMove;

	// �ړ��ł��邩�ǂ���
	bool canMove = false;

	// �ړ���̃}�b�v�`�b�v���擾
	MapChip* mapChip = CheckMapChip(_vPos);
	// �ړ��悪���̏ꍇ
	if (mapChip != nullptr && mapChip->GetType() == MapChip::TYPE::FLOOR) {
		GameObject* obj = CheckObject(_vPos);
		// �ړ���ɃI�u�W�F�N�g������ꍇ
		if (obj != nullptr) {
			// �ړ���̃I�u�W�F�N�g���G�̏ꍇ
			if (obj->GetType() == GameObject::TYPE::ENEMY) {

				// �����1�}�X��̃}�b�v�`�b�v�ƃI�u�W�F�N�g�𒲂ׂ�
				Vector3 vTmp = _vPos + vMove;
				MapChip* nextMapChip = CheckMapChip(vTmp);

				// �G�������Ԃ������
				// �v���C���[���������~�[�g�{�b�N�X�Ƈ@�܂��͇A�œG������
				// �@�ǁi�}�b�v�`�b�v�������`�悳��Ă��Ȃ��}�X�͕ǂƂ��Ĉ����j
				// �A�~�[�g�{�b�N�X

				// �@�̏�������
				if (nextMapChip != nullptr) {
					if (CheckMapChip(vTmp)->GetType() == MapChip::TYPE::NONE ||
						CheckMapChip(vTmp)->GetType() == MapChip::TYPE::WALL)
					{
						canMove = true;
					}
				}
				// �����Ȃ��}�X
				else {
					canMove = true;
				}

				if (!canMove) {
					// �A�̏�������
					GameObject* nextObj = CheckObject(vTmp);
					if (nextObj != nullptr && nextObj->GetUse())
					{
						if (CheckObject(vTmp)->GetType() == GameObject::TYPE::MEATBOX) {
							canMove = true;
						}
					}
				}

				// �G�������Ԃ���ꍇ
				if (canMove) {
					obj->Destroy();
					CreateEffect(Effect::TYPE::EXPLOSION, vTmp, _mode);
				}
			}
		}
		// �ړ���ɃI�u�W�F�N�g���Ȃ��ꍇ
		else {
			canMove = true;
		}
	}

	// �ړ��ł���ꍇ
	if (canMove) {
		// �G�t�F�N�g�𐶐�
		CreateEffect(Effect::TYPE::IMPACT, _vPos, _mode);

		// �}�b�v�f�[�^�̍X�V
		_mapData->SetGameObject(this, _vPos);
		return true;
	}
	// �ړ��ł��Ȃ��ꍇ
	else {
		// ���̈ʒu�ɖ߂�
		_vPos = vOldPos;
		return false;
	}

	return false;
}

MapChip* MeatBox::CheckMapChip(Vector3 vPos)
{
	return _mapData->GetMapChip(vPos);
}

GameObject* MeatBox::CheckObject(Vector3 vPos)
{
	return _mapData->GetGameObject(vPos);
}
